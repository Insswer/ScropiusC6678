/*
 * os.c
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#include "sc_os.h"
#include "string.h"
#include "c66xx_arch_config.h"
#include "c66xx_cache.h"
#include "c66xx_cic.h"
#include "c66xx_corepac.h"
#include "c66xx_debug.h"
#include "c66xx_exception.h"
#include "c66xx_init.h"
#include "c66xx_io.h"
#include "c66xx_ipc.h"
#include "c66xx_itbl.h"
#include "c66xx_memarea.h"
#include "c66xx_mpu.h"
#include "c66xx_msmc.h"
#include "c66xx_perm.h"
#include "c66xx_rpc.h"
#include "c66xx_spinlock.h"
#include "c66xx_sysirq.h"
#include "c66xx_timer.h"
#include "c66xx_arch_config.h"


static struct os_cfg oss[OS_NUM];
static struct core_cfg cores[USING_CORE];

struct os_timer_cfg c6x_timer = {
	.timer_int_num = 14,
	.hardware_timer = NULL,
};

extern int ucmain(void);


struct core_cfg *init_core(void)
{
	struct core_cfg *core;

	struct cic_device *pCic_dev0;
	struct cic_device *pCic_dev1;

	int core_num = BSP_GetCoreId();

	if (core_num >= USING_CORE)
		return NULL;

	core = &cores[core_num];
	memset(core, 0, sizeof(struct core_cfg));

	core->bp = 1;
	core->raw_num = core_num;
	core->os = core_num;
	core->vectors = init_vectors(core_num);

	/* init SOC architecture */
	BSP_ArchInit();
	core->entry_point = (void *)ucmain;
	core->ready = 1;
	return core;
}


struct os_cfg   *init_os(struct core_cfg *core)
{
	struct os_cfg *os;

	if (core == NULL || core->ready == 0)
		return NULL;

	/* UCOSII 0 */
	if (core->raw_num == 0) {
		os = &oss[0];
		os->entry_point = (unsigned long)core->entry_point;
		os->kind = UCOSII;
		INIT_LIST_HEAD(&os->cores);
		list_add(&core->cores_os_node, &os->cores);
		os->os_num = 0;
		/* init timer */
		c6x_timer.hardware_timer = (void *)BSP_InitC6xTimer(BSP_GetCoreId(), 14);
		os->timer = &c6x_timer;
	/* UCOSII 1 */
	} else {
		os = &oss[1];
		os->entry_point = (unsigned long)core->entry_point;
		os->kind = UCOSII;
		INIT_LIST_HEAD(&os->cores);
		list_add(&core->cores_os_node, &os->cores);
		os->os_num = 1;
		/* init timer */
		c6x_timer.hardware_timer = (void *)BSP_InitC6xTimer(BSP_GetCoreId(), 14);
		os->timer = &c6x_timer;
	}

	return os;
}

void start_core(struct os_cfg *os)
{
	int (*entry_point)(int);
	if (os != NULL)
		entry_point = (int(*)(int))os->entry_point;
	(*entry_point)(os->os_num);
}

void sc_timer_tick(void)
{
	 struct C6xTimerDev *pTimer;
	 pTimer = BSP_GetC6xTimer(BSP_GetCoreId());
	 (pTimer->ack_timer_int)(pTimer);
}

