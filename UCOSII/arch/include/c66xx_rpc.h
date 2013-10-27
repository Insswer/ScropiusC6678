
#ifndef C66XX_RPC_H
#define C66XX_RPC_H


#include "list.h"
#include "parcel.h"

struct IpcTransactionData *BSP_GetTransactionData(unsigned int msg, unsigned int from);
void wait(void);

/* RPC number for all RPC functions */
#define	  	NO_AVAILABLE	0xdeadbeef
#define		RPC_FUNC_TEST	0
#define		RPC_MAX			8
#define		RPC_FUNC_NUM	(RPC_MAX)
#define		RPC_DATA_SIZE	64

#define		IPC_DIRECT		0
#define		IPC_BROADCAST	1
#define		IPC_ASYN		2


/* function slot defined in cfg */
struct FuncSlot {
	unsigned int handle;
	void (*stub_proxy)();
	void (*stub)();
};


#define RPC_FUNC_SLOT(handle,stub_proxy_f,stub_f)			\
		{handle,(void(*)())stub_proxy_f, (void(*)())stub_f}

struct IpcTransactionHdr {
	unsigned int ipc_type;
	unsigned int src_core;
	unsigned int dst_core;
	struct list_head list_node;
};

struct IpcTransactionRpc {
	unsigned int  rpc_code;
//	unsigned char rpc_data[RPC_DATA_SIZE];
	struct parcel rpc_data;
};

struct IpcTransactionData {
	struct IpcTransactionHdr ipc_hdr;
	struct IpcTransactionRpc ipc_rpc_data;
	unsigned long reply;
};

struct RpcNode {
	struct list_head node;
};

#endif /* C66XX_RPC_H */
