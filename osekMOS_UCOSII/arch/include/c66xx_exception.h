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
 * @file 	c66xx_exception.h
 * @brief
 *	<li>功能：c66xx的异常操作头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */
#ifndef _C66XX_EXCEPTION_H
#define _C66XX_EXCEPTION_H

/**************************** 引用部分 *********************************/

#include "osprop.h"
#include "osapi.h"
#include "stdTypes.h"

//每核心中断类型配置表
extern OSBYTE	osekConfig_InterruptTypeTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM];
//每核心中断入口函数配置列表
extern T_OSEK_TASK_Entry	osekConfig_InterruptEntryTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM];
extern OSDWORD osekInterrupt_IsrStackTop;

/**************************** 声明部分 *********************************/

void BSP_EnableDspInterrupt(unsigned int dspInt);
void BSP_DisableDspInterrupt(unsigned int);
int BSP_AckDspInterrupt(unsigned int);
void BSP_SetDspInterrupt(unsigned int);
void BSP_ClearDspInterrupt(unsigned int);
void BSP_IsrHandlerEntry(unsigned int);
void BSP_ClearDspInterruptAll(void);
int BSP_GetExecutionState(void);


/**************************** 定义部分 *********************************/
#define INT_TO_PRI(vecNum) (16 - vecNum)
#define PRI_TO_INT(pri)    (16 - pri)


#endif /* _C66XX_EXCEPTION_H */
