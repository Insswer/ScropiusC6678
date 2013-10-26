/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_timer,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_timer.c
 * @brief
 *	<li>功能：定义了C66XX体系相关的定时器模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

/**************************** 引用部分 *********************************/

#include "c66xx_timer.h"
#include "c66xx_arch.h"
#include "c66xx_io.h"
#include "c66xx_debug.h"
#include "c66xx_corepac.h"
#include "c66xx_exception.h"
#include "stdTypes.h"
#include "c66xx_itbl.h"
#include "list.h"
/**************************** 声明部分 *********************************/
extern void TimingInt(void);
/**************************** 定义部分 *********************************/
struct C6xTimerDev timer_devs[16];

struct test_tick_chain {
	int core;
	int tick;
	struct list_head list;
}core0_tick, core1_tick;
static struct list_head tick_chain_head;
static int tick_times = 0;


static void test_time_tick(void)
{
	struct test_tick_chain *chain;
	int core = BSP_GetCoreId();
	list_for_each_entry(chain, &tick_chain_head, list) {
		if (chain->core == core)
			break;
	}
	chain->tick++;
	if (core == 0)
		tick_times++;
	else
		tick_times--;
	printf("core%d ticks %d\n", core, chain->tick);
	printf("core%d print total tick times %d\n", core, tick_times);
}


/*
 * @brief
 *    BSP_TimeInterruptISR：时钟中断的BSP电平处理函数，该函数主要会应答相关的定时器。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 *
 */
void BSP_TimeInterruptISR(void)
{
	struct C6xTimerDev *pTimer;
	TimingInt();
//	test_time_tick();
	pTimer = BSP_GetC6xTimer(BSP_GetCoreId());
	pTimer->ack_timer_int(pTimer);
}

/*
 * @brief
 *    InitTimer：内部函数，初始化定时器并启动。
 * @param		struct C6xTimerDev *：定时器实例的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void InitTimer(struct C6xTimerDev *pTimer)
{
	struct CorepacIntDev *corepac_int_dev;
	unsigned long tmp;
	/* unsigned long clock_src; */

	if (pTimer == NULL) {
		return;
	}

	int id = pTimer->id;


	tmp = readl(TIMER_EMUMGTCLKSPD_REG(id));
	/* generally clk_div is 6 */
	pTimer->clk_div = ((tmp & (0xf << 16)) >> 16);


	/* FIXME:
	 * 1ghz/6 => ABOUT 0.000000006 S per timer count
	 * 1MS -> 0.001 ->  166666
	 * 1S  -> 166666666
	 * we set 1s first...
	 * clock_src = pll_stat_cur.sysclk_stat[0].sysclk / 6;
	 */
	pTimer->reload_value = 166666666;
	writel(pTimer->reload_value, TIMER_PRDLO_REG(id));
	writel(pTimer->reload_value, TIMER_RELLO_REG(id));

	tmp = readl(TIMER_TGCR_REG(id));
	pTimer->mode = TIMER_DUAL_32BIT_UNCHAINED_MODE;
	tmp &= ~((0x3 << 2)|(0x1 << 0));
	tmp |= (TIMER_DUAL_32BIT_UNCHAINED_MODE << 2|(0x1 << 0));
	writel(tmp, TIMER_TGCR_REG(id));


	/* clock mode? pulse mode? */
	tmp = readl(TIMER_TCR_REG(id));
	tmp &= ~((0x1 << 3)|(0x1 << 8)|(0x1 << 9));
	tmp |= (0x1 << 3);
	writel(tmp, TIMER_TCR_REG(id));

	/* enable interrupt */
	tmp = readl(TIMER_INTCTLSTAT_REG(pTimer->id));
	tmp &= ~(0x1 << 0);
	tmp |= (0x1 << 0);
	writel(tmp, TIMER_INTCTLSTAT_REG(pTimer->id));

	pTimer->init_ready = 1;

	tmp = readl(TIMER_CNTLO_REG(pTimer->id));
	tmp &= ~(0xffffffff);
	writel(tmp, TIMER_CNTLO_REG(pTimer->id));

	tmp = readl(TIMER_TCR_REG(pTimer->id));
	tmp &= ~(0x3 << 6);
	tmp |= (0x2 << 6);
	writel(tmp, TIMER_TCR_REG(pTimer->id));
	corepac_int_dev = BSP_GetCurCorepacIntc();
	corepac_int_dev->pFunc_sets->corepac_map_dsp_int(IRQ_TINT, 14);
	BSP_EnableDspInterrupt(14);

	DEBUG_DEV("timer%d starts...\n", pTimer->id);

#if 0
	INIT_LIST_HEAD(&tick_chain_head);
	core0_tick.core = 0;
	core0_tick.tick = 0;
	core1_tick.core = 1;
	core0_tick.tick = 1;
	list_add(&core0_tick.list, &tick_chain_head);
	list_add(&core1_tick.list, &tick_chain_head);
#endif
}


/*
 * @brief
 *    AckTimerInt：内部函数，应答时钟中断
 * @param		struct C6xTimerDev *：定时器实例的指针。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	-1：失败。
 * <p>			0：函数正常返回。
 */
int AckTimerInt(struct C6xTimerDev *pTimer)
{
	unsigned long tmp;
	if (pTimer == NULL) {
		DEBUG_DEV("pTimer is null...how can this happen...\n");
		return -1;
	}
	if (!pTimer->init_ready) {
		DEBUG_DEV("timer start before init..\n");
		return -1;
	}


	tmp = readl(TIMER_INTCTLSTAT_REG(pTimer->id));

	if (tmp & (0x1 << 1)) {
		tmp |= (0x1 << 1);
		writel(tmp, TIMER_INTCTLSTAT_REG(pTimer->id));
		return 0;
	} else {
		return -1;
	}

}

/*
 * @brief
 *    BSP_InitC6xTimer：初始化指定定时器并启动该定时器
 * @param		num：定时器编号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	struct C6xTimerDev *：定时器实例的指针。
 *
 */
struct C6xTimerDev *BSP_InitC6xTimer(unsigned int num)
{
	if (num > 15) {
		DEBUG_DEV("bad num..\n");
		return NULL;
	}
	struct C6xTimerDev *pTimer_dev = &timer_devs[num];
	pTimer_dev->id = num;

	if (pTimer_dev->init_timer != NULL)
		pTimer_dev->init_timer(pTimer_dev);
	else {
		pTimer_dev->init_timer = InitTimer;
		pTimer_dev->ack_timer_int = AckTimerInt;
		pTimer_dev->init_timer(pTimer_dev);
	}
	return pTimer_dev;
}

/*
 * @brief
 *    BSP_GetC6xTimer：给定定时器编号获得相对应的定时器实例。
 * @param		num：定时器编号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	struct C6xTimerDev *：定时器实例的指针。
 *
 */
struct C6xTimerDev *BSP_GetC6xTimer(unsigned int num)
{
	if (num > 15) {
		DEBUG_DEV("bad num..\n");
		return NULL;
	}

	struct C6xTimerDev *pTimer_dev = &timer_devs[num];
	return pTimer_dev;
}


























