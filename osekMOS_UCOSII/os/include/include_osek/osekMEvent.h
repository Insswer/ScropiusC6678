/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMEVENT_H_
#define OSEKMEVENT_H_

#include "stdTypes.h"


/**************************** 声明部分 *********************************/

// 事件管理提供给外部的公共调用函数

/**
 * @brief
 *    SetEvent：设置事件。
 * @param[in]	taskId：任务ID号。
 * @param[in]	mask：事件掩码。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的任务ID号。]
 * @returns     [E_OS_ACCESS:指定任务不是扩展任务。]
 * @returns     [E_OS_STATE:指定任务处于SUSPEND状态。]
 */
StatusType  SetEvent( TaskType taskId, EventMaskType mask );

/**
 * @brief
 *    ClearEvent：清除事件。
 * @param[in]	mask：事件掩码。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ACCESS:运行任务不是扩展任务。]
 * @returns     [E_OS_CALLEVEL:在中断中调用了此函数。]
 */
StatusType  ClearEvent( EventMaskType mask );

/**
 * @brief
 *    WaitEvent：等待事件。
 * @param[in]	mask：事件掩码。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ACCESS:运行任务不是扩展任务。]
 * @returns     [E_OS_RESOURCE:运行任务还拥有其他资源或系统处于关调度的情况。]
 * @returns     [E_OS_CALLEVEL:在中断中使用了此调用。]
 */
StatusType  WaitEvent( EventMaskType mask );

/**
 * @brief
 *    GetEvent：获取事件。
 * @param[in]	taskId：任务的ID号。
 * @param[out]  mask：  事件掩码变量的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的任务ID号。]
 * @returns     [E_OS_ACCESS:指定任务不是扩展任务。]
 * @returns     [E_OS_STATE:指定任务处于SUSPEND状态。]
 */
StatusType  GetEvent( TaskType taskId, EventMaskRefType mask );

// 内部函数
StatusType  osekEvent_SetEvent(T_OSEK_KERNEL_OsKernelRef kernelPtr,T_OSEK_TASK_ControlBlock *tcbPtr,EventMaskType   mask);


/**************************** 定义部分 *********************************/

#define DeclareEvent(event)

#endif /* OSEKMEVENT_H_ */
