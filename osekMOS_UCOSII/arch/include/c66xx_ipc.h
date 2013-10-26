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
 * @file 	c66xx_ipc.h
 * @brief
 *	<li>功能：c66xx的IPC相关操作头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

#ifndef _C66XX_IPC_H
#define _C66XX_IPC_H

/**************************** 引用部分 *********************************/

#include "list.h"

/**************************** 声明部分 *********************************/

void BSP_InitIPCModule(int dspInt);
void BSP_InterruptCore(int id, unsigned long msg);
unsigned long BSP_ReadIpcMsg(void);
void BSP_DoIpc(void);
void BSP_AckIpc(void);
void BSP_DoIpcSimple(void);
int BSP_MessageListIsEmpty(void);

/**************************** 定义部分 *********************************/

#define		NMICR_BASE	0x02620200
#define		NMIGR(id)	(NMICR_BASE + id * 0x4)


#define		IPCGR_BASE	0x02620240
#define		IPCGR(id)	(IPCGR_BASE + id * 0x4)

#define		IPCGRH		0x0262027c

#define		IPCAR_BASE	0x02620280
#define		IPCAR(id)	(IPCAR_BASE + id * 0x4)


#define		IPCARH		0x026202bc

#define		KICK0		0x02620038
#define		KICK1		0x0262003c

#define		KICK0_VAL	0x83e70b13
#define		KICK1_VAL	0x95a4f1e0

#define		GET_MSG(msg)		((msg & 0xfffff000) >> 12)
#define	    GET_SENDER(msg)		((msg & 0xff0) >> 4)

#define		MK_MSG(msg,sender)	((((msg) & 0xfffff) << 12)|(((sender) & 0xff) << 4))
#define     MAIL_BOX_MSG		(0xc0ffe)
#define		MK_MSG_BOX(sender)	((((MAIL_BOX_MSG) & 0xfffff) << 12)|(((sender)&0xff)<<4))

struct Message {
	unsigned long message_ptr;
	char message_content[256];
	struct list_head message_list;
	void (*callback)(void *data);
	int  from;
};


struct MessageBox {
	struct list_head box_list;
};

struct MessageList {
	struct list_head message_head;
	int num;
};


int BSP_SendMessage(struct Message *message, int to);
struct Message *BSP_ReadMessage(void);

/*
 * @brief
 *    BSP_UnlockBfg：解锁Boot CFG寄存器组
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_UnlockBfg(void)
{
	*(volatile unsigned long *)KICK0 = KICK0_VAL;
	*(volatile unsigned long *)KICK1 = KICK1_VAL;
}

/*
 * @brief
 *    BSP_UnlockBfg：锁Boot CFG寄存器组
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_LockBfg(void)
{
	*(volatile unsigned long *)KICK0 = 0x0;
}

#endif /* _C66XX_IPC_H */
