/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: osekOccConfig.h,v $
 * Revision 1.0  2008/03/24 13:13:00
 * 创建文件
*/

/*
 * @file 	osekOccConfig.h
 * @brief
 *	<li>功能：将OCC产生的配置宏定义转换为DeltaOSEK内部需要的宏定义。</li>
 * @author
 * @date 	2008-03-24
 * <p>部门：
 */


#ifndef _OSEK_OCC_CONFIG_H
#define _OSEK_OCC_CONFIG_H

#include "osprop.h"

/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/


// 系统配置部分

#if defined(OCC_BCC1) || defined(OCC_BCC2)	// 系统中只允许有基本任务
#define CONFIG_OSEK_SYSTEM_BT
#endif

#if defined(OCC_ECC1) || defined(OCC_ECC2)	// 系统中允许有基本任务和扩展任务
#define CONFIG_OSEK_SYSTEM_ET
#endif

#if defined(OCC_BCC1) || defined(OCC_ECC1)	// 系统中只允许一个优先级上最多有一个任务
#define CONFIG_OSEK_SYSTEM_BP
#endif

#if defined(OCC_BCC2) || defined(OCC_ECC2)	// 系统中允许一个优先级上有多个任务，允许基本任务被多次激活
#define CONFIG_OSEK_SYSTEM_EP
#define CONFIG_OSEK_SYSTEM_MACTIVE
#endif


#if defined(OCC_EXTSTATUS)	// 允许系统有扩展信息返回
#define CONFIG_OSEK_SYSTEM_EXTSTATUS
#endif


#define CONFIG_OSEK_SYSTEM_APPMODES  OCC_NAPPMODES	// 应用模式数


#define CONFIG_OSEK_SYSTEM_RTI_LEVEL  OCC_ORTIDEBUGLEVEL	// 调试级别


#if defined(OCC_GETSERVICEID)	// 获取服务ID
#define CONFIG_OSEK_SYSTEM_GETSERVICEID
#endif

#if defined(OCC_PARAMETERACCESS)	// 获取访问参数
#define CONFIG_OSEK_SYSTEM_PARAMETERACCESS
#endif



// 任务配置部分
#if defined(OCC_NOPREEMPT) // 系统中只有非抢占的任务
#define CONFIG_OSEK_TASK_NOPREEMPT
#endif

#if defined(OCC_MIXPREEMPT) // 系统中有非抢占和抢占的任务
#define CONFIG_OSEK_TASK_MIXPREEMPT
#endif

#if defined(OCC_FULLPREEMPT) // 系统中都是允许抢占的任务
#define CONFIG_OSEK_TASK_FULLPREEMPT
#endif

#if defined(OCC_EXTPRIORS) // 表明支持64级优先级，如果未定义则表明支持32级优先级
#define CONFIG_OSEK_TASK_EXTPRIORS
#define CONFIG_OSEK_TASK_PRIORITY_NUMBER 64
#else
#define CONFIG_OSEK_TASK_PRIORITY_NUMBER 32
#endif


#define CONFIG_OSEK_TASK_NUMBER OCC_NTSKS	// 系统中的任务总数
#define CONFIG_OSEK_TASK_MAX_ID ((OSDWORD)(CONFIG_OSEK_TASK_NUMBER))  // 不包括IDLE任务在内的最大任务ID
#define OSEK_TASK_IDLE_ID ((OSDWORD)(CONFIG_OSEK_TASK_NUMBER - 1))	// IDLE任务的ID,不再使用
#define CONFIG_OSEK_TASK_TOTAL_ACTIVATE_NUMBER OCC_TOTALACTIVENUMBER	// 系统中最多的任务激活总次数


#if defined(OCC_STKCHECK)	// 堆栈检查
#define CONFIG_OSEK_TASK_STACK_CHECK
#endif

#define CONFIG_OSEK_TASK_STACK_BOTTOM



// 资源配置部分

#if defined(OCC_RESOURCE)	// 使用资源管理功能
#define CONFIG_OSEK_RESOURCE
#endif

#if defined(OCC_RESSCHEDULER)	// 使用了调度器资源
#define CONFIG_OSEK_RESOURCE_SCHEDULER
#endif

#if defined(OCC_INRES)	// 使用了内部资源
#define CONFIG_OSEK_RESOURCE_INTERNAL
#endif

#if defined(OCC_ISRRES)	// 使用了中断资源
#define CONFIG_OSEK_RESOURCE_ISR
#endif

#define CONFIG_OSEK_RESOURCE_NUMBER OCC_NALLRES	// 系统中的资源总数（不包括调度器资源）



// HOOK配置部分

#if defined(OCC_HOOKERROR) // 用户要使用Error hook
#define CONFIG_OSEK_HOOK_ERROR
#endif

#if defined(OCC_HOOKPRETASK) // 用户要使用任务进入时hook
#define CONFIG_OSEK_HOOK_PRETASK
#endif

#if defined(OCC_HOOKPOSTTASK) // 用户要使用任务退出前hook
#define CONFIG_OSEK_HOOK_POSTTASK
#endif

