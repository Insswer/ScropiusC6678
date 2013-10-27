

/**************************** 寮曠敤閮ㄥ垎 *********************************/
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
#include "c66xx_arch_config.h"


/**************************** 澹版槑閮ㄥ垎 *********************************/
extern struct FuncSlot coreRpcFunc[];
extern struct RpcNode rpc_nodes[];
/**************************** 瀹氫箟閮ㄥ垎 *********************************/
static int module_ready = 0;
struct MessageBox MessageBoxNode[8];
struct MessageList PerCoreMessageList[8];


static void BSP_InitIPCMessageBox(void)
{
	int i;
	for (i = 0; i < 8; i++) {
		INIT_LIST_HEAD(&MessageBoxNode[i].box_list);
		INIT_LIST_HEAD(&PerCoreMessageList[i].message_head);
		PerCoreMessageList[i].num = 0;
	}
}


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



int BSP_MessageListIsEmpty(void)
{
	int core = BSP_GetCoreId();
	if (list_empty(&PerCoreMessageList[core].message_head)&&PerCoreMessageList[core].num == 0)
		return 1;
	else
		return 0;
}



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
		for (i = 0;i < ARCH_CORE_NUM; i++)
			INIT_LIST_HEAD(&rpc_nodes[i].node);
		BSP_InitIPCMessageBox();
	}
	module_ready = 1;
}


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


unsigned long BSP_ReadIpcMsg(void)
{

	unsigned long tmp;
	unsigned int id;
	id = BSP_GetCoreId();
	tmp = readl(IPCAR(id));

	tmp &= (0xfffffff0);
	return tmp;
}



void BSP_AckIpc(void)
{
	unsigned long tmp;
	unsigned int id;

	id = BSP_GetCoreId();
	tmp = 0xffffffff;
	writel(tmp, IPCAR(id));
}



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
 *    BSP_DoIpcSimple锛欼PC鏍搁棿涓柇鐨勭畝鍖栫増鏈��
 * @param		鏃犮��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	鏃犮��
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

