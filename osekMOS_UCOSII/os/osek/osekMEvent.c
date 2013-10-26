/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#include "osapi.h"
#include "cfg.h"
/**************************** 实现部分 *********************************/

#if defined(CONFIG_OSEK_SYSTEM_ET)

/*
 * @brief
 *    osekEvent_SetEvent：设置事件（内部函数）。
 * @param
 * @param[in]	tcbPtr：指向任务控制块的指针。
 *          	mask：事件掩码。
 * @param[out]  无。
 * @returns:    E_OK： 成功。
 *              [E_OS_ACCESS:指定任务不是扩展任务。]
 *              [E_OS_STATE:指定任务处于SUSPEND状态。]
 *              osekTask_WakeTask调用的返回值：E_OK--成功唤醒；OSEK_TASK_NEED_DISPATCH--成功唤醒并且需要重新调度；E_OS_LIMIT--没有足够的就绪块可用。
 */

StatusType osekEvent_SetEvent(T_OSEK_KERNEL_OsKernelRef kernelPtr,T_OSEK_TASK_ControlBlock *tcbPtr,EventMaskType   mask)
{

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果指定任务不是扩展任务，返回E_OS_ACCESS
	if( (tcbPtr->configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		return E_OS_ACCESS;
	}

	// 如果任务处于SUSPEND状态，返回E_OS_STATE
	if( tcbPtr->status == SUSPENDED )
	{
		return E_OS_STATE;
	}
#endif

	// 设置任务的已设置事件集
	tcbPtr->setEvent |= mask;

	// 如果任务处于等待状态，且已设置的事件满足其等待的事件条件，则唤醒任务
    if( tcbPtr->status == WAITING )
    {
    	if (( tcbPtr->waitEvent & tcbPtr->setEvent) != 0 )
        {
        	return osekTask_WakeTask(kernelPtr,tcbPtr);
        }
    }

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    SetEvent：设置事件。
 * @param
 * @param[in]	taskId：任务ID号。
 *          	mask：事件掩码。
 * @param[out]  无。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的任务ID号。]
 * <p>          [E_OS_ACCESS:指定任务不是扩展任务。]
 * <p>          [E_OS_STATE:指定任务处于SUSPEND状态。]
 */
StatusType  SetEvent( TaskType taskId, EventMaskType mask )
{
	T_OSEK_TASK_ControlBlock *tcbPtr;
	StatusType status;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr=&osekKernel_osKernel[core];
	//获取任务映射表项
	LocalTaskType localTask;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果任务ID超出范围，返回E_OS_ID
	if( taskId > CONFIG_OSEK_TASK_MAX_ID )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_SetEvent,taskId) ;
	}
#endif

	//获取局部任务ID
	localTask=osekTask_tasksMap[taskId];
	//判断任务是否是本地任务
	if(core != GET_LOCAL_CORE_ID(localTask))
	{

		status = rpc_setEvent_stub(GET_LOCAL_TASK_ID(localTask),mask,GET_LOCAL_CORE_ID(localTask));
		return status;
	}
	else
	{
		tcbPtr = &(kernelPtr->osTasks->osekTask_TaskTable[GET_LOCAL_TASK_ID(localTask)]);

		OSEK_TARGET_DisableOSInt(osIntSave);

		// 调用内部函数osekEvent_SetEvent设置事件
		status = osekEvent_SetEvent(kernelPtr,tcbPtr,mask);

		// 如果需要，实施调度，此处会切换到另一个任务执行
		if( status == OSEK_TASK_NEED_DISPATCH )
		{
			osekTask_Dispatch();
		}
		else
		{
			OSEK_TARGET_EnableOSInt(osIntSave);
			if( status != E_OK )
			{
				OSEK_HOOK_ErrorHook(status,OSServiceId_SetEvent,taskId) ;
			}
			else
			{
				return status;
			}
		}

		OSEK_TARGET_EnableOSInt(osIntSave);
	}

	return E_OK;

}


/*
 * @brief
 *    ClearEvent：清除事件。
 * @param
 * @param[in]	mask：事件掩码。
 * @param[out]  无。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ACCESS:运行任务不是扩展任务。]
 * <p>          [E_OS_CALLEVEL:在中断中调用了此函数。]
 */
