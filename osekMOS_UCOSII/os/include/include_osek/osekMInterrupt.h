/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMINTERRUPT_H_
#define OSEKMINTERRUPT_H_

/**************************** 声明部分 *********************************/


// 内部函数
void osekInterrupt_Initialize( void );

// 中断管理提供给外部的公共调用函数

/**
 * @brief
 *    DisableAllInterrupts：禁止所有中断。
 * @param   	无。
 * @returns     无。
 */
void DisableAllInterrupts( void );

/**
 * @brief
 *    EnableAllInterrupts：使能所有中断。
 * @param   	无。
 * @returns     无。
 */
void EnableAllInterrupts( void );

/**
 * @brief
 *    SuspendAllInterrupts：挂起所有中断。
 * @param   	无。
 * @returns     无。
 */
void SuspendAllInterrupts( void );

/**
 * @brief
 *    ResumeAllInterrupts：恢复所有中断。
 * @param   	无。
 * @returns     无。
 */
void ResumeAllInterrupts( void );

/**
 * @brief
 *    SuspendOSInterrupts：挂起OS相关的中断。
 * @param   	无。
 * @returns     无。
 */
void SuspendOSInterrupts( void );

/**
 * @brief
 *    ResumeOSInterrupts：恢复OS相关的中断。
 * @param   	无。
 * @returns     无。
 */
void ResumeOSInterrupts( void );


/**************************** 定义部分 *********************************/

#define ISR(isrName) void isrName(void)


#endif /* OSEKMINTERRUPT_H_ */
