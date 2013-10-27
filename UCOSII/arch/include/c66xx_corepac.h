/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_corepac,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_corepac.h
 * @brief
 *	<li>功能：c66xx的相关corepac操作头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

#ifndef _C66XX_COREPAC_H
#define _C66XX_COREPAC_H


/**************************** 引用部分 *********************************/
#include "c66xx_sysirq.h"
#include "c66xx_cache.h"
/**************************** 声明部分 *********************************/
struct CorepacIntDev *BSP_GetCurCorepacIntc(void);
void	BSP_InitCurCorepac(void);
int 	BSP_GetCurEvent(int dspInt);
int		BSP_GetCurCombineState(int efnum);
int		BSP_GetCurCombineAllState(int efnum, int *array);

struct Corepac *BSP_GetCurCorepac(void);

/**************************** 定义部分 *********************************/
#define		CORE_PAC_BASE	0x01800000


/****************************INTC  部分********************************/

#define		COREPAC_EVENT_FLAG_BASE				CORE_PAC_BASE
#define		COREPAC_EVENT_FLAG_REG(num)			(COREPAC_EVENT_FLAG_BASE + (num * 4))

#define		COREPAC_EVENT_SET_BASE				(CORE_PAC_BASE + 0x20)
#define		COREPAC_EVENT_SET_REG(num)			(COREPAC_EVENT_SET_BASE + (num * 4))

#define		COREPAC_EVENT_CLEAR_BASE			(CORE_PAC_BASE + 0x40)
#define		COREPAC_EVENT_CLEAR_REG(num)		(COREPAC_EVENT_CLEAR_BASE + (num * 4))

#define		COREPAC_EVENT_MASK_BASE				(CORE_PAC_BASE + 0x80)
#define		COREPAC_EVENT_MASK_REG(num)			(COREPAC_EVENT_MASK_BASE + (num * 4))

#define		COREPAC_MASK_EVENT_FLAG_BASE		(CORE_PAC_BASE + 0xa0)
#define		COREPAC_MASK_EVENT_FLAG_REG(num)	(COREPAC_MASK_EVENT_FLAG_BASE + (num * 4))

#define		COREPAC_INT_MUX_BASE				(CORE_PAC_BASE + 0x104)
#define		COREPAC_INT_MUX_REG(num)			(COREPAC_INT_MUX_BASE + (num * 4))

#define		COREPAC_ADV_EVENT_GEN_MUX0			(CORE_PAC_BASE + 0x10140)
#define		COREPAC_ADV_EVENT_GEN_MUX1			(CORE_PAC_BASE + 0x10144)

#define		COREPAC_INT_EXC_STATUS_REG			(CORE_PAC_BASE + 0x180)
#define		COREPAC_INT_EXC_CLEAR_REG			(CORE_PAC_BASE + 0x184)
#define		COREPAC_DROP_INT_MASK_REG			(CORE_PAC_BASE + 0x188)

#define		COREPAC_EXCE_MASK_BASE				(CORE_PAC_BASE + 0xc0)
#define		COREPAC_EXCE_MASK_REG(num)			(COREPAC_EXCE_MASK_BASE + (num * 4))

#define		COREPAC_MASK_EXCE_FLAG_BASE			(CORE_PAC_BASE + 0xe0)
#define		COREPAC_MASK_EXCE_FLAG_REG(num)		(COREPAC_MASK_EXCE_FLAG_BASE + (num * 4))


/* L1 & L2 */



/* XMC */






struct CorepacIntFuncSets {
	int (*corepac_get_event_flag)(int efnum);
	int (*corepac_set_event_flag)(int efnum);
	int (*corepac_clr_event_flag)(int efnum);
	void (*corepac_mask_combine_event)(int efnum);
	void (*corepac_unmask_combine_event)(int efnum);
	unsigned long (*corepac_mask_combine_read_event)(int efnum);
	int (*corepac_map_dsp_int)(int efnum, int dsp_int);
	void (*corepac_mapped_dsp_ints_get)(int *array);
	int  (*corepac_mapped_event_get)(int dsp_int);
};

struct CorepacIntDev {
	int core_id;
	struct CorepacIntFuncSets *pFunc_sets;
};

struct Corepac {
	unsigned core_id;
	struct CorepacIntDev *corepac_int_device;
};



#endif /* _C66XX_COREPAC_H */
