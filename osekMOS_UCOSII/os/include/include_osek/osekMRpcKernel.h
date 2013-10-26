/*
 * osekMCrossCore.h
 *
 *  Created on: 2013-7-22
 *      Author: JackayChang
 */

#ifndef OSEKMCROSSCORE_H_
#define OSEKMCROSSCORE_H_

#include "stdTypes.h"
/* OS RPC Type */
#define	RPC_ACTIVATE_TASK	(1)
#define	IPC_ACTIVATE_TASK	(1)
#define	RPC_GET_TASK_STATE	(2)
#define	IPC_GET_TASK_STATE	(2)
#define	RPC_SET_EVENT		(3)
#define	IPC_SET_EVENT		(3)
#define	RPC_SET_REL_ALARM	(4)
#define	IPC_SET_REL_ALARM	(4)
#define	RPC_SET_ABS_ALARM	(5)
#define	IPC_SET_ABS_ALARM	(5)
#define	RPC_GET_ALARM		(6)
#define	IPC_GET_ALARM		(6)
#define	RPC_CANCEL_ALARM	(7)
#define	IPC_CANCEL_ALARM	(7)

StatusType rpc_activateTask_stub(TaskType task,CoreIDType target);
StatusType rpc_activateTask_Server(unsigned int msg,struct IpcTransactionData *data);
StatusType rpc_getTaskState_stub(TaskType taskId, TaskStateRefType state,CoreIDType target);
StatusType rpc_getTaskState_Server(unsigned int msg, struct IpcTransactionData *data);
StatusType rpc_setEvent_stub(TaskType task,EventMaskType event,CoreIDType target);
StatusType rpc_setEvent_Server(unsigned int msg,struct IpcTransactionData *data);
StatusType rpc_setRelAlarm_stub(AlarmType almId, TickType increment, TickType cycle,CoreIDType target);
StatusType rpc_setRelAlarm_Server(unsigned int msg,struct IpcTransactionData *data);
StatusType rpc_setAbsAlarm_stub(AlarmType almId, TickType start, TickType cycle,CoreIDType target);
StatusType rpc_setAbsAlarm_Server(unsigned int msg,struct IpcTransactionData *data);
StatusType rpc_getAlarm_stub(AlarmType almId,TickRefType tick,CoreIDType target);
StatusType rpc_getAlarm_Server(unsigned int msg,struct IpcTransactionData *data);
StatusType rpc_cancelAlarm_stub(AlarmType almID,CoreIDType target);
StatusType rpc_cancelAlarm_Server(unsigned int msg,struct IpcTransactionData *data);




#endif /* OSEKMCROSSCORE_H_ */
