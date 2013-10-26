/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/



#ifndef OCC_OSPROP_H
#define OCC_OSPROP_H

/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/
#define DEV_DEBUG

/**************************** 定义部分 *********************************/
#define CONFIG_OSEK_OCC


/* 操作系统对象宏定义 */
//目标板类型定义
#define		OCC_TMS320C6713

//系统核心数
#define 	CONFIG_OSEK_KERNEL_CORE_NUMS		4
//系统状态定义
#define		OCC_EXTSTATUS

//符合类定义
#define		OCC_ECC1

//调试级别定义
#define		OCC_ORTIDEBUGLEVEL

//RESSCHEDULER定义
#define		OCC_RESSCHEDULER

//STKCHECK定义
#define		OCC_STKCHECK

//HOOKSTARTUP定义
#define		OCC_HOOKSTARTUP

//HOOKSHUTDOWN定义
#define		OCC_HOOKSHUTDOWN

//HOOKPRETASK定义
#define		OCC_HOOKPRETASK

//HOOKPOSTTASK定义
#define		OCC_HOOKPOSTTASK

//HOOKERROR定义
#define		OCC_HOOKERROR

//GETSERVICEID定义
#define		OCC_GETSERVICEID

//PARAMETERACCESS定义
#define		OCC_PARAMETERACCESS

//HOOKIDLE定义
#define		OCC_HOOKIDLE

//FLOATINGPOINT定义
#define		OCC_FLOATINGPOINT

//资源对象定义
#define		OCC_RESOURCE

//中断总数
#define		OCC_ISRLEVELNUM		16//47

//中断栈大小定义
#define		OCC_ISRSTACKSIZE		4096



/* 应用模式对象宏定义 */
//APPMODE对象总数
#define		OCC_NAPPMODES		1



/* 中断对象宏定义 */
//中断类型定义
#define		OCC_ISR2



/* 任务对象宏定义 */
//任务调度策略
#define		OCC_FULLPREEMPT

//任务优先级定义
#define		OCC_EXTPRIORS

//全局任务数目,不包含各个核心上的IDLE任务
#define		OCC_NTSKS		10

//就绪任务队列最大数
#define		OCC_TOTALACTIVENUMBER		8



/* 计数器对象宏定义 */
//COUNTER对象总数宏定义
#define		OCC_NCTRS		1
#define		OCC_COUNTER



/* 报警对象宏定义 */
//ALARM对象总数
#define		OCC_NALMS		4
#define		OCC_ALARM
#define		OCC_ALMSETEVENT


//报警机制的宏定义
#define		OCC_ALMACTIVTASK



#endif/*	ifndef OCC_OSPROP_H	*/



