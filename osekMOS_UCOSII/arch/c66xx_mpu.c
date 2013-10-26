/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_mpu.c,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_mpu.c
 * @brief
 *	<li>功能：定义了C66XX体系的MPU模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */
 /**************************** 引用部分 *********************************/
#include "c66xx_rpc.h"
#include "c66xx_ipc.h"
#include "list.h"
#include "c66xx_io.h"
#include "c66xx_cache.h"
#include "stdio.h"
#include "string.h"
#include "osekOccConfig.h"
#include "c66xx_mpu.h"
#include "c66xx_memarea.h"
#include "c66xx_perm.h"
#include "rb_tree.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

struct mpu main_mpu[4];
struct mpu xmc;
unsigned short xmc_bitmaps = 0;
static struct mem_area mpu_mem_areas[15 * 2 + 5 + 1];
static struct mem_area xmp_mem_areas[16];

struct mem_area *BSP_GetFreeXmpMemArea(int *indx)
{
	int i,j = 0;
	if (indx == NULL)
		return NULL;
	/* need lock */
	for (i = 0; i < 16; i++) {
		if (xmc_bitmaps & (1 << i))
			break;
		else
			j++;
	}
	if (j == 16)
		return NULL;
	xmc_bitmaps |= (1 << i);
	*indx = i;
	return &xmp_mem_areas[i];
}

int BSP_FreeXmpMemArea(unsigned long indx)
{
	if (xmc_bitmaps & (1 << indx))
		xmc_bitmaps &= ~(0x1 << indx);
	else
		return -1;
	memset(&xmp_mem_areas[indx], 0, sizeof(struct mem_area));
	return 0;
}

struct mem_area *BSP_GetXmpMemAreaByIndx(unsigned int indx)
{

	if (indx > 15)
		return NULL;
	/* need lock */
	if (xmc_bitmaps & (1 << indx))
		return NULL;
	xmc_bitmaps |= (1 << indx);
	return &xmp_mem_areas[indx];
}

struct mem_area *__BSP_GetMpuMemArea(unsigned int pid, unsigned int indx)
{
	if (pid < 2)
		if (indx < 16)
			return &mpu_mem_areas[pid * 15 + indx];
		else
			return NULL;
	else if (pid == 2)
		if (indx < 5)
			return &mpu_mem_areas[15 * 2 + indx];
		else
			return NULL;
	else if (pid == 3)
		if (indx == 0)
			return &mpu_mem_areas[15 * 2 + 5];
		else
			return NULL;
	else
		return NULL;
}

static void __BSP_ReadConfig(unsigned int id)
{
	unsigned long tmp;
	if (id > 3)
		return;
	/* init mpu0 */
	main_mpu[id].id = id;
	main_mpu[id].area_root = RB_ROOT;
	tmp = readl(MPU_CONF_REG(id));
	if (((tmp & (((unsigned long)0xff << 24)) >> 24)) == 0x6)
		main_mpu[id].address_alignment = 64;
	else
		main_mpu[id].address_alignment = 1;
	main_mpu[id].num_fix_address = ((tmp & (0xf << 20)) >> 20);
	main_mpu[id].num_prog_address = ((tmp & (0xf << 16)) >> 16);
	main_mpu[id].num_support_aid = ((tmp & (0xf << 12)) >> 12);
	if (tmp & 0x1)
		main_mpu[id].assume_allowed = 1;
	else
		main_mpu[id].assume_allowed = 0;
}

