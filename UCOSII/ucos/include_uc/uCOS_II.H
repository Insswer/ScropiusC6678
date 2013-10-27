/*
*********************************************************************************************************
* uC/OS-IIʵʱ�����ں�
* ��Ҫ�İ��ļ�
* �� ��: uCOS_II.H ucos�ڲ���������趨
* �� ��: Jean J. Labrosse
*********************************************************************************************************
*/
#include "includes.h"
#include "os_cpu.h"
#include "os_cfg.h"

//���ӵĶ���
#define  OS_VERSION              252       //����UCOSII�İ汾��,������2.52�汾

#ifdef   OS_GLOBALS						   //��� OS_GLOBALS �ѱ�������, ������뽫�ᱻ����
#define  OS_EXT							   //���� OS_EXT
#else
#define  OS_EXT  extern					   //���򣬶��� OS_EXT Ϊ extern
#endif


//������stm32f10x_type.h������ö�ٵķ�ʽ�����ˣ����Բ���Ҫ�ٶ�����
//#ifndef  FALSE						   //�Ƿ�δ���� FALSE
//#define  FALSE                     0	   //���û�������� FALSE Ϊ 0
//#endif
//
//#ifndef  TRUE							   //�Ƿ�δ���� TRUE
//#define  TRUE                      1	   //���û�������� TRUE Ϊ 1
//#endif

#define  OS_PRIO_SELF           0xFF       //���� OS_PRIO_SELF Ϊ 0xFF

#if OS_TASK_STAT_EN > 0
#define  OS_N_SYS_TASKS            2       //ϵͳ������Ϊ2
#else
#define  OS_N_SYS_TASKS            1	   //ϵͳ������Ϊ1
#endif

#define  OS_STAT_PRIO       (OS_LOWEST_PRIO - 1)        //ͳ���������ȼ�
#define  OS_IDLE_PRIO       (OS_LOWEST_PRIO)            //�����������ȼ�

#define  OS_EVENT_TBL_SIZE ((OS_LOWEST_PRIO) / 8 + 1)   //�¼��б��С
#define  OS_RDY_TBL_SIZE   ((OS_LOWEST_PRIO) / 8 + 1)   //�����б��С

#define  OS_TASK_IDLE_ID       65535                    //����}��ϵͳ�����ID
#define  OS_TASK_STAT_ID       65534

#define  OS_EVENT_EN       (((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0))

//*********************************************************************************************************
//����״̬�� TASK STATUS (OSTCBStat��λ����)
#define  OS_STAT_RDY            0x00        // (�������״̬��)���ھ���״̬
#define  OS_STAT_SEM            0x01        // (�������״̬��)�����ź�״̬
#define  OS_STAT_MBOX           0x02        // (�������״̬��)��������״̬
#define  OS_STAT_Q              0x04        // (�������״̬��)����Q״̬
#define  OS_STAT_SUSPEND        0x08        // ��ʾ���񱻹���
#define  OS_STAT_MUTEX          0x10        // (�������״̬��)����MUTEX״̬
#define  OS_STAT_FLAG           0x20        // (�������״̬��)����FLAG״̬

//*********************************************************************************************************
//�¼�����(OS_EVENT types)
#define  OS_EVENT_TYPE_UNUSED      0        // �����¼����͵�����(���¼�����------------0)
#define  OS_EVENT_TYPE_MBOX        1		// �����¼����͵�����(����Ϊ��������--------1)
#define  OS_EVENT_TYPE_Q           2		// �����¼����͵�����(��Ϣ����Ϊ��������----2)
#define  OS_EVENT_TYPE_SEM         3	    // �����¼����͵�����(�ź�Ϊ��������------3)
#define  OS_EVENT_TYPE_MUTEX       4	    // �����¼����͵�����(�������ź�Ϊ��������4)
#define  OS_EVENT_TYPE_FLAG        5		// �����¼����͵�����(�¼���־��Ϊ��������--5)

//*********************************************************************************************************
//�¼���־(EVENT FLAGS)
#define  OS_FLAG_WAIT_CLR_ALL      0        // ��������ָ���¼���־λ��0 ------ 0
#define  OS_FLAG_WAIT_CLR_AND      0

#define  OS_FLAG_WAIT_CLR_ANY      1        // ��������ָ���¼���־λ��0 ------ 1
#define  OS_FLAG_WAIT_CLR_OR       1

#define  OS_FLAG_WAIT_SET_ALL      2        // ��������ָ���¼���־λ��1 ------ 2
#define  OS_FLAG_WAIT_SET_AND      2

#define  OS_FLAG_WAIT_SET_ANY      3        // ��������ָ���¼���־λ��1 ------ 3
#define  OS_FLAG_WAIT_SET_OR       3

// �����Ҫ�ڵõ������־�󣬻ָ����¼���־������˳�
#define  OS_FLAG_CONSUME        0x80        // ���峣OS_FLAG_CONSUMEΪ0x80

											
#define  OS_FLAG_CLR               0		// ���� OS_FLAG_CLR Ϊ��0
#define  OS_FLAG_SET               1	    // ���� OS_FLAG_SET Ϊ��1

//*********************************************************************************************************
//��������'opt'�У������� OSSemDel(), OSMboxDel(), OSQDel() �� OSMutexDel()����
#define  OS_DEL_NO_PEND            0		// ����ѡ��ֻ�����Ѿ�û���κ������ڵȴ���ź�ʱ������ɾ����ź�
#define  OS_DEL_ALWAYS             1		// ������û�������ڵȴ���ź���b��ɾ����ź�

//*********************************************************************************************************
//OS???PostOpt() OPTIONS(����)
//��Щ���������� OSMboxPostOpt() �� OSQPostOpt()}����.
#define  OS_POST_OPT_NONE       0x00       	// ����һ����Ϣ(������)��һ��ȴ���Ϣ�����ȼ���ߵ�����
#define  OS_POST_OPT_BROADCAST  0x01        // ������Ϣ�����еȴ������Ϣ������
#define  OS_POST_OPT_FRONT      0x02        // �Ժ���ȳ�ʽ����Ϣ(����OSQPostFront())

//*********************************************************************************************************
//�������� TASK OPTIONS (�鿴OSTaskCreateExt())
#define  OS_TASK_OPT_STK_CHK  0x0001        // ���Ƿ���������ջ���
#define  OS_TASK_OPT_STK_CLR  0x0002        // ���Ƿ���ն�ջ
#define  OS_TASK_OPT_SAVE_FP  0x0004        // ���Ƿ񱣴渡��Ĵ������ֵ����������������и����ʱ��Ч�����������Ӳ����صĴ������

//*********************************************************************************************************
//������� ERROR CODES
#define OS_NO_ERR                 0			// ����سɹ�;

