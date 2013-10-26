/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/


#include "osapi.h"
#include "cfg.h"
#include "c66xx_ipc.h"
#include "c66xx_rpc.h"
#include "osekMRpcFunc.h"

//Task Priority Num define
#define CONFIG_OSEK_TASK_PRIORITY_NUMBER 64
//Extend Task Stack Size Define
#define TASK_STACK_SIZE	256

//Task Map define
OsTaskMapType	osekTask_tasksMap[CONFIG_OSEK_TASK_NUMBER]=
{
	GET_GLOBAL_OBJ(0,C0_Task1),//task1
	GET_GLOBAL_OBJ(0,C0_Task2),//task2
	GET_GLOBAL_OBJ(0,C0_Task3),//task3
	GET_GLOBAL_OBJ(1,C1_Task1),//task4
	GET_GLOBAL_OBJ(1,C1_Task2),//task5
	GET_GLOBAL_OBJ(2,C2_Task1),
	GET_GLOBAL_OBJ(2,C2_Task2),
	GET_GLOBAL_OBJ(3,C3_Task1),
	GET_GLOBAL_OBJ(3,C3_Task2),
	GET_GLOBAL_OBJ(3,C3_Task3),
};
//Alarm Map define
OsAlarmMapType	osekAlarm_alarmsMap[CONFIG_ALARM_NUMBER]=
{
	GET_GLOBAL_OBJ(0,C0_Alarm1),
	GET_GLOBAL_OBJ(0,C0_Alarm2),
	GET_GLOBAL_OBJ(1,C1_Alarm1),
	GET_GLOBAL_OBJ(1,C1_Alarm2),
	GET_GLOBAL_OBJ(3,C3_Alarm1),
	GET_GLOBAL_OBJ(3,C3_Alarm2),
};


/********************************************中断配置********************************************/


OSBYTE	osekConfig_InterruptTypeTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM] =
{
	{/***Core 0***/
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		1,
		2,
	},
	{/***Core 1***/
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		1,
		2,
	},
	{/***Core 2***/
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		1,
		2,
	},
	{/***Core 3***/
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		2,
		1,
		1,
		2,
	}
};

//中断入口函数配置列表
T_OSEK_TASK_Entry	osekConfig_InterruptEntryTable[CONFIG_OSEK_KERNEL_CORE_NUMS][OCC_ISRLEVELNUM] =
{
	{/***Core 0***/
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		BSP_DoIpc,
		BSP_TimeInterruptISR,//(T_OSEK_TASK_Entry)Core0_TimingInt,
		0,
	},	
	{/***Core 1***/
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		BSP_DoIpc,
		BSP_TimeInterruptISR,//(T_OSEK_TASK_Entry)Core1_TimingInt,
		0,
	},

	{/***Core 2***/
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		BSP_TimeInterruptISR,
		0,
	},
	{/***Core 3***/
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		0,
		BSP_TimeInterruptISR,
		0,
	}
};




/******************************Core0***********************************/
//CORE0 Task Stack
OSDWORD C0_TaskStack[CORE0_TASK_NUM-1][TASK_STACK_SIZE];
//任务对象配置表
T_OSEK_TASK_ConfigTable core0_taskConfig_table[CORE0_TASK_NUM]=
{
	{
		(T_OSEK_TASK_Entry)FuncStartTask,
		C0_Task1,
		0,
		&(C0_TaskStack[C0_Task1][TASK_STACK_SIZE-1]),
		&(C0_TaskStack[C0_Task1][0]),
	},			/*StartTask	*/
	{
		(T_OSEK_TASK_Entry)FuncIntActivateTask,
		C0_Task2,
		0|OSEK_TASK_EXTENDED,
		&(C0_TaskStack[C0_Task2][TASK_STACK_SIZE-1]),
		&(C0_TaskStack[C0_Task2][0]),
	},			/*IntActivateTask	*/
	{
		(T_OSEK_TASK_Entry)FuncTenmsTask,
		C0_Task3,
		OSEK_TASK_ACTIVE,
		0,
		0,
	},			/*TenmsTask	*/
	{
		0,
		(CORE0_TASK_NUM-1),
		OSEK_TASK_ACTIVE,
		0,
		0,
	}			/* NULLTASK */
};
T_OSEK_TASK_ControlBlock core0_taskControlblock_table[CORE0_TASK_NUM];
T_OSEK_TARGET_TaskContext core0_taskContext_table[CORE0_TASK_NUM] __attribute__ ((aligned (8))) ;
//T_OSEK_TASK_ReadyBlock core0_taskRead_block[CORE0_TASK_TOTAL_ACTIVATE_NUMBER];
T_OSEK_TASK_ReadyBlock core0_taskRead_block[CORE0_TASK_NUM];
//CORE0_TASK_TOTAL_ACTIVATE_NUMBER与CORE0_TASK_NUM有什么对应关系

