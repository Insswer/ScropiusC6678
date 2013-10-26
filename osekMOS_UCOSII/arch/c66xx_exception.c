/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_exception,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_exception.c
 * @brief
 *	<li>功能：定义了C66XX中断异常处理模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

/**************************** 引用部分 *********************************/
#include <stdio.h>
#include "c66xx_exception.h"
#include "c66xx_itbl.h"
#include "c66xx_debug.h"
#include "c66xx_io.h"
#include "osprop.h"
#include "osapi.h"
#include "cfg.h"

/**************************** 声明部分 *********************************/

//每核心中断类型配置表
extern OSBYTE	osekConfig_InterruptTypeTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM];
//每核心中断入口函数配置列表
extern T_OSEK_TASK_Entry	osekConfig_InterruptEntryTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM];
extern T_OSEK_KERNEL_OsKernel osekKernel_osKernel[];
//每核心的中断栈位置
static void* stackBackup[CONFIG_OSEK_KERNEL_CORE_NUMS];


/**************************** 定义部分 *********************************/
/*
 * @brief
 *    BSP_EnableDspInterrupt：使能给定的DSP中断号所对应的DSP中断。
 * @param		dspInt：给定DSP中断号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_EnableDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}

	tmp = BSP_ReadIer();
	tmp |= (1 << dspInt);
	BSP_WriteIer(tmp);
}


/*
 * @brief
 *    BSP_DisableDspInterrupt：屏蔽给定的DSP中断号所对应的DSP中断。
 * @param		dspInt：给定DSP中断号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_DisableDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}

	tmp = BSP_ReadIer();
	tmp &= ~(1 << dspInt);
	BSP_WriteIer(tmp);
}

/*
 * @brief
 *    BSP_AckDspInterrupt：应答一个DSP中断。
 * @param		dspInt：给定DSP中断号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
int BSP_AckDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return -1;
	}

	tmp = BSP_ReadIfr();
	if (tmp & (1 << dspInt)) {
		tmp = 0;
		tmp |= (1 << dspInt);
		BSP_WriteIcr(tmp);
	}else {
		DEBUG_DEV("dsp_int%d is not exist\n",dspInt);
		return -1;
	}
	return 0;
}

/*
 * @brief
 *    BSP_SetDspInterrupt：人工主动触发一个DSP中断，该函数主要用于DEBUG中断控制及相关模块。
 * @param		dspInt：给定DSP中断号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_SetDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}
	tmp = 0;
	tmp |= (1 << dspInt);
	BSP_WriteIsr(tmp);
}

/*
 * @brief
 *    BSP_ClearDspInterrupt：清掉一个给定DSP中断。
 * @param		dspInt：给定DSP中断号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_ClearDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}
	tmp = 0;
	tmp |= (1 << dspInt);
	BSP_WriteIcr(tmp);
}

/*
 * @brief
 *    BSP_ClearDspInterruptAll：清掉所有DSP中断（无视是否该中断已经发生）。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_ClearDspInterruptAll(void)
{
	unsigned long tmp;
	tmp = 0xfff0;
	BSP_WriteIcr(tmp);
}

/*
 * @brief
 *    BSP_GetExecutionState：清掉所有DSP中断（无视是否该中断已经发生）。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	int, 0 用户模式, 1特权模式。
 */
int BSP_GetExecutionState(void)
{
	unsigned long tmp = BSP_ReadTSR();
	if (tmp & (1 << 6))
		return 0;
	else
		return 1;
}
/*
 * @brief
 *    BSP_IsrHandlerEntry：内部使用函数，根据中断号进行相关中断处理。
 * @param		vectorNum：发生的DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static RawIsrHandlerEntry(unsigned int vectorNum)
{
	int core = BSP_GetCoreId();
    if (1 == osekConfig_InterruptTypeTable[core][vectorNum])
    {
        if (osekConfig_InterruptEntryTable[core][vectorNum])
        {
        	osekConfig_InterruptEntryTable[core][vectorNum]();
        }
    }
    else
    {
    	printf("warnning...\n");
    	if (0 == osekKernel_osKernel[core].osekInterrupt_NestedLevl)  /* switch stack */
    	{
    	    stackBackup[core] = BSP_GetCurSp();
    	    osekTarget_LoadBTSP_do((void *)osekKernel_osKernel[core].osekInterrupt_IsrStackTop);
    	}

    	if (osekConfig_InterruptEntryTable[vectorNum])
    	{
    		osekKernel_osKernel[core].osekInterrupt_NestedLevl++;
    	    BSP_EnableInt();
    	    osekConfig_InterruptEntryTable[core][vectorNum]();
    	    BSP_DisableInt();
    	    osekKernel_osKernel[core].osekInterrupt_NestedLevl--;
    	}

    	if (0 == osekKernel_osKernel[core].osekInterrupt_NestedLevl)
    	{
    	    osekTarget_LoadBTSP_do((void *)stackBackup[core]);  /* resume stack */
    	}
    }


    /* Do dispatch */
    if (1 == osekKernel_osKernel[core].osekTask_IsrTriggeredTaskSwitchNecessary)
    {
    	/* clear dispatch flag */
    	osekKernel_osKernel[core].osekTask_IsrTriggeredTaskSwitchNecessary = 0;
    	/* call dispatch */
    	 osekTask_Dispatch();
    }
}

void SysTick_Handler(void);

void BSP_OtherOSIsrHandlerEntry(int corenum, unsigned long vectorNum)
{

	if (corenum == 0) {
		if (14 == vectorNum) {
			SysTick_Handler();
		}
	}
}

/*
 * @brief
 *    BSP_IsrHandlerEntry：中断处理总入口函数。
 * @param		vectorNum：发生的DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_IsrHandlerEntry(unsigned int vectorNum)
{
	int core = BSP_GetCoreId();
//	if (UC_CORE != core) {
//		BSP_ClearDspInterrupt(vectorNum);
//		BSP_DisableDspInterrupt(vectorNum);
//		RawIsrHandlerEntry(vectorNum);
//		BSP_EnableDspInterrupt(vectorNum);
//	} else {
		BSP_ClearDspInterrupt(vectorNum);
		BSP_OtherOSIsrHandlerEntry(core, vectorNum);
//	}
}
