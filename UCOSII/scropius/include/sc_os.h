/*
 * sc_os.h
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#ifndef __SC_OS_H
#define __SC_OS_H

#include "scropius_cfg.h"
#include "sc_int.h"
#include "sc_includes.h"
#include "list.h"
#include "c66xx_exception.h"


/* os kind */
#define SINGLE_CORE		0
#define	SMP				1
#define AMP				2

struct core_cfg {
	int os;	/* corresponding os num for this core */
	int	bp;		/* 1 if this core is bp */
	int ready;	/* 1 if this core is ready to run */
	int raw_num;	/* raw number for this core */
	void *entry_point;	/* entry point of this core */
	struct list_head cores_os_node;
	struct sc_vector_tbl *vectors;
};

struct os_timer_cfg {
	unsigned long timer_int_num;	/* timer int num */
	void *hardware_timer;	/* address of the structure of hardware timer */
};

struct os_cfg {
	int os_num;	/* os num */
	int kind; /* os kind */
	unsigned long entry_point;	/* entry point of this os */
	struct list_head cores;		/* if this os is SMP or AMP, there are cores in this list */
	struct os_timer_cfg *timer;
};

struct core_cfg *init_core(void);
struct os_cfg   *init_os(struct core_cfg *);
void start_core(struct os_cfg *);
void sc_timer_tick(void);

#ifdef TMS320C6678
static inline void sc_enable_timer(void)
{
	BSP_EnableDspInterrupt(14);
}

static inline void sc_disable_timer(void)
{
	BSP_DisableDspInterrupt(14);
}
#else
static inline void sc_enable_timer(void)
{
}

static inline void sc_disable_timer(void)
{
}
#endif
#endif