void BSP_InitMpu(void)
{
	unsigned long tmp;
	int i;
	struct mem_area *pmem_area;
	/* read basic config information */
	for (i = 0; i < 4; i++)
		__BSP_ReadConfig((unsigned int)i);

	/* init  */
	memset(mpu_mem_areas, 0, sizeof(mpu_mem_areas));
	memset(xmp_mem_areas, 0, sizeof(xmp_mem_areas));
	memset(&xmc, 0, sizeof(xmc));
	xmc.id = XMC;
	xmc.area_root = RB_ROOT;

	/* init mpu0's mpsar and mpear */
	tmp = readl(MPU_PROG0_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 0);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_PROG0_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_PROG0_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 0;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_012_PROG1_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 1);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG1_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG1_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 1;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_012_PROG2_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 2);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG2_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG2_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 2;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_012_PROG3_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 3);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG3_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG3_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 3;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_012_PROG4_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 4);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG4_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG4_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 4;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG5_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 5);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG5_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG5_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 5;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG6_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 6);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG6_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG6_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 6;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG7_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 7);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG7_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG7_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 7;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG8_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 8);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG8_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG8_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 8;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG9_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 9);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG9_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG9_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 9;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG10_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 10);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG10_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG10_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 10;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG11_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 11);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG11_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG11_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 11;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG12_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 12);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG12_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG12_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 12;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG13_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 13);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG13_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG13_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 13;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG14_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 14);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG14_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG14_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 14;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	tmp = readl(MPU_02_PROG15_MPSAR_REG(0));
	pmem_area = __BSP_GetMpuMemArea(0, 15);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG15_MPEAR_REG(0));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG15_MPPA_REG(0));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 15;
	BSP_InsertMemArea(&main_mpu[0].area_root, pmem_area);

	/* init mpu1's mpsar and mpear */
	tmp = readl(MPU_PROG0_MPSAR_REG(1));
	pmem_area = __BSP_GetMpuMemArea(1, 0);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_PROG0_MPEAR_REG(1));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_PROG0_MPPA_REG(1));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 0;
	BSP_InsertMemArea(&main_mpu[1].area_root, pmem_area);

	tmp = readl(MPU_012_PROG1_MPSAR_REG(1));
	pmem_area = __BSP_GetMpuMemArea(1, 1);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG1_MPEAR_REG(1));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG1_MPPA_REG(1));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 1;
	BSP_InsertMemArea(&main_mpu[1].area_root, pmem_area);

	tmp = readl(MPU_012_PROG2_MPSAR_REG(1));
	pmem_area = __BSP_GetMpuMemArea(1, 2);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG2_MPEAR_REG(1));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG2_MPPA_REG(1));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 2;
	BSP_InsertMemArea(&main_mpu[1].area_root, pmem_area);

	tmp = readl(MPU_012_PROG3_MPSAR_REG(1));
	pmem_area = __BSP_GetMpuMemArea(1, 3);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG3_MPEAR_REG(1));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG3_MPPA_REG(1));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 3;
	BSP_InsertMemArea(&main_mpu[1].area_root, pmem_area);

	tmp = readl(MPU_012_PROG4_MPSAR_REG(1));
	pmem_area = __BSP_GetMpuMemArea(1, 4);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG4_MPEAR_REG(1));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG4_MPPA_REG(1));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 4;
	BSP_InsertMemArea(&main_mpu[1].area_root, pmem_area);

	/* init mpu2's mpsar and mpear */
	tmp = readl(MPU_PROG0_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 0);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_PROG0_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_PROG0_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 0;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_012_PROG1_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 1);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG1_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG1_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 1;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_012_PROG2_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 2);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG2_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG2_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 2;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_012_PROG3_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 3);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG3_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG3_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 3;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_012_PROG4_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 4);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_012_PROG4_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_012_PROG4_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 4;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG5_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 5);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG5_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG5_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 5;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG6_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 6);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG6_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG6_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 6;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG7_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 7);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG7_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG7_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 7;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG8_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 8);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG8_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG8_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 8;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG9_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 9);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG9_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG9_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 9;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG10_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 10);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG10_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG10_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 10;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG11_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 11);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG11_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG11_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 11;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG12_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 12);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG12_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG12_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 12;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG13_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 13);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG13_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG13_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 13;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG14_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 14);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG14_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG14_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 14;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	tmp = readl(MPU_02_PROG15_MPSAR_REG(2));
	pmem_area = __BSP_GetMpuMemArea(2, 15);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_02_PROG15_MPEAR_REG(2));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_02_PROG15_MPPA_REG(2));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 15;
	BSP_InsertMemArea(&main_mpu[2].area_root, pmem_area);

	/* init mpu3's mpsar and mpear */
	tmp = readl(MPU_PROG0_MPSAR_REG(3));
	pmem_area = __BSP_GetMpuMemArea(3, 0);
	pmem_area->area_start = tmp;
	tmp = readl(MPU_PROG0_MPEAR_REG(3));
	pmem_area->area_size = tmp - pmem_area->area_start + 1;
	tmp = readl(MPU_PROG0_MPPA_REG(3));
	pmem_area->aid = ((tmp & (0x1ffff << 9)) >> 9);
	pmem_area->perm = ((tmp & (0xff << 0)) >> 0);
	pmem_area->mpu_indx = 0;
	BSP_InsertMemArea(&main_mpu[3].area_root, pmem_area);

	writel(0x1, MPU_FLT_CLR_REG(0));
	writel(0x1, MPU_FLT_CLR_REG(1));
	writel(0x1, MPU_FLT_CLR_REG(2));
	writel(0x1, MPU_FLT_CLR_REG(3));

	/* get First two mapped area in XMP */
	pmem_area = BSP_GetXmpMemAreaByIndx(0);
	tmp = readl(XMPAXH(0));
	pmem_area->area_start = (tmp & (0xfffff000));
	pmem_area->seg_size = (tmp & 0x1f);
	pmem_area->area_size = BSP_GetSizeFromSeg(pmem_area->seg_size);
	tmp = readl(XMPAXL(0));
	pmem_area->seg_id = (tmp & 0xf0000000) >> 28;
	pmem_area->remap_addr = (tmp & 0x0ffffff0);
	pmem_area->perm = (tmp & 0xf);
	pmem_area->mpx_id = 0;
	BSP_InsertMemArea(&xmc.area_root, pmem_area);

	pmem_area = BSP_GetXmpMemAreaByIndx(1);
	tmp = readl(XMPAXH(1));
	pmem_area->area_start = (tmp & (0xfffff000));
	pmem_area->seg_size = (tmp & 0x1f);
	pmem_area->area_size = BSP_GetSizeFromSeg(pmem_area->seg_size);
	tmp = readl(XMPAXL(1));
	pmem_area->seg_id = (tmp & 0xf0000000) >> 28;
	pmem_area->remap_addr = (tmp & 0x0ffffff0);
	pmem_area->perm = (tmp & 0xf);
	pmem_area->mpx_id = 1;
	BSP_InsertMemArea(&xmc.area_root, pmem_area);

}

