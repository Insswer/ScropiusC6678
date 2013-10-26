/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/


#include "osapi.h"
#include "cfg.h"

#if defined(CONFIG_OSEK_TARGET_SOFTBITMAP)
// 与硬件无关的优先级位图实现

// 优先级索引表，用于快速定位当前的最高优先级，数值越大优先级越高。
OSBYTE osekTask_PriorityIndexTable[256] = {
0,0,1,1,2,2,2,2,3,3,3,3,3,3,3,3,
4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7

};

#endif /* #if defined(CONFIG_OSEK_TARGET_SOFTBITMAP) */


/**************************** 实现部分 *********************************/
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  OSEK_TASK_CallTaskEntry
 *  Description:  Call running Task Entry on the core
 *  	  Notes:  This has been rewritten because the restoring of BTSP or ETSP  destroy the
 *  			  local vars in function osekTask_Dispatch  and osekTask_TerminateDispatch.
 * -------------------------------------------
 */
void OSEK_TASK_CallTaskEntry()
{
	//获取核心ID
	CoreIDType core = GetCoreID();

	/*Call Task Function entry*/
	(*((osekKernel_osKernel[core].osekTask_RunningTask)->taskControlBlock->configTable.entry))();

}


/*
 * @brief
 *    osekTask_ChangeRunningTaskPriority: 把当前任务的优先级调整为指定的优先级，
 *                                        调整后，任务仍然要处于对应优先级就绪队列的链头（内部函数）。
 * @param[in]	newPri：新的优先级
 * @returns 	无
 *
 */
void osekTask_ChangeRunningTaskPriority( T_OSEK_KERNEL_OsKernelRef kernelPtr , OSPRIOTYPE newPri )
{
	T_OSEK_TASK_ControlBlock *tcbPtr=kernelPtr->osekTask_RunningTask->taskControlBlock;

	// 先将任务从原来优先级的就绪队列中移出，然后清除优先级位图中的对应位
	OSEK_TASK_MoveTaskFromReadyTable(kernelPtr,tcbPtr->curPriority);
	OSEK_TASK_ClearPriorityMap(kernelPtr,tcbPtr->curPriority);

	tcbPtr->curPriority = newPri;

	// 先将优先级位图中新优先级对应位置位，然后将任务插入到新优先级就绪队列的头部
	OSEK_TASK_SetPriorityBitMap(kernelPtr,newPri);
	OSEK_TASK_AddTask2ReadyTableAtHead(kernelPtr,kernelPtr->osekTask_RunningTask);
}

#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
/*
 * ---  FUNCTION  ----------------------------
 *         Name:  osekTask_ChangeResourcePri
 *  Description: change task pri to resource  pri .this only used for extend task
 * -------------------------------------------
 */
static void osekTask_ChangeResourcePri()
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	/* get kernel ptr*/
	OsKernelRef kernelPtr = &(osKernel[core]);
	/*check if need to chage running task prio before runinng*/
	if( kernelPtr->osekTask_RunningTask->taskControlBlock->curPriority < kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.inresPriority )
	{
		osekTask_ChangeRunningTaskPriority(kernelPtr,kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.inresPriority );
	}
}
#endif


/*
 * @brief
 *    osekTask_ActiveTask：激活指定的任务，此为非API函数，用于对已经明确了任务有效性后的激活操作（内部函数）。
 *
 * @param[in]	tcbPtr：需要激活的任务控制结构的指针。
 * @returns 	E_OK: 成功激活；
 *              E_OS_LIMIT: 任务状态不对，或者超过了最大激活次数；
 *              OSEK_TASK_NEED_DISPATCH: 成功激活并且需要重新调度。
 */
