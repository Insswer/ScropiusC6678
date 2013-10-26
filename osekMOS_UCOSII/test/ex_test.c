/*
 * ex_test.c
 *
 *  Created on: 2013-10-5
 *      Author: insswer
 */
#include "c66xx_cache.h"
#include "osekExTest.h"
#include "c66xx_init.h"
#include "stdTypes.h"

void ex_main(void)
{
	BSP_DisableL2Cache();
	volatile CoreIDType core=GetCoreID();
	BSP_ArchInit();
}
