/*
 * main.c
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#include "sc_includes.h"
#include "sc_os.h"


int main(void)
{
	struct core_cfg *core;
	struct os_cfg *os;

	if((core = init_core()) == NULL)
		goto failed;

	os = init_os(core);

	if (os != NULL)
		start_core(os);
	else
		printf("failed to start os%d\n", os->os_num);

failed:
	return -1;
}


