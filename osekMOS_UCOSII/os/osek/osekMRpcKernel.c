/*
 * osekMRpcStubs.c
 *
 *  Created on: 2013-7-22
 *      Author: JackayChang
 */
#include "c66xx_rpc.h"
#include "osekMRpcKernel.h"
#include "osapi.h"
#include "cfg.h"
#include "c66xx_io.h"
#include "c66xx_ipc.h"
#include "c66xx_itbl.h"
#include "string.h"
#include "stdio.h"
#include "list.h"
#include "c66xx_cache.h"
#include "parcel.h"
#include "c66xx_mpu.h"



extern struct RpcNode rpc_nodes[];
extern struct IpcTransactionData coreIpcDatas[CONFIG_OSEK_KERNEL_CORE_NUMS][IPC_DATA_NUMS];

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_activateTask_stub
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_activateTask_stub(TaskType task,CoreIDType target)
{

	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_ACTIVATE_TASK];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_ACTIVATE_TASK;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;
		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put task ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, task);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_ACTIVATE_TASK);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		ret = (StatusType)ipc_data->reply;

	}
	return ret;
}
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_activateTask_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_activateTask_Server(unsigned int msg, struct IpcTransactionData *data)
{
	TaskType task;
	T_OSEK_TASK_ControlBlock *tcbPtr;
	StatusType status;
	CoreIDType core = BSP_GetCoreId();

	if(data == NULL)
	{
		printf("Error...\n");
		return 0xff;
	}

	if (msg != data->ipc_rpc_data.rpc_code) {
		printf("OOPS! msg not equal to rpc_code!\n");
		return 0xff;
	}

	/* get task id from RPC data */
	task = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);
	tcbPtr = &((osekKernel_osKernel[core].osTasks)->osekTask_TaskTable[task]);

	/* Disable Interrupt */

	/* do activate task */
	status = osekTask_ActiveTask(&osekKernel_osKernel[core],tcbPtr);
	if(status == OSEK_TASK_NEED_DISPATCH)
		osekKernel_osKernel[core].osekTask_IsrTriggeredTaskSwitchNecessary = 1;

	/* put return status into rpc data */
	data->reply = (unsigned long)status;
	BSP_L1DWriteback();

	/* do task schedule on local core in this  fun or in called fun */

	return status;
}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_getTaskState_stub
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_getTaskState_stub(TaskType taskId, TaskStateRefType state,CoreIDType target)
{
	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_GET_TASK_STATE];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_GET_TASK_STATE;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;
		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put task ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, taskId);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_GET_TASK_STATE);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		/* get task state from ipc_data */
		*state = (TaskStateType)GetUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data);
		ret = (StatusType)ipc_data->reply;

	}
	return ret;
}
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_getTaskState_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_getTaskState_Server(unsigned int msg, struct IpcTransactionData *data)
{
	TaskType task;
	TaskStateType state;
	StatusType status = E_OK;
	CoreIDType core = BSP_GetCoreId();

	if(data == NULL)
	{
		printf("Error...\n");
		return 0xff;
	}

	if (msg != data->ipc_rpc_data.rpc_code) {
		printf("OOPS! msg not equal to rpc_code!\n");
		return 0xff;
	}

	/* get task id */
	task = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);

	/* get task state from task ptr */
	if(task == osekKernel_osKernel[core].osekTask_RunningTask->taskControlBlock->configTable.taskId)
	{
		state = RUNNING;
	}
	else
	{
		state = ((osekKernel_osKernel[core].osTasks)->osekTask_TaskTable[task].status & OSEK_TASK_STATE_MASK);
	}
	/* put task state back into remote core */
	PutUnsignedInt(&data->ipc_rpc_data.rpc_data, state);
	/* write result back to remote core */
	data->reply = (unsigned long)status;
	BSP_L1DWriteback();

	return status;
}
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_setEvent_stub
 *  Description:
 * -------------------------------------------
 */
#if defined ( CONFIG_OSEK_SYSTEM_ET )


StatusType rpc_setEvent_stub(TaskType task,EventMaskType event,CoreIDType target)
{
	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_SET_EVENT];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_SET_EVENT;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;
		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put task ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, task);
		/* put event mask into rpc data */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, event);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_SET_EVENT);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		ret = (StatusType)ipc_data->reply;

	}
	return ret;

}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_setEvent_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_setEvent_Server(unsigned int msg, struct IpcTransactionData *data)
{
	StatusType status;
	TaskType 	task;
	EventMaskType event;
	T_OSEK_TASK_ControlBlock *tcbPtr;
	CoreIDType core = BSP_GetCoreId();

	if(data == NULL)
	{
		printf("Set event server Error");
		return (StatusType)0xff;
	}

	/* get event mask and task id from RPC data */
	event = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);
	task  = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);

	/* get task ptr */
	tcbPtr = &((osekKernel_osKernel[core].osTasks)->osekTask_TaskTable[task]);
	/* disable Int here */

	/* call osekEvent_SetEvent */
	status = osekEvent_SetEvent(&osekKernel_osKernel[core],tcbPtr,event);
	if(status == OSEK_TASK_NEED_DISPATCH)
			osekKernel_osKernel[core].osekTask_IsrTriggeredTaskSwitchNecessary = 1;

	/* write result back to remote core */
	data->reply = (unsigned long)status;
	BSP_L1DWriteback();
	return status;
}