#define OS_ERR_EVENT_TYPE         1			// ����ָ���¼�(���)���͵�ָ��;
#define OS_ERR_PEND_ISR           2			// ���жϷ����ӳ����е��� OS�����ź���Accept()����.
#define OS_ERR_POST_NULL_PTR      3			// �û������ָ�롣��ݹ������ﲻ֧�ֿ�ָ��;
#define OS_ERR_PEVENT_NULL        4			// 'pevent'��ָ��ָ��;
#define OS_ERR_POST_ISR           5			// ��ͼ���жϷ����ӳ����е���OSMutexPost()����[�ͷ�һ��mutex];
#define OS_ERR_QUERY_ISR          6			// ��ͼ���ж��ӳ����е���OSMutexQuery()[�õ�mutex��ǰ״̬��Ϣ]
#define OS_ERR_INVALID_OPT        7			// �����opt������Ч;
#define OS_ERR_TASK_WAITING       8			// ��һ���һ�����ϵ������ڵȴ���Ϣ�����е���Ϣ;

#define OS_TIMEOUT               10			// ��Ϣû����ָ�������������͵�;
#define OS_TASK_NOT_EXIST        11			// ָ�������񲻴�;

#define OS_MBOX_FULL             20			// ��Ϣ�����Ѿ�����������Ϣ������;

#define OS_Q_FULL                30			// ��Ϣ�������Ѿ�����;

#define OS_PRIO_EXIST            40			// ���ȼ�ΪPIP�������Ѿ�����;
#define OS_PRIO_ERR              41			// �����е�����ԭ�����ȼ�������;
#define OS_PRIO_INVALID          42			// ����ָ�������ȼ�����OS_LOWEST_PRIO;

#define OS_SEM_OVF               50			// �ź���ֵ���;

#define OS_TASK_DEL_ERR          60			// ָ��Ҫɾ������񲻴���
#define OS_TASK_DEL_IDLE         61			// ���������ͼɾ���������(Idle task);
#define OS_TASK_DEL_REQ          62			// ��ǰ�����յ�4�����������ɾ������;
#define OS_TASK_DEL_ISR          63			// ���������ͼ���жϴ��������ɾ������;

#define OS_NO_MORE_TCB           70			// ϵͳ��û��OS_TCB���Է����������;

#define OS_TIME_NOT_DLY          80			// Ҫ���ѵ���������ʱ״̬;
#define OS_TIME_INVALID_MINUTES  81			// ������󣬷��������59;
#define OS_TIME_INVALID_SECONDS  82			// ��������������59
#define OS_TIME_INVALID_MILLI    83			// �򷵻ز�����󣬺��������999;
#define OS_TIME_ZERO_DLY         84		    // �ĸ����ȫΪ0

#define OS_TASK_SUSPEND_PRIO     90			// Ҫ��������񲻴���
#define OS_TASK_SUSPEND_IDLE     91			// ��ͼ����uC/OS-II�еĿ�������(Idle task)

#define OS_TASK_RESUME_PRIO     100		    // Ҫ���ѵ����񲻴���;
#define OS_TASK_NOT_SUSPENDED   101			// Ҫ���ѵ������ڹ���״̬

#define OS_MEM_INVALID_PART     110			// û�п��е��ڴ���;
#define OS_MEM_INVALID_BLKS     111			// û��Ϊÿһ���ڴ���b����2���ڴ��;
#define OS_MEM_INVALID_SIZE     112			// �ڴ���С����������һ��ָ���;
#define OS_MEM_NO_FREE_BLKS     113			// �ڴ����Ѿ�û�пռ������ڴ��;
#define OS_MEM_FULL             114			// �ڴ����Ѿ������ٽ��ܸ���ͷŵ��ڴ�顣�������˵���û��������
#define OS_MEM_INVALID_PBLK     115
#define OS_MEM_INVALID_PMEM     116		    // 'pmem'�ǿ�ָ��;
#define OS_MEM_INVALID_PDATA    117			// pdata�ǿ�ָ��;
#define OS_MEM_INVALID_ADDR     118			// �Ƿ���ַ������ַΪ��ָ��;

#define OS_ERR_NOT_MUTEX_OWNER  120			// ����mutex������ʵ���ϲ���ռ��mutex;

#define OS_TASK_OPT_ERR         130			// ������OSTaskCreateExt()����b��ʱ��û��ָ��

#define OS_ERR_DEL_ISR          140			// ��ͼ���жϷ����ӳ�����ɾ��(��Ϣ�����䡢�ź�����Ϣ���С��������ź�)
#define OS_ERR_CREATE_ISR       141			// ��ͼ���жϷ����ӳ����н�b(�¼���־�顢�������ź�);

#define OS_FLAG_INVALID_PGRP    150			// pgrp��һ���ָ��;
#define OS_FLAG_ERR_WAIT_TYPE   151			// 'wait_type'����ָ���Ĳ���֮һ;
#define OS_FLAG_ERR_NOT_RDY     152			// ָ�����¼���־û�з���;
#define OS_FLAG_INVALID_OPT     153		    // opt����ָ���Ĳ���֮һ;
#define OS_FLAG_GRP_DEPLETED    154			// ϵͳû��ʣ��Ŀ����¼���־�飬��Ҫ���OS_CFG.H�е��¼���־����Ŀ����

//*********************************************************************************************************
//�¼����ƿ�(EVENT CONTROL BLOCK)
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
typedef struct {						   // ����һ���¼����ƿ�ṹ(OS_EVENT)
    INT8U   OSEventType;                   // �¼�����
    INT8U   OSEventGrp;                    // �ȴ��������ڵ���
    INT16U  OSEventCnt;                    // ������(���¼����ź�ʱ)
    void   *OSEventPtr;                    // ָ����Ϣ������Ϣ���е�ָ��
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE]; // �ȴ������б�
} OS_EVENT;
#endif

//*********************************************************************************************************
//�¼���־���ƿ� (EVENT FLAGS CONTROL BLOCK)
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)//���汾Ϊ2.51 �� �¼���־���� ������¼���־����0ʱ
typedef struct {                             // ����һ��OS_FLAG_GRP�ṹ
    INT8U         OSFlagType;                // ��4����ָ��������Ƿ���ָ���¼���־���ָ��
    void         *OSFlagWaitList;            // ����һ��ȴ��¼��������б�
    OS_FLAGS      OSFlagFlags;               // ����һϵ�б���ǰ�¼���־״̬��λ
} OS_FLAG_GRP;    							 // �¼���־��
											      
typedef struct {                            // ����һ���¼���־�ȴ��б�ڵ�(OS_FLAG_NODE)�ṹ
    void         *OSFlagNodeNext;           // ����˫��OS_FLAG_NODE��ݽṹt��ĺ�һ��t��
    void         *OSFlagNodePrev;           // ����˫��OS_FLAG_NODE��ݽṹt���ǰһ��t��
    void         *OSFlagNodeTCB;            // ָ��ĳ��ȴ��¼���־���е��¼���־����Ŀ��ƿ�
    void         *OSFlagNodeFlagGrp;        // ��һ����ָ���¼���־���ָ��
    OS_FLAGS      OSFlagNodeFlags;          // ��4ָ������ȴ��¼���־���е���Щ�¼���־
    INT8U         OSFlagNodeWaitType;       // ָ��ȴ��¼���־���е������¼���־�ķ���(�롢��)
                                            /*      OS_FLAG_WAIT_AND                                   */
                                            /*      OS_FLAG_WAIT_ALL                                   */
                                            /*      OS_FLAG_WAIT_OR                                    */
                                            /*      OS_FLAG_WAIT_ANY                                   */
} OS_FLAG_NODE;
#endif


