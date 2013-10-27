/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_memarea.c,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_memarea.c
 * @brief
 *	<li>功能：c66xx的地址空间描述。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */



#include "c66xx_memarea.h"
#include "c66xx_mpu.h"
#include <stdlib.h>

int BSP_InsertMemArea(struct rb_root *root, struct mem_area *area)
{
	if (root == NULL)
		return -1;
	if (area == NULL)
		return -1;
	struct rb_node **new = &(root->rb_node);
	struct rb_node *parent = NULL;
	while (*new) {
		struct mem_area *this = rb_entry(*new, struct mem_area, rbn);
		parent = *new;

		if (this->area_start == area->area_start)
			return -1;
		else if (this->area_start < area->area_start)
			new = &((*new)->rb_left);
		else if (this->area_start > area->area_start)
			new = &((*new)->rb_right);
	}
	rb_link_node(&area->rbn, parent, new);
	rb_insert_color(&area->rbn, root);
	return 0;
}

int BSP_RemoveMemArea(struct rb_root *root, struct mem_area *area, int (*free)(struct mem_area *))
{
	if (root == NULL || area == NULL)
		return -1;
	rb_erase(&area->rbn, root);
	if (free != NULL)
		return ((*free)(area));
	else
		return 0;
}

void BSP_MidorderTraverse(struct rb_node *rb_node, void (*fn)(struct mem_area *area))
{
	struct mem_area *node;
	if (rb_node != NULL) {
		BSP_MidorderTraverse(rb_node->rb_left, fn);
		node = rb_entry(rb_node, struct mem_area, rbn);
		if (fn != NULL)
			(*fn)(node);
		BSP_MidorderTraverse(rb_node->rb_right, fn);
	}
}


struct mem_area *BSP_LookupMemArea(struct rb_root *root, unsigned long start_address)
{
	if (root == NULL)
		return NULL;
	struct rb_node *node = root->rb_node;
	while (node) {
		struct mem_area *ma = rb_entry(node, struct mem_area, rbn);

		if (start_address == ma->area_start)
			return ma;
		else if (ma->area_start < start_address)
			node = node->rb_left;
		else if (ma->area_start > start_address)
			if (ma->area_start + ma->area_size - 1 > start_address)
				return ma;
			else
				node = node->rb_right;
	}
	return NULL;
}

void BSP_IterateAllMemArea(struct rb_root *root, void (*fn)(struct mem_area *area))
{
	if (root != NULL)
		BSP_MidorderTraverse(root->rb_node, fn);
}
