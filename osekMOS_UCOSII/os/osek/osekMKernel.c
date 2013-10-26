/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#include "osapi.h"

/*引入声明*/
extern T_OSEK_KERNEL_TaskControl osekKernel_coreTasks[CONFIG_OSEK_KERNEL_CORE_NUMS];
extern T_OSEK_KERNEL_CounterControl osekKernel_coreCounters[CONFIG_OSEK_KERNEL_CORE_NUMS];
extern T_OSEK_KERNEL_AlarmControl osekKernel_coreAlarms[CONFIG_OSEK_KERNEL_CORE_NUMS];

//全局核心数据数组
T_OSEK_KERNEL_OsKernel osekKernel_osKernel[CONFIG_OSEK_KERNEL_CORE_NUMS];

/* 核心初始化函数*/
void osekKernel_Initialize()
{//初始化各个核心的数据结构,可以由一个核心(如Core0)调用然后初始化全部数据,或者由每个核心自己负责核心数据的初始化

	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];

	//初始化主要模块的核心数据结构指针
	kernelPtr->osTasks = &(osekKernel_coreTasks[core]);
	kernelPtr->osCounters = &(osekKernel_coreCounters[core]);
	kernelPtr->osAlarms = &(osekKernel_coreAlarms[core]);

	//中断嵌套初始化
	kernelPtr->osekInterrupt_NestedLevl = 0;
	kernelPtr->osekInterrupt_NestedAllInt = 0;
	kernelPtr->osekInterrupt_NestedAllInt = 0;
	kernelPtr->osekInterrupt_NestedOsInt = 0;
	kernelPtr->osekInterrupt_IsrStackTop = 0;

	//Target初始化
	kernelPtr->osekTarget_OSIntMask = 0 ;
	kernelPtr->osekTarget_AllIntMask = 0 ;
	kernelPtr->osekTarget_NestedAllIntMask = 0 ;
	kernelPtr->osekTarget_NestedOsIntMask = 0 ;

	kernelPtr->osekTarget_SavedBTSP = 0 ;

	//Resource初始化

	//自旋锁初始化
}