StatusType osekTask_ActiveTask(T_OSEK_KERNEL_OsKernelRef kernelPtr,T_OSEK_TASK_ControlBlock *tcbPtr)
{
	T_OSEK_TASK_ReadyBlock *runningTaskPtr = kernelPtr->osekTask_RunningTask;
	// 对于非SUSPEND状态的任务，如果允许多次激活，并且激活次数小于最大激活次数，则添加新的就绪块并对激活次数进行累加后返回E_OK，否则返回E_OS_LIMIT。
	if( tcbPtr->status != SUSPENDED )
	{
#if defined(CONFIG_OSEK_SYSTEM_MACTIVE)
		if(tcbPtr->curActiveNum < (tcbPtr->configTable).maxActive)
		{
			if( OSEK_TASK_AddaReadyBlock(kernelPtr,tcbPtr) == E_OK )
			{
				tcbPtr->curActiveNum++;
				return E_OK;
			}
			else
			{
				return E_OS_LIMIT;
			}
		}
		else
		{
			return E_OS_LIMIT ;
		}
#else /* #if defined(CONFIG_OSEK_SYSTEM_MACTIVE)  */
		return (E_OS_LIMIT ) ;
#endif /* #else defined(CONFIG_OSEK_SYSTEM_MACTIVE)  */
	}


#if defined(CONFIG_OSEK_SYSTEM_ET)
	// 新激活的任务，其已设置的事件必需清零。
	tcbPtr->setEvent = 0;
#endif  /* #if defined(CONFIG_OSEK_SYSTEM_ET) */

	// 对于SUSPEND状态的任务，添加新的就绪块到就绪队列尾部，累加激活次数并修改任务状态为就绪
	if( OSEK_TASK_AddaReadyBlock(kernelPtr,tcbPtr) == E_OS_LIMIT )
	{
		return E_OS_LIMIT;// 没有空闲的就绪块可用
	}
#if defined(CONFIG_OSEK_SYSTEM_EP)
	tcbPtr->curActiveNum = 1;
#endif
	tcbPtr->status = OSEK_TASK_FIRST_RUN | READY;

	// 如果当前任务的优先级比激活任务的优先级低，则检查是否需要调度。
	if( tcbPtr->curPriority > runningTaskPtr->taskControlBlock->curPriority )
	{
		// 当前任务允许抢占
		if( (runningTaskPtr->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
		{
#if defined(CONFIG_OSEK_INTERRUPT_ONLY_ISR2) || defined(CONFIG_OSEK_INTERRUPT_ISR_ALL)
			// 如果在中断中调用的此函数，则推迟切换，并且置中断退出切换标志，否则返回OSEK_TASK_NEED_DISPATCH
			if( kernelPtr->osekInterrupt_NestedLevl != 0 )
			{
				kernelPtr->osekTask_IsrTriggeredTaskSwitchNecessary = 1;
			}
			else
#if defined(CONFIG_OSEK_RESOURCE_ISR)
	    	if(kernelPtr->osekResource_ISRLast == 0)	// 系统没有使用中断资源
#endif  /* defined(CONFIG_OSEK_RESOURCE_ISR) */

#endif /* #if defined(CONFIG_OSEK_INTERRUPT_ONLY_ISR2)|| defined(CONFIG_OSEK_INTERRUPT_ISR_ALL) */
			{
				return OSEK_TASK_NEED_DISPATCH;
			}
		}
	}

	return E_OK;
}


/*
 * @brief
 *    osekTask_WakeTask：唤醒等待事件的任务（内部函数）。
 *
 * @param[in]	tcbPtr：需要唤醒的任务控制结构的指针。
 * @returns 	E_OK: 成功唤醒；
 *              OSEK_TASK_NEED_DISPATCH: 成功唤醒并且需要重新调度;
 *              E_OS_LIMIT: 没有足够的就绪块可用。
 *
 */
StatusType osekTask_WakeTask(T_OSEK_KERNEL_OsKernelRef kernelPtr,T_OSEK_TASK_ControlBlock *tcbPtr)
{
	T_OSEK_TASK_ReadyBlock *runningTaskPtr=kernelPtr->osekTask_RunningTask;

	// 添加新的就绪块到就绪队列的尾部
	if( OSEK_TASK_AddaReadyBlock(kernelPtr,tcbPtr) == E_OS_LIMIT )
	{
		return E_OS_LIMIT;// 没有空闲的就绪块可用
	}
	tcbPtr->status = READY;

	// 如果当前任务的优先级比唤醒任务的优先级低，则检查是否需要调度。
	if( tcbPtr->curPriority > runningTaskPtr->taskControlBlock->curPriority )
	{
		// 当前任务允许抢占
		if( (runningTaskPtr->taskControlBlock->configTable.property & OSEK_TASK_NONPREEMPT ) == 0 )
		{
#if defined(CONFIG_OSEK_INTERRUPT_ONLY_ISR2) || defined(CONFIG_OSEK_INTERRUPT_ISR_ALL)
			// 如果在中断中调用的此函数，则推迟切换，并且置中断退出切换标志，否则返回OSEK_TASK_NEED_DISPATCH
			if( kernelPtr->osekInterrupt_NestedLevl != 0 )
			{
				kernelPtr->osekTask_IsrTriggeredTaskSwitchNecessary = 1;
			}
			else
#endif /* #if defined(CONFIG_OSEK_INTERRUPT_ONLY_ISR2)|| defined(CONFIG_OSEK_INTERRUPT_ISR_ALL) */
			{
				return OSEK_TASK_NEED_DISPATCH;
			}
		}
	}

	return E_OK;
}


/*
 * @brief
 *    osekTask_WaitTask：使当前任务进入等待状态（内部函数）。
 *
 * @param[in]	无
 * @returns 	无
 *
 */
void osekTask_WaitTask( T_OSEK_KERNEL_OsKernelRef kernelPtr )
{
	T_OSEK_TASK_ControlBlock *tcbPtr;
	T_OSEK_TASK_ReadyBlock *runningTaskPtr=kernelPtr->osekTask_RunningTask;
	tcbPtr = runningTaskPtr->taskControlBlock;

	// 修改当前任务状态为等待态，将其移出就绪队列，释放就绪块，并清除优先级位图的相应位
	tcbPtr->status = WAITING ;
	OSEK_TASK_MoveTaskFromReadyTable(kernelPtr,tcbPtr->curPriority);
	OSEK_TASK_ReleaseReadyBlock(kernelPtr,runningTaskPtr);
	OSEK_TASK_ClearPriorityMap(kernelPtr,tcbPtr->curPriority);

	//如果使用了内部资源，任务进入等待队列时，优先级应恢复。
#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
#if defined(CONFIG_OSEK_SYSTEM_EP)
	tcbPtr->curPriority = tcbPtr->configTable.basePriority ;
#else
	tcbPtr->curPriority = OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId);
#endif /* #if defined(CONFIG_OSEK_SYSTEM_EP) */
#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

	// 由于是当前任务进入等待状态，因此必需进行重调度。
	osekTask_Dispatch();

}

/*
 * @brief
 *    osekTask_Dispatch: 进行任务切换，即使任务是不可抢占的也会发生切换（内部函数）。
 *    当调用dispatch时,中断是关闭的.此时应该处于任务的堆栈环境.也即最后任务保存的上下文环境是在这个函数中
 * @param[in]	无。
 * @returns		无。
 *
 */
void osekTask_Dispatch()
{
	OSPRIOTYPE pri;
	T_OSEK_TASK_ReadyBlock *oldTask;
	T_OSEK_TASK_ControlBlock *tcbPtr;

	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);

#if defined(CONFIG_OSEK_HOOK_POSTTASK)
	T_OSEK_TASK_ReadyBlock *newTask;
#endif

	// 调度锁未释放，则不进行调度
	if( kernelPtr->osekTask_SchedulerLockLevel  != 0)
	{
		return ;
	}

	// 在中断中也不进行调度
	if(kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		return ;
	}

	//获取当前运行的任务控制块
	oldTask = kernelPtr->osekTask_RunningTask ;
	//获取即将运行的最高任务优先级
	pri = OSEK_TASK_GetHighestPriority(kernelPtr);

#if defined(CONFIG_OSEK_HOOK_POSTTASK)
	//获取最高优先级任务控制块
	newTask = OSEK_TASK_GetReadyTask(kernelPtr,pri);

	// 在执行任务离去扩展时，必须是在当前任务仍然是离去任务的条件下，因此不能立即切换当前任务。
	if( newTask != kernelPtr->osekTask_RunningTask)
	{
		 OSEK_HOOK_PostTaskHook();
	}

	//当前运行的任务置为最高优先级任务的控制块
	osekKernel_osKernel[core].osekTask_RunningTask = newTask;
#else /* #if defined(CONFIG_OSEK_HOOK_POSTTASK) */

	osekKernel_osKernel[core].osekTask_RunningTask = OSEK_TASK_GetReadyTask(kernelPtr,pri);

#endif /* #else defined(CONFIG_OSEK_HOOK_POSTTASK) */

	//获取控制块指针
	tcbPtr = (osekKernel_osKernel[core].osekTask_RunningTask)->taskControlBlock;

	// 只有需要切换的任务不是当前任务时才进行切换
	if( oldTask != kernelPtr->osekTask_RunningTask )
	{
		// 保存离去任务的上下文
		/***********osekTarget_SaveContext:********/
		if( osekTarget_SaveContext(oldTask->taskControlBlock->context) != 0)
		{
			// 任务又切换回来，先检查是否需要根据内部资源来调整优先级，然后进行任务进入的扩展调用。
#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
			if( runningTaskPtr->taskControlBlock->curPriority < kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.inresPriority )
			{
				osekTask_ChangeRunningTaskPriority(kernelPtr, kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.inresPriority );
			}
#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

			OSEK_HOOK_PreTaskHook();
			return;

		}

		// 离去任务是基本任务时，要设置下一个基本任务运行时的栈指针。
		if( (oldTask->taskControlBlock->configTable.property & OSEK_TASK_EXTENDED) == 0  )
		{
			/***********OSEK_TARGET_SaveBTSP:已改********/
			OSEK_TARGET_SaveBTSP(kernelPtr,(oldTask->taskControlBlock->context)->sp);
		}

		// 任务第一次运行时，需要调整栈指针，然后采用直接调用的方式进入任务
		/********************Warnning**********************************/
		/* 在多核中调用恢复任务堆栈操作后,本函数中的局部变量将失效.因此在重新加载任务堆栈后,后面的所有操作、函数调用等都不能使用局部的变量
		 * 如果是函数调用,则最好不要使用参数.而任务的局部变量修改/访问都是错误的.
		 */
		if( (tcbPtr->status & OSEK_TASK_FIRST_RUN) != 0 )
		{
			// 清除第一次运行标志
			tcbPtr->status &= (~OSEK_TASK_FIRST_RUN);

			// 扩展任务有独立的栈，因此如果要运行的是扩展任务则必需进行栈切换；如果是从扩展任务进入基本任务，也需要进行栈切换。
#if defined(CONFIG_OSEK_SYSTEM_ET)
			if((tcbPtr->configTable.property & OSEK_TASK_EXTENDED) != 0)
			{
				/***********osekTarget_LoadETSP********/
				osekTarget_LoadETSP(tcbPtr->configTable.stackTop);
			}
			else
#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET) */
			{
				/***********OSEK_TARGET_LoadBTSP********/
				OSEK_TARGET_LoadBTSP(kernelPtr->osekTarget_SavedBTSP);
			}

			// 根据内部资源的优先级调整运行任务的优先级

#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
			/* because sp restore we have to call a function to check resource pri change */
			osekTask_ChangeResourcePri();
#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

			OSEK_HOOK_PreTaskHook();

			// 初次进入任务时，中断都是使能的。
			OSEK_TARGET_EnableOSIntWithoutSave();

			// 进入任务运行
			OSEK_TASK_CallTaskEntry();
		}
		// 如果任务不是第一次运行，则直接恢复其上下文来切换到任务
		else /* if( (osekTask_RunningTask->taskControlBlock->status & OSEK_TASK_FIRST_RUN) != 0 ) */
		{
			osekTarget_RestoreContext(tcbPtr->context);
		}

	} /* if( oldTask != osekTask_RunningTask ) */

#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
	else	// 不发生任务切换，需要恢复运行任务的内部资源优先级
	{
		if( tcbPtr->curPriority < tcbPtr->configTable.inresPriority )
		{
			osekTask_ChangeRunningTaskPriority(kernelPtr,tcbPtr->configTable.inresPriority );
		}
	}
#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */
}


