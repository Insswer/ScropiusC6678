/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#include "osapi.h"
// 全局变量


// ISR2类中断堆栈空间
static OSBYTE isr2Stack[CONFIG_OSEK_KERNEL_CORE_NUMS][CONFIG_OSEK_INTERRUPT_STACK_SIZE];


/**************************** 实现部分 *********************************/

/*
 * @brief
 *    osekInterrupt_Initialize：中断初始化（内部函数）。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void osekInterrupt_Initialize(void)
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	//为核心设置中断栈地址。isr2Stack[core]是一个指针
	osekKernel_osKernel[core].osekInterrupt_IsrStackTop = (OSDWORD)&isr2Stack[core][0] + (OSDWORD)(CONFIG_OSEK_INTERRUPT_STACK_SIZE - 0x20);
}


/*
 * @brief
 *    DisableAllInterrupts：禁止所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void DisableAllInterrupts( void )
{
	//获取核心ID
	CoreIDType core = GetCoreID();

	OSEK_TARGET_DisableAllInt(core);
}


/*
 * @brief
 *    EnableAllInterrupts：使能所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void EnableAllInterrupts( void )
{
	//获取核心ID
	CoreIDType core = GetCoreID();

	OSEK_TARGET_EnableAllInt(core);
}


/*
 * @brief
 *    SuspendAllInterrupts：挂起所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void SuspendAllInterrupts( void )
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	if( osekKernel_osKernel[core].osekInterrupt_NestedAllInt == 0 )
	{
		OSEK_TARGET_DisableNestedAllInt(core);
	}

	osekKernel_osKernel[core].osekInterrupt_NestedAllInt++;
}


/*
 * @brief
 *    ResumeAllInterrupts：恢复所有中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void ResumeAllInterrupts( void )
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	// 如果禁止中断层数为0，直接返回
	if( osekKernel_osKernel[core].osekInterrupt_NestedAllInt == 0)
	{
		return;
	}

	// 禁止中断层数减1，减到0时使能所有中断
	osekKernel_osKernel[core].osekInterrupt_NestedAllInt--;
	if( osekKernel_osKernel[core].osekInterrupt_NestedAllInt == 0 )
	{
		OSEK_TARGET_EnableNestedAllInt(core);
	}
}


/*
 * @brief
 *    SuspendOSInterrupts：挂起OS相关的中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void SuspendOSInterrupts( void )
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	if( osekKernel_osKernel[core].osekInterrupt_NestedOsInt == 0 )
	{
		OSEK_TARGET_DisableNestedOsInt(core);
	}

	osekKernel_osKernel[core].osekInterrupt_NestedOsInt++;
}


/*
 * @brief
 *    ResumeOSInterrupts：恢复OS相关的中断。
 * @param
 * @param[in]	无。
 * @param[out]  无。
 * @returns:    无。
 */
void ResumeOSInterrupts( void )
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	// 如果禁止中断层数为0，直接返回
	if( osekKernel_osKernel[core].osekInterrupt_NestedOsInt == 0 )
	{
		return;
	}

	// 禁止中断层数减1，减到0时使能OS相关的中断
	osekKernel_osKernel[core].osekInterrupt_NestedOsInt--;

	if( osekKernel_osKernel[core].osekInterrupt_NestedOsInt == 0 )
	{
		OSEK_TARGET_EnableNestedOsInt(core);
	}
}
