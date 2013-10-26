/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/
/*
 * $Log: osekMessage.c,v $
 * Revision 1.0  2008/03/25 08:48:00  chenlr
 * �����ļ���
 */

/*
 * @file 	osekMessage.c
 * @brief
 *	<li>���ܣ�ʵ�ַ��OSEK COM��׼��ͨ�Ź��?�ܡ�</li>
 * @author Chen-LiRong
 * @date 2008-03-25
 * <p>���ţ�
 */



/**************************** ���ò��� *********************************/

#include "osprop.h"

#include    <string.h>                          // ��ʵ���ļ���ʹ�������е�memcpy()
#include    "osapi.h"


#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)
#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0

/*************************** ǰ������� ******************************/

// �ֲ�����
// ����Ϣ
StatusType msg_LockMessage( MsgType msg );
// ����Ϣ����ʱ����֪ͨ
#if defined(OSEK_MSG_USE_NOTIFICATION)
StatusType msg_SignalMessage( MsgType msg );
#endif  /* defined(OSEK_MSG_USE_NOTIFICATION) */


/**************************** ���岿�� *********************************/

// ȫ�ֱ�
// ��Ϣ���ƿ��
T_OSEK_MESSAGE_ControlBlock osekMsg_MessageTable[CONFIG_OSEK_COM_NUMBER_OF_MESSAGES];

// �ֲ�����
// ������Ϣ
#define msg_UnlockMessage( msg )  ((msg)->messageStatus &= ((OSBYTE)~OSEK_COM_MESSAGE_LOCKED))
// ���ñ�־
#define msg_SetFlag(msg) ((msg)->messageFlag = TRUE)


/**************************** ʵ�ֲ��� *********************************/

/*
 * @brief
 *    Msg_Init�������Ϣ������Ϣ��ʼ�����е���Ϣ���ƿ飨�ڲ�����
 * @param		�ޡ�
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns: �ޡ�
 */

void Msg_Init( void )
{
    OSMSGIDTYPE i;

    for( i = 0; i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES; i++ )
    {
        osekMsg_MessageTable[i].messageStatus = 0;
		osekMsg_MessageTable[i].messageConfigTable = osekConfig_MsgTable[i];

#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0
		osekMsg_MessageTable[i].messageFlag = FALSE;
#endif

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
		// ���ڶ�������Ϣ����ʼ����a�дָ���������Ϣ����
        if (osekMsg_MessageTable[i].messageConfigTable.fifoLength != 0)
        {
            osekMsg_MessageTable[i].fifoWrite = osekMsg_MessageTable[i].messageConfigTable.messagePtr;
            osekMsg_MessageTable[i].fifoRead = osekMsg_MessageTable[i].fifoWrite;
            osekMsg_MessageTable[i].fifoCount = 0;
        }
#endif

    }
}


#if defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS)

/*
 * @brief
 *    GetMessageStatus����ȡ��Ϣ��״̬��
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:    [E_COM_ID: ��Ч��Ϣ]
 * <p>          E_COM_LOCKED: ��Ϣ����
 * <p>          E_COM_BUSY: ��Ϣ����æ״̬
 * <p>          E_COM_NOMSG: ��Ϣ����������Ϣ
 * <p>          E_COM_LIMIT: ��Ϣ������
 * <p>          E_OK: ���������κ�״̬
 *
 * <p>Notes:    ��ID��Ч�⣬���������κ�����¶�������error hook��
 */

