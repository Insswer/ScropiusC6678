/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/
/*
 * $Log: osekHook.c,v $
 * Revision 1.0  2008/04/10 14:54:00
 * 创建文件
*/

/*
 * @file 	osekHook.c
 * @brief
 *	<li>功能：实现HOOK相关内容。</li>
 * @author
 * @date 	2008-04-10
 * <p>部门：
 */


/**************************** 引用部分 *********************************/

#include "osprop.h"

#include "osapi.h"

/*************************** 前向声明部分 ******************************/

/**************************** 定义部分 *********************************/

// 全局变量
#if defined(CONFIG_OSEK_SYSTEM_GETSERVICEID) || defined(CONFIG_OSEK_SYSTEM_PARAMETERACCESS)
OSBYTE  osekHook_ServiceId = 0;
OSDWORD osekHook_ObjId = 0;
#endif


#if defined(CONFIG_OSEK_HOOK_ERROR)
OSBYTE  osekHook_ErrorNestedLevel = 0;
#endif

/**************************** 实现部分 *********************************/


/**
 * @brief
 *          错误回调入口函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
void ErrorHook( StatusType error )
{


}


/**
 * @brief
 *          任务前回调入口函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
void PreTaskHook( void )
{

}

/**
 * @brief
 *          任务后回调入口函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
void PostTaskHook( void )
{


}


/**
 * @brief
 *          启动系统回调入口函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
void StartupHook( void )
{


}

/**
 * @brief
 *          关闭系统回调入口函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
void ShutdownHook( StatusType error )
{


}


/**
 * @brief
 *          空闲任务回调入口函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
void IdleLoopHook( void )
{

}

