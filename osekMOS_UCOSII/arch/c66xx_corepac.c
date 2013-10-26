/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_corepac,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_corepac.c
 * @brief
 *	<li>功能：定义了C66XX体系DSP Corepac模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
*/

/**************************** 引用部分 *********************************/

#include "c66xx_corepac.h"
#include "c66xx_io.h"
#include "c66xx_debug.h"
#include "c66xx_mpu.h"
#include "osprop.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

/*
 * @brief
 *    GetEventFlag：内部使用函数，获得指定事件的状态。
 * @param		efnum：事件号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    0，事件没有发生。
 * <p>			1，事件发生了。
 */
static int GetEventFlag(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_FLAG_REG(num));
	if (tmp & (1 << indx))
		return 1;
	else
		return 0;
}

/*
 * @brief
 *    SetEventFlag：内部使用函数，人工设置给定事件的状态。
 * @param		efnum：事件号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    上一次设置事件前，该事件的状态。
 */
static int SetEventFlag(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;
	int last;

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_FLAG_REG(num));
	if (tmp & (1 << indx))
		last = 1;
	else
		last = 0;

	tmp = readl(COREPAC_EVENT_SET_REG(num));
	tmp |= (1 << indx);
	writel(tmp, COREPAC_EVENT_SET_REG(num));

	return last;
}

/*
 * @brief
 *    ClrEventFlag：内部使用函数，人工清除给定事件的状态。
 * @param		efnum：事件号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    上一次清除事件前，该事件的状态。
 */
static int ClrEventFlag(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;
	int last;

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_FLAG_REG(num));
	if (tmp & (1 << indx))
		last = 1;
	else
		last = 0;

	tmp = readl(COREPAC_EVENT_CLEAR_REG(num));
	tmp |= (1 <<  indx);
	writel(tmp, COREPAC_EVENT_CLEAR_REG(num));

	return last;
}

/*
 * @brief
 *    UnmaskCombineEvent：内部使用函数，解除给定组合事件的屏蔽状态。
 * @param		efnum：组合事件号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void UnmaskCombineEvent(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;

	if (efnum < 4) {
		DEBUG_DEV("bad efnum, you cannot combine 0-3 itself\n");
		return;
	}

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_MASK_REG(num));
	tmp &= ~(1 << indx);
	writel(tmp, COREPAC_EVENT_MASK_REG(num));

}

/*
 * @brief
 *    MaskCombineEvent：内部使用函数，屏蔽给定组合事件。
 * @param		efnum：组合事件号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void MaskCombineEvent(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;

	if (efnum < 4) {
		DEBUG_DEV("bad efnum, you cannot combine 0-3 itself\n");
		return;
	}

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_MASK_REG(num));
	tmp |= (1 << indx);
	writel(tmp, COREPAC_EVENT_MASK_REG(num));
}

/*
 * @brief
 *    MaskCombineReadEvent：内部使用函数，找到发生的组合中断。
 * @param		efnum：组合事件号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	返回经过处理的组合中断状态寄存器的值，某位为1表示该位对应的组合事件已经发生。某位为0表示
 * <p>			该位对应的组合事件要么没发生要么被屏蔽了。
 */
static unsigned long MaskCombineReadEvent(int efnum)
{
	unsigned long tmp, tmp1, tmp2;
	if (efnum > 3) {
		DEBUG_DEV("bad efnum\n");
		return 0;
	}
	tmp1 = readl(COREPAC_MASK_EVENT_FLAG_REG(efnum));
	tmp2 = readl(COREPAC_EVENT_MASK_REG(efnum));

	tmp = 0;
	tmp |= tmp1;
	tmp &= ~(tmp2);

	return tmp;
}

/*
 * @brief
 *    MaskCombineReadEvent：内部使用函数，找到发生的组合中断。
 * @param		efnum：事件号。
 * 				dspInt: DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	返回上一次dspInt对应的efnum。
 */
