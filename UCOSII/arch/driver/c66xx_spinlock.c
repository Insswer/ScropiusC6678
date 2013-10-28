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
 * @file 	c66xx_spinlock.c
 * @brief
 *	<li>功能：定义了C66XX体系的自旋锁模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

/**************************** 引用部分 *********************************/

#include "c66xx_spinlock.h"
#include "c66xx_io.h"
#include <stdio.h>
#include "c66xx_debug.h"
/**************************** 声明部分 *********************************/
extern unsigned char coreSpinlocks[];
/**************************** 定义部分 *********************************/

/*
 * @brief
 *    SpinLock： 内部函数，锁上给定ID的自旋锁。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，上锁成功。
 * <p>			-1，自旋锁ID不合法。
 */
static inline int SpinLock(int id)
{
	unsigned long tmp;

	if (id < 0 || id > 63) {
		DEBUG_DEV("bad spinlock id\n");
		return -1;
	}


	while(!(readl(SEM_DIRECT(id)) & 0x1));
/*
busy:
	tmp = readl(SEM_DIRECT(id));
	if (!(tmp & 0x1))
		goto busy;
*/
	return 0;
}

/*
 * @brief
 *    SpinUnlock： 内部函数，解锁给定ID的自旋锁。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，解锁成功。
 * <p>			-1，自旋锁ID不合法。
 */
static inline int SpinUnlock(int id)
{
	unsigned long tmp;
	if (id < 0 || id > 63) {
		printf("bad spinlock id\n");
		return -1;
	}

	tmp = readl(SEM_DIRECT(id));
	tmp |= 0x1;
	writel(tmp, SEM_DIRECT(id));

	return 0;
}

/*
 * @brief
 *    TrySpinlock： 内部函数，试图获取一个自旋锁，如果失败不会进入自旋。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，获取成功。
 * <p>			-1，自旋锁ID不合法或获取失败。
 */
static inline int TrySpinlock(int id)
{
	unsigned long tmp;

	if (id < 0 || id > 63) {
		printf("bad spinlock id\n");
		return -1;
	}

	tmp = readl(SEM_DIRECT(id));

	if (!(tmp & 0x1))
		return -1;

	return 0;
}

/*
 * @brief
 *    BSP_GetSpinlock： 锁上给定ID的自旋锁。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，上锁成功。
 * <p>			-1，自旋锁ID不合法。
 */
int BSP_GetSpinlock(int id)
{
	return SpinLock(id);
}

/*
 * @brief
 *    BSP_ReleaseSpinlock： 解锁给定ID的自旋锁。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，解锁成功。
 * <p>			-1，自旋锁ID不合法。
 */
int BSP_ReleaseSpinlock(int id)
{
	return SpinUnlock(id);
}

/*
 * @brief
 *    BSP_TryToGeSpinlock： 试图获取一个自旋锁，如果失败不会进入自旋。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，获取成功。
 * <p>			-1，自旋锁ID不合法或获取失败。
 */
int BSP_TryToGeSpinlock(int id)
{
	return TrySpinlock(id);
}

/*
 * @brief
 *    BSP_ClearAllSpinlock： 重置所有自旋锁，所有自旋锁将全部释放。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 *
 */
void BSP_ClearAllSpinlock(void)
{
	int i;
	unsigned long tmp;
	tmp = 0x1;
	for (i = 0; i < 64; i++)
		writel(tmp, SEM_DIRECT(i));
}

/*
 * @brief
 *    BSP_GetSpinlockIrqSaved： 锁上给定ID的自旋锁，保存之前上下文后关闭中断。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，上锁成功。
 * <p>			-1，自旋锁ID不合法。
 */
int BSP_GetSpinlockIrqSaved(int id)
{
	BSP_DisableInt();
	return SpinLock(id);
}

/*
 * @brief
 *    BSP_ReleaseSpinlockIrqRestore： 解锁给定ID的自旋锁，恢复上下文。
 * @param		id：自旋锁编号
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	0，解锁成功。
 * <p>			-1，自旋锁ID不合法。
 */
int BSP_ReleaseSpinlockIrqRestore(int id)
{
	BSP_EnableInt();
	return SpinUnlock(id);
}
