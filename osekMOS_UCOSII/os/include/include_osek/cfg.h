/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OCC_CFG_H
#define OCC_CFG_H

/**************************** 引用部分 *********************************/
#include "stdTypes.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/



/* 应用模式对象定义 */
//APPMODE对象序列号的定义
#define		AppMode		0


/* 中断对象定义 */
//中断服务程序入口函数声明
extern		void		BSP_TimeInterruptISR(void);
//任务入口函数引用
/*core 0 */
extern	void	FuncStartTask(void);
extern	void	FuncIntActivateTask(void);
extern	void	FuncTenmsTask(void);
/*core 1 */
extern	void	FuncOS_5ms_task(void);
extern  void	FuncEventTask(void);


extern	void	FuncJobStatusTask(void);
extern	void	FuncFuelMassCalTask(void);
extern	void	FuncJ1939Task(void);



/********************************************Global Config********************************************/

#define CORE0
#define CORE1
#define	CORE2


#define OCC_ISRLEVELNUM				16

/* Alarm Num Config */
#define CONFIG_ALARM_NUMBER			6

#define CONFIG_COUNTER_NUMBER		4
#define CORE0_COUNTER_NUM			2
#define CORE1_COUNTER_NUM			1
#define CORE2_COUNTER_NUM			1
#define CORE3_COUNTER_NUM			1

#define SysCounter 0

/*global task id define */
#define Task1		0
#define Task2		1
#define Task3		2
#define Task4		3
#define Task5		4
#define Task6		5
#define Task7		6
#define Task8		7
#define Task9		8
#define TaskA		9

//global alarm id define
#define Alarm1	0
#define Alarm2	1
#define Alarm3	2
#define Alarm4	3
#define Alarm5	4
#define Alarm6	5

//local task id define
/**** core 0 ****/
/*Task define */
/*CORE0_TASK_NUM include the idle task on the core */
#define CORE0_TASK_NUM		4
#define C0_Task1			0
#define C0_Task2			1
#define C0_Task3			2
/*Alarm define */
#define CORE0_ALM_NUM		2
#define C0_Alarm1			0
#define C0_Alarm2			1

/*core 1 */
/*Task define */
/*CORE1_TASK_NUM include the idle task on the core */
#define CORE1_TASK_NUM		3
#define C1_Task1			0
#define C1_Task2			1
/*Alarm define */
#define CORE1_ALM_NUM		2
#define C1_Alarm1			0
#define C1_Alarm2			1

/*core 2 */
/*Task define */
/*CORE2_TASK_NUM include the idle task on the core */
#define CORE2_TASK_NUM		3
#define C2_Task1			0
#define C2_Task2			0

/*core 3 */
/*Task define */
/*CORE3_TASK_NUM include the idle task on the core */
#define CORE3_TASK_NUM		4
#define C3_Task1			0
#define C3_Task2			1
#define C3_Task3			2
/*Alarm define */
#define CORE3_ALM_NUM		2
#define C3_Alarm1			0
#define C3_Alarm2			1


//自旋锁ID
#define OSEK_CONFIG_SPINLOCK_NUM	 19
//系统自旋锁ID
#define	SPIN_LOCK_OS_SETUP	0
#define	SPIN_LOCK_COUNTER   1

#define	IPC_MESSAGE_BOX_LOCK_BASE 	(2)
#define CORE_BOX_LOCK(i)			(IPC_MESSAGE_BOX_LOCK_BASE + i)

#define RPC_LIST_LOCK_BASE			(10)
#define RPC_LIST_LOCK(i)			(RPC_LIST_LOCK_BASE + i)


//RPC使用的数据包的可支配数目
#define IPC_DATA_NUMS  10

//自旋锁数目及保留的自旋锁数目
#define CONFIG_OSEK_SPINLOCK_NUMS		8
#define CONFIG_OSEK_SPINLOCK_RESERVERD	10

//Task全局映射表
extern OsTaskMapType	osekTask_tasksMap[CONFIG_OSEK_TASK_NUMBER];
//Alarm全局映射表
extern OsAlarmMapType	osekAlarm_alarmsMap[CONFIG_ALARM_NUMBER];
extern T_OSEK_SPINLOCK_Lock osekSpin_LockTable[CONFIG_OSEK_SPINLOCK_NUMS];
//每核心中断类型配置表
extern OSBYTE	osekConfig_InterruptTypeTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM];
//每核心中断入口函数配置列表
extern T_OSEK_TASK_Entry	osekConfig_InterruptEntryTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM];

//每核心任务配置表
extern T_OSEK_KERNEL_TaskControl osekKernel_coreTasks[CONFIG_OSEK_KERNEL_CORE_NUMS];
//每核心Counter配置表
extern T_OSEK_KERNEL_CounterControl osekKernel_coreCounters[CONFIG_OSEK_KERNEL_CORE_NUMS];
//每核心Alarm配置表
extern T_OSEK_KERNEL_AlarmControl osekKernel_coreAlarms[CONFIG_OSEK_KERNEL_CORE_NUMS];

#endif/*	ifndef OCC_CFG_H	*/

