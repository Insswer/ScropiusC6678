/*
 * c66xx_msmc.c
 *
 *  Created on: 2013-10-19
 *      Author: insswer
 */

#include "c66xx_io.h"
#include "c66xx_msmc.h"
#include "string.h"

static unsigned long msm_bitmaps = 0;



void *BSP_AllocShareMemoryById(unsigned long id)
{
	if (id > 31)
		return NULL;
	if (msm_bitmaps & (1 << id)) {
		memset((void *)ID_TO_MEMORY(id), 0, MSM_ENTRY_SIZE);
		msm_bitmaps |= (1 << id);
		return (void *)ID_TO_MEMORY(id);
	} else
		return NULL;
}

void *BSP_AllocFreeShareMemory(void)
{
	int i, j = 0;
	for (i = 0;i < 32; i++) {
		if (msm_bitmaps & (1 << i))
			break;
		else
			j++;
	}

	if (j == 32)
		return NULL;
	return BSP_AllocShareMemoryById(i);
}

void BSP_FreeShareMemory(void *addr)
{
	if ((unsigned long)addr < 0x0c000000 || (unsigned long)addr > 0x0c4fffff)
		return;
	unsigned long id = MEMORY_TO_ID((unsigned long)addr);
	msm_bitmaps &= ~(1 << id);
	memset((void *)ID_TO_MEMORY(id), 0, MSM_ENTRY_SIZE);
}

