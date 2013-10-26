/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_memarea.h,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_memarea.h
 * @brief
 *	<li>功能：声明了内存地址相关的头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */
#ifndef C66XX_MEMAREA_H_
#define C66XX_MEMAREA_H_

/**************************** 引用部分 *********************************/
#include "c66xx_perm.h"
#include "list.h"
#include "rb_tree.h"

/**************************** 声明部分 *********************************/



/**************************** 定义部分 *********************************/
/* 这里包括了两种地址空间，一种是Corepac看到的32位地址空间，另一种是XMC的36位扩展地址空间  */



struct mem_area {
	unsigned long area_start;
	unsigned long area_size;

	unsigned long perm;
	unsigned long aid;
	unsigned long mpu_indx;

	struct rb_node rbn;

	/* for xmc */
	/*
	 * 11 - 4K
	 * 12 - 8k
	 * 13 - 16K
	 * ...
	 * 31 - 4GB
	 *
	 * */
	unsigned long seg_size;	/* 0 for disabled */
	unsigned long seg_id;
	unsigned long remap_addr;
	unsigned long mpx_id;
};

struct mem_area *BSP_LookupMemArea(struct rb_root *root, unsigned long start_address);
void BSP_MidorderTraverse(struct rb_node *rb_node, void (*fn)(struct mem_area *area));
int BSP_RemoveMemArea(struct rb_root *root, struct mem_area *area, int (*free)(struct mem_area *));
int BSP_InsertMemArea(struct rb_root *root, struct mem_area *area);
void BSP_ShowAllMemArea(struct rb_root *root);

#endif /* C66XX_MEMAREA_H_ */
