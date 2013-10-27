
/**************************** 引用部分 *********************************/
#include "c66xx_cic.h"
#include "c66xx_debug.h"
#include "c66xx_init.h"
#include "c66xx_io.h"
#include "string.h"
/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/
/* 每一个channel一个的CIC结构体，该结构体文件内部使用  */
static struct cic_device cic_chips[CIC_CHIPS_NUM];


/*
 * @brief
 *    BSP_GetCicChip：指定channel返回相关的CIC控制器。
 * @param		channel：channel号。
 * 			    struct cic_device：CIC结构体。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    无。
 */
struct cic_device	*BSP_GetCicChip(unsigned int channel)
{
	if (channel > 4)
		return NULL;
	else
		return &cic_chips[channel];
}



/*
 * @brief
 *    CicInit：内部使用的初始化函数，当前没有意义
 * @param		id：channel号。
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    无。
 */
void CicInit(int id, struct cic_device *cic_dev)
{
	/* 本函数暂无用处 */
	DEBUG_DEV("Information about CIC%d\n",id);
	DEBUG_DEV("Global interrupt: %d\n",cic_dev->cic_global_interrupt_is_enable(cic_dev));
}


/*
 * @brief
 *    GlobalEnableInterrupt：内部使用的函数，该函数用于启用CIC的全局外部中断。
 * @param		struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    无。
 */
static void GlobalEnableInterrupt(struct cic_device *cic_dev)
{
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	tmp = readl(CIC_GLOBAL_ENABLE_HINT_REG(cic_dev->cic_id));
	/* 第0位控制全局外部中断 */
	tmp &= ~(1 << 0);
	tmp |= (1 << 0);
	writel(tmp, CIC_GLOBAL_ENABLE_HINT_REG(cic_dev->cic_id));
}


/*
 * @brief
 *    GlobalDisableInterrupt：内部使用的函数，该函数用于关闭CIC的全局外部中断。
 * @param		struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    无。
 */
static void GlobalDisableInterrupt(struct cic_device *cic_dev)
{
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	tmp = readl(CIC_GLOBAL_ENABLE_HINT_REG(cic_dev->cic_id));
	tmp &= ~(1 << 0);
	writel(tmp, CIC_GLOBAL_ENABLE_HINT_REG(cic_dev->cic_id));
}


/*
 * @brief
 *    GlobalInterruptIsEnable：内部使用的函数，该函数判断给定CIC控制器的全局外部中断是否有效。
 * @param		struct cic_device*：CIC控制器结构体
 * @param[in]	struct cic_device*：CIC控制器结构体
 * @param[out]	无。
 * @returns:    1，有效。
 * <p>			0，无效。
 */
static int GlobalInterruptIsEnable(struct cic_device *cic_dev)
{
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return -1;
	}
	tmp = readl(CIC_GLOBAL_ENABLE_HINT_REG(cic_dev->cic_id));

	if (tmp & 0x1)
		return 1;
	else
		return 0;
}

/*
 * @brief
 *    SysInterruptStatusSet：内部使用的函数，该函数用于人工设置系统中断的状态。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void SysInterruptStatusSet(int sysIntNum, struct cic_device *cic_dev)
{
	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	sysIntNum &= 0x3ff;
	writel(sysIntNum, CIC_STATUS_SET_INDX_REG(cic_dev->cic_id));
}


/*
 * @brief
 *    SysInterruptStatusClear：内部使用的函数，该函数用于人工清除系统中断的状态。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void SysInterruptStatusClear(int sysIntNum, struct cic_device *cic_dev)
{
	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	sysIntNum &= 0x3ff;
	writel(sysIntNum, CIC_STATUS_CLR_INDX_REG(cic_dev->cic_id));
}


/*
 * @brief
 *    SysInterruptEnableStatusGet：内部使用的函数，该函数用于获得系统中断的状态。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	1，系统中断使能。
 * <p>			0，系统中断被屏蔽。
 */
static int	SysInterruptEnableStatusGet(int sysIntNum, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long index;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return -1;
	}

	if (sysIntNum >= 160||sysIntNum < 0) {
		DEBUG_DEV("bad sysintnum\n");
		return -1;
	}

	index = sysIntNum % 32;
	num = sysIntNum / 32;


	tmp = readl(CIC_ENABLE_REG(cic_dev->cic_id, num));

	if (tmp & (1 << index))
		return 1;
	else
		return 0;
}


