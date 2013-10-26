/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMALARM_H_
#define OSEKMALARM_H_

#include "osekOccConfig.h"
#include "stdTypes.h"
/**************************** 结构定义 *********************************/

/// ALARM 配置信息表结构
struct T_OSEK_ALARM_ConfigTable_Struct
{
	// 相关任务的ID
    TaskType		taskId;
    // ECC 支持事件机制
#if defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_ALARM_EVENT)
    // 如果setEvent==0 则alarm 激活任务,否则它为该任务设置事件（该属性在ECC情况下才存在）
    EventMaskType		setEvent;
#endif
    // 相关 Counter的ID
    CtrType		ctrId;
    // Alarm回调函数入口（该属性在配置了ALARM回调时才存在）
#if defined(CONFIG_OSEK_ALARM_CALLBACK)
    T_OSEK_ALARM_CallBackEntry		callBack;
#endif
};

/// ALARM 控制块结构
struct T_OSEK_ALARM_ControlBlock_Struct
{
    // 列表中的下一个 alarm
	T_OSEK_ALARM_ControlBlockRef	nextAlarm;
    // 列表中的前一个 alarm
	T_OSEK_ALARM_ControlBlockRef	prevAlarm;

    // 如果使用了扩展状态
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
	// ALARM ID（该属性在配置了扩展返回状态时才存在）
	AlarmType		alarmId;
#endif

	// ALARM 的到期时间值
	TickType		expirationTick;
	// 周期性 ALARM 的周期值
	TickType		periodTick;

	// ALARM 的配置信息表
	T_OSEK_ALARM_ConfigTable	configTable;
};

/// ALARM 自动类型配置信息表结构
struct T_ALARM_AutoTypeConfigTable_Struct
{
    // ALARM ID
	AlarmType	alarm;
    // ALARM 起始时间值
    TickType        startTick;
    // 周期性 ALARM 的周期值
    TickType        periodTick;

#if (CONFIG_OSEK_SYSTEM_APPMODES > 1)
    // ALARM 是否自动启动（该属性在应用模式大于1时才存在）
    OSDWORD         autoStart;
#endif  /* (CONFIG_OSEK_SYSTEM_APPMODES > 1) */
};


/**************************** 声明部分 *********************************/

// ALARM 管理模块对外接口

/**
 * @brief
 *    SetAbsAlarm：设置绝对时间ALARM。
 * @param[in]	almId: ALARM控制块的ID号。
 * @param[in]   start: 以tick为单位的绝对起始时间值。
 * @param[in]   cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @returns     E_OK：成功。
 * @returns     E_OS_STATE:指定的ALARM已经在用。
 * @returns     [E_OS_ID:无效的ALARM。]
 * @returns     [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */
StatusType SetAbsAlarm( AlarmType almId, TickType start, TickType cycle );

/**
 * @brief
 *    SetRelAlarm：设置相对时间ALARM。
 * @param[in]	almId: ALARM控制块的ID号。
 * @param[in]   increment: 以tick为单位的相对起始时间值。
 * @param[in]   cycle: 周期性ALARM的周期值。对于一次性的ALARM，该值必须为0。
 * @returns     E_OK：成功。
 * @returns     E_OS_STATE:指定的ALARM已经在用。
 * @returns     [E_OS_ID:无效的ALARM。]
 * @returns     [E_OS_VALUE:输入的起始时间和周期参数超出配置的范围。]
 */
StatusType SetRelAlarm( AlarmType almId, TickType increment, TickType cycle );

/**
 * @brief
 *    GetAlarm：获取指定ALARM到期前的相对时间。
 * @param[in]	almId: ALARM控制块的ID号。
 * @param[out]	tick:  相对时间值的引用。
 * @returns     E_OK：成功。
 * @returns     E_OS_NOFUNC:指定的ALARM未被使用。
 * @returns     [E_OS_ID:无效的ALARM。]
 */
StatusType GetAlarm( AlarmType almId, TickRefType tick );

/**
 * @brief
 *    GetAlarmBase：获取指定ALARM的基本信息。
 * @param[in]	almId: ALARM控制块的ID号。
 * @param[out]	info： ALARM基本信息结构的引用。
 * @returns     E_OK： 成功。
 * @returns     [E_OS_ID:无效的ALARM。]
 */
StatusType GetAlarmBase( AlarmType almId, AlarmBaseRefType info );

/**
 * @brief
 *    CancelAlarm：取消设置的ALARM。
 * @param[in]	almId: ALARM控制块的ID号。
 * @returns     E_OK：成功。
 * @returns     E_OS_NOFUNC:指定的ALARM未在使用。
 * @returns     [E_OS_ID:无效的输入参数。]
 */
StatusType CancelAlarm( AlarmType almId );


// ALARM 管理模块内部函数

//StatusType osAlarm_SetAbsAlarm( AlarmType almId, TickType start, TickType cycle );

//StatusType osAlarm_SetRelAlarm( AlarmType almId, TickType increment, TickType cycle );

//StatusType osAlarm_GetAlarm( AlarmType almId, TickRefType tick );

//StatusType osAlarm_GetAlarmBase( AlarmType almId, AlarmBaseRefType info );

//StatusType osAlarm_Cancel_Alarm( AlarmType almId );

void osekAlarm_Initialize( void );

StatusType SetAlarm(T_OSEK_ALARM_ControlBlockRef alarmPtr,T_OSEK_COUNTER_ControlBlockRef ctrPtr, TickType start, TickType cycle );
StatusType osekAlarm_Check2(T_OSEK_ALARM_ControlBlockRef  alarmPtr, T_OSEK_COUNTER_ControlBlock *ctrPtr );

#endif /* OSEKMALARM_H_ */
