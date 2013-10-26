/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/**
 * $Log: osekMessage.h,v $
 * Revision 1.0  2008/03/26 13:16:00  chenlr
 * �����ļ�
 */

/**
 * @file 	osekMessage.h
 * @brief
 *	<li>���ܣ�������Ϣͨ��ģ��������ⲿAPI���Լ��ڲ�ģ����ȫ��API��</li>
 * @author 	chenlr
 * @date 	2008-03-26
 * <p>���ţ�
 */

/**
 * \defgroup  Message ��Ϣͨ��
 * ͨ�Ź���ģ��,��Ҫʵ���ڲ���Ϣͨ�ŵĹ���
 */
/**@{*/
/** @name ͨ�Ź���ģ��*/
//@{


#ifndef _OSEK_MESSAGE_H
#define _OSEK_MESSAGE_H

/**************************** ���ò��� *********************************/


#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)

/**************************** ����� *********************************/

// �ڲ�����
void Msg_Init( void );

/**
 * @brief
 *    MessageInit��Ӧ���ṩ����Ϣ��ʼ������ԭ�ͣ��ú���StartCOM������á�
 * @param   	�ޡ�
 * @returns     Ӧ���Զ���ķ���״̬������ΪStatusType��
 */
extern StatusType MessageInit (void);

#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0

//  ͨ�Ź����ṩ���ⲿ�Ĺ������ú���

/**
 * @brief
 *    SendMessage��������Ϣ������������Ϣʱ�ú���Ŵ��ڣ���
 * @param[in]	msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	data�����������������ã��׵�ַ����
 * @returns     E_OK            �ɹ����ء�
 * @returns     E_COM_LOCKED    ��Ϣ�����Ѿ�����
 * @returns     [E_COM_ID       ��Ч��Ϣ]
 * @returns     [E_OS_CALLEVEL  ��ISR�е��ò�ʹ������Ч������]
 */
StatusType SendMessage(MsgType msg, AccessNameRef data);

/**
 * @brief
 *    ReceiveMessage��������Ϣ������������Ϣʱ�ú���Ŵ��ڣ���
 * @param[in]	msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @param[in]	data��������ݵĻ���������ã��׵�ַ����
 * @returns     E_OK            �ɹ����ء�
 * @returns     E_COM_LOCKED    ��Ϣ�Ѿ�����
 * @returns     E_COM_NOMSG     ��Ϣ����Ϊ�ա�
 * @returns     E_COM_LIMIT��   ���ϴε�����4����������Ϣ���
 * @returns     [E_COM_ID       ��Ч��Ϣ��]
 * @returns     [E_OS_CALLEVEL  ��ISR�е��ò�ʹ������Ч������]
 */
StatusType ReceiveMessage(MsgType msg, AccessNameRef data);

/**
 * @brief
 *    StopCOM�����������Ϣ״̬������������Ϣʱ�ú���Ŵ��ڣ���
 * @param[in]	ShutdownMode���ر�ģʽ���ڱ�ʵ����δ���ô˲���
 * @returns     E_OK       �ɹ����ء�
 * @returns     E_COM_BUSY ����Ϣ���ڡ�æ��״̬����
 */
StatusType StopCOM(OSBYTE ShutdownMode);

/**
 * @brief
 *    StartCOM���� COM������������Ϣʱ�ú���Ŵ��ڣ���
 * @param		�ޡ�
 * @returns     Ӧ���ṩ����Ϣ��ʼ������ķ���ֵ��
 */
StatusType StartCOM( void );

/**
 * @brief
 *    InitCOM����ʼ��COMģ�飨����������Ϣʱ�ú���Ŵ��ڣ���
 * @param		�ޡ�
 * @returns     E_OK, �ɹ����ء�
 */
StatusType InitCOM( void );

/**
 * @brief
 *    CloseCOM���ر�COMģ�飨����������Ϣʱ�ú���Ŵ��ڣ���
 * @param		�ޡ�
 * @returns     E_OK, �ɹ����ء�
 */
StatusType CloseCOM( void );

