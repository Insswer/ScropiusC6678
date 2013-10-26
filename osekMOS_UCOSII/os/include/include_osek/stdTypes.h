/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef STDTYPES_H_
#define STDTYPES_H_

#include "c66xx_mpu.h"
/***********/



/*****操作系统API返回状态宏定义******/
/// 操作系统API调用返回状态 0：操作成功，用于OSEK的各个组件中
#define E_OK  			( (StatusType)0 )

// 1...16：OS部分的错误码区域
/// 操作系统API调用返回状态 :访问权限错误
#define E_OS_ACCESS 	( (StatusType)1 )
/// 操作系统API调用返回状态 :调用层次错误
#define E_OS_CALLEVEL 	( (StatusType)2 )
/// 操作系统API调用返回状态 :对象ID错误
#define E_OS_ID 		( (StatusType)3 )
/// 操作系统API调用返回状态 :超出范围错误
#define E_OS_LIMIT 		( (StatusType)4 )
/// 操作系统API调用返回状态 :没有此功能
#define E_OS_NOFUNC 	( (StatusType)5 )
/// 操作系统API调用返回状态 :资源未释放
#define E_OS_RESOURCE 	( (StatusType)6 )
/// 操作系统API调用返回状态 :状态错误
#define E_OS_STATE 		( (StatusType)7 )
/// 操作系统API调用返回状态 :数值错误
#define E_OS_VALUE 		( (StatusType)8 )
///操作系统API调用返回状态 :自旋锁未释放
#define E_OS_SPINLOCK	( (StatusType)9 )

// 32...63：COM部分的错误码区域
/// 操作系统API调用返回状态 :消息对象“忙”
#define E_COM_BUSY 		( (StatusType)(1 + 32) )
/// 操作系统API调用返回状态 :无效的消息对象
#define E_COM_ID 		( (StatusType)(2 + 32) )
/// 操作系统API调用返回状态 :消息队列有溢出
#define E_COM_LIMIT 	( (StatusType)(3 + 32) )
/// 操作系统API调用返回状态 :消息对象被锁定
#define E_COM_LOCKED 	( (StatusType)(4 + 32) )
/// 操作系统API调用返回状态 :消息队列中无消息
#define E_COM_NOMSG 	( (StatusType)(5 + 32) )

// 17...31: 实现自定义区域
/// 操作系统API调用返回状态 :内部堆栈溢出
#define E_OS_SYS_STACK      ( (StatusType)17 )
/// 操作系统API调用返回状态 :不正确的功能调用顺序
#define E_OS_SYS_ORDER      ( (StatusType)18 )
/// 操作系统API调用返回状态 :主堆栈溢出
#define E_OS_SYS_MAINSTACK  ( (StatusType)19 )
/// 操作系统API调用返回状态 :中断栈溢出
#define E_OS_SYS_ISRSTACK   ( (StatusType)20 )



/*****************任务属性宏定义******************/
/// 任务属性：在系统启动时激活任务
#define OSEK_TASK_ACTIVE     ( (OSBYTE)0x08 )
/// 任务属性：非抢占任务
#define OSEK_TASK_NONPREEMPT ( (OSBYTE)0x40 )
/// 任务属性：扩展任务
#define OSEK_TASK_EXTENDED   ( (OSBYTE)0x80 )

/// 任务状态：运行状态
#define RUNNING              ((TaskStateType)0)
/// 任务状态：等待状态
#define WAITING              ((TaskStateType)1)
/// 任务状态：就绪状态
#define READY                ((TaskStateType)2)
/// 任务状态：挂起状态
#define SUSPENDED            ((TaskStateType)3)
/// 任务第一次运行
#define OSEK_TASK_FIRST_RUN ((TaskStateType)4)
/// 任务状态掩码
#define OSEK_TASK_STATE_MASK (((TaskStateType)3))
/// 获取栈顶位置
#define OSEK_TASK_GETTOPOFSTACK(addr, size)  ((addr) + ((size)-0x20))
/// 需要执行调度
#define OSEK_TASK_NEED_DISPATCH      ((StatusType)0xFF)

/// 无效的任务ID
#define INVALID_TASK ((OSDWORD)0xFFFFFFFF)
/// 空指针宏定义
#define NULLPTR  ((void *)0)

/*************************重定义数据类型************************************/
/******定义系统基本数据类型******/
/// OSBYTE类型定义
typedef unsigned char       OSBYTE;
/// OSBYTEPTR类型定义
typedef unsigned char      *OSBYTEPTR;
/// OSWORD类型定义
typedef unsigned short      OSWORD;
/// OSWORDPTR类型定义
typedef unsigned short      *OSWORDPTR;
/// OSDWORD类型定义
typedef unsigned int        OSDWORD;
typedef unsigned char       OSCARDINAL;
/// OSSIGNEDDWORD类型定义
typedef signed int          OSSIGNEDDWORD;
/// OSPRIOTYPE类型定义
typedef signed short         OSPRIOTYPE;
/// OSTASKTYPE类型定义
typedef OSDWORD             OSTASKTYPE;

