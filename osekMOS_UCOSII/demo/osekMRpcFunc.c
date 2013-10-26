/*
 * osekMRpcFunc.c
 *
 *  Created on: 2013-5-8
 *      Author: insswer
 */

#include "c66xx_rpc.h"
#include "osekMRpcFunc.h"
#include "cfg.h"
#include "c66xx_io.h"
#include "c66xx_ipc.h"
#include "c66xx_itbl.h"
#include "string.h"
#include "stdio.h"
#include "list.h"
#include "c66xx_cache.h"
#include "parcel.h"
#include "c66xx_mpu.h"


/*
 *	相关数据结构介绍：
 *
 *	ipc_transaction_data: 表示一次ipc跨核通信的对话，包括ipc头部,ipc数据以及应答数据
 *	ipc_transaction_hdr:  ipc头部信息，包括的信息为ipc类型（可以扩展），发起核和目标核，链表结构
 *  ipc_transaction_rpc:  ipc数据，包括rpc代码和rpc数据，rpc代码代表了跨核函数的handle，handle全局必须唯一
 *  					  ，rpc数据位parcel结构，该结构用于将跨核数据字节流化方便其他核进行读取。
 *  parcel：该结构用于实现基础数据类型以及指针类型的字节流化，注意parcel的存取采用栈的方式，必须保证先存的数据最后读取，否则将出错！
 *  rpc_node:			      每个核一个的rpc链表结点，A核发送B核的远程通信时，必须将构建的ipc_transaction_data结构体挂
 *						      接在B核对应的rpc_node上。
 *	func_slot：			      全局范围内的rpc函数集合，rpc函数在集合的index要保持和函数handle一致。
 *
 *  如何编写跨核函数：
 *  1. 决定跨核函数的参数以及返回类型，如果有多个返回数据，必须将返回数据以指针的形式加入的函数参数中，在函数返回值不为NO_AVAILABLE时才可用
 *	2. 以该文件和模板编写stub_proxy和stub函数。
 *	3. 编写真正实现具体功能的函数。
 *
 *	关于返回值：
 *		返回值NO_AVAILABLE代表返回值当前无效，远程函数还未处理完毕。请不用将函数返回值设计成NO_AVAILABLE
 *
 *	关于内存管理：
 *		该模块附加了一套高效的内存管理方式，当然也可以采用库函数的实现。
 *
 *	后期改进：
 *		当前ipc函数在目标的中断状态下执行，这不是一个好的执行方式，期待通过任务的方式让ipc函数在目标的进程上下文中执行。
 *  */
extern struct RpcNode rpc_nodes[];
extern struct IpcTransactionData coreIpcDatas[CONFIG_OSEK_KERNEL_CORE_NUMS][IPC_DATA_NUMS];
extern struct FuncSlot coreRpcFunc[];

#define IPC_FUNC_TEST	0


int func_test_stub_proxy(char *str, int i,unsigned int target)
{
	unsigned int coreId;
	int ret;
	coreId = BSP_GetCoreId();

	struct IpcTransactionData *ipc_data = &coreIpcDatas[coreId][IPC_FUNC_TEST];


	if (ipc_data != NULL) {
		ipc_data->ipc_hdr.ipc_type = IPC_DIRECT;
		ipc_data->ipc_hdr.src_core = coreId;
		ipc_data->ipc_hdr.dst_core = target;
		ipc_data->ipc_rpc_data.rpc_code = RPC_FUNC_TEST;
		ipc_data->reply = (unsigned long)NO_AVAILABLE;
		InitParcel(&ipc_data->ipc_rpc_data.rpc_data);
		PutPointer(&ipc_data->ipc_rpc_data.rpc_data, (unsigned long)str);
		PutSignedInt(&ipc_data->ipc_rpc_data.rpc_data, i);
		list_add(&ipc_data->ipc_hdr.list_node, &rpc_nodes[target].node);
		BSP_L1DWriteback();
		BSP_InterruptCore(target, RPC_FUNC_TEST);
		while (ipc_data->reply == NO_AVAILABLE) {
			wait();
			BSP_L1DInvalid();
		}
		ret = (int)ipc_data->reply;

		return ret;
	} else {
		return -1;
	}
}

int __func_test(char * str, int i)
{
	unsigned int coreId = BSP_GetCoreId();
	printf("[core%d]: string = %s, i = %d\n",coreId, str,i);
	return 0;
}


void func_test_stub(unsigned int msg, struct IpcTransactionData *data)
{
	char *str;
	int i;
	int ret;

	if (data == NULL) {
		printf("OOPS! ipc_transaction is NULL! \n");
		return;
	}

	if (msg != data->ipc_rpc_data.rpc_code) {
		printf("OOPS! msg not equal to rpc_code!\n");
		return;
	}
	/* 必须和put_xxxx相反的顺序取出parcel中的数据 */
	i = GetSignedInt(&data->ipc_rpc_data.rpc_data);
	str = (char *)GetPointer(&data->ipc_rpc_data.rpc_data);
	ret = __func_test(str, i);
	data->reply = (unsigned long)ret;
	BSP_L1DWriteback();
}



int func_test(char *str, int i,unsigned int target)
{
	unsigned int coreId;
	int ret;
	/* check arguments */
	if (str == NULL || i < 0) {
		return -1;
	}

	coreId = BSP_GetCoreId();
	if (target != coreId)
		ret = func_test_stub_proxy(str, i, target);
	else
		ret = __func_test(str, i);

	return ret;
}


