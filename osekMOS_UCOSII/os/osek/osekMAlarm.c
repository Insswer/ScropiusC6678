/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#include "osapi.h"
#include "cfg.h"

/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

/**************************** 实现部分 *********************************/

/*
 * @brief
 *    GetAlarmBase：获取指定ALARM的基本信息。
 * @param
 * @param[in]	almId：指向ALARM控制块的指针。
 * @param[out]	info： ALARM基本信息结构的引用。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的ALARM。]
 */

StatusType GetAlarmBase( AlarmType almId, AlarmBaseRefType info )
{
	LocalAlarmType localAlarm;
	CtrType	ctrId;

	//获取核心数据
	CoreIDType core;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId >= CONFIG_ALARM_NUMBER )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarmBase,almId) ;
	}
#endif

	localAlarm = osekAlarm_alarmsMap[almId];
	core = GET_LOCAL_CORE_ID(localAlarm);

	// 通过almId找到相应counter的配置信息，并将其赋值给输出参数info
	ctrId=(osekKernel_osKernel[core].osAlarms)->osekAlarm_AlarmTable[GET_LOCAL_ALARM_ID(localAlarm)].configTable.ctrId;
	*info=(osekKernel_osKernel[core].osCounters)->osekCounter_CounterTable[ctrId].configTable;

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    GetAlarm：获取指定ALARM到期前的相对时间。
 * @param
 * @param[in]	almId: 指向ALARM控制块的指针。
 * @param[out]	tick:  相对时间值的引用。
 * @returns:    E_OK：成功。
 * <p>          E_OS_NOFUNC:指定的ALARM未被使用。
 * <p>          [E_OS_ID:无效的ALARM。]
 */

StatusType GetAlarm( AlarmType almId, TickRefType tick )
{
	LocalAlarmType 	localAlarm;
	StatusType 		status;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

	//获取核心数据
	CoreIDType core=GetCoreID();

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId >=CONFIG_ALARM_NUMBER )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarmBase,almId) ;
	}
#endif

	localAlarm=osekAlarm_alarmsMap[almId];
	if(core == GET_LOCAL_CORE_ID(localAlarm))
	{
		alarmPtr=&((osekKernel_osKernel[core].osAlarms)->osekAlarm_AlarmTable[GET_LOCAL_ALARM_ID(localAlarm)]);
		if(alarmPtr->prevAlarm == (T_OSEK_ALARM_ControlBlockRef)0)
		{
			OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_GetAlarm,almId);
		}

		// 获取指定ALARM所对应的counter
		ctrPtr=&((osekKernel_osKernel[core].osCounters)->osekCounter_CounterTable[alarmPtr->configTable.ctrId]);
		// 屏蔽中断
		OSEK_TARGET_DisableOSInt(osIntSave);
		// 获取计数器的当前计数值
		*tick=ctrPtr->currentTick;

		if(alarmPtr->expirationTick >= *tick)
		{
			*tick=alarmPtr->expirationTick - *tick;
		}
		else
		{
			*tick = alarmPtr->expirationTick+ctrPtr->configTable.maxallowedvalue + 1 - *tick;
		}
		// 打开中断
		OSEK_TARGET_EnableOSInt(osIntSave);
	}
	else
	{
		//发送请求到对应核心
		status = rpc_getAlarm_stub(GET_LOCAL_ALARM_ID(localAlarm),tick,GET_LOCAL_CORE_ID(localAlarm));
		return status;
	}

	// 成功，返回E_OK
	return E_OK;

}

/*
 * @brief
 *    SetAlarm：设置ALARM（内部函数）。
 * @param		almId: 指向ALARM控制块的指针。
 *              start: 以tick为单位的起始时间值。
 *              cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */
