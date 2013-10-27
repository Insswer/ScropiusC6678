
#include "c66xx_rpc.h"
#include "c66xx_ipc.h"
#include "list.h"
#include "c66xx_io.h"
#include "c66xx_cache.h"
#include "stdio.h"
#include "c66xx_mpu.h"


struct RpcNode rpc_nodes[MAX_CORE_NUM];
struct FuncSlot coreRpcFunc[RPC_FUNC_NUM];

struct IpcTransactionData *BSP_GetTransactionData(unsigned int msg, unsigned int to)
{
	struct IpcTransactionHdr *hdr;
	struct IpcTransactionData *data;

	list_for_each_entry(hdr, &rpc_nodes[to].node, list_node) {
		data = container_of(hdr, struct IpcTransactionData, ipc_hdr);
		if (data->ipc_rpc_data.rpc_code == msg)
			return data;
	}
	return NULL;
}

void wait(void)
{
	int i;
	for (i = 0;i < 0x1000; i++);
}
