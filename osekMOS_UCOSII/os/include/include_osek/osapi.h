/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/**
 * $Log: osapi.h,v $
 * Revision 1.0  2008/03/04 10:34:00
 * 创建文件
*/

/**
 * @file 	osapi.h
 * @brief
 *	<li>功能：本文件作为头文件使用的统一外部头文件，其包含了操作系统各个组成部分的子头文件。</li>
 * @author
 * @date 	2008-03-04
 * <p>部门：
 */


#ifndef _OS_API_H
#define _OS_API_H

#include "osekOccConfig.h"
#include "stdTypes.h"



#if defined(CONFIG_OSEK_SYSTEM_GETSERVICEID)

#define OSErrorGetServiceId()       (osekHook_ServiceId)

// OSEK OS 服务组标识
#define OSSYSTEMSERVICEIDBASE       0x00    // 操作系统执行控制服务组
#define OSTASKSERVICEIDBASE         0x10    // 任务管理服务组
#define OSISRSERVICEIDBASE          0x20    // 中断处理服务组
#define OSRESOURCESERVICEIDBASE     0x30    // 资源管理服务组
#define OSEVENTSERVICEIDBASE        0x40    // 事件控制服务组
#define OSMESSAGESERVICEIDBASE      0x50    // 消息通信服务组
#define OSCOUNTERSERVICEIDBASE      0x70    // counter服务组
#define OSALARMSERVICEIDBASE        0x80    // alarm服务组
#define OSHOOKSERVICEIDBASE         0x90    // hook例程组


#define OSServiceId_NoService                   (OSSERVICEIDTYPE)0xFF

// 操作系统执行控品?
#define OSServiceId_StartOS                     (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x02 )
#define OSServiceId_ShutdownOS                  (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x04 )
#define OSServiceId_GetActiveApplicationMode    (OSSERVICEIDTYPE)( OSSYSTEMSERVICEIDBASE + 0x06 )
// 任务管理服务
#define OSServiceId_ActivateTask                (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x00 )
#define OSServiceId_TerminateTask               (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x02 )
#define OSServiceId_ChainTask                   (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x04 )
#define OSServiceId_Schedule                    (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x06 )
#define OSServiceId_GetTaskID                   (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x08 )
#define OSServiceId_GetTaskState                (OSSERVICEIDTYPE)( OSTASKSERVICEIDBASE + 0x0A )
// 中断处理服务
#define OSServiceId_ResumeAllInterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x00 )
#define OSServiceId_SuspendAllinterrupts        (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x02 )
#define OSServiceId_ResumeOSInterrupts          (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x04 )
#define OSServiceId_SuspendOSinterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x06 )
#define OSServiceId_EnableAllInterrupts         (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x08 )
#define OSServiceId_DisableAllInterrupts        (OSSERVICEIDTYPE)( OSISRSERVICEIDBASE + 0x0A )
// 资源管理服务
#define OSServiceId_GetResource                 (OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x00 )
#define OSServiceId_ReleaseResource             (OSSERVICEIDTYPE)( OSRESOURCESERVICEIDBASE + 0x02 )
// 事件控制服务
#define OSServiceId_SetEvent                    (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x00 )
#define OSServiceId_ClearEvent                  (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x02 )
#define OSServiceId_GetEvent                    (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x04 )
#define OSServiceId_WaitEvent                   (OSSERVICEIDTYPE)( OSEVENTSERVICEIDBASE + 0x06 )
// 消息通信服务
#define OSServiceId_SendMessage                 (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x00 )
#define OSServiceId_ReceiveMessage              (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x02 )
#define OSServiceId_GetMessageResource          (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x04 )
#define OSServiceId_ReleaseMessageResource      (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x06 )
#define OSServiceId_GetMessageStatus            (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x08 )
#define OSServiceId_StartCOM                    (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x0A )
#define OSServiceId_StopCOM                     (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x0C )
#define OSServiceId_InitCOM                     (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x0E )
#define OSServiceId_CloseCOM                    (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x10 )
#define OSServiceId_ReadFlag                    (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x12 )
#define OSServiceId_ResetFlag                   (OSSERVICEIDTYPE)( OSMESSAGESERVICEIDBASE + 0x14 )
// counter服务
#define OSServiceId_InitCounter                 (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x00 )
#define OSServiceId_CounterTrigger              (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x02 )
#define OSServiceId_GetCounterValue             (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x04 )
#define OSServiceId_GetCounterInfo              (OSSERVICEIDTYPE)( OSCOUNTERSERVICEIDBASE + 0x06 )
// alarm服务
#define OSServiceId_GetAlarmBase                (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x00 )
#define OSServiceId_GetAlarm                    (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x02 )
#define OSServiceId_SetRelAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x04 )
#define OSServiceId_SetAbsAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x06 )
#define OSServiceId_CancelAlarm                 (OSSERVICEIDTYPE)( OSALARMSERVICEIDBASE + 0x08 )
// hook例程
#define OSServiceId_ErrorHook                   (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x00 )
#define OSServiceId_PreTaskHook                 (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x02 )
#define OSServiceId_PostTaskHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x04 )
#define OSServiceId_StartupHook                 (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x06 )
#define OSServiceId_ShutdownHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x08 )
#define OSServiceId_IdleLoopHook                (OSSERVICEIDTYPE)( OSHOOKSERVICEIDBASE + 0x0A )