StatusType  ClearEvent( EventMaskType mask )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果运行任务不是扩展任务，返回E_OS_ACCESS
	if( (kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_ClearEvent,0) ;
	}

	// 如果是从中断中调用的此函数，返回E_OS_CALLEVEL
	if( kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_ClearEvent,0) ;
	}
#endif

	OSEK_TARGET_DisableOSInt(osIntSave);
	// 将指定事件从任务的已设置事件集中清除掉
	kernelPtr->osekTask_RunningTask->taskControlBlock->setEvent &= (EventMaskType)(~mask);
	OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    WaitEvent：等待事件。
 * @param
 * @param[in]	mask：事件掩码。
 * @param[out]  无。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ACCESS:运行任务不是扩展任务。]
 * <p>          [E_OS_RESOURCE:运行任务还拥有其他资源或系统处于关调度的情况。]
 * <p>          [E_OS_CALLEVEL:在中断中使用了此调用。]
 */
StatusType  WaitEvent( EventMaskType mask )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];

	/* 如果任务占用了自旋锁,则不允许挂起自己 */
	if(kernelPtr->osekTask_RunningTask->taskControlBlock->spinLockList != NULL)
	{
		OSEK_HOOK_ErrorHook(E_OS_SPINLOCK,OSServiceId_WaitEvent,0) ;
	}

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果运行任务不是扩展任务，返回E_OS_ACCESS
	if( (kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_WaitEvent,0) ;
	}

#if defined(CONFIG_OSEK_RESOURCE)
	// 如果运行任务资源列表不为0，返回E_OS_RESOURCE
	if( kernelPtr->osekTask_RunningTask->taskControlBlock->resourceList != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_WaitEvent,0) ;
	}

#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 如果中断资源没有释放，返回E_OS_RESOURCE
    if (osekResource_ISRLast != 0)
    {
        OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_WaitEvent,0) ;
    }
#endif  /* defined(CONFIG_OSEK_RESOURCE_ISR) */

#endif

	// 如果调度锁不为0，返回E_OS_RESOURCE
	if( kernelPtr->osekTask_SchedulerLockLevel != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_WaitEvent,0) ;
	}

	// 如果中断嵌套数不为0，返回E_OS_CALLEVEL
	if( kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_WaitEvent,0) ;
	}
#endif

	// 关中断
	OSEK_TARGET_DisableOSInt(osIntSave);

	// 设置运行任务的等待事件掩码
	kernelPtr->osekTask_RunningTask->taskControlBlock->waitEvent = mask;

	// 如果运行任务已被设置的事件不符合其等待的事件，则将任务切换到等待状态
	if( (kernelPtr->osekTask_RunningTask->taskControlBlock->setEvent & mask) == 0 )
	{
		osekTask_WaitTask(kernelPtr);
	}

	// 开中断
	OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    GetEvent：获取事件。
 * @param
 * @param[in]	taskId：任务的ID号。
 * @param[out]  mask：  事件掩码变量的引用。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的任务ID号。]
 * <p>          [E_OS_ACCESS:指定任务不是扩展任务。]
 * <p>          [E_OS_STATE:指定任务处于SUSPEND状态。]
 */
StatusType  GetEvent( TaskType taskId, EventMaskRefType mask )
{

	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_TaskControlRef taskControl=osekKernel_osKernel[core].osTasks;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果任务ID超出范围，返回E_OS_ID
	if( taskId > (osekKernel_osKernel[core].osTasks->taskCnt -1 ) )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetEvent,taskId) ;
	}

	// 如果指定任务不是扩展任务，返回E_OS_ACCESS
	if( ((taskControl->osekTask_TaskTable[taskId]).configTable.property & OSEK_TASK_EXTENDED) == 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_ACCESS,OSServiceId_GetEvent,taskId) ;
	}

	// 如果指定任务处于SUSPEND状态，返回E_OS_STATE
	if( taskControl->osekTask_TaskTable[taskId].status == SUSPENDED )
	{
		OSEK_HOOK_ErrorHook(E_OS_STATE,OSServiceId_GetEvent,taskId) ;
	}
#endif

	// 将指定任务已设置的事件输出到输出参数中
	*mask = taskControl->osekTask_TaskTable[taskId].setEvent;

	// 成功，返回E_OK
	return E_OK;

}

#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET)  */
