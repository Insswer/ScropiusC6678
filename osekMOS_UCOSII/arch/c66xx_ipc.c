/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_ipc,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_ipc.c
 * @brief
 *	<li>功能：定义了C66XX体系核间通信模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

/**************************** 引用部分 *********************************/
#include "c66xx_ipc.h"
#include "c66xx_io.h"
#include "c66xx_cic.h"
#include "c66xx_corepac.h"
#include "c66xx_itbl.h"
#include "c66xx_cache.h"
#include "c66xx_spinlock.h"
#include "stdio.h"
#include "c66xx_debug.h"
#include "c66xx_exception.h"
#include "c66xx_ipc.h"
#include "c66xx_rpc.h"
#include "list.h"
#include "cfg.h"
#include "osekOccConfig.h"


/**************************** 声明部分 *********************************/
extern struct FuncSlot coreRpcFunc[];
extern struct RpcNode rpc_nodes[];
/**************************** 定义部分 *********************************/
static int module_ready = 0;
struct MessageBox MessageBoxNode[8];
struct MessageList PerCoreMessageList[8];

/*
 * @brief
 *    BSP_InitIPCMessageBox：初始化IPC邮箱模块
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void BSP_InitIPCMessageBox(void)
{
	int i;
	for (i = 0; i < 8; i++) {
		INIT_LIST_HEAD(&MessageBoxNode[i].box_list);
		INIT_LIST_HEAD(&PerCoreMessageList[i].message_head);
		PerCoreMessageList[i].num = 0;
	}
}

/*
 * @brief
 *    BSP_SendMessage：发送邮件信息
 * @param		message，所要发送的信息。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	-1, 参数不合法。
 * 				0,  成功
 */
int BSP_SendMessage(struct Message *message, int to)
{
	int core = BSP_GetCoreId();
	struct MessageBox *pBox = &MessageBoxNode[to];
	if(message == NULL)
		return -1;

	if (to < 0 || to > 7)
		return -1;
	message->from = core;
	BSP_GetSpinlock(CORE_BOX_LOCK(to));
	list_add_tail(&message->message_list, &pBox->box_list);
	BSP_ReleaseSpinlock(CORE_BOX_LOCK(to));

	BSP_InterruptCore(to, MAIL_BOX_MSG);
	return 0;
}

/*
 * @brief
 *    BSP_ReadMessage：从本核心的消息队列读取一个消息。
 * @param		无
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	message
 */
struct Message *BSP_ReadMessage(void)
{
	struct Message *msg;
	int core = BSP_GetCoreId();
	void *data = (void *)0;
	if (BSP_MessageListIsEmpty())
		return NULL;

	msg = list_entry(PerCoreMessageList[core].message_head.next, struct Message, message_list);
	list_del(&msg->message_list);
	if (msg->callback != NULL) {
		data = (void *)msg->message_ptr;
		msg->callback(data);
	}
	return msg;
}


/*
 * @brief
 *    BSP_MessageListIsEmpty：本核心的消息队列是否为空。
 * @param		无
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	1, 队列为空。
 * 				0, 队列不空
 */
int BSP_MessageListIsEmpty(void)
{
	int core = BSP_GetCoreId();
	if (list_empty(&PerCoreMessageList[core].message_head)&&PerCoreMessageList[core].num == 0)
		return 1;
	else
		return 0;
}

