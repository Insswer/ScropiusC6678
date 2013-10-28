
#include "c66xx_timer.h"
#include "c66xx_init.h"
#include "c66xx_itbl.h"
#include "c66xx_cic.h"
#include "c66xx_corepac.h"
#include "c66xx_sysirq.h"
#include "c66xx_exception.h"
#include "c66xx_ipc.h"
#include "c66xx_spinlock.h"
#include "c66xx_cache.h"
#include <stdio.h>

/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

unsigned long BSP_CurrentMode;

static void set_all_memory_non_cachable(void)
{
	int i;
	unsigned long tmp;
	for (i = 0; i < 256; i++) {
		tmp = readl(MAR_ADDR(i));
		tmp &= ~(MAR_PREFETCH|MAR_CACHE);
		writel(tmp, MAR_ADDR(i));
	}
}


void BSP_ArchInit(void)
{
	struct cic_device *pCic_dev0;
	struct cic_device *pCic_dev1;

	BSP_DisableL2Cache();
	BSP_InitCic();
	BSP_InitCurCorepac();
	set_all_memory_non_cachable();

	pCic_dev0 = BSP_GetCicChip(0);
	pCic_dev0->cic_global_enable_interrupt(pCic_dev0);
	pCic_dev1 = BSP_GetCicChip(1);
	pCic_dev1->cic_global_enable_interrupt(pCic_dev1);


//	BSP_InitIPCModule(13);
	BSP_ClearAllSpinlock();

//	BSP_InitC6xTimer(core_num);
//	BSP_EnableInt();

	if (BSP_GetExecutionState())
		BSP_CurrentMode = SUPERVISOR_MODE;
	else
		BSP_CurrentMode = USER_MODE;
}