/*
 * @brief
 *    osekTask_TerminateDispatch：结束当前任务，并选择优先级最高的就绪任务运行（内部函数）。
 * @param[in]	无。
 * @returns		无。
 *
 */
void osekTask_TerminateDispatch( )
{
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];

	OSPRIOTYPE pri;
	T_OSEK_TASK_ControlBlock *tcbPtr;

	//获取最高优先级
	pri = OSEK_TASK_GetHighestPriority(kernelPtr);

	//获取最高优先级任务控制块
	kernelPtr->osekTask_RunningTask = OSEK_TASK_GetReadyTask(kernelPtr,pri);
	tcbPtr = kernelPtr->osekTask_RunningTask->taskControlBlock;

	// 任务第一次运行时，需要调整栈指针，然后采用直接调用的方式进入任务
	if( (tcbPtr->status & OSEK_TASK_FIRST_RUN) != 0 )
	{
		// 清除第一次运行标志
		tcbPtr->status &= (~OSEK_TASK_FIRST_RUN);

		// 扩展任务有独立的栈，因此如果要运行的是扩展任务则必需进行栈切换；如果是从扩展任务进入基本任务，也需要进行栈切换。
#if defined(CONFIG_OSEK_SYSTEM_ET)
		if((tcbPtr->configTable.property & OSEK_TASK_EXTENDED) != 0)
		{
			/*****************不用修改********************/
			osekTarget_LoadETSP(tcbPtr->configTable.stackTop);
		}
		else
#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET) */
		{
			OSEK_TARGET_LoadBTSP(kernelPtr->osekTarget_SavedBTSP);
		}

#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
			/* because sp restore we have to call a function to check resource pri change */
			osekTask_ChangeResourcePri();
#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

		// 调用任务进入的扩展调用，使能中断，并进入任务运行
		OSEK_HOOK_PreTaskHook();
		OSEK_TARGET_EnableOSIntWithoutSave();

		//调用任务入口
		OSEK_TASK_CallTaskEntry();
	}
	else	// 任务不是第一次运行，则直接恢复其上下文来切换到任务
	{
#if defined(CONFIG_OSEK_SYSTEM_ET)
		if( (tcbPtr->configTable.property & OSEK_TASK_EXTENDED) == 0  )
#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET) */
		{
			OSEK_TARGET_SaveBTSP(kernelPtr,(tcbPtr->context)->sp);
		}
		osekTarget_RestoreContext(tcbPtr->context);
	}

}


