/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/
#ifndef OSEKMRPC_H_
#define OSEKMRPC_H_

#include "list.h"
#include "parcel.h"
#include "osekOccConfig.h"

/* RPC number for all RPC functions */
#define	  	NO_AVAILABLE	0xdeadbeef
#define		RPC_FUNC_TEST	0
#define		RPC_MAX			1

#define		RPC_FUNC_NUM	(RPC_MAX)

#define		RPC_DATA_SIZE	64

#define		IPC_DIRECT		0
#define		IPC_BROADCAST	1
#define		IPC_ASYN		2


/* function slot defined in cfg */
struct func_slot {
	unsigned int handle;
	void (*stub_proxy)();
	void (*stub)();
};



#define RPC_FUNC_SLOT(handle,stub_proxy_f,stub_f)			\
		{handle,(void(*)())stub_proxy_f, (void(*)())stub_f}



struct ipc_transaction_hdr {
	unsigned int ipc_type;
	unsigned int src_core;
	unsigned int dst_core;
	struct list_head list_node;
};

struct ipc_transaction_rpc {
	unsigned int  rpc_code;
//	unsigned char rpc_data[RPC_DATA_SIZE];
	struct parcel rpc_data;
};

struct ipc_transaction_data {
	struct ipc_transaction_hdr ipc_hdr;
	struct ipc_transaction_rpc ipc_rpc_data;
	unsigned long reply;
};

struct ipc_transaction_data *get_ipc_transaction_data(unsigned int msg, unsigned int from);

struct rpc_node {
	struct list_head node;
};

#endif /* OSEKMRPC_H_ */