/*************************Kernel相关数据结构************************************/
/*全局任务/Alarm到局部任务/ALARM转换相关宏*/

#define GET_LOCAL_CORE_ID(obj)			((OSBYTE)(obj & 7))
#define GET_GLOBAL_OBJ(coreId,objId)	((objId<<3)|coreId)

#define GET_LOCAL_TASK_ID(task)			(task >> 3)
#define GET_LOCAL_ALARM_ID(alarm)		(alarm >> 3)

/* get local core id */
#define GetCoreID()		BSP_GetCoreId()

//定义核心运行状态
///运行状态
#define CORE_RUNNING	((OSBYTE)0)
///停止状态
#define	CORE_STOPPED	((OSBYTE)1)
/*核心运行状态*/
typedef OSBYTE	OsCoreState;
//任务映射表
typedef OSDWORD OsTaskMapType;
//任务映射表项:高三位为所在核心ID,低29位为对应本地任务ID
typedef OSDWORD LocalTaskType;
//Alarm映射表
typedef OSDWORD OsAlarmMapType;
//Alarm映射表项:高三位为所在核心ID,低29位为对应本地AlarmID
typedef OSDWORD LocalAlarmType;

/*核心ID类型*/
typedef OSBYTE	CoreIDType;
typedef struct T_OSEK_KERNEL_TaskControl_struct T_OSEK_KERNEL_TaskControl;
typedef T_OSEK_KERNEL_TaskControl *T_OSEK_KERNEL_TaskControlRef;

typedef struct T_OSEK_KERNEL_CounterControl_struct T_OSEK_KERNEL_CounterControl;
typedef T_OSEK_KERNEL_CounterControl *T_OSEK_KERNEL_CounterControlRef;

typedef struct T_OSEK_KERNEL_AlarmControl_struct T_OSEK_KERNEL_AlarmControl;
typedef T_OSEK_KERNEL_AlarmControl *T_OSEK_KERNEL_AlarmControlRef;

typedef struct T_OSEK_KERNEL_OsKernel_struct T_OSEK_KERNEL_OsKernel;
typedef T_OSEK_KERNEL_OsKernel *T_OSEK_KERNEL_OsKernelRef;

/* ********************************************自旋锁相关数据结构定义******************************************** */
#define	TRYTOGETSPINLOCK_SUCCESS	((OSWORD)0)
#define	TRYTOGETSPINLOCK_NOSUCCESS	((OSWORD)1)
typedef OSWORD	SpinlockIdType ;
typedef OSWORD TryToGetSpinlockType;
typedef struct T_OSEK_SPINLOCK_Lock_struct	T_OSEK_SPINLOCK_Lock;
typedef T_OSEK_SPINLOCK_Lock	*T_OSEK_SPINLOCK_LockRef;


/******定义状态数据类型******/
/// AppModeType类型定义
typedef OSBYTE				AppModeType;
/// StatusType类型定义
typedef unsigned char   	StatusType;


/******定义任务相关基本数据类型******/
/// TaskType类型定义
typedef OSDWORD				TaskType;
/// TaskRefType类型定义
typedef TaskType			*TaskRefType;
/// TaskStateType类型定义
typedef OSBYTE				TaskStateType;
/// TaskStateRefType类型定义
typedef TaskStateType		*TaskStateRefType;

/******定义事件相关基本数据类型******/
/// EventMaskType类型定义
typedef OSDWORD			EventMaskType;
/// EventMaskRefType类型定义
typedef EventMaskType	*EventMaskRefType;

typedef OSDWORD			IntMaskType;

/// OSCALLBACK类型定义
typedef void          (*OSCALLBACK)( void );

/******定义COM相关基本数据类型******/
// COM特定的数据类型定义
/// FlagType类型定义
typedef unsigned char      *FlagType;
/// FlagValue类型定义
typedef unsigned char       FlagValue;
/// 消息对象ID的类型定义
typedef OSBYTE              OSMSGIDTYPE;
/// AccessNameRef类型定义：消息数据域的地址
typedef void*               AccessNameRef;
/// 标志未被设置
#define FALSE         ((FlagValue)0)
/// 标志被设置
#define TRUE          ((FlagValue)1)


#define TASK( TaskName ) void Func##TaskName( void )
/// T_OSEK_TASK_Entry类型定义
typedef void (*T_OSEK_TASK_Entry)( void );  // 任务函数入口


/// 任务函数声明的定义
#define DeclareTask(TaskID)     TASK(TaskID)
/// 中断处理函数声明的定义
#define DeclareISR(isrName)     void isrName(void)


// 资源管理控制结构的相关类型定义
/// ResourceType类型定义
typedef struct T_OSEK_RESOURCE_ControlBlock_Struct *ResourceType;
/// T_OSEK_RESOURCE_ControlBlock类型定义
typedef struct T_OSEK_RESOURCE_ControlBlock_Struct T_OSEK_RESOURCE_ControlBlock;