// �����������Ϣ״̬��ع���
#if defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS)
/**
 * @brief
 *    GetMessageStatus����ȡ��Ϣ��״̬������������Ϣ״̬ʱ�ú���Ŵ��ڣ���
 * @param[in]	msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @returns     [E_COM_ID: ��Ч��Ϣ]
 * @returns     E_COM_LOCKED: ��Ϣ����
 * @returns     E_COM_BUSY: ��Ϣ����æ״̬
 * @returns     E_COM_NOMSG: ��Ϣ����������Ϣ
 * @returns     E_COM_LIMIT: ��Ϣ������
 * @returns     E_OK: ���������κ�״̬
 *
 * <p>Notes:    ��ID��Ч�⣬���������κ�����¶�������error hook��
 */
StatusType GetMessageStatus( MsgType msg );
#endif  /* defined(CONFIG_OSEK_COM_USE_MESSAGE_STATUS) */

// ���Ҫʹ����Ϣ�����־
#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0
/**
 * @brief
 *    ReadFlag����ȡ��־������������Ϣ�������ñ�־ʱ�ú���Ŵ��ڣ���
 * @param[in]	flagName��ָ����־����ơ�
 * @returns     ��־ֵ��
 */
FlagValue ReadFlag(FlagType flag);

/**
 * @brief
 *    ResetFlag����λ��־������������Ϣ�������ñ�־ʱ�ú���Ŵ��ڣ���
 * @param[in]	flagName��ָ����־����ơ�
 * @returns     E_OK, �ɹ����ء�
 */
StatusType ResetFlag(FlagType flag);
#endif  /* CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0 */

// ���Ҫʹ����Ϣ��Դ
#if defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE)
/**
 * @brief
 *    GetMessageResource����ָ����Ϣ�����״̬����Ϊ��æ��������������Ϣ��Դʱ�ú���Ŵ��ڣ���
 * @param[in]	msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @returns     E_OK, �ɹ����ء�
 * @returns     E_COM_LOCKED, ��Ϣ�Ѿ�����
 * @returns     E_COM_BUSY, ��Ϣ�Ѿ�������Ϊ��æ��״̬��
 * @returns     [E_COM_ID,   ()չ״̬)   ��Ч��Ϣ��]
 */
StatusType GetMessageResource( MsgType msg );

/**
 * @brief
 *    ReleaseMessageResource�������Ϣ����ġ�æ��״̬������������Ϣ��Դʱ�ú���Ŵ��ڣ���
 * @param[in]	msg��ָ���ض���Ϣ������ƿ��ָ�롣
 * @returns     E_OK, �ɹ����ء�
 * @returns     [E_COM_ID,   ()չ״̬)   ��Ч��Ϣ��]
 */
StatusType ReleaseMessageResource( MsgType msg );
#endif /* defined(CONFIG_OSEK_COM_USE_MESSAGE_RESOURCE) */

#endif  /* CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0  */

/**************************** ���岿�� *********************************/

#if defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_CALLBACK) || defined(CONFIG_OSEK_MSG_FLAG)
#define OSEK_MSG_USE_NOTIFICATION
#endif /* defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT) || defined(CONFIG_OSEK_MSG_CALLBACK) || defined(CONFIG_OSEK_MSG_FLAG) */

#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0