//*********************************************************************************************************
// �趨һ����Ϣ���е���ݽṹ (MESSAGE MAILBOX DATA)
#if OS_MBOX_EN > 0
typedef struct {						   // ����һ��OS_MBOX_DATA�ṹ
    void   *OSMsg;                         // �����Ϣ����������Ϣ�����ָ��.OSQOut��ָ��Ķ��е�Ԫ��
										   // �����ݡ��������ǿյģ�.OSMsg��һ��NULLָ��
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE]; // ��Ϣ���еĵȴ������б�
    INT8U   OSEventGrp;                    // ��OSEventTbl[]���ʹ��
} OS_MBOX_DATA;
#endif

//*********************************************************************************************************
//�趨һ���ڴ����ݽṹ (MEMORY PARTITION DATA STRUCTURES)
#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
typedef struct {                       // ʹ���ڴ���ƿ飨memory control blocks������ݽṹ4����ÿһ
									   // ���ڴ����ϵͳ�е�ÿ���ڴ���������Լ����ڴ���ƿ顣
    void   *OSMemAddr;                 // ָ���ڴ������ʼ��ַ��ָ��
    void   *OSMemFreeList;             // ָ����һ������ڴ���ƿ������һ����е��ڴ���ָ��
    INT32U  OSMemBlkSize;              // �ڴ�������ڴ��Ĵ�С�����û���b���ڴ����ʱָ����
    INT32U  OSMemNBlks;                // �ڴ�������ܵ��ڴ������Ҳ���û���b���ڴ����ʱָ����
    INT32U  OSMemNFree;                // �ڴ�����е�ǰ���Եÿ����ڴ����
} OS_MEM;


typedef struct {
    void   *OSAddr;                    // ָ���ڴ�����ʼ��ַ��ָ��
    void   *OSFreeList;                /// ָ������ڴ���б���ʼ��ַ��ָ��
    INT32U  OSBlkSize;                 // ÿ���ڴ��Ĵ�С
    INT32U  OSNBlks;                   // ���ڴ�����ڴ������
    INT32U  OSNFree;                   // ���е��ڴ����Ŀ
    INT32U  OSNUsed;                   // ʹ�õ��ڴ����Ŀ
} OS_MEM_DATA;
#endif

//*********************************************************************************************************
//�������ź����(MUTUAL EXCLUSION SEMAPHORE DATA)
#if OS_MUTEX_EN > 0	// ����(1)���߲�������ź���ش���
typedef struct {	// ����ָ������Ϊ(OS_MUTEX_DATA)����ݽṹ��ָ��
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur                */
    INT8U   OSEventGrp;                     // ���Ƶȴ�mutex�������б�
    INT8U   OSValue;                        // ��ǰmutex��ֵ.1��ʾ����ʹ�ã�0��ʾ����ʹ��
    INT8U   OSOwnerPrio;                    // ռ��mutex��������ȼ�
    INT8U   OSMutexPIP;                     // mutex�����ȼ��̳����ȼ�PIP
} OS_MUTEX_DATA;
#endif

//*********************************************************************************************************
// ��Ϣ������� (MESSAGE QUEUE DATA)
/*
���п��ƿ���һ������ά����Ϣ������Ϣ����ݽṹ����������µ�һЩ�������Ȼ�ڸ���ǰ����
һ��[.]4��ʾ��������ݽṹ�е�һ����
1).OSQPtr: �ڿ��ж��п��ƿ���t�����еĶ��п��ƿ顣һ����b����Ϣ���У�����Ͳ��������ˡ�
2).OSQStart: ��ָ����Ϣ���е�ָ���������ʼ��ַ��ָ�롣�û�Ӧ�ó�����ʹ����Ϣ����֮ǰ�����ȶ��������
��
3).OSQEnd: ��ָ����Ϣ���н���Ԫ����һ���ַ��ָ�롣��ָ��ʹ����Ϣ���й���һ��ѭ���Ļ�����
4).OSQIn: ��ָ����Ϣ�����в�����һ����Ϣ��λ�õ�ָ�롣��.OSQIn��.OSQEnd���ʱ��.OSQIn������ָ��
��Ϣ���е���ʼ��Ԫ��
5).OSQOut: ��ָ����Ϣ��������һ��ȡ����Ϣ��λ�õ�ָ�롣��.OSQOut��.OSQEnd���ʱ��.OSQOut������ָ
����Ϣ���е���ʼ��Ԫ��
6).OSQSize: ����Ϣ�������ܵĵ�Ԫ���ֵ���ڽ�b��Ϣ����ʱ���û�Ӧ�ó����ġ���uC/OS-II��,��ֵ��
�������65,535��
7).OSQEntries: ����Ϣ�����е�ǰ����Ϣ��������Ϣ�����ǿյ�ʱ����ֵΪ0������Ϣ���������Ժ󣬸�ֵ��
.OSQSizeֵһ�� ����Ϣ���иոս�bʱ����ֵΪ0��
*/

#if OS_Q_EN > 0
typedef struct os_q {                   // ����һ��OS_Q���п��ƿ�                                    */
    struct os_q   *OSQPtr;              /* Link to next queue control block in list of free blocks     */
    void         **OSQStart;            /* Pointer to start of queue data                              */
    void         **OSQEnd;              /* Pointer to end   of queue data                              */
    void         **OSQIn;               /* Pointer to where next message will be inserted  in   the Q  */
    void         **OSQOut;              /* Pointer to where next message will be extracted from the Q  */
    INT16U         OSQSize;             /* Size of queue (maximum number of entries)                   */
    INT16U         OSQEntries;          /* Current number of entries in the queue                      */
} OS_Q;								 

typedef struct {						// ����һ����Ϣ�������(OS_Q_DATA)�ṹ
    void          *OSMsg;               // �����Ϣ����������Ϣ�����ָ��
    INT16U         OSNMsgs;             // ��Ϣ�����е���Ϣ��
    INT16U         OSQSize;             // ��Ϣ���е��ܵ���                                      */
    INT8U          OSEventTbl[OS_EVENT_TBL_SIZE];  /* List of tasks waiting for event to occur         */
    INT8U          OSEventGrp;          //��OSEventTbl[]һ���ϣ�����Ϣ���еĵȴ������б�
} OS_Q_DATA;
#endif

//*********************************************************************************************************
//�ź���ݽṹ (SEMAPHORE DATA)
#if OS_SEM_EN > 0
typedef struct {						    // ����һ���ź���ݽṹ(OS_SEM_DATA)
    INT16U  OSCnt;                          // �����ź�����ֵ
    INT8U   OSEventTbl[OS_EVENT_TBL_SIZE];  // ��������ȴ��б�
    INT8U   OSEventGrp;                     // ����ȴ��¼���������
} OS_SEM_DATA;
#endif

//*********************************************************************************************************
//�����ջ��� (TASK STACK DATA)
#if OS_TASK_CREATE_EXT_EN > 0
typedef struct {					   // ����һ���ջ��ݽṹ(OS_STK_DATA)
    INT32U  OSFree;                    // ��ջ��δʹ�õ��ֽ���
    INT32U  OSUsed;                    // ��ջ����ʹ�õ��ֽ���
} OS_STK_DATA;
#endif

