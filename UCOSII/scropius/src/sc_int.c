/*
 * sc_int.c
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#include "sc_int.h"
#include "sc_includes.h"
#include <stdio.h>


static struct sc_vector_tbl vectors[USING_CORE];


int sc_request_irq(int (*isr)(int, unsigned int), int core, int vector_num)
{
	struct sc_vector_tbl *pVector;
	if (core >= USING_CORE)
		return -1;

	if (vector_num > 15)
		return -1;

	pVector = &vectors[core];

	if (pVector->vectors_isr[vector_num] != NULL)
		return -1;

	pVector->vectors_isr[vector_num] = isr;
	return 0;
}

int illegal_vectors(int core, unsigned int vector_num)
{
	printf("illegal vectors, how can you get here...\n");
	return -1;
}

struct sc_vector_tbl *init_vectors(int core)
{
	struct sc_vector_tbl *vec;
	int i;
	if (core >= USING_CORE)
		return NULL;

	vec = &vectors[USING_CORE];
	vec->raw_core_num = core;

	/* TMS320C6678 */
	for (i = 1; i < 4; i++)
		vec->vectors_isr[i] = illegal_vectors;

	for (i = 4; i < 16; i++)
		vec->vectors_isr[i] = NULL;

	/* init hardware vectors */
	BSP_InitializeIntCtr();
	BSP_InitializeVector();

	return vec;
}


int Scropius_ISRHandlerEntry(int core, unsigned int vector_num)
{
	struct sc_vector_tbl *pVector = &vectors[core];
	if (0 != (pVector->vectors_isr[vector_num])(core, vector_num))
		return -1;
	return 0;
}