int BSP_SetMemAreaNewPerm(unsigned int addr, unsigned perm, unsigned int aid)
{
	struct mpu *pmpu;
	struct mem_area *pmem_area;
	int mpu_indx;
	unsigned long tmp;
	if (addr >= 0x01d00000 || addr < 0x02620400)
		mpu_indx = 0;
	else if (addr >= 0x34000000 || addr < 0x340c0000)
		mpu_indx = 1;
	else if (addr >= 0x02a00000 || addr < 0x02ac0000)
		mpu_indx = 2;
	else if (addr >= 0x02640000 || addr < 0x02640800)
		mpu_indx = 3;
	else
		return -1;

	pmpu = &main_mpu[mpu_indx];
	pmem_area = BSP_LookupMemArea(&pmpu->area_root, addr);

	if (pmem_area == NULL)
		return -1;

	pmem_area->aid = aid;
	pmem_area->perm = perm;

	switch(pmem_area->mpu_indx) {
		case 0:
			tmp = readl(MPU_PROG0_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_PROG0_MPPA_REG(mpu_indx));
			break;
		case 1:
			tmp = readl(MPU_012_PROG1_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_012_PROG1_MPPA_REG(mpu_indx));
			break;
		case 2:
			tmp = readl(MPU_012_PROG2_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_012_PROG2_MPPA_REG(mpu_indx));
			break;
		case 3:
			tmp = readl(MPU_012_PROG3_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_012_PROG3_MPPA_REG(mpu_indx));
			break;
		case 4:
			tmp = readl(MPU_012_PROG4_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_012_PROG4_MPPA_REG(mpu_indx));
			break;
		case 5:
			tmp = readl(MPU_02_PROG5_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG5_MPPA_REG(mpu_indx));
			break;
		case 6:
			tmp = readl(MPU_02_PROG6_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG6_MPPA_REG(mpu_indx));
			break;
		case 7:
			tmp = readl(MPU_02_PROG7_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG7_MPPA_REG(mpu_indx));
			break;
		case 8:
			tmp = readl(MPU_02_PROG8_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG8_MPPA_REG(mpu_indx));
			break;
		case 9:
			tmp = readl(MPU_02_PROG9_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG9_MPPA_REG(mpu_indx));
			break;
		case 10:
			tmp = readl(MPU_02_PROG10_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG10_MPPA_REG(mpu_indx));
			break;
		case 11:
			tmp = readl(MPU_02_PROG11_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG11_MPPA_REG(mpu_indx));
			break;
		case 12:
			tmp = readl(MPU_02_PROG12_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG12_MPPA_REG(mpu_indx));
			break;
		case 13:
			tmp = readl(MPU_02_PROG13_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG13_MPPA_REG(mpu_indx));
			break;
		case 14:
			tmp = readl(MPU_02_PROG14_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG14_MPPA_REG(mpu_indx));
			break;
		case 15:
			tmp = readl(MPU_02_PROG15_MPPA_REG(mpu_indx));
			tmp &= ~((0xff << 0)|(0x1ffff << 9));
			tmp |= ((aid << 9)|(perm << 0));
			writel(tmp, MPU_02_PROG15_MPPA_REG(mpu_indx));
			break;
	}
	return 0;
}