static int MapDspInt(int efnum, int dspInt)
{
	int indx;
	int num;
	unsigned long tmp;
	unsigned long last;

	if (dspInt < 4 || dspInt > 15) {
		DEBUG_DEV("bad dsp_int\n");
		return -1;
	}

	if (efnum > 128) {
		DEBUG_DEV("bad efnum \n");
		return -1;
	}

	indx = dspInt % 4;
	num = dspInt / 4 - 1;

	tmp = readl(COREPAC_INT_MUX_REG(num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));
	tmp &= ~(0xff << (indx * 8));
	tmp |= (efnum << (indx * 8));
	writel(tmp, COREPAC_INT_MUX_REG(num));
	DEBUG_DEV("map %d to dsp %d, in register 0x%08x\n",efnum, dspInt, COREPAC_INT_MUX_REG(num));
	DEBUG_DEV("COREPAC_INT_MUX_REG(%d) is now 0x%08x\n",num, readl(COREPAC_INT_MUX_REG(num)));
	return (int)last;
}

/*
 * @brief
 *    MappedEventGet：内部使用函数，返回给定DSP中断映射的事件号。
 * @param		dspInt：    DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	给定DSP中断所对应的事件号。
 */
static int  MappedEventGet(int dspInt)
{
	int indx;
	int num;
	unsigned long tmp;
	unsigned long last;

	if (dspInt < 4 || dspInt > 15) {
		DEBUG_DEV("bad dsp_int\n");
		return -1;
	}

	indx = dspInt % 4;
	num = dspInt / 4 - 1;

	tmp = readl(COREPAC_INT_MUX_REG(num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));
	return (int)last;
}


/*
 * @brief
 *    MappedDspIntsGet：内部使用函数，返回所有DSP中断所对应的事件号。
 * @param		array：    一个数组，该数组用于保存输出。
 * @param[in]	无。
 * @param[out]	array:	一个数组，该数组用于保存输出。
 * @returns:	无。
 */
static void MappedDspIntsGet(int *array)
{
	int i;

	for (i = 4;i < 16; i++)
		array[i-4] = MappedEventGet(i);

}

/* C66XX的Corepac控制结构体 */
static struct CorepacIntFuncSets CorepacOperations = {
		.corepac_get_event_flag = GetEventFlag,
		.corepac_set_event_flag = SetEventFlag,
		.corepac_clr_event_flag = ClrEventFlag,
		.corepac_mask_combine_event = MaskCombineEvent,
		.corepac_unmask_combine_event = UnmaskCombineEvent,
		.corepac_map_dsp_int = MapDspInt,
		.corepac_mapped_dsp_ints_get = MappedDspIntsGet,
		.corepac_mapped_event_get = MappedEventGet,
		.corepac_mask_combine_read_event = MaskCombineReadEvent,
};

/* 每个CPU都有的Corepac中断控制器的实体 */
static struct CorepacIntDev PerCpuCorepacIntDevices[MAX_CORE_NUM];
static struct Corepac PerCpuCorepac[MAX_CORE_NUM];


/*
 * @brief
 *    BSP_GetCurCorepacIntc：返回执行该函数对应核的Corepac中断控制器的实例。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	struct CorepacIntDev *：执行该函数对应核的Corepac中断控制器的实例。
 */
struct CorepacIntDev *BSP_GetCurCorepacIntc(void)
{
	int id = BSP_GetCoreId();
	return &PerCpuCorepacIntDevices[id];
}

/*
 * @brief
 *    BSP_GetCurCorepac：返回执行该函数的Corepac对象指针。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	struct Corepac *：执行该函数对应核的Corepac指针。
 */
struct Corepac *BSP_GetCurCorepac(void)
{
	int id = BSP_GetCoreId();
	return &PerCpuCorepac[id];
}

