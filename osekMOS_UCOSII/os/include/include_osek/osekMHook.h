/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/
#ifndef _OSEK_HOOK_H
#define _OSEK_HOOK_H

/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"

/**************************** 声明部分 *********************************/

#if defined(CONFIG_OSEK_SYSTEM_GETSERVICEID) || defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS)
extern OSBYTE  osekHook_ServiceId;
extern OSDWORD osekHook_ObjId;
#endif

#if defined(CONFIG_OSEK_HOOK_ERROR)
extern OSBYTE  osekHook_ErrorNestedLevel;
#endif


/**************************** 定义部分 *********************************/

// ErrorHook 声明
#if defined(CONFIG_OSEK_HOOK_ERROR)
/// 错误处理的HOOK函数原型（在配置了错误回调时该函数才存在）
void ErrorHook( StatusType error );
#endif

// PreTaskHook 声明及相关宏定义
#if defined(CONFIG_OSEK_HOOK_PRETASK)
/// 任务切换前的HOOK函数原型（在配置了任务前回调时该函数才存在）
void PreTaskHook( void );
#define OSEK_HOOK_PreTaskHook PreTaskHook
#else
#define OSEK_HOOK_PreTaskHook()
#endif

// PostTaskHook 声明及相关宏定义
#if defined(CONFIG_OSEK_HOOK_POSTTASK)
/// 任务切换后的HOOK函数原型（在配置了任务后回调时该函数才存在）
void PostTaskHook( void );
#define OSEK_HOOK_PostTaskHook  PostTaskHook
#else
#define OSEK_HOOK_PostTaskHook()
#endif

// StartupHook 声明及相关宏定义
#if defined(CONFIG_OSEK_HOOK_STARTUP)
/// 系统启动的HOOK函数原型（在配置了启动回调时该函数才存在）
void StartupHook( void );
#define OSEK_HOOK_StartupHook StartupHook
#else
#define OSEK_HOOK_StartupHook()
#endif

// ShutdownHook 声明及相关宏定义
#if defined(CONFIG_OSEK_HOOK_SHUTDOWN)
/// 系统关闭的HOOK函数原型（在配置了关闭回调时该函数才存在）
void ShutdownHook( StatusType error );
#define OSEK_HOOK_ShutdownHook(error) (ShutdownHook(error))
#else
#define OSEK_HOOK_ShutdownHook(error)
#endif

// IdleLoopHook 声明及相关宏定义
#if defined(CONFIG_OSEK_HOOK_IDLE)
/// 系统空闲的HOOK函数原型（在配置了空闲回调时该函数才存在）
void IdleLoopHook( void );
#define OSEK_HOOK_IdleHook  IdleLoopHook
#else
#define OSEK_HOOK_IdleHook()
#endif



#if defined(CONFIG_OSEK_SYSTEM_GETSERVICEID) || defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS)
#define OSEK_HOOK_SET_INFO(id,param) \
	osekHook_ServiceId = (id); \
 	osekHook_ObjId =(OSDWORD)(param)

#define OSEK_HOOK_CLEAR_INFO() 	(osekHook_ServiceId = OSServiceId_NoService)
#else
 #define OSEK_HOOK_SET_INFO(id,param) 	((void)0)
 #define OSEK_HOOK_CLEAR_INFO()	((void)0)
#endif


// OSEK_HOOK_ErrorHook 宏定义
#if defined(CONFIG_OSEK_HOOK_ERROR)

#define OSEK_HOOK_ErrorHook(error,id,param)  \
	if( osekHook_ErrorNestedLevel == 0 ) \
	{ \
		OSEK_TARGET_OSIntSave( _save ); \
		OSEK_TARGET_DisableOSInt(_save); \
		osekHook_ErrorNestedLevel++; \
		OSEK_HOOK_SET_INFO((id),(param));\
 		ErrorHook((error)); \
 		OSEK_HOOK_CLEAR_INFO();\
 		osekHook_ErrorNestedLevel--; \
		OSEK_TARGET_EnableOSInt(_save); \
	}\
	return error

#else

#define OSEK_HOOK_ErrorHook(error,id,param) return error

#endif

#endif