int BSP_GetMemAreaPerm(unsigned int addr, unsigned long *aid, unsigned long *perm)
{
	struct mem_area *pmem_area;
	struct mpu *pmpu;
	int mpu_indx;
	if (aid == NULL || perm == NULL)
		return -1;

	if (addr >= 0x01d00000 || addr < 0x02620400)
		mpu_indx = 0;
	else if (addr >= 0x34000000 || addr < 0x340c0000)
		mpu_indx = 1;
	else if (addr >= 0x02a00000 || addr < 0x02ac0000)
		mpu_indx = 2;
	else if (addr >= 0x02640000 || addr < 0x02640800)
		mpu_indx = 3;
	else
		return -1;
	pmpu = &main_mpu[mpu_indx];
	pmem_area = BSP_LookupMemArea(&pmpu->area_root, addr);

	if (pmem_area == NULL)
		return -1;
	*aid = pmem_area->aid;
	*perm = pmem_area->perm;

	return 0;
}

int BSP_CreatNewXMPArea(unsigned long baddr, unsigned long remap_addr, unsigned long seg_size,
		unsigned long seg_id, unsigned long aid, unsigned perm)
{
	int indx;
	struct mem_area *pmem;
	unsigned long tmp = 0;
	if (baddr < 0x80000000)
		return -1;
	pmem = BSP_GetFreeXmpMemArea(&indx);
	if (pmem == NULL)
		return -1;
	memset(pmem, 0, sizeof(struct mem_area));

	pmem->aid =aid;
	pmem->area_start = (baddr & 0xfffff000);
	pmem->area_size = BSP_GetSizeFromSeg(seg_size);
	pmem->mpx_id = (unsigned long)indx;
	pmem->perm = perm;
	pmem->remap_addr = remap_addr;
	pmem->seg_id = seg_id;
	pmem->seg_size = seg_size;

	tmp |= ((baddr & 0xfffff000) | (seg_size & 0x1f));
	writel(tmp, XMPAXH(indx));

	tmp |= ((remap_addr & 0x0ffffff0)|(seg_id & ((unsigned long)0xf << 28))|(perm & 0xf));
	writel(tmp, XMPAXL(indx));

	BSP_InsertMemArea(&xmc.area_root, pmem);

	return indx;
}

int BSP_RemoveXMPArea(unsigned long indx)
{
	if (xmc_bitmaps & (1 << indx)) {
		struct mem_area *pmem = &xmp_mem_areas[indx];
		BSP_RemoveMemArea(&xmc.area_root, pmem, NULL);
		BSP_FreeXmpMemArea(indx);
		return 0;
	} else
		return -1;
}