#if defined(OCC_HOOKSTARTUP) // 用户要使用系统启动的hook
#define CONFIG_OSEK_HOOK_STARTUP
#endif

#if defined(OCC_HOOKSHUTDOWN) // 用户要使用系统关闭的hook
#define CONFIG_OSEK_HOOK_SHUTDOWN
#endif

#if defined(OCC_HOOKIDLE) // 用户要使用系统空闲的hook
#define CONFIG_OSEK_HOOK_IDLE
#endif



// 中断配置部分
#if defined(OCC_ISR1)&& (!defined(OCC_ISR2))	// 系统中只有类型1的中断
#define CONFIG_OSEK_INTERRUPT_ONLY_ISR1
#endif

#if defined(OCC_ISR2)&& (!defined(OCC_ISR1))	// 系统中只有类型2的中断
#define CONFIG_OSEK_INTERRUPT_ONLY_ISR2
#endif

#if defined(OCC_ISR1)&& defined(OCC_ISR2)	// 系统中既有类型1又有类型2的中断
#define CONFIG_OSEK_INTERRUPT_ISR_ALL
#endif

#if (!defined(OCC_ISR1))&& (!defined(OCC_ISR2))	// 系统中没有中断
#define CONFIG_OSEK_INTERRUPT_ISR_NULL
#endif

#define CONFIG_OSEK_INTERRUPT_WITH_TYPE

#define CONFIG_OSEK_INTERRUPT_STACK_SIZE OCC_ISRSTACKSIZE	// 中断堆栈的尺寸

#define CONFIG_OSEK_INTERRUPT_ISRLEVEL_NUMBER OCC_ISRLEVELNUM	// 中断级数


// Alarm配置部分

#if defined(OCC_ALARM)	// 使用ALARM功能
#define CONFIG_OSEK_ALARM
#endif

#if defined(OCC_ALMSETEVENT)	// 使用ALARM的设置事件功能
#define CONFIG_OSEK_ALARM_EVENT
#endif

#if defined(OCC_ALMCALLBACK)	// 使用ALARM的回调函数
#define CONFIG_OSEK_ALARM_CALLBACK
#endif

#define CONFIG_OSEK_ALARM_NUMBER    OCC_NALMS	// 系统ALARM总数
#if defined(OCC_NAUTOALMS)
#define CONFIG_OSEK_ALARM_AUTO_NUMBER	OCC_NAUTOALMS	// 自动启动的ALARM总数
#endif



// COUNTER配置部分

#if defined(OCC_COUNTER)	// 使用了计数器功能
#define CONFIG_OSEK_COUNTER
#endif

#if defined(OCC_NCTRS)	// 系统中计数器的总数
#define CONFIG_OSEK_COUNTER_NUMBER  OCC_NCTRS
#endif


// 事件配置部分



// 通信配置部分

#if defined(OCC_USEMESSAGERESOURCE)	// 使用了通信资源
#define		CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE
#endif

#if defined(OCC_USEMESSAGESTATUS)	// 使用了消息状态
#define		CONFIG_OSEK_COM_USE_MESSAGE_STATUS
#endif

// 通信范围
#if defined(OCC_LOCALMESSAGESONLY)	// 只有内部通信
#define		CONFIG_OSEK_COM_LOCALMESSAGESONLY
#endif

// 消息通知机制
#if defined(OCC_SIGACTIVTASK)	// 配置了消息到达时激活任务
#define		CONFIG_OSEK_MSG_ACTIVATETASK
#endif

#if defined(OCC_SIGEVENT)	// 配置了消息到达时设置事件
#define		CONFIG_OSEK_MSG_SETEVENT
#endif

#if defined(OCC_SIGCALLBACK)	// 配置了消息到达时回调函数
#define		CONFIG_OSEK_MSG_CALLBACK
#endif

#if defined(OCC_SIGFLAG)	// 配置了消息到达时设置标志
#define		CONFIG_OSEK_MSG_FLAG
#endif


#if defined (CONFIG_OSEK_COM_LOCALMESSAGESONLY)

// 总消息数量, 配置COM组件服务的开关宏
#define CONFIG_OSEK_COM_NUMBER_OF_MESSAGES		OCC_NMSGS

// 标志数量, 配置控制块中是否有标志属性，以及配置通知机制相关的服务
#define CONFIG_OSEK_COM_NUMBER_OF_FLAGS			OCC_NFLAGS

// 队列型消息数量, 配置剪裁控制块结构
#define CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES	OCC_NQUEMSGS

#endif  /* CONFIG_OSEK_COM_LOCALMESSAGESONLY */



// 编译配置部分
#define  CONFIG_OSEK_COMPILER_CCS



// 目标机配置部分
#if defined(OCC_FLOATINGPOINT)	// 使用了浮点处理
#define CONFIG_OSEK_TARGET_FLOAT
#endif

// DSP Ti TMS320C6000目标机系列
#if defined(OCC_TMS320C6713)
#define CONFIG_OSEK_TARGET_DSP
#define CONFIG_OSEK_TARGET_TMS320C6000
#define CONFIG_OSEK_TARGET_SOFTBITMAP
#endif



#endif