/*
 * @brief
 *    SysInterruptRawStatusSet：内部使用的函数，该函数用于人工设置系统中断的裸（raw）状态。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void	 SysInterruptRawStatusSet(int sysIntNum, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	if (sysIntNum >= 160||sysIntNum < 0) {
		DEBUG_DEV("bad sysintnum\n");
		return;
	}

	num = sysIntNum / 32;
	indx = sysIntNum % 32;

	tmp = readl(CIC_RAW_STATUS_REG(cic_dev->cic_id, num));
	tmp &= ~(1 << indx);
	tmp |= (1 << indx);
	writel(tmp, CIC_RAW_STATUS_REG(cic_dev->cic_id, num));
}


/*
 * @brief
 *    SysInterruptRawStatusGet：内部使用的函数，该函数用于获得系统中断的裸（raw）状态。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	1，中断触发。
 * <p>			0，中断未触发。
 */
static int	SysInterruptRawStatusGet(int sysIntNum, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long index;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return -1;
	}

	if (sysIntNum >= 160||sysIntNum < 0) {
		DEBUG_DEV("bad sysintnum\n");
		return -1;
	}

	num = sysIntNum / 32;
	index = sysIntNum % 32;

	tmp = readl(CIC_RAW_STATUS_REG(cic_dev->cic_id, num));
	if (tmp & (1 << index))
		return 1;
	else
		return 0;
}

/*
 * @brief
 *    SysInterruptEnable：内部使用的函数，该函数用于使能给定系统中断。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void SysInterruptEnable(int sysIntNum, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	if (sysIntNum >= 160||sysIntNum < 0) {
		DEBUG_DEV("bad sysintnum\n");
		return;
	}

	num = sysIntNum / 32;
	indx = sysIntNum % 32;

	tmp = readl(CIC_ENABLE_REG(cic_dev->cic_id, num));
	tmp &= (1 << indx);
	tmp |= (1 << indx);
	writel(tmp, CIC_ENABLE_REG(cic_dev->cic_id, num));
}

/*
 * @brief
 *    SysInterruptDisable：内部使用的函数，该函数用于屏蔽给定系统中断。
 * @param		sysIntNum：系统中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void SysInterruptDisable(int sysIntNum, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	if (sysIntNum >= 160||sysIntNum < 0) {
		DEBUG_DEV("bad sysIntNum\n");
		return;
	}

	num = sysIntNum / 32;
	indx = sysIntNum % 32;

	tmp = readl(CIC_ENABLE_CLR_REG(cic_dev->cic_id, num));
	tmp &= (1 << indx);
	tmp |= (1 << indx);
	writel(tmp, CIC_ENABLE_CLR_REG(cic_dev->cic_id, num));
}

/*
 * @brief
 *    SysInterruptChannelMap：内部使用的函数，该函数用于将一个系统中断映射到一个指定CIC控制器的Channel上。
 * @param		sysInt：系统中断号
 * 				channelToMap：CIC控制器的某个Channel
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 	-1：映射失败。
 * <p>			 其他：上一次被映射到给定Channel的系统中断。
 */
static int	SysInterruptChannelMap(int sysInt,int channelToMap, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;
	int last;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return -1;
	}

	if (sysInt >= 160||sysInt < 0) {
		DEBUG_DEV("bad sysintnum\n");
		return -1;
	}

	if (channelToMap >= 160||channelToMap < 0) {
		DEBUG_DEV("bad channel\n");
		return -1;
	}


	num = channelToMap / 4;
	indx = channelToMap % 4;

	tmp = readl(CIC_CH_MAP_REG(cic_dev->cic_id, num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));
	tmp &= ~(0xf << (indx * 8));
	tmp |= (sysInt << (indx * 8));
	writel(tmp, CIC_CH_MAP_REG(cic_dev->cic_id, num));

	return last;

}

/*
 * @brief
 *    SysInterruptChannelGet：内部使用的函数，该函数用于获得一个给定channel上的系统中断号。
 * @param		channel：给定Channel
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 	-1：参数错误。
 * <p>		 	其他：映射到给定Channel的系统中断。
 */
