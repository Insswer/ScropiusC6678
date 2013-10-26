/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_rpc,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_rpc.c
 * @brief
 *	<li>功能：定义了C66XX体系RPC模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

 /**************************** 引用部分 *********************************/
#include "c66xx_rpc.h"
#include "c66xx_ipc.h"
#include "list.h"
#include "c66xx_io.h"
#include "c66xx_cache.h"
#include "stdio.h"
#include "osekOccConfig.h"
#include "c66xx_mpu.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/
/* RPC管理结点，每一个CPU持有一个结点 */
struct RpcNode rpc_nodes[MAX_CORE_NUM];

/*
 * @brief
 *    BSP_GetTransactionData：根据msg返回核间通信的IPC数据
 * @param		msg：核间中断使用的中断信息。
 * 				to：  核间中断发送的目标
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	struct IpcTransactionData *：IPC数据实例
 */
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
