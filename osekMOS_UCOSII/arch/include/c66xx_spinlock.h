/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_spinlock,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_spinlock.h
 * @brief
 *	<li>功能：定义了C66XX体系自旋锁的头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

#ifndef _C66XX_SPINLOCK_H
#define _C66XX_SPINLOCK_H


/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/

int BSP_GetSpinlock(int id);
int BSP_ReleaseSpinlock(int id);
int BSP_TryToGeSpinlock(int id);
void BSP_ClearAllSpinlock(void);
int BSP_GetSpinlockIrqSaved(int id);
int BSP_ReleaseSpinlockIrqRestore(int id);
/**************************** 定义部分 *********************************/

#define		SPINLOCK_BASE_REG		0x02640000
#define		SEM_PID					(SPINLOCK_BASE_REG + 0x0)
#define		SEM_RST_RUN				(SPINLOCK_BASE_REG + 0x08)
#define		SEM_EOI					(SPINLOCK_BASE_REG + 0x0c)
#define		SEM_DIRECT(num)			(SPINLOCK_BASE_REG + 0x100 + num * 0x4)
#define		SEM_INDIRECT(num)		(SPINLOCK_BASE_REG + 0x200 + num * 0x4)
#define		SEM_QUERY(num)			(SPINLOCK_BASE_REG + 0x300 + num * 0x4)
#define		SEM_FLAGL(num)			(SPINLOCK_BASE_REG + 0X400 + num * 0x4)
#define		SEM_FLAGL_CLEAR(num)	SEM_FLAGL(num)
#define		SEM_FLAGH(num)			(SPINLOCK_BASE_REG + 0x440 + num * 0x4)
#define		SEM_FLAGH_CLEAR(num)	SEM_FLAGH(num)
#define		SEM_FLAGL_SET(num)		(SPINLOCK_BASE_REG + 0x480 + num * 0x4)
#define		SEM_FLAGH_SET(num)		(SPINLOCK_BASE_REG + 0X4c0 + num * 0x4)
#define		SEM_ERR					(SPINLOCK_BASE_REG + 0x500)
#define		SEM_ERR_CLEAR			(SPINLOCK_BASE_REG + 0x504)
#define		SEM_ERR_SET				(SPINLOCK_BASE_REG + 0x508)

#define		SPFLAG_ATOMIC			0x0
#define		SPFLAG_KERNEL			0x1
#define		SPFLAG_INT				SPFLAG_ATOMIC



#endif /* _C66XX_SPINLOCK_H */