static int	SysInterruptChannelGet(int channel, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;
	int last;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return -1;
	}

	if (channel >= 160||channel < 0) {
		DEBUG_DEV("bad channel\n");
		return -1;
	}

	num = channel / 4;
	indx = channel % 4;

	tmp = readl(CIC_CH_MAP_REG(cic_dev->cic_id, num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));

	return last;
}

/*
 * @brief
 *    HostInterruptMapGet：内部使用的函数，该函数用于获得一个给定channel上的主机（host）中断号。
 * @param		channelToMap：给定Channel
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns: -1：参数错误。其他：映射到给定Channel的系统中断。
 */
static int	HostInterruptMapGet(int channelToMap, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;
	int last;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return -1;
	}

	num = channelToMap / 4;
	indx = channelToMap % 4;
	tmp = readl(CIC_HINT_MAP_REG(cic_dev->cic_id, num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));

	return last;
}


/*
 * @brief
 *    HostInterruptEnable：内部使用的函数，该函数用于使能一个主机中断。
 * @param		hostInt：给定主机中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void HostInterruptEnable(int hostInt, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	num = hostInt / 4;
	indx = hostInt % 4;

	tmp = readl(CIC_ENABLE_HINT_REG(cic_dev->cic_id, num));
	tmp &= ~(1 << indx);
	tmp |= (1 << indx);
	writel(tmp, CIC_ENABLE_HINT_REG(cic_dev->cic_id, num));
}

/*
 * @brief
 *    HostInterruptDisable：内部使用的函数，该函数用于屏蔽一个主机中断。
 * @param		hostInt：给定主机中断号
 * 				struct cic_device*：CIC控制器结构体
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void	 HostInterruptDisable(int hostInt, struct cic_device *cic_dev)
{
	unsigned long num;
	unsigned long indx;
	unsigned long tmp;

	if (cic_dev == NULL) {
		DEBUG_DEV("bad cic_dev\n");
		return;
	}

	num = hostInt / 4;
	indx = hostInt % 4;

	tmp = readl(CIC_ENABLE_HINT_REG(cic_dev->cic_id, num));
	tmp &= ~(1 << indx);
	writel(tmp, CIC_ENABLE_HINT_REG(cic_dev->cic_id, num));
}


/*
 * @brief
 *    BSP_InitCic：初始化CIC控制器，该函数应该在体系初始化的时候被调用。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void  BSP_InitCic(void)
{
	int i;
	memset(cic_chips, 0, sizeof(cic_chips));
	for (i = 0;i < CIC_CHIPS_NUM; i++) {
		if (cic_chips[i].is_ready)
			continue;
		cic_chips[i].is_ready = 1;
		cic_chips[i].cic_id = i;
		cic_chips[i].cic_init = CicInit;
		cic_chips[i].cic_global_enable_interrupt = GlobalEnableInterrupt;
		cic_chips[i].cic_global_disable_interrupt = GlobalDisableInterrupt;
		cic_chips[i].cic_host_interrupt_disable = HostInterruptDisable;
		cic_chips[i].cic_host_interrupt_enable = HostInterruptEnable;
		cic_chips[i].cic_sys_interrupt_channel_map = SysInterruptChannelMap;
		cic_chips[i].cic_host_interrupt_map_get = HostInterruptMapGet;
		cic_chips[i].cic_sys_interrupt_disable = SysInterruptDisable;
		cic_chips[i].cic_sys_interrupt_enable = SysInterruptEnable;
		cic_chips[i].cic_sys_interrupt_enable_status_get = SysInterruptEnableStatusGet;
		cic_chips[i].cic_sys_interrupt_raw_status_get = SysInterruptRawStatusGet;
		cic_chips[i].cic_sys_interrupt_raw_status_set = SysInterruptRawStatusSet;
		cic_chips[i].cic_sys_interrupt_status_clear = SysInterruptStatusClear;
		cic_chips[i].cic_sys_interrupt_status_set = SysInterruptStatusSet;
		cic_chips[i].cic_sys_interrupt_channel_get = SysInterruptChannelGet;
		cic_chips[i].cic_global_interrupt_is_enable = GlobalInterruptIsEnable;
		cic_chips[i].cic_init(i, &cic_chips[i]);
	}
}