StatusType SetAlarm(T_OSEK_ALARM_ControlBlockRef alarmPtr,T_OSEK_COUNTER_ControlBlockRef ctrPtr, TickType start, TickType cycle )
{
	StatusType	status = E_OK;

	// 获取指定ALARM所对应的counter
	alarmPtr->expirationTick = start;
	alarmPtr->periodTick = cycle;

	alarmPtr->nextAlarm = ctrPtr->alarm;
	ctrPtr->alarm = alarmPtr;

	//Counter控制块中的第一个alarm的prevAlarm指向自己
	alarmPtr->prevAlarm = alarmPtr;

	if(alarmPtr->nextAlarm != (T_OSEK_ALARM_ControlBlockRef)0)
	{
		alarmPtr->nextAlarm->prevAlarm = alarmPtr;
	}

	if(start == ctrPtr->currentTick)
	{
		status = osekAlarm_Check2(alarmPtr,ctrPtr);
	}

	return status;
}
/*
 * @brief
 *    SetRelAlarm：设置相对时间ALARM。
 * @param		almId: 指向ALARM控制块的指针。
 *              increment: 以tick为单位的相对起始时间值。
 *              cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    E_OK：成功。
 * <p>          E_OS_STATE:指定的ALARM已经在用。
 * <p>          [E_OS_ID:无效的ALARM。]
 * <p>          [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */

StatusType  SetRelAlarm( AlarmType almId, TickType increment, TickType cycle )
{
	LocalAlarmType localAlarm;
	StatusType status;
	CtrType     ctrId;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;
	OSEK_TARGET_OSIntSave( osIntSave ) ;
	//获取核心数据
	CoreIDType core=GetCoreID();
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &(osekKernel_osKernel[core]);

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId >= CONFIG_ALARM_NUMBER )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarmBase,almId) ;
	}
#endif

	localAlarm = osekAlarm_alarmsMap[almId];
	if(core == GET_LOCAL_CORE_ID(localAlarm))
	{
		alarmPtr = (kernelPtr->osAlarms->osekAlarm_AlarmTable) + GET_LOCAL_ALARM_ID(localAlarm);
		ctrId = alarmPtr->configTable.ctrId;
		ctrPtr = (kernelPtr->osCounters->osekCounter_CounterTable) + ctrId;


	#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		// 如果起始时间值大于counter配置的最大值，返回E_OS_VALUE
		if( increment > ctrPtr->configTable.maxallowedvalue )
		{
			OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetRelAlarm,almId) ;
		}

		// 对于周期性ALARM，如果周期值大于counter配置的最大值或低于最小周期值，返回E_OS_VALUE
		if(( cycle != 0 ) && ( (cycle > ctrPtr->configTable.maxallowedvalue) || (cycle < ctrPtr->configTable.mincycle) ) )
		{
			OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetRelAlarm,almId) ;
		}
	#endif
		 // 如果指定ALARM已经在用，返回E_OS_STATE
		if(alarmPtr->prevAlarm != (T_OSEK_ALARM_ControlBlockRef)0)
		{
			OSEK_HOOK_ErrorHook(E_OS_STATE,OSServiceId_SetRelAlarm,almId) ;
		}


		// 屏蔽中断
	    OSEK_TARGET_DisableOSInt(osIntSave);

	    // 将相对值转换为绝对值
	    if((ctrPtr->configTable.maxallowedvalue - ctrPtr->currentTick) < increment)
	    {
	    	increment -= (ctrPtr->configTable.maxallowedvalue - ctrPtr->currentTick + 1);
	    }
	    else
	    {
	    	increment += ctrPtr->currentTick;
	    }

	    // 调用SetAlarm设置ALARM
		status = SetAlarm(alarmPtr,ctrPtr,increment,cycle);
	    if(status == OSEK_TASK_NEED_DISPATCH)
	    {
	    	osekTask_Dispatch();
	    }
		// 打开中断
		OSEK_TARGET_EnableOSInt(osIntSave);
	}
	else
	{
		//将请求发送到远程核心
		status = rpc_setRelAlarm_stub(GET_LOCAL_ALARM_ID(localAlarm),increment,cycle,GET_LOCAL_CORE_ID(localAlarm));
		return status;
	}

	// 成功，返回E_OK
	return E_OK;

}
/*
 * @brief
 *    SetAbsAlarm：设置绝对时间ALARM。
 * @param		almId: 指向ALARM控制块的指针。
 *              start: 以tick为单位的绝对起始时间值。
 *              cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    E_OK：成功。
 * <p>          E_OS_STATE:指定的ALARM已经在用。
 * <p>          [E_OS_ID:无效的ALARM。]
 * <p>          [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */

