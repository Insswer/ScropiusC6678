/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_itbl,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_itbl.h
 * @brief
 *	<li>功能：声明了C66XX寄存器相关的汇编函数头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */


#ifndef _C66XX_ITBL_H
#define _C66XX_ITBL_H


/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/

/* 汇编导出的函数，不要使用这个，应该使用BSP_GetCoreId */
unsigned int 	BSP_RawGetCoreId(void);
void		 	BSP_EnableInt(void);
void 			BSP_DisableInt(void);

void			BSP_DisableIntIrqSave(void);
void			BSP_EnableIntIrqRestore(void);

void 			BSP_InitializeIntCtr(void);
void			BSP_initializeVector(void);

unsigned long	BSP_GetCurDp(void);
unsigned long	BSP_GetCurSp(void);

void			BSP_IntMaskSave(unsigned long *);
void			BSP_IntMaskRestore(unsigned long);

unsigned long 	BSP_ReadIer(void);
void			BSP_WriteIer(unsigned long);
void		    BSP_WriteIsr(unsigned long);
void			BSP_WriteIcr(unsigned long);
unsigned long	BSP_ReadIfr(void);
void			BSP_MemoryFence(void);
void			BSP_SetIdle(void);
unsigned long 	BSP_ReadTSR(void);
unsigned long 	BSP_ReadCSR(void);
unsigned long 	BSP_ReadIRP(void);


void			BSP_EnableIer(unsigned int dsp_int);
void			BSP_DisableIer(unsigned int dsp_int);


/**************************** 定义部分 *********************************/



#endif /* _C66XX_ITBL_H */