//COUNTER对象配置表
T_OSEK_COUNTER_ConfigTable core0_counterCfg_table[CORE0_COUNTER_NUM] =
{
	{
		0xfffffff,	// 允许计数器的最大值
		10,
		0
	},
	{
		0xffff,	// 允许计数器的最大值
		20,
		5
	}
};
T_OSEK_COUNTER_ControlBlock core0_counter_table[CORE0_COUNTER_NUM];


//ALARM对象配置表
T_OSEK_ALARM_ConfigTable core0_almControl_table[CORE0_ALM_NUM] =
{
	{
		C0_Task1,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0x10,
#endif
		SysCounter,
	},
	{
		C0_Task2,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0,
#endif
		SysCounter,
	}
};

T_OSEK_ALARM_ControlBlock core0_alm_table[CORE0_ALM_NUM];
//T_OSEK_ALARM_AutoTypeConfigTable core0_auto_alm_table[Core0_AUTO_ALM_NUM];


/******************************Core1***********************************/
//CORE1 Task Stack
OSDWORD C1_TaskStack[CORE1_TASK_NUM-1][TASK_STACK_SIZE];
//任务对象配置表
T_OSEK_TASK_ConfigTable core1_taskConfig_table[CORE1_TASK_NUM]=
{
	{
		(T_OSEK_TASK_Entry)FuncOS_5ms_task,
		C1_Task1,
		0|OSEK_TASK_EXTENDED|OSEK_TASK_ACTIVE,
		&C1_TaskStack[C1_Task1][TASK_STACK_SIZE-1],
		&C1_TaskStack[C1_Task1][0],
	},			/*StartTask	*/
	{
		(T_OSEK_TASK_Entry)FuncEventTask,
		C1_Task2,
		0|OSEK_TASK_EXTENDED,
		&C1_TaskStack[C1_Task2][TASK_STACK_SIZE-1],
		&C1_TaskStack[C1_Task2][0],
	},			/*IntActivateTask	*/
	{
		0,
		(CORE1_TASK_NUM-1),
		OSEK_TASK_ACTIVE,
		0,
		0,
	},			/* NULLTASK */
};
T_OSEK_TASK_ControlBlock core1_taskControlblock_table[CORE1_TASK_NUM];
T_OSEK_TARGET_TaskContext core1_taskContext_table[CORE1_TASK_NUM] __attribute__ ((aligned (8))) ;
//T_OSEK_TASK_ReadyBlock core0_taskRead_block[CORE0_TASK_TOTAL_ACTIVATE_NUMBER];
T_OSEK_TASK_ReadyBlock core1_taskRead_block[CORE1_TASK_NUM];
//CORE0_TASK_TOTAL_ACTIVATE_NUMBER与CORE0_TASK_NUM有什么对应关系

//COUNTER对象配置表
T_OSEK_COUNTER_ConfigTable core1_counterCfg_table[CORE1_COUNTER_NUM] =
{
	{
		0xffffffff,	// 允许计数器的最大值
		10,
		0
	},
};
T_OSEK_COUNTER_ControlBlock core1_counter_table[CORE1_COUNTER_NUM];

//ALARM对象配置表
T_OSEK_ALARM_ConfigTable core1_almControl_table[CORE1_ALM_NUM] =
{
	{
		C1_Task2,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0,
#endif
		SysCounter,
	},
	{
		C1_Task2,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0x30,
#endif
		SysCounter,
	}
};

T_OSEK_ALARM_ControlBlock core1_alm_table[CORE1_ALM_NUM];
//T_OSEK_ALARM_AutoTypeConfigTable core0_auto_alm_table[Core0_AUTO_ALM_NUM];

