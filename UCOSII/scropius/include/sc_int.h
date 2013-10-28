/*
 * sc_int.h
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#ifndef SC_INT_H_
#define SC_INT_H_

#include "scropius_cfg.h"

int Scropius_ISRHandlerEntry(int, unsigned int);
struct sc_vector_tbl *init_vectors(int core);
int sc_request_irq(int (*isr)(int, unsigned int), int core, int vector_num);

struct sc_vector_tbl {
	int raw_core_num;	/* which core this vector belongs to */
	int (*vectors_isr[VECTORS_NUM])(int , unsigned int);
};

#endif /* SC_INT_H_ */
