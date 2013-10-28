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
 * @file 	c66xx_timer.h
 * @brief
 *	<li>功能：定义了C66XX体系相关定时器的头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */



#ifndef		_C66XX_TIMER_H
#define 	_C66XX_TIMER_H

/**************************** 引用部分 *********************************/

#include "c66xx_io.h"

/**************************** 声明部分 *********************************/

struct C6xTimerDev *BSP_InitC6xTimer(unsigned int num, unsigned int timer_int);
struct C6xTimerDev *BSP_GetC6xTimer(unsigned int num);
void BSP_TimeInterruptISR(void);

/**************************** 定义部分 *********************************/

#define TIMER_BASE_ADDR   0x02200000
#define TIMER_CHAN_MULT   0x00010000


#define TIMER_BASE(chan)				(TIMER_BASE_ADDR + ((chan) * TIMER_CHAN_MULT))
#define TIMER_EMUMGTCLKSPD_REG(chan)	(TIMER_BASE(chan) + 0x04)
#define TIMER_CNTLO_REG(chan)			(TIMER_BASE(chan) + 0x10)
#define TIMER_CNTHI_REG(chan)			(TIMER_BASE(chan) + 0x14)
#define TIMER_PRDLO_REG(chan)			(TIMER_BASE(chan) + 0x18)
#define TIMER_PRDHI_REG(chan)			(TIMER_BASE(chan) + 0x1c)
#define TIMER_TCR_REG(chan)				(TIMER_BASE(chan) + 0x20)
#define TIMER_TGCR_REG(chan)			(TIMER_BASE(chan) + 0x24)
#define TIMER_WDTCR_REG(chan)			(TIMER_BASE(chan) + 0x28)
#define TIMER_RELLO_REG(chan)			(TIMER_BASE(chan) + 0x34)
#define TIMER_RELHI_REG(chan)			(TIMER_BASE(chan) + 0x38)
#define TIMER_CAPLO_REG(chan)   		(TIMER_BASE(chan) + 0x3c)
#define TIMER_CAPHI_REG(chan)			(TIMER_BASE(chan) + 0x40)
#define TIMER_INTCTLSTAT_REG(chan)		(TIMER_BASE(chan) + 0x44)

#define	 	TIMER_64BIT_GEN_MODE				0
#define	 	TIMER_DUAL_32BIT_UNCHAINED_MODE		1
#define		TIMER_64BIT_WDOG_MODE				2
#define		TIMER_DUAL_32BIT_CHAINED_MODE		3

#define TIMER_0                      0
#define TIMER_1                      1
#define TIMER_2                      2
#define TIMER_3                      3
#define TIMER_4                      4
#define TIMER_5                      5
#define TIMER_6                      6
#define TIMER_7                      7
#define TIMER_8			     		  8
#define TIMER_9                      9
#define TIMER_10                     10
#define TIMER_11                     11
#define TIMER_12              	     12
#define TIMER_13                      13
#define TIMER_14                     14
#define TIMER_15                     15

#define MSEC_PER_SEC	1000L
#define USEC_PER_MSEC	1000L
#define NSEC_PER_USEC	1000L
#define NSEC_PER_MSEC	1000000L
#define USEC_PER_SEC	1000000L
#define NSEC_PER_SEC	1000000000L
#define FSEC_PER_SEC	1000000000000000L

#define TIMER_DIVISOR(t) \
	((readl(TIMER_EMUMGTCLKSPD_REG(t)) & (0xf << 16)) >> 16)


struct C6xTimerDev {
	int id;
	int clk_div;
	int mode;
	int init_ready;
	unsigned long reload_value;
	void (*init_timer)(struct C6xTimerDev *, int);
	int (*ack_timer_int)(struct C6xTimerDev *);
};



#endif /* _C66XX_TIMER_H */
