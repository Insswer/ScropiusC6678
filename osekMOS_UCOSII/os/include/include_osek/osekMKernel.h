/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMKERNEL_H_
#define OSEKMKERNEL_H_

#include "stdTypes.h"
#include "osekOccConfig.h"



/*核心上任务控制列表结构体*/
struct T_OSEK_KERNEL_TaskControl_struct
{
	//核心上任务总数
	OSWORD taskCnt;

	//核心上任务的配置结构表
	T_OSEK_TASK_ConfigTable *taskCfgTable;

	// 任务控制结构数组，每个任务一个控制结构。ONFIG_OSEK_TASK_NUMBER在每个核心上应该具有不同的值,因此,这里可能需要使用指针
	T_OSEK_TASK_ControlBlock *osekTask_TaskTable;

	//CONFIG_OSEK_TASK_NUMBER在每个核心上应该具有不同的值,因此,这里可能需要使用指针
	T_OSEK_TARGET_TaskContext *osekTask_ContextTable ;

	// 用于在就绪队列中排队的就绪块
	T_OSEK_TASK_ReadyBlock *osekTask_ReadyBlock;

	// 空闲就绪块链表头指针
	T_OSEK_TASK_ReadyBlock *osekTask_FreeReadyBlock;
	// 就绪任务队列的链表头变量数组，一个优先级对应一个链表头。

	T_OSEK_TASK_ReadyTaskTableItem *osekTask_ReadyTaskTable;
};

/*核心上Alarm控制结构体*/
struct T_OSEK_KERNEL_AlarmControl_struct
{
#if CONFIG_OSEK_ALARM_NUMBER > 0

	//核心上的Alarm总数
	OSWORD almCnt;

	/// ALARM 配置信息表声明（该变量在配置了ALARM时才存在）
	T_OSEK_ALARM_ConfigTable *osekConfig_AlarmTable;

	// ALARM 控制块表声明
	T_OSEK_ALARM_ControlBlock *osekAlarm_AlarmTable;

#if defined(CONFIG_OSEK_ALARM_AUTO_NUMBER)
	//核心上的自启动Alarm总数
	OSWORD autoAlmCnt;
	/// 自动类型ALARM 扩展配置信息表声明（该变量在配置了自动类型ALARM时才存在）
	T_OSEK_ALARM_AutoTypeConfigTable *osekConfig_AutoAlarmTable;
#endif
#endif

};
#if defined(CONFIG_OSEK_COUNTER_NUMBER)
struct T_OSEK_KERNEL_CounterControl_struct
{
	//核心上Counter数
	OSWORD counterCnt;
	/// COUNTER 配置信息表声明
	T_OSEK_COUNTER_ConfigTable *osekConfig_CounterTable;

	//Counter控制块指针
	T_OSEK_COUNTER_ControlBlock *osekCounter_CounterTable;


};
#endif

/*每核心全局数据结构体*/
struct T_OSEK_KERNEL_OsKernel_struct
{
	//核心OS的应用模式
	OSBYTE osekExecution_CurrentAppMode;
	//核心运行状态
	volatile OsCoreState osState;
	/***************任务控制相关变量***********************/
	//任务控制链表指针
	T_OSEK_KERNEL_TaskControlRef osTasks;
	//当前运行任务
	T_OSEK_TASK_ReadyBlock *osekTask_RunningTask;
	//调度器锁
	OSDWORD osekTask_SchedulerLockLevel;
	// 优先级位图表，8个优先级一组，最多8组。
	OSBYTE osekTask_PriorityBitMap[( (OSBYTE) (CONFIG_OSEK_TASK_PRIORITY_NUMBER) ) >> 3];
	// 用于表示某个优先级组是否置位，一个位表示一个组。
	OSBYTE osekTask_PriorityBitMapMajor;

	/******************中断管理相关变量********************/
	// 中断服务程序中调试标志
	OSDWORD osekTask_IsrTriggeredTaskSwitchNecessary;
	// 中断嵌套深度
	OSDWORD osekInterrupt_NestedLevl;
	// 全局中断开关嵌套深度
	OSDWORD osekInterrupt_NestedAllInt;
	// ISR2类中断嵌套深度
	OSDWORD osekInterrupt_NestedOsInt;
	// 中断栈栈顶
	OSDWORD osekInterrupt_IsrStackTop;

	/*******************Counter管理相关变量*******************/
	T_OSEK_KERNEL_CounterControlRef osCounters;
	/*******************Alarm管理相关变量*******************/
	T_OSEK_KERNEL_AlarmControlRef osAlarms;

	/*******************target管理相关变量*******************/
	// OS相关中断掩码.这个变量在原来的OSEK中只定义了,而没有发现使用的地方
	OSDWORD osekTarget_OSIntMask ;
	// 所有中断掩码
	OSDWORD osekTarget_AllIntMask ;
	// 嵌套的所有中断的掩码
	OSDWORD osekTarget_NestedAllIntMask ;
	// 嵌套的OS相关中断的掩码
	OSDWORD osekTarget_NestedOsIntMask ;
	// 用于记录基本任务的当前可用栈位置
	OSDWORD osekTarget_SavedBTSP ;

	/****************资源管理相关变量**********************/
	//资源相关
	ResourceType    osekResource_ISRLast;
};

/*所有核心的全局数据结构体数组*/
extern T_OSEK_KERNEL_OsKernel 	osekKernel_osKernel[CONFIG_OSEK_KERNEL_CORE_NUMS];
/* 核心数据初始化函数*/
void osekKernel_Initialize();


#endif /* OSEKMKERNEL_H_ */