/// T_OSEK_TARGET_TaskContext（任务上下文结构）类型重定义
typedef struct T_OSEK_TARGET_TaskContext_struct T_OSEK_TARGET_TaskContext;
/// T_OSEK_TARGET_ExceptionFrame（X86的异常框架结构）类型重定义
typedef struct T_OSEK_TARGET_ExceptionFrame_struct T_OSEK_TARGET_ExceptionFrame;

/// T_OSEK_TASK_ControlBlock（任务管理控制结构类型）重定义
typedef struct T_OSEK_TASK_ControlBlock_Struct T_OSEK_TASK_ControlBlock;
typedef struct T_OSEK_TASK_ReadyBlock_Struct T_OSEK_TASK_ReadyBlock;

/// T_OSEK_TASK_ConfigTable（任务配置表结构）类型重定义
typedef struct T_OSEK_TASK_ConfigTable_Struct T_OSEK_TASK_ConfigTable;

/// T_OSEK_MESSAGE_ControlBlock（消息控制结构）类型重定义
typedef struct T_OSEK_MESSAGE_ControlBlock_Struct  T_OSEK_MESSAGE_ControlBlock;
/// MsgType类型定义
typedef T_OSEK_MESSAGE_ControlBlock*        MsgType;

/// T_OSEK_MESSAGE_ConfigTable（消息配置表结构）类型重定义
typedef struct T_OSEK_MESSAGE_ConfigTable_Struct  T_OSEK_MESSAGE_ConfigTable;

/**************************** Counter模块类型定义  *********************************/
/// TickType类型定义
typedef OSDWORD             TickType;
/// TickRefType类型定义
typedef TickType*           TickRefType;

/// CtrType（Counter ID）类型定义
typedef unsigned char       CtrType;

/// CtrInfoType（Counter特性即配置信息）类型重定义
typedef struct T_OSEK_COUNTER_ConfigTable_Struct     CtrInfoType;
// CtrInfoRefType（指向Counter特性信息的指针）类型定义
typedef CtrInfoType*        CtrInfoRefType;

/// T_OSEK_COUNTER_ControlBlock（COUNTER管理控制结构）类型重定义
typedef struct T_OSEK_COUNTER_ControlBlock_Struct  T_OSEK_COUNTER_ControlBlock;
typedef T_OSEK_COUNTER_ControlBlock *T_OSEK_COUNTER_ControlBlockRef;

/// T_OSEK_COUNTER_ConfigTable（COUNTER配置表结构）类型重定义
typedef struct T_OSEK_COUNTER_ConfigTable_Struct T_OSEK_COUNTER_ConfigTable;


/**************************** Alarm模块类型定义  *********************************/
// AlarmType类型定义
typedef OSDWORD 		AlarmType;
// AlarmTypeRef类型定义
typedef AlarmType		*AlarmRefType;

// AlarmBaseType（Counter配置表信息）类型定义
typedef T_OSEK_COUNTER_ConfigTable            		AlarmBaseType;
typedef AlarmBaseType								*AlarmBaseRefType;


/// T_OSEK_ALARM_ControlBlock（ALARM管理控制结构）类型重定义
typedef struct T_OSEK_ALARM_ControlBlock_Struct T_OSEK_ALARM_ControlBlock;
typedef T_OSEK_ALARM_ControlBlock* T_OSEK_ALARM_ControlBlockRef;

/// T_OSEK_ALARM_ConfigTable（ALARM配置表结构）类型重定义
typedef struct T_OSEK_ALARM_ConfigTable_Struct T_OSEK_ALARM_ConfigTable;

/// T_OSEK_ALARM_AutoTypeConfigTable（自动类型ALARM配置表结构）类型重定义
typedef struct T_ALARM_AutoTypeConfigTable_Struct T_OSEK_ALARM_AutoTypeConfigTable;


// 根据测试集的ext_bcc1_full_alarm_t2用例增加的
/// 报警回调函数原型
#define ALARMCALLBACK(alarmCallback)  void alarmCallback( void )
/// ALARM回调函数入口类型定义
typedef void          (*T_OSEK_ALARM_CallBackEntry)( void );

/// OSSERVICEIDTYPE类型定义
typedef OSBYTE  OSSERVICEIDTYPE;


/**************************** 核心ID  *********************************/
//核心ID定义
#define OS_CORE_ID_MASTER 	0
#define OS_CORE_ID_0		1
#define OS_CORE_ID_1		2
#define OS_CORE_ID_2		3
#define OS_CORE_ID_3		4
#define OS_CORE_ID_4		5
#define OS_CORE_ID_5		6
#define OS_CORE_ID_6		7


#define	UC_CORE				2

//定义核心状态
#define CORE_STATUS_WAIT		0
#define CORE_STATUS_RUN			1
#define CORE_STATUS_SHUTDOWN 	2


#endif /* STDTYPES_H_ */