/*************************************API接口*********************************************/
/*
 * @brief
 *    ActivateTask：激活指定的任务。
 * @param[in]	taskId：任务的ID号。
 * @returns		E_OK：成功激活
 * <p>			E_OS_LIMIT：任务状态不对或者超过了最大激活次数
 * <p>          [E_OS_ID：无效的任务ID]
 */
StatusType ActivateTask( TaskType taskId )
{
	StatusType status;
	T_OSEK_TASK_ControlBlock *tcbPtr;
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];
	//获取任务映射表项
	LocalTaskType localTask;

	//声明保存中断变量
	OSEK_TARGET_OSIntSave( osIntSave );

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 任务ID无效，返回E_OS_ID
	if( taskId > (TaskType)CONFIG_OSEK_TASK_MAX_ID )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_ActivateTask,taskId) ;
	}
#endif /* #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

	//获取局部任务ID
	localTask = osekTask_tasksMap[taskId];
	//判断需要激活的任务是否是本地核心上的任务
	if(core != GET_LOCAL_CORE_ID(localTask))
	{
		status = rpc_activateTask_stub(GET_LOCAL_TASK_ID(localTask),GET_LOCAL_CORE_ID(localTask));
		return status;
	}
	else
	{//进行本地激活任务操作

		//获取任务TCB
		tcbPtr = &(kernelPtr->osTasks->osekTask_TaskTable[GET_LOCAL_TASK_ID(localTask)]);

		//关本地中断
		OSEK_TARGET_DisableOSInt(osIntSave);

		// 调用内部函数激活任务,激活指定的任务
		status = osekTask_ActiveTask(kernelPtr,tcbPtr);
		// 如果激活任务后需要重调度，则实施调度
		if( status == OSEK_TASK_NEED_DISPATCH )
		{
			osekTask_Dispatch();
		}
		else
		{
			if( status != E_OK )
			{
				OSEK_TARGET_EnableOSInt(osIntSave);
				OSEK_HOOK_ErrorHook(status,OSServiceId_ActivateTask,taskId) ;
			}
		}

		OSEK_TARGET_EnableOSInt(osIntSave);
	}

	return E_OK;
}


/*
 * @brief
 *    TerminateTask：结束当前任务, 正确情况下是不会返回的。
 * @param[in]	无
 * @returns		[E_OS_RESOURCE：还有资源没有释放；]
 * <p>			[E_OS_CALLEVEL：在中断中调用了该功能。]
 *
 */
StatusType TerminateTask( void )
{
	T_OSEK_TASK_ControlBlock *tcbPtr;
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];
	tcbPtr = kernelPtr->osekTask_RunningTask->taskControlBlock;

	 /*  如果任务有占用的自旋锁没有释放*/
	if(tcbPtr->spinLockList != NULL)
	{
		OSEK_HOOK_ErrorHook(E_OS_SPINLOCK,OSServiceId_TerminateTask,0) ;
	}

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)