/// ��Ϣ������Ϣ��ṹ������������Ϣʱ�ú���Ŵ��ڣ�
struct T_OSEK_MESSAGE_ConfigTable_Struct
{
    /// ��Ϣ�����ID(����Ϣ�����е����)
    OSMSGIDTYPE    messageId;
    /// ָ����Ϣʵ���ָ��
    void*          messagePtr;
    /// ���ֽ�Ϊ��λ�ĵ�����Ϣ�ߴ�
    OSDWORD        messageSize;
// �����������Ϣ����ʱ��������������¼�
#if defined(CONFIG_OSEK_MSG_ACTIVATETASK) || defined(CONFIG_OSEK_MSG_SETEVENT)
    /// ��Ϣ����ʱ��֪ͨ����������������Ϣ���Ｄ������������¼�ʱ�����ԲŴ��ڣ�
    TaskType       messageSigTask;
#endif
// �����������Ϣ����ʱ�����¼�
#if defined(CONFIG_OSEK_MSG_SETEVENT)
    /// ��Ϣ����ʱ�����õ��¼�������������Ϣ���������¼�ʱ�����ԲŴ��ڣ�
    EventMaskType  messageSigEvent;
#endif
// �����������Ϣ����ʱ�ص���
#if defined(CONFIG_OSEK_MSG_CALLBACK)
    /// ��Ϣ����ʱ�����õĻص�������������Ϣ����ص�ʱ�����ԲŴ��ڣ�
    OSCALLBACK     messageSigCallBack;
#endif
// �����������Ϣ����ʱ���ñ�־
#if defined(CONFIG_OSEK_MSG_FLAG)
    /// ��Ϣ����ʱ�����õı�־������������Ϣ�������ñ�־ʱ�����ԲŴ��ڣ�
    OSBYTE         messageSigFlag;
#endif
// ����ж�������Ϣ����
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
    /// ָ����Ϣ�������Ԫ�ص�ָ�루�������˶�������Ϣ����ʱ�����ԲŴ��ڣ�
    OSBYTEPTR      fifoLast;
    /// ��Ϣ���еĳ��ȣ��������˶�������Ϣ����ʱ�����ԲŴ��ڣ�
    OSBYTE         fifoLength;
#endif
};

/// ��Ϣ���ƿ�ṹ������������Ϣʱ�ú���Ŵ��ڣ�
struct T_OSEK_MESSAGE_ControlBlock_Struct
{
    /// ��Ϣ״̬��Ϣ
    OSBYTE          messageStatus;
    /// ��Ϣ������Ϣ��
	T_OSEK_MESSAGE_ConfigTable messageConfigTable;
// ����ж�������Ϣ����
#if CONFIG_OSEK_COM_NUMBER_OF_QUEUED_MESSAGES > 0
	///��һ����Ϣд������е�λ�ã��������˶�������Ϣ����ʱ�����ԲŴ��ڣ�
    OSBYTEPTR       fifoWrite;
    /// ��������һ���ȡ����Ϣ��λ�ã��������˶�������Ϣ����ʱ�����ԲŴ��ڣ�
    OSBYTEPTR       fifoRead;
    /// ������������Ϣ�����������˶�������Ϣ����ʱ�����ԲŴ��ڣ�
    OSBYTE          fifoCount;
#endif

// �����Ϣ�����־�����0
#if CONFIG_OSEK_COM_NUMBER_OF_FLAGS > 0
	/// ����Ϣ����ʹ�õı�־������������Ϣ�������ñ�־ʱ�����ԲŴ��ڣ�
    OSBYTE          messageFlag;
#endif

};

// ��Ϣ������ƿ������
extern T_OSEK_MESSAGE_ControlBlock osekMsg_MessageTable[CONFIG_OSEK_COM_NUMBER_OF_MESSAGES];
/// �ⲿ��Ϣ���ñ����������������Ϣʱ�ú���Ŵ��ڣ�
extern const T_OSEK_MESSAGE_ConfigTable osekConfig_MsgTable[];


/// b���ر�ͨ��ģ��ĺ꣨����������Ϣʱ�ú���Ŵ��ڣ�
#define COM_SHUTDOWN_IMMEDIATE  0

/// ��Ϣ״̬:��Ϣ�Ѿ���������������Ϣʱ�ú���Ŵ��ڣ�
#define OSEK_COM_MESSAGE_LOCKED             0x01
/// ��Ϣ״̬:��Ϣ�����ڡ�æ��״̬������������Ϣʱ�ú���Ŵ��ڣ�
#define OSEK_COM_MESSAGE_BUSY               0x02
/// ��Ϣ״̬:��Ϣ�������������Ϣʱ�ú���Ŵ��ڣ�
#define OSEK_COM_MESSAGE_OVERFLOW           0x80

/// ����Ϣ�����־�ĺ꣨����������Ϣʱ�ú���Ŵ��ڣ�
#define OSEK_COM_NO_FLAG                0xFF

#endif  /* CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0 */

#endif  /* defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY) */

#ifdef __cplusplus
}
#endif	/* __cplusplus */

#endif	/* _OSEK_MESSAGE_H */


/* end of include file */
//@}
/** @}*/