//*********************************************************************************************************
//������ƿ� (TASK CONTROL BLOCK)
typedef struct os_tcb {
    OS_STK        *OSTCBStkPtr;        //��ǰTCB��ջ��ָ��

#if OS_TASK_CREATE_EXT_EN > 0		   //�������OSTaskCreateExt()����
    void          *OSTCBExtPtr;        //ָ���û������������ƿ�()չָ��)
    OS_STK        *OSTCBStkBottom;     //ָ��ָ��ջ�׵�ָ��
    INT32U         OSTCBStkSize;       //�趨��ջ����
    INT16U         OSTCBOpt;           //����OS_TCB��ѡ����
    INT16U         OSTCBId;            //����ID  (0..65535)
#endif

    struct os_tcb *OSTCBNext;          //����ָ��TCB��˫��t�ӵĺ�t��
    struct os_tcb *OSTCBPrev;          //����ָ��TCB��˫��t�ӵ�ǰt��

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
//�����ϸ����¼�����ʱ
    OS_EVENT      *OSTCBEventPtr;      //����ָ���¼����ƿ��ָ��
#endif

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
//�����������,���崫�ݸ��������Ϣָ��
    void          *OSTCBMsg;           /* Message received from OSMboxPost() or OSQPost()              */
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
#if OS_TASK_DEL_EN > 0
    OS_FLAG_NODE  *OSTCBFlagNode;      ///�����¼���־�ڵ��ָ��
#endif    
    OS_FLAGS       OSTCBFlagsRdy;      //��������׼����ϵ�������ƿ��е�����?
#endif

    INT16U         OSTCBDly;           //������������ȴ�ʱ����������
    INT8U          OSTCBStat;          //���������״̬��
    INT8U          OSTCBPrio;          //������������ȼ�(0 == highest, 63 == lowest)

    INT8U          OSTCBX;             //����ָ���������ȼ��ĵ�3λ����=priority&0x07
    INT8U          OSTCBY;             //����ָ���������ȼ��ĸ�3λ����=priority>>3
    INT8U          OSTCBBitX;          //�����3λ������Ӧֵ��0��7��������OSMapTbl[priority&0x07]
    INT8U          OSTCBBitY;          //�����3λ������Ӧֵ��0��7��������OSMapTbl[priority>>3]

#if OS_TASK_DEL_EN > 0				   //������� OSTaskDel() ������뺯��
    BOOLEAN        OSTCBDelReq;        //�������ڱ�ʾ�������Ƿ���ɾ��
#endif
} OS_TCB;

//*********************************************************************************************************
//ȫ�ֱ� (GLOBAL VARIABLES)
OS_EXT  INT32U            OSCtxSwCtr;               //�������л��Ĵ���(ͳ�����������)

#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)	    //�������Ϣ�¼������������Ϣ�¼���>0
OS_EXT  OS_EVENT         *OSEventFreeList;          //�����¼������б�ָ��
OS_EXT  OS_EVENT          OSEventTbl[OS_MAX_EVENTS];//�¼����ƿ��
#endif
//������汾����2.51���¼���־������������¼���־
#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
OS_EXT  OS_FLAG_GRP       OSFlagTbl[OS_MAX_FLAGS];  //����һ���¼���־�б�
OS_EXT  OS_FLAG_GRP      *OSFlagFreeList;           //����һ����е��¼���־��
#endif

#if OS_TASK_STAT_EN > 0							    //�����������OS_TaskStat()����
OS_EXT  INT8S             OSCPUUsage;               //����CPU ʹ����
OS_EXT  INT32U            OSIdleCtrMax;             //���������м���ֵ
OS_EXT  INT32U            OSIdleCtrRun;             //���嵱ǰ�Ŀ��м���ֵ
OS_EXT  BOOLEAN           OSStatRdy;                //����ͳ����������־
OS_EXT  OS_STK            OSTaskStatStk[OS_TASK_STAT_STK_SIZE];/* Statistics task stack          */
#endif

OS_EXT  INT8U             OSIntNesting;             //�����ж�Ƕ�ײ���
OS_EXT  INT8U             OSIntExitY;				//���ں���OSInieExt( )

OS_EXT  INT8U             OSLockNesting;            //������Ƕ�׼�����

OS_EXT  INT8U             OSPrioCur;                //�����������е���������ȼ�
OS_EXT  INT8U             OSPrioHighRdy;            //�������������ȼ���ľ�����������ȼ�

OS_EXT  INT8U             OSRdyGrp;                        /* Ready list group                         */
OS_EXT  INT8U             OSRdyTbl[OS_RDY_TBL_SIZE];       //����Ҫ���е������б�

OS_EXT  BOOLEAN           OSRunning;                       //�������Ѿ���ʼ=1,�����ڲ�����״̬=0

OS_EXT  INT8U             OSTaskCtr;                       //�������������

OS_EXT  volatile  INT32U  OSIdleCtr;                        //����32λ��������ļ�����

OS_EXT  OS_STK            OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE];//������������ջջ��ָ��


OS_EXT  OS_TCB           *OSTCBCur;                        //����ָ����������������ƿ��ָ��
OS_EXT  OS_TCB           *OSTCBFreeList;                   //�����������ƿ�ָ��
OS_EXT  OS_TCB           *OSTCBHighRdy;                    //����ָ����߼����ȼ�����������ƿ��ָ��
OS_EXT  OS_TCB           *OSTCBList;                       //����������ƿ��б��׵�ַ
OS_EXT  OS_TCB           *OSTCBPrioTbl[OS_LOWEST_PRIO + 1];//����������ƿ����ȼ���
OS_EXT  OS_TCB            OSTCBTbl[OS_MAX_TASKS + OS_N_SYS_TASKS];//���嵱ǰ������ƿ��б�

//������룺��}���������ʱ���������´���
//OS_MEM_EN���� (1) ���߽�ֹ (0) �����ڴ���ش���
//OS_MAX_MEM_PART ����ڴ�����Ŀ
#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
OS_EXT  OS_MEM           *OSMemFreeList;            //�����ڴ���ƿ�(t��)ָ��
OS_EXT  OS_MEM            OSMemTbl[OS_MAX_MEM_PART];/* Storage for memory partition manager            */
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)				//������룺OS_Q_EN ���� (1)������Ϣ������ش���
//������룺Ӧ���������п��ƿ����Ŀ > 0
OS_EXT  OS_Q             *OSQFreeList;              /* Pointer to list of free QUEUE control blocks    */
OS_EXT  OS_Q              OSQTbl[OS_MAX_QS];        /* Table of QUEUE control blocks                   */
#endif

#if OS_TIME_GET_SET_EN > 0   					    //�������OSTimeGet() �������
OS_EXT  volatile  INT32U  OSTime;                   //��ǰϵͳʱ����ֵ(in ticks)
#endif

extern  INT8U  const      OSMapTbl[];               /* Priority->Bit Mask lookup table                 */
extern  INT8U  const      OSUnMapTbl[];             /* Priority->Index    lookup table                 */