#if defined(CONFIG_OSEK_RESOURCE)
	// 还有资源没有释放，返回E_OS_RESOURCE
	if( tcbPtr->resourceList != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_TerminateTask,0) ;
	}

	// 没有释放调度器资源，返回E_OS_RESOURCE
	if( kernelPtr->osekTask_SchedulerLockLevel != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_TerminateTask,0) ;
	}

#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 如果中断资源没有释放，返回E_OS_RESOURCE
    if (kernelPtr->osekResource_ISRLast != 0)
    {
        OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_TerminateTask,0) ;
    }
#endif  /* defined(CONFIG_OSEK_RESOURCE_ISR) */

#endif /* #if defined(CONFIG_OSEK_RESOURCE) */


	// 在中断中使用了该功能，返回E_OS_CALLEVEL
	if( kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_TerminateTask,0) ;
	}
#endif /* #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

	// 因为不会返回，因此直接关闭中断，而不必保存中断状态。
	OSEK_TARGET_DisableOSIntWithoutSave();

	// 执行任务离去的扩展调用
	OSEK_HOOK_PostTaskHook();

//操作链表
	// 从就绪队列中移出相应的就绪块，归还到空闲就绪块链表中，并处理优先级位图
	OSEK_TASK_MoveTaskFromReadyTable(kernelPtr,tcbPtr->curPriority);
	OSEK_TASK_ReleaseReadyBlock(kernelPtr,kernelPtr->osekTask_RunningTask);
	OSEK_TASK_ClearPriorityMap(kernelPtr,tcbPtr->curPriority);

#if defined(CONFIG_OSEK_SYSTEM_MACTIVE)
	tcbPtr->curActiveNum-- ;
	// 如果任务的当前激活次数不为零，则将任务置为初始状态。
	if( tcbPtr->curActiveNum != 0)
	{
		tcbPtr->status = OSEK_TASK_FIRST_RUN | READY;
	}
	// 否则将任务置为结束的挂起状态
	else
	{
		tcbPtr->status = SUSPENDED ;
	}
	// 恢复任务的优先级
	tcbPtr->curPriority = tcbPtr->configTable.basePriority ;
#else /* #if defined(CONFIG_OSEK_SYSTEM_MACTIVE) */

	// 任务不支持多次激活，被置为结束的挂起状态
	kernelPtr->osekTask_RunningTask->taskControlBlock->status = SUSPENDED ;
	// 一个优先级上只允许一个任务时，任务的优先级是通过其ID号转换的。
	tcbPtr->curPriority = OSEK_TASK_ID2PRI((tcbPtr->configTable).taskId);

#endif /* #else defined(CONFIG_OSEK_SYSTEM_MACTIVE) */

	// 执行任务离去的重调度
	osekTask_TerminateDispatch();

	// 程序不会执行到这里，但增加该语句是避免编译器报警告
	return E_OK;
}


/*
 * @brief
 *    ChainTask：结束当前任务并且激活指定的任务。
 * @param[in]	taskId：需要激活的任务ID号
 * @returns		E_OS_LIMIT：需要激活的任务状态不对，或者超过了最大激活次数。
 * <p>          [E_OS_RESOURCE：还有资源没有释放；]
 * <p>			[E_OS_CALLEVEL：在中断中调用此函数；]
 * <p>			[E_OS_ID：需要激活的任务ID号不合法；]
 *
 */
StatusType ChainTask( TaskType taskId )
{
	T_OSEK_TASK_ControlBlock *tcbPtr;
	T_OSEK_TASK_ControlBlock *runningTcb;
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];
	//获取任务映射表项
	LocalTaskType localTask;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

	runningTcb = kernelPtr->osekTask_RunningTask->taskControlBlock;

	 /*  如果任务有占用的自旋锁没有释放*/
	if(runningTcb->spinLockList != NULL)
	{
		OSEK_HOOK_ErrorHook(E_OS_SPINLOCK,OSServiceId_TerminateTask,0) ;
	}

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)

#if defined(CONFIG_OSEK_RESOURCE)
	// 如果还有资源没有释放，返回E_OS_RESOURCE
	if( runningTcb->resourceList != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_ChainTask,taskId) ;
	}

	// 如果调度器资源没有释放，返回E_OS_RESOURCE
	if( kernelPtr->osekTask_SchedulerLockLevel != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_ChainTask,taskId) ;
	}
#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 如果中断资源没有释放，返回E_OS_RESOURCE
    if (kernelPtr->osekResource_ISRLast != 0)
    {
        OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_ChainTask,taskId) ;
    }
#endif  /* defined(CONFIG_OSEK_RESOURCE_ISR) */
#endif /* #if defined(CONFIG_OSEK_RESOURCE) */

	// 如果在中断中调用该函数，返回E_OS_CALLEVEL
	if( kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_ChainTask,taskId) ;
	}

	// 如果任务ID不合法，返回E_OS_ID
	if( taskId > CONFIG_OSEK_TASK_MAX_ID )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_ChainTask,taskId) ;
	}