StatusType GetMessageStatus( MsgType msg )
{
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

	// �����Ϣ�Ƿ���Ч����Ч����E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_GetMessageStatus,msg);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

	// �����Ϣ��������E_COM_LOCKED
    if( (msg->messageStatus & OSEK_COM_MESSAGE_LOCKED) != 0 )
    {
		return E_COM_LOCKED;
    }

#if defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE)
	// �����Ϣ����æ״̬������E_COM_BUSY
    if( (msg->messageStatus & OSEK_COM_MESSAGE_BUSY) != 0 )
    {
		return E_COM_BUSY;
    }
#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE) */

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES
	// ���ڶ�������Ϣ�����������Ϣ��Ϊ0������E_COM_NOMSG��������������E_COM_LIMIT
    if( (msg->messageConfigTable.fifoLength != 0) )
#endif
    {
        if( msg->fifoCount == 0 )
        {
			return E_COM_NOMSG;
        }
        if( (msg->messageStatus & OSEK_COM_MESSAGE_OVERFLOW) != 0 )
        {
			return E_COM_LIMIT;
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0 */

    return E_OK;

}

#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS) */


/*
 * @brief
 *    SendMessage��������Ϣ��
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 *              data�����������������ã��׵�ַ����
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:E_OK            �ɹ����ء�
 * <p>      E_COM_LOCKED    ��Ϣ�����Ѿ�����
 * <p>      [E_COM_ID       ��Ч��Ϣ]
 * <p>      [E_OS_CALLEVEL  ��ISR�е��ò�ʹ������Ч������]
 *
 * <p>Notes:��
 */

StatusType SendMessage(MsgType msg, AccessNameRef data)
{
    OSEK_TARGET_OSIntSave( osIntSave );

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

	// �����Ϣ�Ƿ���Ч����Ч����E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_SendMessage,msg);
    }

    // ����Ǵ�ISR�е��õĸ÷��������ISR�е��õ���ȷ�ԡ����Ƕ�����Ϣʹ���˷ǿ�����ʽ�����Ƕ�����Ϣ������E_OS_CALLEVEL
    if ( osekInterrupt_NestedLevl != 0 )
    {
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES //���������ȫ�����Ƕ�����Ϣ
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES == 0                                  //ȫ�����ǷǶ�����Ϣ
		// �ǿ�����ʽ
        if( msg->messageConfigTable.messagePtr == data )
#else														                        //���ж�����Ϣ���зǶ�����Ϣ
        //�Ƕ�����Ϣʹ���˷ǿ�����ʽ�����Ƕ�����Ϣ
        if( (msg->messageConfigTable.messagePtr == data) || (msg->messageConfigTable.fifoLength != 0) )
#endif
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */
        {
			OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_SendMessage,msg);
        }
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

    OSEK_TARGET_DisableOSInt(osIntSave);


	// ��ͼ����Ϣ�����ʧ�ܷ���E_COM_LOCKED
    if (msg_LockMessage( msg ) != E_OK )
    {
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_LOCKED,OSServiceId_SendMessage,msg);
    }

    OSEK_TARGET_EnableOSInt(osIntSave);

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES //������㣬���Ƕ�����Ϣ
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                  //������㣬���ǷǶ�����Ϣ
    // �Ƕ�����Ϣ
    if( msg->messageConfigTable.fifoLength == 0 )
#endif
    {
        // ����ǿ�����ʽ���ͣ���ָ������ݿ�������Ϣ������
        if (msg->messageConfigTable.messagePtr != data )
        {
            memcpy( (void*)msg->messageConfigTable.messagePtr, data, msg->messageConfigTable.messageSize );
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                  //������㣬��ȫ�ǷǶ�����Ϣ
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES //���ж�����Ϣ���зǶ�����Ϣ
    // ������Ϣ��ֻ��ʹ�ÿ�����ʽ
    else
#endif
    {
        // ������Ϣ����ڶ��г��ȣ�������Ϣ״̬Ϊ�����´ν�����Ϣʱ�ɻ����������Ϣ;ָ�����δ�����͵���Ϣ�����У���򵥵ض����
        if( msg->fifoCount == msg->messageConfigTable.fifoLength )
        {
            msg->messageStatus |= OSEK_COM_MESSAGE_OVERFLOW;
        }
        else
        {
            // ��ָ����ݿ�������Ϣ������
            memcpy( (void*)msg->fifoWrite, data, msg->messageConfigTable.messageSize );

            // ���дָ�뵽�����β�������������ͷ��
            if( msg->fifoWrite == msg->messageConfigTable.fifoLast )
            {
                msg->fifoWrite = (OSBYTEPTR)((msg->messageConfigTable).messagePtr);
            }
            // ������Ϣ���е�дָ��ǰ��һ��
            else
            {
                msg->fifoWrite += msg->messageConfigTable.messageSize;
            }

            msg->fifoCount++;
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0 */

    // ������Ϣ����
    msg_UnlockMessage( msg );

#if defined(OSEK_MSG_USE_NOTIFICATION)
	// ����Ϣ����ʱ����֪ͨ�ź�

    OSEK_TARGET_DisableOSInt(osIntSave);

    if ((msg_SignalMessage( msg ) == OSEK_TASK_NEED_DISPATCH)        // ��֪ͨ�������������񱻼���
        && (osekInterrupt_NestedLevl == 0)                           // �����������񼶵��õĴ˷�������ò���ϵͳ����ǲ����
    )
    {
    	osekTask_Dispatch();
    }

    OSEK_TARGET_EnableOSInt(osIntSave);

#endif  /* defined(OSEK_MSG_USE_NOTIFICATION) */

    return E_OK;

}


/*
 * @brief
 *    ReceiveMessage��������Ϣ��
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 *              data��������ݵĻ���������ã��׵�ַ����
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:E_OK            �ɹ����ء�
 * <p>      E_COM_LOCKED    ��Ϣ�Ѿ�����
 * <p>      E_COM_NOMSG     ��Ϣ����Ϊ�ա�
 * <p>      E_COM_LIMIT��   ���ϴε�����4����������Ϣ���
 * <p>      [E_COM_ID       ��Ч��Ϣ��]
 * <p>      [E_OS_CALLEVEL  ��ISR�е��ò�ʹ������Ч������]
 *
 * <p>Notes:��
 */

StatusType ReceiveMessage(MsgType msg, AccessNameRef data)
{
    OSEK_TARGET_OSIntSave( osIntSave );
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
	// �Ƚ�����״̬ȱʡ����ΪE_OK
    StatusType status = E_OK;
#endif

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

    // �����Ϣ�Ƿ���Ч����Ч����E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_ReceiveMessage,msg);
    }

    // �Ǵ�ISR�е��õĸ÷��������ISR�е��õ���ȷ�ԡ����Ƕ�����Ϣʹ���˷ǿ�����ʽ�����Ƕ�����Ϣ������E_OS_CALLEVEL
    if ( osekInterrupt_NestedLevl != 0 )
    {
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES  //���������ȫ�����Ƕ�����Ϣ
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES == 0                                   //ȫ�����ǷǶ�����Ϣ
        // �ǿ�����ʽ
        if( msg->messageConfigTable.messagePtr == data )
#else														                         //���ж�����Ϣ���зǶ�����Ϣ
        //�Ƕ�����Ϣʹ���˷ǿ�����ʽ�����Ƕ�����Ϣ
        if( (msg->messageConfigTable.messagePtr == data) || (msg->messageConfigTable.fifoLength != 0) )
#endif
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */
        {
			OSEK_HOOK_ErrorHook(E_OS_CALLEVEL,OSServiceId_ReceiveMessage,msg);
        }
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

    OSEK_TARGET_DisableOSInt(osIntSave);


    // ��ͼ����Ϣ�����ʧ�ܷ���E_COM_LOCKED
    if( msg_LockMessage( msg ) != E_OK )
    {
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_LOCKED,OSServiceId_ReceiveMessage,msg);
    }

    OSEK_TARGET_EnableOSInt(osIntSave);

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES  //������㣬���Ƕ�����Ϣ
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                   //������㣬���ǷǶ�����Ϣ
    // ���ڷǶ�����Ϣ��������Կ�����ʽ������Ϣ������Ϣ�����е���ݿ�����ָ��λ��
    if( msg->messageConfigTable.fifoLength == 0 )
#endif
    {
        if( msg->messageConfigTable.messagePtr != data )
        {
            memcpy( data, (void*)msg->messageConfigTable.messagePtr, msg->messageConfigTable.messageSize );
        }
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES */

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0                                   //������㣬��ȫ�ǷǶ�����Ϣ
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != CONFIG_OSEK_COM_NUMBER_OF_MESSAGES  //���ж�����Ϣ���зǶ�����Ϣ
    // ���ڶ�����Ϣ�����Կ�����ʽ������Ϣ
    else
#endif
    {
        // ������û����Ϣ��������Ϣ���󣬷���E_COM_NOMSG
        if( msg->fifoCount == 0 )
        {
            msg_UnlockMessage( msg );
			OSEK_HOOK_ErrorHook(E_COM_NOMSG,OSServiceId_ReceiveMessage,msg);
        }

        // �����Ϣ�������������״̬��������statusΪE_COM_LIMIT
        if( (msg->messageStatus & OSEK_COM_MESSAGE_OVERFLOW) != 0 )
        {
            status = E_COM_LIMIT;
            msg->messageStatus &= (OSBYTE)~OSEK_COM_MESSAGE_OVERFLOW;
        }
        // ����Ϣ�����п�����Ϣ
        memcpy( data, (void*)msg->fifoRead, msg->messageConfigTable.messageSize );

        // ����ָ�뵽����Ϣ����β�������������ͷ��
        if( msg->fifoRead == msg->messageConfigTable.fifoLast )
        {
            msg->fifoRead = (OSBYTEPTR)((msg->messageConfigTable).messagePtr);
        }
        // ������Ϣ���еĶ�ָ��ǰ��һ��
        else
        {
            msg->fifoRead += msg->messageConfigTable.messageSize;
        }

        msg->fifoCount--;
    }
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES != 0 */

    msg_UnlockMessage( msg );

#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
    // ���ش�����Ϣ
    if( status != E_OK )
    {
		OSEK_HOOK_ErrorHook(status,OSServiceId_ReceiveMessage,msg);
    }
#endif

    return E_OK;

}


#if defined(OSEK_MSG_USE_NOTIFICATION)

/*
 * @brief
 *    msg_SignalMessage������Ϣ����ʱ�����ã�����йص�֪ͨ���ܣ���������������¼���ص�������ñ�־���ڲ�����
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:OSEK_TASK_NEED_DISPATCH��0xFF��            ��Ҫ���µ�ȡ�
 *          0������ֵ������������õ�������ķ���ֵ��   ����Ҫ���µ�ȡ�
 *
 * Notes:
 */

StatusType msg_SignalMessage( MsgType msg )
{

#if defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT)
    TaskType taskId;
	T_OSEK_TASK_ControlBlock *tcbPtr;
#endif

#if defined(CONFIG_OSEK_MSG_FLAG)
	// ��������˱�־�������ñ�־������0
    if( msg->messageConfigTable.messageSigFlag != OSEK_COM_NO_FLAG )
    {
        msg_SetFlag(msg);
        return 0;
    }
#endif  /* defined(CONFIG_OSEK_MSG_FLAG) */

#if defined(CONFIG_OSEK_MSG_CALLBACK)
    // ��������˻ص�����������ҷ���0
    if (msg->messageConfigTable.messageSigCallBack != 0 )
    {
        (msg->messageConfigTable.messageSigCallBack)();
        return 0;
    }
#endif  /* defined(CONFIG_OSEK_MSG_CALLBACK) */

#if defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_ACTIVATETASK)
	// ��������������¼��򼤻����񣬻�ȡ����ID�źͿ��ƿ�
    taskId = msg->messageConfigTable.messageSigTask;
    // ���ǲ��Է�������4�д������࣬���֤����û�����⣬��������Զ����ִ�еġ�
    //if( taskId == INVALID_TASK )
    //{
    //	return 0;
    //}
	tcbPtr = &osekTask_TaskTable[taskId];
#endif  /* defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_ACTIVATETASK) */

#if defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_MSG_SETEVENT)
	// ��������������õ��¼����������¼�����������
    if( msg->messageConfigTable.messageSigEvent != 0 )
    {
	 	return osekEvent_SetEvent(tcbPtr,msg->messageConfigTable.messageSigEvent);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_ET) && defined(CONFIG_OSEK_MSG_SETEVENT) */

#if defined(CONFIG_OSEK_MSG_ACTIVATETASK)
  	// ������������㣬�����������輤��������򼤻����񲢷�������
  	return osekTask_ActiveTask(tcbPtr);
#endif  /* defined(CONFIG_OSEK_MSG_ACTIVATETASK) */

    return 0;  /* ���򲻻�ִ�е�����˾�ֻ�Ƿ�ֹ������� */

}

#endif  /* defined(OSEK_MSG_USE_NOTIFICATION) */


/*
 * @brief
 *    msg_LockMessage�������ͻ������Ϣ��ʱ����ñ���������Ϣ���ڲ�����
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:E_OK, �ɹ����ء�
 *          E_COM_LOCKED, ��Ϣ�Ѿ�����
 * Notes:  ������Ӧ���ڽ�ֹ�жϵ�����±����á�
 */

StatusType msg_LockMessage( MsgType msg )
{
    // �����Ϣ�����Ѿ���������E_COM_LOCKED
    if ((msg->messageStatus & (OSEK_COM_MESSAGE_LOCKED )) != 0 )
    {
        return E_COM_LOCKED;
    }

    // ����Ϣ
    msg->messageStatus |= OSEK_COM_MESSAGE_LOCKED;

    return E_OK;
}


#if defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE)

/*
 * @brief
 *    GetMessageResource����ָ����Ϣ�����״̬����Ϊ��æ����
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:E_OK, �ɹ����ء�
 * <p>      E_COM_LOCKED, ��Ϣ�Ѿ�����
 * <p>      E_COM_BUSY, ��Ϣ�Ѿ�������Ϊ��æ��״̬��
 * <p>      [E_COM_ID,   ()չ״̬)   ��Ч��Ϣ��]
 */

StatusType GetMessageResource( MsgType msg )
{
    OSEK_TARGET_OSIntSave( osIntSave );

#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;
	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

    // �����Ϣ�Ƿ���Ч����Ч����E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_GetMessageResource,msg);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */


    OSEK_TARGET_DisableOSInt(osIntSave);

    // ��ͼ����Ϣ�����ʧ�ܷ���E_COM_LOCKED
    if( msg_LockMessage(msg) != E_OK )
    {
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_LOCKED,OSServiceId_GetMessageResource,msg);
    }

    // �����Ϣ����æ״̬������E_COM_BUSY
    if( (msg->messageStatus & OSEK_COM_MESSAGE_BUSY) != 0 )
    {
        msg_UnlockMessage( msg );
        OSEK_TARGET_EnableOSInt(osIntSave);
		OSEK_HOOK_ErrorHook(E_COM_BUSY,OSServiceId_GetMessageResource,msg);
    }

    // ������Ϣ����Ϊ��æ��״̬
    msg->messageStatus |= OSEK_COM_MESSAGE_BUSY;
    // ������Ϣ
    msg_UnlockMessage( msg );
    OSEK_TARGET_EnableOSInt(osIntSave);

    return E_OK;

}


/*
 * @brief
 *    ReleaseMessageResource�������Ϣ����ġ�æ��״̬��
 * @param		msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns:E_OK, �ɹ����ء�
 * <p>      [E_COM_ID,   ()չ״̬)   ��Ч��Ϣ��]
 */

StatusType ReleaseMessageResource( MsgType msg )
{
#if defined(CONFIG_OSEK_SYSTEM_EXTSTATUS)
    OSMSGIDTYPE i;

	i = 0;
	while( (i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES) && (msg != &(osekMsg_MessageTable[i])) )
	{
		i++;
	}

    // �����Ϣ�Ƿ���Ч����Ч����E_COM_ID
    if (i == CONFIG_OSEK_COM_NUMBER_OF_MESSAGES )
    {
		OSEK_HOOK_ErrorHook(E_COM_ID,OSServiceId_ReleaseMessageResource,msg);
    }
#endif  /* defined(CONFIG_OSEK_SYSTEM_EXTSTATUS) */

    // �����Ϣ�ġ�æ��״̬
    msg->messageStatus &= (OSBYTE)~OSEK_COM_MESSAGE_BUSY;
    return E_OK;

}

#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE) */


#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0

/*
 * @brief
 *    ReadFlag����ȡ��־��
 * @param		flagName��ָ����־����ơ�
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns: ��־ֵ��
 */

FlagValue ReadFlag(FlagType flagName)
{
    return (*flagName);
}

/*
 * @brief
 *    ResetFlag����λ��־��
 * @param		flagName��ָ����־����ơ�
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns: E_OK, �ɹ����ء�
 */

StatusType ResetFlag( FlagType flagName )
{
    OSEK_TARGET_OSIntSave( osIntSave );

    OSEK_TARGET_DisableOSInt(osIntSave);

    *flagName = FALSE;

    OSEK_TARGET_EnableOSInt(osIntSave);

    return E_OK;
}

#endif /* CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0 */


/*
 * @brief
 *    StartCOM���� COM��
 * @param		�ޡ�
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns��Ӧ���ṩ����Ϣ��ʼ������ķ���ֵ��
 */

StatusType StartCOM( void )
{
    StatusType osRetVal;

    // ����COMģ��ĳ�ʼ������
    Msg_Init();
    // ����Ӧ���ṩ�ĳ�ʼ������
    osRetVal = MessageInit();

    return osRetVal;
}


/*
 * @brief
 *    InitCOM����ʼ��COMģ�顣
 * @param		�ޡ�
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns: E_OK, �ɹ����ء�
 */

StatusType InitCOM( void )
{
    return E_OK;
}


/*
 * @brief
 *    CloseCOM���ر�COMģ�顣
 * @param		�ޡ�
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @returns: E_OK, �ɹ����ء�
 */

StatusType CloseCOM( void )
{
    return E_OK;
}

/*
 * @brief
 *    StopCOM�����������Ϣ״̬��
 * @param		ShutdownMode���ر�ģʽ���ڱ�ʵ����δ���ô˲���
 * @param[in]	�ޡ�
 * @param[out]	�ޡ�
 * @return: E_OK,       �ɹ����ء�
 * <p>      E_COM_BUSY, ����Ϣ���ڡ�æ��״̬����
 */

StatusType StopCOM(OSBYTE ShutdownMode)
{
    OSMSGIDTYPE i;

    for( i = 0; i < CONFIG_OSEK_COM_NUMBER_OF_MESSAGES; i++ )
    {
        if( (osekMsg_MessageTable[i].messageStatus & ( OSEK_COM_MESSAGE_BUSY | OSEK_COM_MESSAGE_LOCKED )) != 0 )
        {
			OSEK_HOOK_ErrorHook(E_COM_BUSY,OSServiceId_StopCOM,&osekMsg_MessageTable[i]);
        }
    }

    return E_OK;
}

#endif  /* CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0 */
#endif  /* defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY) */