//*********************************************************************************************************
//����ԭ�� (FUNCTION PROTOTYPES)
//����Լ��ĺ��� (Target Independent Functions)
//�¼���־���� (EVENT FLAGS MANAGEMENT)
//OSFlagAccept() ����¼���־�麯��(��־���ָ�롢�¼���־λ���ȴ��¼���־λ�ķ�ʽ��������ָ��)
//OSFlagCreate() ��bһ���¼���־��(��ֵ��������)
//OSFlagDel() ɾ��һ���¼���־��(ָ�롢���ֵ������ֵ)
//OSFlagPend() �ȴ��¼���־����¼���־λ(�¼���ָ�롢��Ҫ���ı�־λ���ȴ��¼���־λ�ķ�ʽ��
//����ȴ��ʱ�ӽ��ġ��������ʱ�ӽ���)
//OSFlagPost() ��λ����0�¼���־���еı�־λ(ָ�롢��־λ�����ֵ��������)
//OSFlagQuery() ��ѯ�¼���־��ĵ�ǰ�¼���־״̬(�¼���־���ָ�롢��������ָ��)

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)

#if OS_FLAG_ACCEPT_EN > 0
OS_FLAGS      OSFlagAccept(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *err);
#endif

OS_FLAG_GRP  *OSFlagCreate(OS_FLAGS flags, INT8U *err);

#if OS_FLAG_DEL_EN > 0
OS_FLAG_GRP  *OSFlagDel(OS_FLAG_GRP *pgrp, INT8U opt, INT8U *err);
#endif

OS_FLAGS      OSFlagPend(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *err);
OS_FLAGS      OSFlagPost(OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U operation, INT8U *err);

#if OS_FLAG_QUERY_EN > 0
OS_FLAGS      OSFlagQuery(OS_FLAG_GRP *pgrp, INT8U *err);
#endif
#endif

//*********************************************************************************************************
//��Ϣ������� (MESSAGE MAILBOX MANAGEMENT)
//OSMboxAccept () �鿴��Ϣ����(��Ϣ����ָ��)
//OSMboxCreate () ��b����ʼ��һ����Ϣ����(msg ����Ϊ�պ�����)
//OSMboxDel () ɾ����Ϣ����(��Ϣ����ָ�롢ɾ�������������ָ��)
//OSMboxPend () �ȴ�һ����Ϣ���亯��(��Ϣ����ָ�롢����ȴ��ʱ�ӽ��ġ��������ָ��)
//OSMboxPost () ������Ϣ����(��Ϣ����ָ�롢����ʵ�ʷ��͸��������Ϣ)
//OSMboxPostOpt () �����䷢��һ����Ϣ(����ָ�롢��Ϣ�����)
//OSMboxQuery () ��ѯһ������ĵ�ǰ״̬(�ź�ָ�롢״̬��ݽṹָ��)


#if OS_MBOX_EN > 0

#if OS_MBOX_ACCEPT_EN > 0
void         *OSMboxAccept(OS_EVENT *pevent);
#endif

OS_EVENT     *OSMboxCreate(void *msg);

#if OS_MBOX_DEL_EN > 0
OS_EVENT     *OSMboxDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

void         *OSMboxPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);

#if OS_MBOX_POST_EN > 0
INT8U         OSMboxPost(OS_EVENT *pevent, void *msg);
#endif

#if OS_MBOX_POST_OPT_EN > 0
INT8U         OSMboxPostOpt(OS_EVENT *pevent, void *msg, INT8U opt);
#endif

#if OS_MBOX_QUERY_EN > 0
INT8U         OSMboxQuery(OS_EVENT *pevent, OS_MBOX_DATA *pdata);
#endif
#endif

//*********************************************************************************************************
//�ڴ������ (MEMORY MANAGEMENT)
//OSMemCreate () ��b����ʼ��һ���ڴ���(��ʼ��ַ����Ҫ���ڴ����Ŀ���ڴ���С�����ش����ָ��)
//OSMemGet () ���ڴ������һ���ڴ��
//OSMemPut () �ͷ�һ���ڴ�飬�ڴ������ͷŻ�ԭ��������ڴ���
//OSMemQuery () �õ��ڴ������Ϣ


#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)

OS_MEM       *OSMemCreate(void *addr, INT32U nblks, INT32U blksize, INT8U *err);
void         *OSMemGet(OS_MEM *pmem, INT8U *err);
INT8U         OSMemPut(OS_MEM *pmem, void *pblk);

#if OS_MEM_QUERY_EN > 0
INT8U         OSMemQuery(OS_MEM *pmem, OS_MEM_DATA *pdata);
#endif

#endif

//*********************************************************************************************************
//�������ź������ (MUTUAL EXCLUSION SEMAPHORE MANAGEMENT)
//OSMutexAccept () �޵ȴ�ػ�ȡ�������ź�[���񲻹���](�ź�ָ�롢�������)
//OSMutexCreate () ��b����ʼ��һ������ź�(���ȼ��̳����ȼ�(PIP)��������ָ��)
//OSMutexDel () ɾ������ź�(�ź�ָ�롢ɾ�����������ָ��)
//OSMutexPend () �ȴ�һ������ź�(ָ�롢�ȴ�ʱʱ�ޡ�������ָ��)
//OSMutexPost () �ͷ�һ������ź�(�������ź�ָ��)
//OSMutexQuery () ��ѯһ������ź��ĵ�ǰ״̬(�������ź�ָ�롢״̬��ݽṹָ��)


#if OS_MUTEX_EN > 0

#if OS_MUTEX_ACCEPT_EN > 0
INT8U         OSMutexAccept(OS_EVENT *pevent, INT8U *err);
#endif

OS_EVENT     *OSMutexCreate(INT8U prio, INT8U *err);

#if OS_MUTEX_DEL_EN > 0
OS_EVENT     *OSMutexDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

void          OSMutexPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
INT8U         OSMutexPost(OS_EVENT *pevent);

#if OS_MUTEX_QUERY_EN > 0
INT8U         OSMutexQuery(OS_EVENT *pevent, OS_MUTEX_DATA *pdata);
#endif

#endif

//*********************************************************************************************************
//��Ϣ���й��� (MESSAGE QUEUE MANAGEMENT)
//OSQAccept () �����Ϣ�������Ƿ��Ѿ�����Ҫ����Ϣ(��Ϣ���е�ָ��)
//OSQCreate () ��bһ����Ϣ����(��Ϣ�ڴ���Ļ��ַ(ָ������)����Ϣ�ڴ���Ĵ�С)
//OSQDel () ɾ��һ����Ϣ����(��Ϣ����ָ�롢ɾ�����������ָ��)
//OSQFlush () �����Ϣ����(ָ��õ���Ϣ���е�ָ��)
//OSQPend () ����ȴ���Ϣ�����е���Ϣ(��Ϣ����ָ�롢����ȴ��ʱ�ӽ��ġ��������ָ��)
//OSQPost () ����Ϣ���з���һ����ϢFIFO(��Ϣ����ָ�롢���͵���Ϣ)
//OSQPostFront () ����Ϣ���з���һ����ϢLIFO(��Ϣ����ָ�롢���͵���Ϣ)
//OSQPostOpt () ����Ϣ���з���һ����ϢLIFO(��Ϣ����ָ�롢���͵���Ϣ���������)
//OSQQuery () ��ѯһ����Ϣ���еĵ�ǰ״̬(�ź�ָ�롢״̬��ݽṹָ��)

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)

#if OS_Q_ACCEPT_EN > 0
void         *OSQAccept(OS_EVENT *pevent);
#endif