#endif /* #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */



	//获取局部任务ID
	localTask=osekTask_tasksMap[taskId];
	//判断任务是否是本地任务
	if(core != GET_LOCAL_CORE_ID(localTask))
	{

	}
	else
	{

		//获取任务的控制块
		tcbPtr = &(kernelPtr->osTasks->osekTask_TaskTable[GET_LOCAL_TASK_ID(localTask)]);

		OSEK_TARGET_DisableOSInt(osIntSave);

		// 当前任务chain自己时，不算作一次激活请求，任务处于初始就绪态，需释放内部资源
		if(tcbPtr == runningTcb)
		{
			OSEK_HOOK_PostTaskHook();
			tcbPtr->status = OSEK_TASK_FIRST_RUN | READY;
			// 将任务从就绪队列头部取下
			OSEK_TASK_MoveTaskFromReadyTable(kernelPtr,tcbPtr->curPriority);

	#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
	#if defined(CONFIG_OSEK_SYSTEM_EP)
			if(tcbPtr->curPriority > tcbPtr->configTable.basePriority)
			{
				OSEK_TASK_ClearPriorityMap(kernelPtr,tcbPtr->curPriority);
				tcbPtr->curPriority = tcbPtr->configTable.basePriority ;
				OSEK_TASK_SetPriorityBitMap(kernelPtr,tcbPtr->configTable.basePriority);
			}
	#else /* #if defined(CONFIG_OSEK_SYSTEM_EP) */
			if(tcbPtr->curPriority > OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId))
			{
				OSEK_TASK_ClearPriorityMap(kernelPtr,tcbPtr->curPriority);
				tcbPtr->curPriority = OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId);
				OSEK_TASK_SetPriorityBitMap(kernelPtr,OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId));
			}
	#endif /* #else defined(CONFIG_OSEK_SYSTEM_EP) */
	#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

			// 将任务(当前运行的任务)挂到就绪队列尾部
			OSEK_TASK_AddTask2ReadyTableAtTail(kernelPtr,kernelPtr->osekTask_RunningTask);

		}
		else
		{
			// 对于非SUSPEND状态的任务，如果不允许多次激活，或激活次数达到最大值，返回E_OS_LIMIT。
			if( tcbPtr->status != SUSPENDED )
			{
	#if defined(CONFIG_OSEK_SYSTEM_MACTIVE)
				if(tcbPtr->curActiveNum == tcbPtr->configTable.maxActive)
				{
					OSEK_TARGET_EnableOSInt(osIntSave);
					OSEK_HOOK_ErrorHook(E_OS_LIMIT,OSServiceId_ChainTask,taskId) ;
				}
	#else /* #if defined(CONFIG_OSEK_SYSTEM_MACTIVE) */
				OSEK_TARGET_EnableOSInt(core);
				OSEK_HOOK_ErrorHook(E_OS_LIMIT,OSServiceId_ChainTask,taskId) ;
	#endif /* #else defined(CONFIG_OSEK_SYSTEM_MACTIVE) */
			}

			// 执行任务离去的扩展调用
			OSEK_HOOK_PostTaskHook();

			// 从就绪队列中移出当前任务相应的就绪块，归还到空闲就绪块链表中，并处理优先级位图
			OSEK_TASK_MoveTaskFromReadyTable(kernelPtr,runningTcb->curPriority);
			OSEK_TASK_ReleaseReadyBlock(kernelPtr,kernelPtr->osekTask_RunningTask);
			OSEK_TASK_ClearPriorityMap(kernelPtr,runningTcb->curPriority);

	#if defined(CONFIG_OSEK_SYSTEM_MACTIVE)
			runningTcb->curActiveNum-- ;
			// 如果任务的当前激活次数不为零，则将任务置为初始状态。
			if( runningTcb->curActiveNum != 0)
			{
				runningTcb->status = OSEK_TASK_FIRST_RUN | READY;
			}
			// 否则将任务置为结束的挂起状态
			else
			{
				runningTcb->status = SUSPENDED ;
			}
			// 恢复任务的优先级
			runningTcb->curPriority = runningTcb->configTable.basePriority ;

	#else /*  #if defined(CONFIG_OSEK_SYSTEM_MACTIVE) */

			// 任务不支持多次激活，被置为结束的挂起状态
			kernelPtr->osekTask_RunningTask->taskControlBlock->status = SUSPENDED ;
			// 一个优先级上只允许一个任务时，任务的优先级是通过其ID号转换的。
			runningTcb->curPriority = OSEK_TASK_ID2PRI(runningTcb->configTable.taskId);

	#endif /* #if defined(CONFIG_OSEK_SYSTEM_MACTIVE) */

			// 将需要激活的任务插入到就绪队列中，并累加其激活次数，设置其状态为就绪初始态。
			if( OSEK_TASK_AddaReadyBlock(kernelPtr,tcbPtr) == E_OS_LIMIT )
			{
				OSEK_TARGET_EnableOSInt(osIntSave);
				return E_OS_LIMIT;	// 没有空闲的就绪块可用
			}
	#if defined(CONFIG_OSEK_SYSTEM_EP)
			tcbPtr->curActiveNum++ ;
	#endif
			if( tcbPtr->status == SUSPENDED )
			{
				tcbPtr->status = OSEK_TASK_FIRST_RUN | READY;
			}
		}
	#if defined(CONFIG_OSEK_SYSTEM_ET)
		// 新激活的任务，其已设置的事件必需清零。
		tcbPtr->setEvent = 0;
	#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET) */

	   	osekTask_TerminateDispatch();
	}

	// 程序不会执行到这里，但增加该语句是避免编译器报警告
	return E_OK;

}