/******************************Core2***********************************/
//任务对象配置表
T_OSEK_TASK_ConfigTable core2_taskConfig_table[CORE2_TASK_NUM]=
{
	{
		(T_OSEK_TASK_Entry)FuncStartTask,
		C2_Task1,
		0,
		0,
		0,
	},			/*StartTask	*/
	{
		(T_OSEK_TASK_Entry)FuncIntActivateTask,
		C2_Task2,
		OSEK_TASK_ACTIVE,
		0,
		0,
	},			/*IntActivateTask	*/
	{
			(T_OSEK_TASK_Entry)FuncTenmsTask,
			C3_Task3,
			0,
			0,
			0,
	},			/*TenmsTask	*/
	{
		0,
		(CORE2_TASK_NUM-1),
		OSEK_TASK_ACTIVE,
		0,
		0,
	},			/* NULLTASK */
};
T_OSEK_TASK_ControlBlock core2_taskControlblock_table[CORE2_TASK_NUM];
T_OSEK_TARGET_TaskContext core2_taskContext_table[CORE2_TASK_NUM] __attribute__ ((aligned (8))) ;
//T_OSEK_TASK_ReadyBlock core0_taskRead_block[CORE0_TASK_TOTAL_ACTIVATE_NUMBER];
T_OSEK_TASK_ReadyBlock core2_taskRead_block[CORE2_TASK_NUM];
//CORE0_TASK_TOTAL_ACTIVATE_NUMBER与CORE0_TASK_NUM有什么对应关系

//COUNTER对象配置表
T_OSEK_COUNTER_ConfigTable core2_counterCfg_table[CORE2_COUNTER_NUM] =
{
	{
		0xffffffff,	// 允许计数器的最大值
		10,
		0
	},
};
T_OSEK_COUNTER_ControlBlock core2_counter_table[CORE2_COUNTER_NUM];


/******************************Core3***********************************/
//任务对象配置表
T_OSEK_TASK_ConfigTable core3_taskConfig_table[CORE3_TASK_NUM]=
{
	{
		(T_OSEK_TASK_Entry)FuncStartTask,
		C3_Task1,
		0,
		0,
		0,
	},			/*StartTask	*/
	{
		(T_OSEK_TASK_Entry)FuncIntActivateTask,
		C3_Task2,
		0,
		0,
		0,
	},			/*IntActivateTask	*/
	{
		(T_OSEK_TASK_Entry)FuncTenmsTask,
		C3_Task3,
		0,
		0,
		0,
	},			/*TenmsTask	*/
	{
		0, (CORE3_TASK_NUM-1),
		OSEK_TASK_ACTIVE,
		0,
		0,
	},			/* NULLTASK */
};
T_OSEK_TASK_ControlBlock core3_taskControlblock_table[CORE3_TASK_NUM];
T_OSEK_TARGET_TaskContext core3_taskContext_table[CORE3_TASK_NUM] __attribute__ ((aligned (8))) ;
//T_OSEK_TASK_ReadyBlock core0_taskRead_block[CORE0_TASK_TOTAL_ACTIVATE_NUMBER];
T_OSEK_TASK_ReadyBlock core3_taskRead_block[CORE3_TASK_NUM];
//CORE0_TASK_TOTAL_ACTIVATE_NUMBER与CORE0_TASK_NUM有什么对应关系

//COUNTER对象配置表
T_OSEK_COUNTER_ConfigTable core3_counterCfg_table[CORE3_COUNTER_NUM] =
{
	{
		0xffffffff,	// 允许计数器的最大值
		10,
		0
	},
};
T_OSEK_COUNTER_ControlBlock core3_counter_table[CORE3_COUNTER_NUM];


//ALARM对象配置表
T_OSEK_ALARM_ConfigTable core3_almControl_table[CORE3_ALM_NUM] =
{
	{
		C3_Task1,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0,
#endif
		SysCounter,
	},
	{
		C3_Task2,
#if (defined(OCC_ECC1) || defined(OCC_ECC2)) && defined(OCC_ALMSETEVENT)
		0,
#endif
		SysCounter,
	}
};

T_OSEK_ALARM_ControlBlock core3_alm_table[CORE3_ALM_NUM];
//T_OSEK_ALARM_AutoTypeConfigTable core0_auto_alm_table[Core0_AUTO_ALM_NUM];