OS_EVENT     *OSQCreate(void **start, INT16U size);

#if OS_Q_DEL_EN > 0
OS_EVENT     *OSQDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

#if OS_Q_FLUSH_EN > 0
INT8U         OSQFlush(OS_EVENT *pevent);
#endif

void         *OSQPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);

#if OS_Q_POST_EN > 0
INT8U         OSQPost(OS_EVENT *pevent, void *msg);
#endif

#if OS_Q_POST_FRONT_EN > 0
INT8U         OSQPostFront(OS_EVENT *pevent, void *msg);
#endif

#if OS_Q_POST_OPT_EN > 0
INT8U         OSQPostOpt(OS_EVENT *pevent, void *msg, INT8U opt);
#endif

#if OS_Q_QUERY_EN > 0
INT8U         OSQQuery(OS_EVENT *pevent, OS_Q_DATA *pdata);
#endif

#endif

//*********************************************************************************************************
//�ź����� (SEMAPHORE MANAGEMENT)
//OSSemAccept() ������صȴ�����һ���ź�����
//OSSemCreate() ��b����ʼ��һ���ź�(����һ���ź�ֵ)
//OSSemDel() ɾ��һ���ź�(�ź�ָ�롢ɾ�����������ָ��)
//OSSemPend () �ȴ�һ���ź�����(�ź�ָ�롢����ȴ��ʱ�ӽ��ġ��������ָ��)
//OSSemPost () ����һ���ź�����(�ź�ָ��)
//OSSemQuery () ��ѯһ���ź��ĵ�ǰ״̬(�ź�ָ�롢״̬��ݽṹָ��)

#if OS_SEM_EN > 0

#if OS_SEM_ACCEPT_EN > 0
INT16U        OSSemAccept(OS_EVENT *pevent);
#endif

OS_EVENT     *OSSemCreate(INT16U cnt);

#if OS_SEM_DEL_EN > 0
OS_EVENT     *OSSemDel(OS_EVENT *pevent, INT8U opt, INT8U *err);
#endif

void          OSSemPend(OS_EVENT *pevent, INT16U timeout, INT8U *err);
INT8U         OSSemPost(OS_EVENT *pevent);

#if OS_SEM_QUERY_EN > 0
INT8U         OSSemQuery(OS_EVENT *pevent, OS_SEM_DATA *pdata);
#endif

#endif
	   
//*********************************************************************************************************
//������� (TASK MANAGEMENT)
//OSTaskChangePrio () �ı�һ����������ȼ�(����ɵ����ȼ��������µ����ȼ�)
//OSTaskCreate () ��b����(�������ָ�롢���ݲ���ָ�롢���������ջջ��ָ�롢�������ȼ�)
//OSTaskCreateExt () ��b)չ����(�������ָ��/���ݲ���ָ��/���������ջջ��ָ��/�����������ȼ�
//(δ4��)���ȼ���ʶ(�����ȼ���ͬ)/���������ջջ��ָ��/ָ����ջ����(������)
//ָ���û����ӵ�������ָ��/��b�����趨ѡ��)
//OSTaskDel () ɾ������(��������ȼ�)
//OSTaskDelReq () ����һ������ɾ���������������?(��������ȼ�)
//OSTaskResume () ����һ����OSTaskSuspend()������������(��������ȼ�)
//OSTaskStkChk () ��������ջ״̬(�������ȼ��������ջ��ݽṹ)
//OSTaskSuspend () ���������һ������(�������ȼ�)
//OSTaskQuery () ��ȡ������Ϣ(����ָ�롢������ݽṹָ��)

#if OS_TASK_CHANGE_PRIO_EN > 0
INT8U         OSTaskChangePrio(INT8U oldprio, INT8U newprio);
#endif

#if OS_TASK_CREATE_EN > 0
INT8U         OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);
#endif

#if OS_TASK_CREATE_EXT_EN > 0
INT8U         OSTaskCreateExt(void  (*task)(void *pd),
                              void   *pdata,
                              OS_STK *ptos,
                              INT8U   prio,
                              INT16U  id,
                              OS_STK *pbos,
                              INT32U  stk_size,
                              void   *pext,
                              INT16U  opt);
#endif

#if OS_TASK_DEL_EN > 0
INT8U         OSTaskDel(INT8U prio);
INT8U         OSTaskDelReq(INT8U prio);
#endif

#if OS_TASK_SUSPEND_EN > 0
INT8U         OSTaskResume(INT8U prio);
INT8U         OSTaskSuspend(INT8U prio);
#endif

#if OS_TASK_CREATE_EXT_EN > 0
INT8U         OSTaskStkChk(INT8U prio, OS_STK_DATA *pdata);
#endif

#if OS_TASK_QUERY_EN > 0
INT8U         OSTaskQuery(INT8U prio, OS_TCB *pdata);
#endif

//*********************************************************************************************************
//ʱ�ӹ����� (TIME MANAGEMENT)
//OSTimeDly () ������ʱ����(ʱ�ӽ�����)
//OSTimeDlyHMSM () ��һ��������ʱ���ʱ��(�趨ʱ���֡��롢����)
//OSTimeDlyResume () ����һ����OSTimeDly()��OSTimeDlyHMSM()���������(���ȼ�)
//OSTimeGet () ��ȡ��ǰϵͳʱ����ֵ
//OSTimeSet () ���õ�ǰϵͳʱ����ֵ

void          OSTimeDly(INT16U ticks);

#if OS_TIME_DLY_HMSM_EN > 0
INT8U         OSTimeDlyHMSM(INT8U hours, INT8U minutes, INT8U seconds, INT16U milli);
#endif

#if OS_TIME_DLY_RESUME_EN > 0
INT8U         OSTimeDlyResume(INT8U prio);
#endif

#if OS_TIME_GET_SET_EN > 0
INT32U        OSTimeGet(void);
void          OSTimeSet(INT32U ticks);
#endif

void          OSTimeTick(void);

//*********************************************************************************************************
//���Ӻ�����
//OSInit() ��ʼ��UCOS-II����
//OSIntEnter() �жϺ�������ִ��
//OSIntExit() �жϺ����Ѿ����(�����ж�)
//OSSchedLock() ����������
//OSSchedUnlock() ���������
//OSStart() ��������
//OSStatInit() ͳ�������ʼ��
//OSVersion() ��ð汾��

void          OSInit(void);

void          OSIntEnter(void);
void          OSIntExit(void);

#if OS_SCHED_LOCK_EN > 0
void          OSSchedLock(void);
void          OSSchedUnlock(void);
#endif

void          OSStart(void);

void          OSStatInit(void);

INT16U        OSVersion(void);

//*********************************************************************************************************
//�ڲ�����ԭ�� INTERNAL FUNCTION PROTOTYPES
//����Ӧ�ó����в���ʹ������ (Your application MUST NOT call these functions)
 