#endif
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_setRelAlarm_stub
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_setRelAlarm_stub(AlarmType almId, TickType increment, TickType cycle,CoreIDType target)
{
	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_SET_REL_ALARM];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_SET_REL_ALARM;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;

		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put alarm ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, almId);
		/* put alarm increment into rpc data */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, increment);
		/* put alarm cycle into rpc data */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, cycle);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_SET_REL_ALARM);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		ret = (StatusType)ipc_data->reply;

	}
	return ret;
}

/*z
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_setRelAlarm_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_setRelAlarm_Server(unsigned int msg, struct IpcTransactionData *data)
{
	StatusType 	status = E_OK;
	AlarmType 	almID;
	TickType 	increment;
	TickType 	cycle;
	CtrType     ctrId;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;

	CoreIDType core = BSP_GetCoreId();
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);
	if(data == NULL)
	{
		printf("SetRelAlarm server Error");
		return (0xff);
	}

	/* get alarmID Increment cycle from RPC data */
	cycle = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);
	increment = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);
	almID = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);

	/* get locale data pointer */
	alarmPtr = (kernelPtr->osAlarms->osekAlarm_AlarmTable) + almID;
	ctrId = alarmPtr->configTable.ctrId;
	ctrPtr = (kernelPtr->osCounters->osekCounter_CounterTable) + ctrId;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// check alarm increment
	if( increment > ctrPtr->configTable.maxallowedvalue )
	{
		status = E_OS_VALUE;
	}
	// check alarm cycle
	if(( cycle != 0 ) && ( (cycle > ctrPtr->configTable.maxallowedvalue) || (cycle < ctrPtr->configTable.mincycle) ) )
	{
		status = E_OS_VALUE;
	}
#endif
	 // check alarm state
	if(alarmPtr->prevAlarm != (T_OSEK_ALARM_ControlBlockRef)0)
	{
		status = E_OS_STATE;
	}

	if(status == E_OK)
	{
		/* get prop increment */
	    if((ctrPtr->configTable.maxallowedvalue - ctrPtr->currentTick) < increment)
	    {
	    	increment -= (ctrPtr->configTable.maxallowedvalue - ctrPtr->currentTick + 1);
	    }
	    else
	    {
	    	increment += ctrPtr->currentTick;
	    }

	    /*call SetAlarm to activate alarm  */
	    status = SetAlarm(alarmPtr,ctrPtr,increment,cycle);
	    if(status == OSEK_TASK_NEED_DISPATCH)
	    	osekKernel_osKernel[core].osekTask_IsrTriggeredTaskSwitchNecessary = 1;
	}

    data->reply = (unsigned long)status;
	BSP_L1DWriteback();
	return status;
}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_setAbsAlarm_stub
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_setAbsAlarm_stub(AlarmType almId, TickType start, TickType cycle,CoreIDType target)
{
	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_SET_ABS_ALARM];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_SET_ABS_ALARM;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;

		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put alarm ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, almId);
		/* put alarm start time into rpc data */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, start);
		/* put alarm cycle into rpc data */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, cycle);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_SET_ABS_ALARM);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		ret = (StatusType)ipc_data->reply;

	}
	return ret;
}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_setAbsAlarm_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_setAbsAlarm_Server(unsigned int msg, struct IpcTransactionData *data)
{
	StatusType 	status = E_OK;
	AlarmType 	almID;
	TickType 	start;
	TickType 	cycle;
	CtrType     ctrId;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;

	CoreIDType core = BSP_GetCoreId();
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);
	if(data == NULL)
	{
		printf("SetRelAlarm server Error");
		return (0xff);
	}
	/* get alarmID Increment cycle from RPC data */
	cycle = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);
	start = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);
	almID = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);

	/* get local data pointer */
	alarmPtr = (kernelPtr->osAlarms->osekAlarm_AlarmTable) + almID;
	ctrId = alarmPtr->configTable.ctrId;
	ctrPtr = (kernelPtr->osCounters->osekCounter_CounterTable) + ctrId;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// check alarm increment
	if( start > ctrPtr->configTable.maxallowedvalue )
	{
		status = E_OS_VALUE;
	}

	// check alarm cycle
	if(( cycle != 0 ) && ( (cycle > ctrPtr->configTable.maxallowedvalue) || (cycle < ctrPtr->configTable.mincycle) ) )
	{
		status = E_OS_VALUE;
	}
