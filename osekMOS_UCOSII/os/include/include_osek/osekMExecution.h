/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMEXECUTION_H_
#define OSEKMEXECUTION_H_


/**************************** 声明部分 *********************************/

// 执行管理提供给外部的公共调用函数

/**
 * @brief
 *    StartOS：启动操作系统。
 * @param[in]	mode：应用模式。
 * @returns     无。
 */
void StartOS( AppModeType mode );

/**
 * @brief
 *    ShutdownOS：关闭操作系统。
 * @param[in]	error：错误信息。
 * @returns     无。
 */
void ShutdownOS( StatusType error );


void OSMain(AppModeType mode);

void StartCore(CoreIDType CoreID,StatusType * Status);

void ShutdownAllCores();

// 变量声明
extern OSBYTE osekExecution_CurrentAppMode;


/**************************** 定义部分 *********************************/

/**
 * @brief
 *    GetActiveApplicationMode：获取当前的应用模式。
 * @param   	无。
 * @returns     当前应用模式。
 */
#define GetActiveApplicationMode() (osekExecution_CurrentAppMode)


#endif /* OSEKMEXECUTION_H_ */