//OS_Dummy() ��bһ�����⺯��
//OS_EventTaskRdy() ʹһ������������̬(OS_EVENT *pevent, void *msg, INT8U msk)
//OS_EventTaskWait() ʹһ���������ȴ�ĳ�¼�����״̬(ECBָ��)
//OS_EventTO() ���ڳ�ʱ��������Ϊ����̬(ECBָ��)
//OS_EventWaitListInit()�¼����ƿ��б��ʼ��(�¼����ƿ�ָ��)
//OS_FlagInit() ��ʼ���¼���־�ṹ
//OS_FlagUnlink() �����OS_FLAG_NODE���¼���־��ĵȴ�����t����ɾ��(OS_FLAG_NODE *pnode)
//OS_MemInit() ��ʼ���ڴ����
//OS_QInit() ��ʼ���¼����нṹ
//OS_Sched() �����Ⱥ���
//OS_TaskIdle() ����������(ָ��һ����ݽṹ)
//OS_TaskStat() ͳ������(ָ��һ����ݽṹ)
//OS_TCBInit() ��ʼ��������ƿ�TCB(���ȼ�ָ�롢ջ��ָ�롢ջ��ָ�롢�����־��
//��ջ����)չָ�롢ѡ����)


#if OS_TASK_DEL_EN > 0
void          OS_Dummy(void);
#endif

#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
INT8U         OS_EventTaskRdy(OS_EVENT *pevent, void *msg, INT8U msk);
void          OS_EventTaskWait(OS_EVENT *pevent);
void          OS_EventTO(OS_EVENT *pevent);
void          OS_EventWaitListInit(OS_EVENT *pevent);
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
void          OS_FlagInit(void);
void          OS_FlagUnlink(OS_FLAG_NODE *pnode);
#endif

#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
void          OS_MemInit(void);
#endif

#if OS_Q_EN > 0
void          OS_QInit(void);
#endif

void          OS_Sched(void);

void          OS_TaskIdle(void *data);

#if OS_TASK_STAT_EN > 0
void          OS_TaskStat(void *data);
#endif

INT8U         OS_TCBInit(INT8U prio, OS_STK *ptos, OS_STK *pbos, INT16U id, INT32U stk_size, void *pext, INT16U opt);

//*********************************************************************************************************
//���๳�ӳ��������� (FUNCTION PROTOTYPES)
//�ر��Ӻ���ԭ�� (Target Specific Functions)
 

#if OS_VERSION >= 204
void          OSInitHookBegin(void);
void          OSInitHookEnd(void);
#endif

void          OSIntCtxSw(void);

void          OSStartHighRdy(void);

void          OSTaskCreateHook(OS_TCB *ptcb);
void          OSTaskDelHook(OS_TCB *ptcb);

#if OS_VERSION >= 251
void          OSTaskIdleHook(void);
#endif

void          OSTaskStatHook(void);
OS_STK       *OSTaskStkInit(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT16U opt);
unsigned long          OSTaskSwHook(void);

#if OS_VERSION >= 204
void          OSTCBInitHook(OS_TCB *ptcb);
#endif

void          OSTimeTickHook(void);

//*********************************************************************************************************
//����ԭ�� FUNCTION PROTOTYPES
//�����жϺ���ԭ�� (Compiler Specific ISR prototypes)


#ifndef OS_ISR_PROTO_EXT
void          OSCtxSw(void);
void          OSTickISR(void);
#endif

/*
*********************************************************************************************************
*                                   LOOK FOR MISSING #define CONSTANTS
*
* This section is used to generate ERROR messages at compile time if certain #define constants are 
* MISSING in OS_CFG.H.  This allows you to quickly determine the source of the error.
*
* You SHOULD NOT change this section UNLESS you would like to add more comments as to the source of the
* compile time error.
*********************************************************************************************************
*/

//*********************************************************************************************************
//�¼���־�����������

#ifndef OS_FLAG_EN
#error  "OS_CFG.H, Missing OS_FLAG_EN: Enable (1) or Disable (0) code generation for Event Flags"
#else
    #ifndef OS_MAX_FLAGS
    #error  "OS_CFG.H, Missing OS_MAX_FLAGS: Max. number of Event Flag Groups in your application"
    #else
        #if     OS_MAX_FLAGS == 0
        #error  "OS_CFG.H, OS_MAX_FLAGS must be > 0"
        #endif
        #if     OS_MAX_FLAGS > 255
        #error  "OS_CFG.H, OS_MAX_FLAGS must be <= 255"
        #endif
    #endif

    #ifndef OS_FLAG_WAIT_CLR_EN
    #error  "OS_CFG.H, Missing OS_FLAG_WAIT_CLR_EN: Include code for Wait on Clear EVENT FLAGS"
    #endif

    #ifndef OS_FLAG_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_FLAG_ACCEPT_EN: Include code for OSFlagAccept()"
    #endif

    #ifndef OS_FLAG_DEL_EN
    #error  "OS_CFG.H, Missing OS_FLAG_DEL_EN: Include code for OSFlagDel()"
    #endif

    #ifndef OS_FLAG_QUERY_EN
    #error  "OS_CFG.H, Missing OS_FLAG_QUERY_EN: Include code for OSFlagQuery()"
    #endif
#endif

//*********************************************************************************************************
//��Ϣ��������������

#ifndef OS_MBOX_EN
#error  "OS_CFG.H, Missing OS_MBOX_EN: Enable (1) or Disable (0) code generation for MAILBOXES"
#else
    #ifndef OS_MBOX_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_MBOX_ACCEPT_EN: Include code for OSMboxAccept()"
    #endif

    #ifndef OS_MBOX_DEL_EN
    #error  "OS_CFG.H, Missing OS_MBOX_DEL_EN: Include code for OSMboxDel()"
    #endif

    #ifndef OS_MBOX_POST_EN
    #error  "OS_CFG.H, Missing OS_MBOX_POST_EN: Include code for OSMboxPost()"
    #endif

    #ifndef OS_MBOX_POST_OPT_EN
    #error  "OS_CFG.H, Missing OS_MBOX_POST_OPT_EN: Include code for OSMboxPostOpt()"
    #endif

    #ifndef OS_MBOX_QUERY_EN
    #error  "OS_CFG.H, Missing OS_MBOX_QUERY_EN: Include code for OSMboxQuery()"
    #endif
#endif

//*********************************************************************************************************
//�ڴ�����������

#ifndef OS_MEM_EN
#error  "OS_CFG.H, Missing OS_MEM_EN: Enable (1) or Disable (0) code generation for MEMORY MANAGER"
#else
    #ifndef OS_MAX_MEM_PART
    #error  "OS_CFG.H, Missing OS_MAX_MEM_PART: Max. number of memory partitions"
    #else
        #if     OS_MAX_MEM_PART == 0
        #error  "OS_CFG.H, OS_MAX_MEM_PART must be > 0"
        #endif
        #if     OS_MAX_MEM_PART > 255
        #error  "OS_CFG.H, OS_MAX_MEM_PART must be <= 255"
        #endif
    #endif

    #ifndef OS_MEM_QUERY_EN
    #error  "OS_CFG.H, Missing OS_MEM_QUERY_EN: Include code for OSMemQuery()"
    #endif
#endif

//*********************************************************************************************************
//�������ź������������

#ifndef OS_MUTEX_EN
#error  "OS_CFG.H, Missing OS_MUTEX_EN: Enable (1) or Disable (0) code generation for MUTEX"
#else
    #ifndef OS_MUTEX_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_MUTEX_ACCEPT_EN: Include code for OSMutexAccept()"
    #endif

    #ifndef OS_MUTEX_DEL_EN
    #error  "OS_CFG.H, Missing OS_MUTEX_DEL_EN: Include code for OSMutexDel()"
    #endif

    #ifndef OS_MUTEX_QUERY_EN
    #error  "OS_CFG.H, Missing OS_MUTEX_QUERY_EN: Include code for OSMutexQuery()"
    #endif
#endif

//*********************************************************************************************************
//��Ϣ�����������

#ifndef OS_Q_EN
#error  "OS_CFG.H, Missing OS_Q_EN: Enable (1) or Disable (0) code generation for QUEUES"
#else
    #ifndef OS_MAX_QS
    #error  "OS_CFG.H, Missing OS_MAX_QS: Max. number of queue control blocks"
    #else
        #if     OS_MAX_QS == 0
        #error  "OS_CFG.H, OS_MAX_QS must be > 0"
        #endif
        #if     OS_MAX_QS > 255
        #error  "OS_CFG.H, OS_MAX_QS must be <= 255"
        #endif
    #endif

    #ifndef OS_Q_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_Q_ACCEPT_EN: Include code for OSQAccept()"
    #endif

    #ifndef OS_Q_DEL_EN
    #error  "OS_CFG.H, Missing OS_Q_DEL_EN: Include code for OSQDel()"
    #endif

    #ifndef OS_Q_FLUSH_EN
    #error  "OS_CFG.H, Missing OS_Q_FLUSH_EN: Include code for OSQFlush()"
    #endif

    #ifndef OS_Q_POST_EN
    #error  "OS_CFG.H, Missing OS_Q_POST_EN: Include code for OSQPost()"
    #endif

    #ifndef OS_Q_POST_FRONT_EN
    #error  "OS_CFG.H, Missing OS_Q_POST_FRONT_EN: Include code for OSQPostFront()"
    #endif

    #ifndef OS_Q_POST_OPT_EN
    #error  "OS_CFG.H, Missing OS_Q_POST_OPT_EN: Include code for OSQPostOpt()"
    #endif

    #ifndef OS_Q_QUERY_EN
    #error  "OS_CFG.H, Missing OS_Q_QUERY_EN: Include code for OSQQuery()"
    #endif
#endif

//*********************************************************************************************************
//�ź��������

#ifndef OS_SEM_EN
#error  "OS_CFG.H, Missing OS_SEM_EN: Enable (1) or Disable (0) code generation for SEMAPHORES"
#else
    #ifndef OS_SEM_ACCEPT_EN
    #error  "OS_CFG.H, Missing OS_SEM_ACCEPT_EN: Include code for OSSemAccept()"
    #endif

    #ifndef OS_SEM_DEL_EN
    #error  "OS_CFG.H, Missing OS_SEM_DEL_EN: Include code for OSSemDel()"
    #endif

    #ifndef OS_SEM_QUERY_EN
    #error  "OS_CFG.H, Missing OS_SEM_QUERY_EN: Include code for OSSemQuery()"
    #endif
#endif

//*********************************************************************************************************
//��������������

#ifndef OS_MAX_TASKS
#error  "OS_CFG.H, Missing OS_MAX_TASKS: Max. number of tasks in your application"
#else
    #if     OS_MAX_TASKS == 0
    #error  "OS_CFG.H,         OS_MAX_TASKS must be >= 2"
    #endif
    #if     OS_MAX_TASKS > 63
    #error  "OS_CFG.H,         OS_MAX_TASKS must be <= 63"
    #endif
#endif

#ifndef OS_TASK_IDLE_STK_SIZE
#error  "OS_CFG.H, Missing OS_TASK_IDLE_STK_SIZE: Idle task stack size"
#endif

#ifndef OS_TASK_STAT_EN
#error  "OS_CFG.H, Missing OS_TASK_STAT_EN: Enable (1) or Disable(0) the statistics task"
#endif

#ifndef OS_TASK_STAT_STK_SIZE
#error  "OS_CFG.H, Missing OS_TASK_STAT_STK_SIZE: Statistics task stack size"
#endif

#ifndef OS_TASK_CHANGE_PRIO_EN
#error  "OS_CFG.H, Missing OS_TASK_CHANGE_PRIO_EN: Include code for OSTaskChangePrio()"
#endif

#ifndef OS_TASK_CREATE_EN
#error  "OS_CFG.H, Missing OS_TASK_CREATE_EN: Include code for OSTaskCreate()"
#endif

#ifndef OS_TASK_CREATE_EXT_EN
#error  "OS_CFG.H, Missing OS_TASK_CREATE_EXT_EN: Include code for OSTaskCreateExt()"
#endif

#ifndef OS_TASK_DEL_EN
#error  "OS_CFG.H, Missing OS_TASK_DEL_EN: Include code for OSTaskDel()"
#endif

#ifndef OS_TASK_SUSPEND_EN
#error  "OS_CFG.H, Missing OS_TASK_SUSPEND_EN: Include code for OSTaskSuspend() and OSTaskResume()"
#endif

#ifndef OS_TASK_QUERY_EN
#error  "OS_CFG.H, Missing OS_TASK_QUERY_EN: Include code for OSTaskQuery()"
#endif

//*********************************************************************************************************
//ʱ������������

#ifndef OS_TICKS_PER_SEC
#error  "OS_CFG.H, Missing OS_TICKS_PER_SEC: Sets the number of ticks in one second"
#endif

#ifndef OS_TIME_DLY_HMSM_EN
#error  "OS_CFG.H, Missing OS_TIME_DLY_HMSM_EN: Include code for OSTimeDlyHMSM()"
#endif

#ifndef OS_TIME_DLY_RESUME_EN
#error  "OS_CFG.H, Missing OS_TIME_DLY_RESUME_EN: Include code for OSTimeDlyResume()"
#endif

#ifndef OS_TIME_GET_SET_EN
#error  "OS_CFG.H, Missing OS_TIME_GET_SET_EN: Include code for OSTimeGet() and OSTimeSet()"
#endif

//*********************************************************************************************************
//��Ϲ����������

#ifndef OS_MAX_EVENTS
#error  "OS_CFG.H, Missing OS_MAX_EVENTS: Max. number of event control blocks in your application"
#else
    #if     OS_MAX_EVENTS == 0
    #error  "OS_CFG.H, OS_MAX_EVENTS must be > 0"
    #endif
    #if     OS_MAX_EVENTS > 255
    #error  "OS_CFG.H, OS_MAX_EVENTS must be <= 255"
    #endif
#endif

#ifndef OS_LOWEST_PRIO
#error  "OS_CFG.H, Missing OS_LOWEST_PRIO: Defines the lowest priority that can be assigned"
#endif

#ifndef OS_ARG_CHK_EN
#error  "OS_CFG.H, Missing OS_ARG_CHK_EN: Enable (1) or Disable (0) argument checking"
#endif

#ifndef OS_CPU_HOOKS_EN
#error  "OS_CFG.H, Missing OS_CPU_HOOKS_EN: uC/OS-II hooks are found in the processor port files when 1"
#endif

#ifndef OS_SCHED_LOCK_EN
#error  "OS_CFG.H, Missing OS_SCHED_LOCK_EN: Include code for OSSchedLock() and OSSchedUnlock()"
#endif