/*
 * @brief
 *    Schedule：重调度，主要用于任务主动放弃CPU允许其它高优先级任务运行。
 * @param[in]	无。
 * @returns		E_OK：成功。
 * <p>          [E_OS_RESOURCE: 还有资源（包括调度锁资源和中断资源）没有释放；]
 * <p>		    [E_OS_CALLEVEL: 在中断中调用的此函数；]
 *
 */
StatusType Schedule( void )
{
	OSEK_TARGET_OSIntSave( osIntSave ) ;
	T_OSEK_TASK_ControlBlock *tcbPtr;
	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr=&osekKernel_osKernel[core];
	//当前任务控制块指针
	tcbPtr = kernelPtr->osekTask_RunningTask->taskControlBlock;

	 /*  如果任务有占用的自旋锁没有释放*/
	if(tcbPtr->spinLockList != NULL)
	{
		OSEK_HOOK_ErrorHook(E_OS_SPINLOCK,OSServiceId_Schedule,0) ;
	}

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)

#if defined(CONFIG_OSEK_RESOURCE)
	// 如果有资源没有释放，返回E_OS_RESOURCE
	if( tcbPtr->resourceList != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_Schedule,0) ;
	}

	// 如果调度锁资源没有释放，返回E_OS_RESOURCE
	if( kernelPtr->osekTask_SchedulerLockLevel != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_Schedule,0) ;
	}
#if defined(CONFIG_OSEK_RESOURCE_ISR)
	// 如果中断资源没有释放，返回E_OS_RESOURCE
    if (kernelPtr->osekResource_ISRLast != 0)
    {
        OSEK_HOOK_ErrorHook(E_OS_RESOURCE,OSServiceId_Schedule,0) ;
    }
#endif  /* defined(CONFIG_OSEK_RESOURCE_ISR) */

#endif /* #if defined(CONFIG_OSEK_RESOURCE) */

	// 如果在中断中调用的此函数，返回E_OS_CALLEVEL
	if( kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_Schedule,0) ;
	}
#endif /* #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

	OSEK_TARGET_DisableOSInt(osIntSave);

	// 如果任务有内部资源，则先释放内部资源，实际上就是调整优先级
#if defined(CONFIG_OSEK_RESOURCE_INTERNAL)
#if defined(CONFIG_OSEK_SYSTEM_EP)
	if( tcbPtr->curPriority > tcbPtr->configTable.basePriority )
	{
		osekTask_ChangeRunningTaskPriority(kernelPtr,tcbPtr->configTable.basePriority);
	}
#else /* #if defined(CONFIG_OSEK_SYSTEM_EP) */
	if( tcbPtr->curPriority > OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId))
	{
		osekTask_ChangeRunningTaskPriority(kernelPtr,OSEK_TASK_ID2PRI(tcbPtr->configTable.taskId));
	}
#endif /* #else defined(CONFIG_OSEK_SYSTEM_EP) */
#endif /* #if defined(CONFIG_OSEK_RESOURCE_INTERNAL) */

	osekTask_Dispatch();

	OSEK_TARGET_EnableOSInt(osIntSave);

	return E_OK;
}




/*
 * @brief
 *    GetTaskState：获取指定任务的状态，状态包括RUNNING、WAITING、READY、SUSPENDED等。
 * @param[in]	taskId：任务ID号；
 *              state: 任务状态变量的引用，用于保存输出的任务状态。
 * @returns 	E_OK: 成功获取了任务状态；
 * <p>          [E_OS_ID：任务ID号无效。]
 *
 */
StatusType GetTaskState ( TaskType taskId, TaskStateRefType state )
{

	StatusType status;
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
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetTaskState,0) ;
	}
#endif /* #if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */


	//获取局部任务ID
	localTask=osekTask_tasksMap[taskId];
	//判断任务是否是本地任务
	if(core != GET_LOCAL_CORE_ID(localTask))
	{
		printf("state addr %d\n",state);
		status = rpc_getTaskState_stub(GET_LOCAL_TASK_ID(localTask),state,GET_LOCAL_CORE_ID(localTask));
		printf("task state in Get %d\n",*state);
		return status;
	}
	else
	{
		// 任务运行时，其记录状态的变量仍然保持为READY，因此此处将输出状态变量显式赋值为RUNNING。
		if( kernelPtr->osekTask_RunningTask->taskControlBlock->configTable.taskId == GET_LOCAL_TASK_ID(localTask) )
		{
			*state = RUNNING ;
		}
		else
		{
			*state = (kernelPtr->osTasks->osekTask_TaskTable[GET_LOCAL_TASK_ID(localTask)].status & OSEK_TASK_STATE_MASK);
		}
	}


	return E_OK;

}


/*
 * @brief
 *    GetTaskID：获取当前任务的ID号。
 * @param[in]	taskId：保存任务ID的变量的引用
 * @returns		E_OK: 成功获取了任务状态。
 *
 */
StatusType GetTaskID( TaskRefType taskId )
{
	//获取核心ID
	CoreIDType core=GetCoreID();
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);

	*taskId = (kernelPtr->osekTask_RunningTask)->taskControlBlock->configTable.taskId;
	if(*taskId == GET_IDLE_TASK_ID(kernelPtr))
	{
		*taskId = INVALID_TASK;
	}

	return E_OK;

}