/*
 * @brief
 *    BSP_InitIPCModule：初始化IPC模块
 * @param		dspInt：将IPC中断映射到的DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_InitIPCModule(int dspInt)
{
	int ret;
	int i;
	unsigned int core_num;
	struct CorepacIntDev *corepac;
	corepac = BSP_GetCurCorepacIntc();

	if (corepac == NULL) {
		DEBUG_DEV("cannot get corepac...\n");
		return;
	}

	ret = corepac->pFunc_sets->corepac_map_dsp_int(IRQ_IPCLOCAL, 13);
	DEBUG_DEV("last map interrupt %d to %d ...\n", ret, 13);

	BSP_EnableDspInterrupt(dspInt);
	BSP_UnlockBfg();
	core_num = BSP_GetCoreId();
	if (core_num  == 0) {
		for (i = 0;i < CONFIG_OSEK_KERNEL_CORE_NUMS; i++)
			INIT_LIST_HEAD(&rpc_nodes[i].node);
		BSP_InitIPCMessageBox();
	}
	BSP_L1DWriteback();
	module_ready = 1;
}

/*
 * @brief
 *    BSP_InterruptCore：该函数用于触发核间中断。
 * @param		id：要触发的核编号。
 * 				msg：核间中断的信息。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_InterruptCore(int id, unsigned long msg)
{
	unsigned long tmp;
	unsigned int sender;
	if (!module_ready) {
		DEBUG_DEV("you should call init_ipc_module first\n");
		return;
	}

	if (id < 0 || id > 7)
	{
		DEBUG_DEV("bad id.\n");
		return;
	}

	tmp = readl(IPCGR(id));
	tmp &= ~(0xffffffff);
	writel(tmp, IPCGR(id));

	sender = BSP_GetCoreId();
	tmp = (MK_MSG(msg, sender));
	tmp |= (0x1);
	writel(tmp, IPCGR(id));
}

/*
 * @brief
 *    BSP_ReadIpcMsg：读取IPC信息的内容
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	信息
 */
unsigned long BSP_ReadIpcMsg(void)
{

	unsigned long tmp;
	unsigned int id;
	id = BSP_GetCoreId();
	tmp = readl(IPCAR(id));

	tmp &= (0xfffffff0);
	return tmp;
}


/*
 * @brief
 *    BSP_AckIpc：读取IPC信息的内容
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	信息
 */
void BSP_AckIpc(void)
{
	unsigned long tmp;
	unsigned int id;

	id = BSP_GetCoreId();
	tmp = 0xffffffff;
	writel(tmp, IPCAR(id));
}


/*
 * @brief
 *    BSP_DoIpc：IPC核间中断的处理函数
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_DoIpc(void)
{
	unsigned int id;
	unsigned long msg;
	unsigned int from;
	struct IpcTransactionData *data;
	struct MessageBox *pBox;
	struct Message *msg_tmp, *msg_p;
	int i;

	BSP_DisableInt();
	id = BSP_GetCoreId();
	msg = BSP_ReadIpcMsg();

	from = GET_SENDER(msg);
	msg = GET_MSG(msg);
//	DEBUG_DEV("core%d read ipc msg : %lu from : core%d\n",id, msg, from);
	pBox = &MessageBoxNode[id];

	if (msg == MAIL_BOX_MSG) {
		BSP_GetSpinlock(CORE_BOX_LOCK(id));
		if (list_empty(&pBox->box_list)) {
			BSP_ReleaseSpinlock(CORE_BOX_LOCK(id));
			BSP_AckIpc();
			BSP_EnableInt();
			return;
		}
		list_for_each_entry_safe(msg_tmp, msg_p, &pBox->box_list, message_list) {
			list_del(&msg_tmp->message_list);
			list_add_tail(&msg_tmp->message_list ,&PerCoreMessageList[id].message_head);
			PerCoreMessageList[id].num++;
			/* wakeup process that waiting for message */
		}
		BSP_ReleaseSpinlock(CORE_BOX_LOCK(id));
	} else {
		for (i = 0;i < RPC_FUNC_NUM; i ++) {
			if (msg == coreRpcFunc[i].handle) {
				data = BSP_GetTransactionData(msg, id);
				coreRpcFunc[i].stub(msg, data);
			}
		}
	}


	BSP_AckIpc();
	BSP_EnableInt();
}


/*
 * @brief
 *    BSP_DoIpcSimple：IPC核间中断的简化版本。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_DoIpcSimple(void)
{
	unsigned int id;
	unsigned long msg;
	unsigned int from;

	id = BSP_GetCoreId();
	msg = BSP_ReadIpcMsg();

	from = GET_SENDER(msg);
	msg = GET_MSG(msg);

	DEBUG_DEV("core%d read ipc msg : %lu from : core%d\n",id, msg, from);

	BSP_AckIpc();
}