StatusType SetAbsAlarm(  AlarmType almId, TickType start, TickType cycle )
{
	LocalAlarmType 	localAlarm;
	StatusType 		status;
	CtrType    		ctrId;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;
	OSEK_TARGET_OSIntSave( osIntSave ) ;
	//获取核心数据
	CoreIDType core=GetCoreID();


#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId >=CONFIG_ALARM_NUMBER )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarmBase,almId) ;
	}
#endif

	localAlarm=osekAlarm_alarmsMap[almId];
	if(core == GET_LOCAL_CORE_ID(localAlarm))
	{

		alarmPtr = &((osekKernel_osKernel[core].osAlarms)->osekAlarm_AlarmTable[GET_LOCAL_ALARM_ID(localAlarm)]);
		ctrId = alarmPtr->configTable.ctrId;

		ctrPtr = &((osekKernel_osKernel[core].osCounters)->osekCounter_CounterTable[ctrId]);


	#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		// 如果起始时间值大于counter配置的最大值，返回E_OS_VALUE
		if( start > ctrPtr->configTable.maxallowedvalue )
		{
			OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetAbsAlarm,almId) ;
		}

		// 对于周期性ALARM，如果周期值大于counter配置的最大值或低于最小周期值，返回E_OS_VALUE
		if(( cycle != 0 ) && ( (cycle >  ctrPtr->configTable.maxallowedvalue) || (cycle <  ctrPtr->configTable.mincycle) ) )
		{
			OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_SetAbsAlarm,almId) ;
		}
	#endif

		if(alarmPtr->prevAlarm != (T_OSEK_ALARM_ControlBlockRef)0)
		{
			OSEK_HOOK_ErrorHook(E_OS_STATE,OSServiceId_SetAbsAlarm,almId) ;
		}
		// 屏蔽中断
	    OSEK_TARGET_DisableOSInt(osIntSave);

		// 调用SetAlarm设置ALARM
	    status = SetAlarm(alarmPtr,ctrPtr,start,cycle);
	    if(status == OSEK_TASK_NEED_DISPATCH)
	    {
	    	osekTask_Dispatch();
	    }
		// 打开中断
		OSEK_TARGET_EnableOSInt(osIntSave);
	}
	else
	{
		//将请求发送到远程核心
		status = rpc_setAbsAlarm_stub(GET_LOCAL_ALARM_ID(localAlarm),start,cycle,GET_LOCAL_CORE_ID(localAlarm));
		return status;
	}
	// 成功设置ALARM，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    CancelAlarm：取消设置的ALARM。
 * @param		almId: 指向ALARM控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    E_OK：成功。
 * <p>          E_OS_NOFUNC:指定的ALARM未在使用。
 * <p>          [E_OS_ID:无效的输入参数。]
 */

StatusType CancelAlarm( AlarmType almId )
{

	LocalAlarmType 	localAlarm;
	StatusType		status;
	CtrType     	ctrId;
	T_OSEK_ALARM_ControlBlockRef alarmPtr;
	T_OSEK_COUNTER_ControlBlockRef ctrPtr;
	OSEK_TARGET_OSIntSave( osIntSave ) ;
	//获取核心数据
	CoreIDType core=GetCoreID();

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 检查输入的参数almId是否有效，无效返回E_OS_ID
	if( almId >= CONFIG_ALARM_NUMBER )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetAlarmBase,almId) ;
	}