/*
 * @brief
 *    osekTask_Initialize: 初始化任务管理，主要是根据任务的配置进行相关结构的初始化（内部函数）。
 * @param[in]	无
 * @returns		无
 *
 */
void osekTask_Initialize( void )
{
	OSDWORD i;
	T_OSEK_TASK_ControlBlock *tcbPtr;
	//获取核心ID
	CoreIDType core = GetCoreID();
		//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);
	T_OSEK_KERNEL_TaskControlRef taskControl = kernelPtr->osTasks;

	// 初始化调度锁
	kernelPtr->osekTask_SchedulerLockLevel = 0;

	// 初始化优先级位图
#if defined(CONFIG_OSEK_TARGET_HARDBITMAP)
	osekTask_PriorityBitMap1 = 0;
#if defined(CONFIG_OSEK_TASK_EXTPRIORS)
	osekTask_PriorityBitMap2 = 0;
#endif /* #if defined(CONFIG_OSEK_TASK_EXTPRIORS) */
#endif /* #if defined(CONFIG_OSEK_TARGET_HARDBITMAP) */

#if defined(CONFIG_OSEK_TARGET_SOFTBITMAP)

	memset(kernelPtr->osekTask_PriorityBitMap,0,sizeof(kernelPtr->osekTask_PriorityBitMap));
	kernelPtr->osekTask_PriorityBitMapMajor = 0;

#endif /* #if defined(CONFIG_OSEK_TARGET_SOFTBITMAP) */

	// 初始化任务控制结构表、就绪队列表、任务上下文、就绪块表。

/****************Error Here****************/
	memset(taskControl->osekTask_TaskTable,0,(taskControl->taskCnt) * sizeof(T_OSEK_TASK_ControlBlock));
	memset(taskControl->osekTask_ReadyTaskTable,0,(CONFIG_OSEK_TASK_PRIORITY_NUMBER) * sizeof(T_OSEK_TASK_ReadyTaskTableItem));
	memset(taskControl->osekTask_ContextTable,0,(taskControl->taskCnt) * sizeof(T_OSEK_TARGET_TaskContext));
	memset(taskControl->osekTask_ReadyBlock,0,(taskControl->taskCnt) * sizeof(T_OSEK_TASK_ReadyBlock));

	// 将空闲就绪块组织成链表
	for(i=0; i<((OSDWORD)(taskControl->taskCnt-1)); i++)
	{
		((taskControl->osekTask_ReadyBlock)[i]).nextTask = &(taskControl->osekTask_ReadyBlock[i+1]);
	}

	taskControl->osekTask_FreeReadyBlock = &(taskControl->osekTask_ReadyBlock[0]);

	// 根据配置信息初始化各个任务
	tcbPtr = (taskControl->osekTask_TaskTable);

	for( i = 0; i < ((OSDWORD)(taskControl->taskCnt)) ; i++ )
	{

		tcbPtr->configTable = (taskControl->taskCfgTable)[i];

		// 由于优先级越高的任务在配置结构中的相应值越小，必需调整一下。
#if defined(CONFIG_OSEK_SYSTEM_EP)
		tcbPtr->curPriority = tcbPtr->configTable.basePriority ;
#else /* #if defined(CONFIG_OSEK_SYSTEM_EP) */

		tcbPtr->curPriority = OSEK_TASK_ID2PRI((tcbPtr->configTable).taskId );
#endif /* #else defined(CONFIG_OSEK_SYSTEM_EP) */

		// 如果应用模式数大于1，则根据自动启动的配置来激活任务，否则根据任务的激活属性来判定是否激活任务。
#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
        if ((tcbPtr->configTable.autoStart & (1 << osekExecution_CurrentAppMode)) != 0 )
#else /* #if (CONFIG_OSEK_SYSTEM_APPMODES > 1) */
		if( (tcbPtr->configTable.property & OSEK_TASK_ACTIVE ) != 0 )
#endif /* #else (CONFIG_OSEK_SYSTEM_APPMODES > 1) */
		{
			OSEK_TASK_AddaReadyBlock(kernelPtr,tcbPtr);	// 需确保此步执行结果是OK的
#if defined(CONFIG_OSEK_SYSTEM_EP)
			tcbPtr->curActiveNum = 1;
#endif
			tcbPtr->status = OSEK_TASK_FIRST_RUN | READY;
		}
		else
		{
			tcbPtr->status = SUSPENDED ;
		}

		// 初始化任务上下文指针
		tcbPtr->context = (taskControl->osekTask_ContextTable + i);

		// 如果需要进行任务栈的检查，则对栈进行填充。
#if defined(CONFIG_OSEK_TASK_STACK_CHECK)
#if defined(CONFIG_OSEK_SYSTEM_ET)
		if( (tcbPtr->configTable.property & OSEK_TASK_EXTENDED ) != 0 )
		{
		//	memset(tcbPtr->configTable.stackBottom,OSEK_DEBUG_STACKPATTERN,(tcbPtr->configTable.stackTop - tcbPtr->configTable.stackBottom));
		}
#endif /* #if defined(CONFIG_OSEK_SYSTEM_ET) */
#endif /* #if defined(CONFIG_OSEK_TASK_STACK_CHECK) */

		tcbPtr++;
	}

	// 初始化IDLE任务。IDLE任务的配置必须是最后一项,且IDLE任务必须是基本任务。IDLE任务的状态只能是READY
	taskControl->osekTask_TaskTable[GET_IDLE_TASK_ID(kernelPtr)].status = READY;

}