#endif
	 // check alarm state
	if(alarmPtr->prevAlarm != (T_OSEK_ALARM_ControlBlockRef)0)
	{
		status = E_OS_STATE;
	}

	if(status == E_OK)
	{
	    /*call SetAlarm to activate alarm  */
	    status = SetAlarm(alarmPtr,ctrPtr,start,cycle);
	    if(status == OSEK_TASK_NEED_DISPATCH)
	    	osekKernel_osKernel[core].osekTask_IsrTriggeredTaskSwitchNecessary = 1;
	}

    data->reply = (unsigned long)status;
	BSP_L1DWriteback();
	return status;
}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_getAlarm_stub
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_getAlarm_stub(AlarmType almId,TickRefType tick,CoreIDType target)
{
	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_GET_ALARM];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_GET_ALARM;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;

		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put alarm ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, almId);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_GET_ALARM);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		ret = (StatusType)ipc_data->reply;
		if(ret == E_OK)
		{
			*tick = (TickType)GetUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data);
		}
	}
	return ret;
}
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_getAlarm_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_getAlarm_Server(unsigned int msg, struct IpcTransactionData *data)
{
	StatusType 	status = E_OK;
	CoreIDType core = BSP_GetCoreId();
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);

	AlarmType 	almId;
	TickType	tick;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;

	if(data == NULL)
	{
		printf("SetRelAlarm server Error");
		return (0xff);
	}
	/* get alarm ID and tick pointer */
	
	almId = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);

	/* get locale data */
	alarmPtr = (kernelPtr->osAlarms->osekAlarm_AlarmTable) + almId;
	if(alarmPtr->prevAlarm == (T_OSEK_ALARM_ControlBlockRef)0)
	{
		status = E_OS_NOFUNC;
	}
	else
	{
		ctrPtr = (kernelPtr->osCounters->osekCounter_CounterTable) + alarmPtr->configTable.ctrId;


		tick = ctrPtr->currentTick;
		/* adjust tick value */
		if(alarmPtr->expirationTick >= tick)
		{
			tick = alarmPtr->expirationTick - tick;
		}
		else
		{
			tick = alarmPtr->expirationTick + ctrPtr->configTable.maxallowedvalue + 1 - tick;
		}

		/* write alarm tick back to remote core */
		PutUnsignedInt(&data->ipc_rpc_data.rpc_data, tick);
	}
    data->reply = (unsigned long)status;
	BSP_L1DWriteback();
	return status;
}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_cancelAlarm_stub
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_cancelAlarm_stub(AlarmType almID,CoreIDType target)
{
	unsigned int coreId = BSP_GetCoreId();
	StatusType ret = 0xff;

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_CANCEL_ALARM];

	if(ipc_data != NULL)
	{
		/* IPC TYPE */
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		/* core info */
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		/* RPC Type */
		ipc_data->ipc_rpc_data.rpc_code = RPC_CANCEL_ALARM;
		/* RPC return value */
		ipc_data->reply = (unsigned long)NO_AVAILABLE;

		/* Init RPC data */
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		/* put alarm ID into rpc */
		PutUnsignedInt(&ipc_data->ipc_rpc_data.rpc_data, almID);

		/* put RPC struct into list */
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		/* triger RPC  */
		BSP_InterruptCore(target, RPC_CANCEL_ALARM);
		/* wait rpc call to be done */
		while (ipc_data->reply == NO_AVAILABLE)
		{
			wait();
		}
		ret = (StatusType)ipc_data->reply;
	}
	return ret;
}
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  rpc_cancelAlarm_Server
 *  Description:
 * -------------------------------------------
 */
StatusType rpc_cancelAlarm_Server(unsigned int msg, struct IpcTransactionData *data)
{

	StatusType 	status = E_OK;
	CoreIDType core = BSP_GetCoreId();
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);

	AlarmType 	almId;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;

	if(data == NULL)
	{
		printf("SetRelAlarm server Error");
		return (0xff);
	}
	/* get alarm ID and tick pointer */
	almId = GetUnsignedInt(&data->ipc_rpc_data.rpc_data);

	/* get locale data */
	alarmPtr = (kernelPtr->osAlarms->osekAlarm_AlarmTable) + almId;
	if(alarmPtr->prevAlarm == (T_OSEK_ALARM_ControlBlockRef)0)
	{
		status = E_OS_NOFUNC;
	}
	else
	{
		/* remove alarm from counter alarm list */
		if (alarmPtr->prevAlarm == alarmPtr)
		{
			ctrPtr = (osekKernel_osKernel[core].osCounters)->osekCounter_CounterTable + (alarmPtr->configTable.ctrId);
			ctrPtr->alarm = alarmPtr->nextAlarm;
			if (alarmPtr->nextAlarm != (T_OSEK_ALARM_ControlBlockRef) 0)
				ctrPtr->alarm->prevAlarm = ctrPtr->alarm;
		}
		else
		{
			(alarmPtr->prevAlarm)->nextAlarm = alarmPtr->nextAlarm;
			if (alarmPtr->nextAlarm != (T_OSEK_ALARM_ControlBlockRef) 0)
				(alarmPtr->nextAlarm)->prevAlarm = alarmPtr->prevAlarm;
		}
		// 将指定的ALARM设置为未使用的
		alarmPtr->prevAlarm = (T_OSEK_ALARM_ControlBlockRef) 0;
		alarmPtr->nextAlarm = (T_OSEK_ALARM_ControlBlockRef) 0;
		/* enable interrupt if we disabled before */
		printf("Alarm Canceled...\n");
	}

    data->reply = (unsigned long)status;
	BSP_L1DWriteback();
	return status;
}