#endif

	localAlarm=osekAlarm_alarmsMap[almId];
	if(core == GET_LOCAL_CORE_ID(localAlarm))
	{
		// 屏蔽中断
		OSEK_TARGET_DisableOSInt(osIntSave);
		//获取Alarm控制块指针
		alarmPtr=&((osekKernel_osKernel[core].osAlarms)->osekAlarm_AlarmTable[GET_LOCAL_ALARM_ID(localAlarm)]);

		if(alarmPtr->prevAlarm == (T_OSEK_ALARM_ControlBlockRef)0)
		{
			OSEK_TARGET_EnableOSInt(osIntSave);
			OSEK_HOOK_ErrorHook(E_OS_NOFUNC,OSServiceId_CancelAlarm,almId) ;
		}

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

		// 打开中断
		OSEK_TARGET_EnableOSInt(osIntSave);
	}
	else
	{
		status = rpc_cancelAlarm_stub(GET_LOCAL_ALARM_ID(localAlarm),GET_LOCAL_CORE_ID(localAlarm));
		return status;
	}

	// 成功将指定的ALARM取消，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    NotifyAlarm：根据ALARM的配置设置事件或激活任务（内部函数）。
 * @param		almId: 指向ALARM控制块的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 设置事件或激活任务的返回值。
 */

static StatusType NotifyAlarm(T_OSEK_ALARM_ControlBlockRef alarmPtr)
{
	T_OSEK_TASK_ControlBlock *tcbPtr;
	//获取核心数据
	CoreIDType core=GetCoreID();
	T_OSEK_KERNEL_OsKernelRef kernelPtr=&osekKernel_osKernel[core];

	// 通过almId获取相应的任务控制块
	tcbPtr = &(kernelPtr->osTasks->osekTask_TaskTable[alarmPtr->configTable.taskId]);

#if defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_ALARM_EVENT)
	// 如果需要设置事件，则调用事件管理的设置事件功能，并返回结果
	if( alarmPtr->configTable.setEvent)
	{
 		return osekEvent_SetEvent(kernelPtr,tcbPtr,alarmPtr->configTable.setEvent);
	}
	else
#endif
	// 如果不需要设置事件，则激活任务，并返回结果
	{
		return osekTask_ActiveTask(kernelPtr,tcbPtr);
	}

}


/*
 * @brief
 *    osekAlarm_Check1：检查指定counter的ALARM链表，对满足触发时间条件的ALARM进行相应的处理（callback、设置事件或激活任务），
 *                     对于非周期ALARM将其从链表中去掉，对于周期ALARM调整其下一次触发的时间。（内部函数）
 * @param		ctrId:counter ID号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */

void osekAlarm_Check1( CtrType ctrId )
{

    OSBYTE      dispatchFlag = 0;
    T_OSEK_ALARM_ControlBlockRef alarmPtr;
    T_OSEK_COUNTER_ControlBlock *ctrPtr;
    StatusType  status = 0;
	//获取核心数据
	CoreIDType core = GetCoreID();
	// 获取指定计数器的控制块
	ctrPtr=&((osekKernel_osKernel[core].osCounters)->osekCounter_CounterTable[ctrId]);

    alarmPtr = ctrPtr->alarm;


	// 遍历指定counter的ALARM链表
    while (alarmPtr != (T_OSEK_ALARM_ControlBlockRef)0 )
    {
		// 检查alarm是否到期并做相应处理
		status = osekAlarm_Check2(alarmPtr, ctrPtr);
	    // 判断status的值，如果需要调度，则设置调度标志为1
        if( status == OSEK_TASK_NEED_DISPATCH )
		{
			dispatchFlag = 1;
		}
		// 得到链表中的下一个ALARM
        alarmPtr = alarmPtr->nextAlarm;
    }

	// 如果调度标志不为0，则实施调度
    if( dispatchFlag != 0 )
    {
    	osekTask_Dispatch();
    }

}
/*
 * @brief
 *    osekAlarm_Check2：检查指定的单个ALARM，如果满足触发时间则进行相应的处理（callback、设置事件或激活任务），
 *                     对于非周期ALARM将其从链表中去掉，对于周期ALARM调整其下一次触发的时间。（内部函数）
 * @param
 * @param[in]	almId:ALARM ID号,实际为指向ALARM控制结构的指针；
 *              ctrPtr:与该ALARM相关联的COUNTER控制块指针。
 * @param[out]	无。
 * @returns: status: 是否需要调度。
 */
