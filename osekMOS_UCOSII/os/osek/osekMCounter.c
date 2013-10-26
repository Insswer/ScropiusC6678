/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#include "stdTypes.h"
#include "osapi.h"


/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

// 全局变量
// COUNTER控制块表


/**************************** 实现部分 *********************************/

/*
 * @brief
 *    InitCounter：初始化COUNTER的计数值。
 * @param
 * @param[in]	ctrId：COUNTER的ID号。
 *          	value：COUNTER的计数值。
 * @param[out]  无。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的COUNTER ID号。]
 * <p>          [E_OS_VALUE:输入的value值超出范围。]
 * <p>          [E_OS_CALLEVEL:在中断中调用了此函数。]
 */

StatusType InitCounter( CtrType ctrId, TickType value )
{
	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr=&osekKernel_osKernel[core];
	T_OSEK_COUNTER_ControlBlockRef counterPtr;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果COUNTER ID超出范围，返回E_OS_ID
	if( ctrId > (kernelPtr->osCounters->counterCnt - 1) )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_InitCounter,ctrId) ;
	}
	counterPtr = &(kernelPtr->osCounters->osekCounter_CounterTable[ctrId]);

	// 如果输入的计数值超出配置的最大值，返回E_OS_VALUE
	if( value > counterPtr->configTable.maxallowedvalue )
	{
		OSEK_HOOK_ErrorHook(E_OS_VALUE,OSServiceId_InitCounter,ctrId) ;
	}

	// 如果在中断中调用此函数，返回E_OS_CALLEVEL
	if( kernelPtr->osekInterrupt_NestedLevl != 0 )
	{
		OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_InitCounter,ctrId) ;
	}
#endif

	// 用输入的计数值设置COUNTER的当前计数值
	counterPtr->currentTick = value ;

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    GetCounterValue：获取指定 COUNTER 的计数值。
 * @param
 * @param[in]	ctrId：COUNTER的ID号。
 * @param[out]	ticks：COUNTER 计数值变量的引用。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的COUNTER ID号。]
 */


/*
 * 获取本地核心上Counter的计数值,函数只能在本地核心调用.
 * ctrId只能是本地Counter的ID
 */
StatusType GetCounterValue( CtrType ctrId, TickRefType ticks )
{
	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr=&osekKernel_osKernel[core];
	T_OSEK_COUNTER_ControlBlockRef counterPtr;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果COUNTER ID超出范围，返回E_OS_ID
	if( ctrId > (kernelPtr->osCounters->counterCnt - 1) )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetCounterValue,ctrId) ;
	}
#endif

	counterPtr = &(kernelPtr->osCounters->osekCounter_CounterTable[ctrId]);

	// 读取指定计数器的当前计数值到输出参数中
	*ticks =  counterPtr->currentTick ;

	return E_OK;

}


/*
 * @brief
 *    CounterTrigger：触发指定的COUNTER。
 * @param
 * @param[in]	ctrId：COUNTER 的 ID号。
 * @param[out]	无。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的COUNTER ID号。]
 */

StatusType CounterTrigger( CtrType ctrId )
{
	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];

	T_OSEK_COUNTER_ControlBlockRef ctrPtr;
	OSEK_TARGET_OSIntSave( osIntSave ) ;

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果使用了扩展状态，判断COUNTER ID是否超出范围，如是返回E_OS_ID
	if( ctrId > (kernelPtr->osCounters->counterCnt - 1) )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_CounterTrigger,ctrId) ;
	}
#endif

	//获取对应Counter的指针
	ctrPtr = (kernelPtr->osCounters->osekCounter_CounterTable + ctrId);

	OSEK_TARGET_DisableOSInt(osIntSave);

	// 如果COUNTER 当前计数值等于配置的最大计数值，则将计数值复位至0
	/****目前configTable未使用指针,使用.而不是->****/
    if( ctrPtr->currentTick == ctrPtr->configTable.maxallowedvalue )
    {
        ctrPtr->currentTick = 0;
    }
	// 否则COUNTER 当前计数值加1
    else
    {
        ctrPtr->currentTick++;
    }

#if defined(CONFIG_OSEK_ALARM)
	// 检查是否有ALARM 到期并做相应处理（调用回调函数、设置事件或激活任务）
    osekAlarm_Check1( ctrId );
#endif

    OSEK_TARGET_EnableOSInt(osIntSave);

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    GetCounterInfo：获取指定 COUNTER 的基本信息。
 * @param
 * @param[in]	ctrId：COUNTER 的ID号。
 * @param[out]	info： COUNTER 基本信息结构的引用。
 * @returns:    E_OK： 成功。
 * <p>          [E_OS_ID:无效的 COUNTER ID号。]
 */

StatusType GetCounterInfo( CtrType ctrId, CtrInfoRefType info )
{
	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr=&osekKernel_osKernel[core];
	T_OSEK_COUNTER_ControlBlockRef counterPtr;

// 如果使用了扩展状态
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// 如果COUNTER ID超出范围，返回E_OS_ID
	if( ctrId > (kernelPtr->osCounters->counterCnt - 1) )
	{
		OSEK_HOOK_ErrorHook(E_OS_ID,OSServiceId_GetCounterInfo,ctrId) ;
	}
#endif

	counterPtr = &(kernelPtr->osCounters->osekCounter_CounterTable[ctrId]);
	// 获取计数器的配置信息到输出参数中
	//这里configTable未使用指针
	*info =  counterPtr->configTable;

	// 成功，返回E_OK
	return E_OK;

}


/*
 * @brief
 *    osekCounter_Initialize：根据配置信息初始化COUNTER 控制表（内部函数）。
 * @param
 * @param[in]	无。
 * @param[out]	无。
 * @returns:    无。
 */

void osekCounter_Initialize( void )
{
	int i;

	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_CounterControlRef osCounters = osekKernel_osKernel[core].osCounters;

	//如何初始化osekCounter_CounterTable这个指针
	//memset((void*)osekCounter_CounterTable,0,sizeof(osekCounter_CounterTable));
	memset(osCounters->osekCounter_CounterTable,0,(osCounters->counterCnt)*(sizeof(T_OSEK_COUNTER_ControlBlock)));

	for( i = 0; i < osCounters->counterCnt; i++)
	{
		//初始化Counter的配置结构体
		(osCounters->osekCounter_CounterTable[i]).configTable = osCounters->osekConfig_CounterTable[i];
	}

}
