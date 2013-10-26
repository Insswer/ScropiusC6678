/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_init,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_init.c
 * @brief
 *	<li>功能：定义了C66XX体系初始化的模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

#include "c66xx_timer.h"
#include "c66xx_init.h"
#include "c66xx_itbl.h"
#include "c66xx_cic.h"
#include "c66xx_corepac.h"
#include "c66xx_sysirq.h"
#include "c66xx_exception.h"
#include "c66xx_ipc.h"
#include "c66xx_spinlock.h"
#include <stdio.h>

/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

/*
 * @brief
 *    BSP_ArchInit：体系初始化函数
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_ArchInit(void)
{
	struct cic_device *pCic_dev0;
	struct cic_device *pCic_dev1;
	int core_num;

	core_num = BSP_GetCoreId();

	/* BP仅初始化CIC */
	if (core_num == 0)
		BSP_InitCic();

	BSP_InitCurCorepac();
	printf("Init Ctr...\n");
	BSP_InitializeIntCtr();
	printf("Init Vector...\n");
	BSP_InitializeVector();
//	BSP_DisableL1Dcache();

	if (core_num == 0) {
		pCic_dev0 = BSP_GetCicChip(0);
		pCic_dev0->cic_global_enable_interrupt(pCic_dev0);
		pCic_dev1 = BSP_GetCicChip(1);
		pCic_dev1->cic_global_enable_interrupt(pCic_dev1);
	}

	printf("Init IPC...\n");
	BSP_InitIPCModule(13);
	BSP_ClearAllSpinlock();
	printf("Init Timer...\n");
	BSP_InitC6xTimer(core_num);
//	BSP_EnableInt();

	if (BSP_GetExecutionState())
		printf("device runs in supervisor mode.\n");
	else
		printf("device runs in user mode.\n");
}
