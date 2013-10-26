/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMCOUNTER_H_
#define OSEKMCOUNTER_H_

#include "osekOccConfig.h"
#include "stdTypes.h"

/**************************** 声明部分 *********************************/

// COUNTER管理提供给外部的公共调用函数

/**
 * @brief
 *    InitCounter：初始化COUNTER的计数值。
 * @param[in]	ctrId：COUNTER的ID号。
 * @param[in]	value：COUNTER的计数值。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的COUNTER ID号。]
 * @returns     [E_OS_VALUE:输入的value值超出范围。]
 * @returns     [E_OS_CALLEVEL:在中断中调用了此函数。]
 */
StatusType InitCounter( CtrType ctrId, TickType value );

/**
 * @brief
 *    GetCounterValue：获取指定 COUNTER 的计数值。
 * @param[in]	ctrId：COUNTER的ID号。
 * @param[out]	ticks：COUNTER 计数值变量的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的COUNTER ID号。]
 */
StatusType GetCounterValue( CtrType ctrId, TickRefType ticks );

/**
 * @brief
 *    CounterTrigger：触发指定的COUNTER。
 * @param[in]	ctrId：COUNTER 的 ID号。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的COUNTER ID号。]
 */
StatusType CounterTrigger( CtrType ctrId );

/**
 * @brief
 *    GetCounterInfo：获取指定 COUNTER 的基本信息。
 * @param[in]	ctrId：COUNTER 的ID号。
 * @param[out]	info： COUNTER 基本信息结构的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的 COUNTER ID号。]
 */
StatusType GetCounterInfo( CtrType ctrId, CtrInfoRefType info );

// 内部函数
void osekCounter_Initialize( void );


/**************************** 定义部分 *********************************/

/// COUNTER 配置表结构
struct T_OSEK_COUNTER_ConfigTable_Struct
{
    /// 允许的最大计数值
    TickType        maxallowedvalue;
    /// 转换常数：每时基的tick数
    TickType        ticksperbase;

// 如果使用了扩展状态
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    /// ALARM 的最小周期值（该属性在配置了扩展返回状态时才存在）
    TickType        mincycle;
#endif
};

/// COUNTER 控制块结构
struct T_OSEK_COUNTER_ControlBlock_Struct
{
#if defined(CONFIG_OSEK_ALARM)
    /// 指向对应的 ALARMs 的指针（该属性在配置了ALARM时才存在）
	T_OSEK_ALARM_ControlBlockRef    alarm;
#endif
    /// COUNTER 的当前计数值
    TickType            			currentTick;
    /// COUNTER 的配置表
    T_OSEK_COUNTER_ConfigTable    	configTable;
};

#endif /* OSEKMCOUNTER_H_ */