T_OSEK_TASK_ReadyTaskTableItem core0_TaskReadyTaskTable[CONFIG_OSEK_TASK_PRIORITY_NUMBER];
T_OSEK_TASK_ReadyTaskTableItem core1_TaskReadyTaskTable[CONFIG_OSEK_TASK_PRIORITY_NUMBER];
T_OSEK_TASK_ReadyTaskTableItem core2_TaskReadyTaskTable[CONFIG_OSEK_TASK_PRIORITY_NUMBER];
T_OSEK_TASK_ReadyTaskTableItem core3_TaskReadyTaskTable[CONFIG_OSEK_TASK_PRIORITY_NUMBER];


T_OSEK_KERNEL_TaskControl osekKernel_coreTasks[CONFIG_OSEK_KERNEL_CORE_NUMS]=
{
	//osekTask_ReadyTaskTable not inited
	{CORE0_TASK_NUM,core0_taskConfig_table,core0_taskControlblock_table,core0_taskContext_table,core0_taskRead_block,(void *)0,core0_TaskReadyTaskTable},//core0
	{CORE1_TASK_NUM,core1_taskConfig_table,core1_taskControlblock_table,core1_taskContext_table,core1_taskRead_block,(void *)0,core1_TaskReadyTaskTable},//core1
	{CORE2_TASK_NUM,core2_taskConfig_table,core2_taskControlblock_table,core2_taskContext_table,core2_taskRead_block,(void *)0,core2_TaskReadyTaskTable},//core2
	{CORE3_TASK_NUM,core3_taskConfig_table,core3_taskControlblock_table,core3_taskContext_table,core3_taskRead_block,(void *)0,core3_TaskReadyTaskTable}//core3
};
T_OSEK_KERNEL_CounterControl osekKernel_coreCounters[CONFIG_OSEK_KERNEL_CORE_NUMS]=
{
	{CORE0_COUNTER_NUM,core0_counterCfg_table,core0_counter_table},
	{CORE1_COUNTER_NUM,core1_counterCfg_table,core1_counter_table},
	{CORE2_COUNTER_NUM,core2_counterCfg_table,core2_counter_table},
	{CORE3_COUNTER_NUM,core3_counterCfg_table,core3_counter_table}
};
T_OSEK_KERNEL_AlarmControl osekKernel_coreAlarms[CONFIG_OSEK_KERNEL_CORE_NUMS]=
{
	{CORE0_ALM_NUM,core0_almControl_table,core0_alm_table},
	{CORE1_ALM_NUM,core1_almControl_table,core1_alm_table},
	{0,NULLPTR,NULLPTR},
	{CORE3_ALM_NUM,core3_almControl_table,core3_alm_table}
};


OSBYTE coreSpinlocks[OSEK_CONFIG_SPINLOCK_NUM] = {
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9,
		10,
		11,
		12,
		13,
		14,
		15,
		16,
		17,
		18,
};


struct IpcTransactionData coreIpcDatas[CONFIG_OSEK_KERNEL_CORE_NUMS][IPC_DATA_NUMS];

/* 将所有跨核函数都声明在osekMRpcFunc.h中 */



struct FuncSlot coreRpcFunc[RPC_FUNC_NUM] = {
		RPC_FUNC_SLOT(RPC_FUNC_TEST, func_test_stub_proxy, func_test_stub),
		RPC_FUNC_SLOT(RPC_ACTIVATE_TASK,rpc_activateTask_stub,rpc_activateTask_Server),
		RPC_FUNC_SLOT(RPC_GET_TASK_STATE,rpc_getTaskState_stub,rpc_getTaskState_Server),
		RPC_FUNC_SLOT(RPC_SET_EVENT,rpc_setEvent_stub,rpc_setEvent_Server),
		RPC_FUNC_SLOT(RPC_SET_REL_ALARM,rpc_setRelAlarm_stub,rpc_setRelAlarm_Server),
		RPC_FUNC_SLOT(RPC_SET_ABS_ALARM,rpc_setAbsAlarm_stub,rpc_setAbsAlarm_Server),
		RPC_FUNC_SLOT(RPC_GET_ALARM,rpc_getAlarm_stub,rpc_getAlarm_Server),
		RPC_FUNC_SLOT(RPC_CANCEL_ALARM,rpc_cancelAlarm_stub,rpc_cancelAlarm_Server)
};

//自旋锁结构变量
T_OSEK_SPINLOCK_Lock osekSpin_LockTable[CONFIG_OSEK_SPINLOCK_NUMS];