/*
 * @brief
 *    BSP_InitCurCorepac：软件初始化执行该函数对应的Corepac中断控制器的实例。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void	BSP_InitCurCorepac(void)
{
	int i = BSP_GetCoreId();
	PerCpuCorepacIntDevices[i].core_id = i;
	PerCpuCorepacIntDevices[i].pFunc_sets = &CorepacOperations;
	PerCpuCorepac[i].core_id = i;
	PerCpuCorepac[i].corepac_int_device = &PerCpuCorepacIntDevices[i];
	DEBUG_DEV("found corepac %d \n",i);
}

/*
 * @brief
 *    BSP_GetCurEvent：返回给定DSP中断号对应的当前Corepac中断控制器映射的中断事件号。
 * @param		dspInt：给定DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	给定DSP中断当前映射的中断事件号。
 */
int BSP_GetCurEvent(int dspInt)
{
	struct CorepacIntDev *pCur_corepac_int_dev = BSP_GetCurCorepacIntc();
	if (pCur_corepac_int_dev->pFunc_sets == NULL) {
		DEBUG_DEV("current corepac's operations set is NULL!\n");
		return -1;
	}

	int efnum = pCur_corepac_int_dev->pFunc_sets->corepac_mapped_event_get(dspInt);
	return efnum;

}

/*
 * @brief
 *    FindFirstBitZero32：内部函数，该函数用于返回一个32位数的前导零。
 * @param		tmp：给定32位数目。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	-1：该32位数每位全为0。
 * <p>			其他：第一个1位所在的位数。
 */
static int FindFirstBitZero32(unsigned long tmp)
{
	int i;
	for (i = 0;i < 32;i++) {
		if (tmp & (1 << i)) {
			return i;
		} else
			continue;
	}
	return -1;

}



/*
 * @brief
 *    BSP_GetCurCombineState：返回第一个发生的组合中断的中断号。
 * @param		efnum：哪一个组合中断的中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	-1：该组合中断没有中断发生或参数错误。
 * <p>			其他：发生的第一个组合中断的中断号。
 */
int	 BSP_GetCurCombineState(int efnum)
{
	int num;
	unsigned long tmp;
	int kind;
	struct CorepacIntDev *pCur_corepac_int_dev = BSP_GetCurCorepacIntc();

	if (efnum > 3) {
		DEBUG_DEV("only event0-3 can be combined by other events\n");
		return -1;
	}

	if (pCur_corepac_int_dev->pFunc_sets == NULL) {
		DEBUG_DEV("current corepac's operations set is NULL!\n");
		return -1;
	}

	num = efnum / 32;

	tmp = pCur_corepac_int_dev->pFunc_sets->corepac_mask_combine_read_event(efnum);

	kind = FindFirstBitZero32(tmp);

	if (kind == -1)
		return -1;
	else
		return ((32 * num) + kind);

}

/*
 * @brief
 *    BSP_GetCurCombineAllState：返回给定组合中断中发生的所有中断情况。
 * @param		efnum：哪一个组合中断的中断号。
 * 				array：输出参数，用于表示该组合中断中发生的所有中断情况。
 * @param[in]	无。
 * @param[out]	array：输出参数，用于表示该组合中断中发生的所有中断情况。
 * @returns:	-1：该组合中断没有中断发生或参数错误。
 * <p>			0：函数正常返回。
 */
int	BSP_GetCurCombineAllState(int efnum, int *array)
{
	int num;
	unsigned long tmp;
	int i;
	struct CorepacIntDev *pCur_corepac_int_dev = BSP_GetCurCorepacIntc();

	if (efnum > 3) {
		DEBUG_DEV("only event0-3 can be combined by other events\n");
		return -1;
	}

	if (pCur_corepac_int_dev->pFunc_sets == NULL) {
		DEBUG_DEV("current corepac's operations set is NULL!\n");
		return -1;
	}

	num = efnum / 32;

	tmp = pCur_corepac_int_dev->pFunc_sets->corepac_mask_combine_read_event(efnum);

	for (i = 0;i < 32;i ++) {
		if (tmp & (1 << i))
			array[i] = i + num * 32;
		else
			array[i] = -1;
	}

	return 0;
}