StatusType osekAlarm_Check2(T_OSEK_ALARM_ControlBlockRef  alarmPtr, T_OSEK_COUNTER_ControlBlock *ctrPtr )
{
    StatusType  status;

	status = (StatusType)0;

	// 如果ALARM到期
	if ( alarmPtr->expirationTick == ctrPtr->currentTick )
    {
#if defined(CONFIG_OSEK_ALARM_CALLBACK)
		// 如果设置了callback函数，则调用它
        if( alarmPtr->configTable.callBack )
        {
            alarmPtr->configTable.callBack();
        }
        // 否则设置事件或激活任务
		else
        {
#endif
           status = NotifyAlarm( alarmPtr );
#if defined(CONFIG_OSEK_ALARM_CALLBACK)
        }
#endif
        if(alarmPtr->periodTick == 0)
        {
        	if (alarmPtr->prevAlarm == alarmPtr)
        	{
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

        }
        // 否则对于周期性ALARM，重新设置其下一次触发的时间
		else
        {
            alarmPtr->expirationTick = ctrPtr->currentTick + alarmPtr->periodTick;

            if (alarmPtr->expirationTick > ctrPtr->configTable.maxallowedvalue )
            {
                alarmPtr->expirationTick -= (ctrPtr->configTable.maxallowedvalue + 1);
            }
        }
    }

	return status;
}


/*
 * @brief
 *    osekAlarm_Initialize：根据ALARM配置信息表等对ALARM控制块进行初始化（内部函数）。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns: 无。
 */
// 该函数中如果启动自动类型ALARM失败，应有返回值。
void osekAlarm_Initialize( void )
{
	int i;

	//获取核心数据
	CoreIDType core = GetCoreID();
	T_OSEK_KERNEL_AlarmControlRef osAlarms = osekKernel_osKernel[core].osAlarms;

#if defined(CONFIG_OSEK_ALARM_AUTO_NUMBER)
	T_OSEK_ALARM_AutoTypeConfigTable *autoAlmPtr;
#endif

	// 初始化ALARM控制块表
	/*********************这部分的初始化有问题,还没有修改*****************************/
	/*
	 * 问题1:每个CONFIG_OSEK_ALARM_NUMBER在不同核心上的数应该不同,因此需要变量记录当前核心上Alarm数以及自启动Alarm数
	 * 问题2:memset只设置了一个?
	 */
	//memset((void*)osekAlarm_AlarmTable,0,sizeof(osekAlarm_AlarmTable));

	memset(osAlarms->osekAlarm_AlarmTable,0,(osAlarms->almCnt) * sizeof(T_OSEK_ALARM_ControlBlock));

	for( i = 0; i < osAlarms->almCnt; i++)
	{
		//
		(osAlarms->osekAlarm_AlarmTable[i]).configTable = (osAlarms->osekConfig_AlarmTable[i]);

		//在多核中alarmID的实际意义不大
		#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
		osAlarms->osekAlarm_AlarmTable[i].alarmId = (AlarmType)i;
		#endif

	}
	/**************************************************/

#if defined(CONFIG_OSEK_ALARM_AUTO_NUMBER)
	// 设置需自动启动的ALARM
	for( i = 0; i < osAlarms->autoAlmCnt; i++)
	{
		autoAlmPtr = &(osAlarms->osekConfig_AutoAlarmTable[i]);

#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
        if((autoAlmPtr->autoStart & (1 << osekExecution_CurrentAppMode)) != 0 )
		{
#endif  /* (CONFIG_OSEK_SYSTEM_APPMODES > 1) */

			SetAbsAlarm( autoAlmPtr->alarmId, autoAlmPtr->startTick, autoAlmPtr->periodTick );

#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
		}
#endif  /* (CONFIG_OSEK_SYSTEM_APPMODES > 1) */

	}

#endif  /* defined(CONFIG_OSEK_ALARM_AUTO_NUMBER) */

}