#endif


#if defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS)
// 获取服务的第一个参数的宏
// 任务管理服务
#define OSError_StartOS_Mode()              ((AppModeType)osekHook_ObjId)
#define OSError_ActivateTask_TaskID()      ( (TaskType)osekHook_ObjId)
#define OSError_ChainTask_TaskID()          ((TaskType)osekHook_ObjId)
#define OSError_GetTaskState_TaskID()      ( (TaskType)osekHook_ObjId)
// 资源管理服务
#define OSError_GetResource_ResID()         ((ResourceType)osekHook_ObjId)
#define OSError_ReleaseResource_ResID()    ( (ResourceType)osekHook_ObjId)
// 事件控制服务
#define OSError_SetEvent_TaskID()           ((TaskType)osekHook_ObjId)
#define OSError_GetEvent_TaskID()           ((TaskType)osekHook_ObjId)
// 消息通信服务
#define OSError_SendMessage_Message()      ( (MsgType)osekHook_ObjId)
#define OSError_ReceiveMessage_Message()   ( (MsgType)osekHook_ObjId)
#define OSError_GetMessageResource_Message()       ( (MsgType)osekHook_ObjId)
#define OSError_ReleaseMessageResource_Message()   ( (MsgType)osekHook_ObjId)
// alarm 服务
#define OSError_GetAlarmBase_AlarmID()      ((AlarmType)osekHook_ObjId)
#define OSError_GetAlarm_AlarmID()          ((AlarmType)osekHook_ObjId)
#define OSError_SetRelAlarm_AlarmID()       ((AlarmType)osekHook_ObjId)
#define OSError_SetAbsAlarm_AlarmID()       ((AlarmType)osekHook_ObjId)
#define OSError_CancelAlarm_AlarmID()       ((AlarmType)osekHook_ObjId)
// counter服务
#define OSError_InitCounter_CounterID()     ((CtrType)osekHook_ObjId)
#define OSError_CounterTrigger_CounterID()  ((CtrType)osekHook_ObjId)
#define OSError_GetCounterValue_CounterID() ((CtrType)osekHook_ObjId)
#define OSError_GetCounterInfo_CounterID()  ((CtrType)osekHook_ObjId)



#endif  /* defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS) */



// 包含各管理功能模块的头文件
/// 包含目标平台相关头文件
#include "osekMTarget.h"
///	包含中断管理模块头文件
#include "osekMInterrupt.h"
/// 包含HOOK管理模块头文件
#include "osekMHook.h"
/// 包含任务管理模块头文件
#include "osekMTask.h"
/// 包含资源管理模块头文件
//#include "osekResource.h"
/// 包含系统执行管理模块头文件
#include "osekMExecution.h"
/// 包含计数器管理模块头文件
#include "osekMCounter.h"
///	包含报警管理模块头文件
#include "osekMAlarm.h"
///	包含事件管理模块头文件
#include "osekMEvent.h"
///包含自旋锁模块头文件
#include "osekMSpinLock.h"
/// 包含调试管理模块头文件
//#include "osekDebug.h"
///	包含消息管理模块头文件
//#include "osekMessage.h"
///包含内核管理头文件
#include "osekMKernel.h"
///包含内核RPC头文件
#include "osekMRpcKernel.h"
///包含cache管理模块
#include "c66xx_cache.h"
#endif

