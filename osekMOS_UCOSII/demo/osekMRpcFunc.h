/*
 * osekMRpcFunc.h
 *
 *  Created on: 2013-5-21
 *      Author: insswer
 */

#ifndef OSEKMRPCFUNC_H_
#define OSEKMRPCFUNC_H_


/* SLOT 1 */
int func_test(char *str, int i,unsigned int target);
void func_test_stub(unsigned int msg, struct IpcTransactionData *data);
int func_test_stub_proxy(char *str, int i,unsigned int target);





#endif /* OSEKMRPCFUNC_H_ */
