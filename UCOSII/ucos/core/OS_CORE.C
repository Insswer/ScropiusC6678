/*
*********************************************************************************************************
uC/OS-IIʵʱ�����ں�
��Ҫ�İ��ļ�
�ں˹����ļ�

�� ��: OS_CORE.C �ں˽ṹ�����ļ�
�� ��: Jean J. Labrosse
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE	 //���û�ж���OS_MASTER_FILE���ļ�����
#define  OS_GLOBALS		 //����ȫ�̱� OS_GLOBALS
#include "includes.h"
#endif

//*******************************************************************************************************
//                             MAPPING TABLE TO MAP BIT POSITION TO BIT MASK
//ע��: ��ַ������Ƕ�ӦOSRbyTbl[i]��λֵ(0~7)����ֵ���OSMapTbl[]�����(������)

//OSMapTbl[]����������?��ӦOSRdy Grp��OSRbyTbl[i]��λֵ(0~7)
INT8U  const  OSMapTbl[]   = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

//*******************************************************************************************************
//������ȼ�������ұ�(PRIORITY RESOLUTION TABLE)
//ע��: ��ַ�������λģʽ���ҳ����̬������ȼ������񣬸�ֵӦ��ϸ����ȼ�λλֵ(0~7)

//OSUnMapTbl[]��������ȼ�������ұ?��ӦOSRdy Grp��OSRbyTbl[i]��λֵ(0~7)
INT8U  const  OSUnMapTbl[] = {
    0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x00 to 0x0F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x10 to 0x1F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x20 to 0x2F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x30 to 0x3F                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x40 to 0x4F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x50 to 0x5F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x60 to 0x6F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x70 to 0x7F                             */
    7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x80 to 0x8F                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0x90 to 0x9F                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xA0 to 0xAF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xB0 to 0xBF                             */
    6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xC0 to 0xCF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xD0 to 0xDF                             */
    5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,       /* 0xE0 to 0xEF                             */
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0        /* 0xF0 to 0xFF                             */
};

/*
*********************************************************************************************************
*                                       FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static  void  OS_InitEventList(void);
static  void  OS_InitMisc(void);
static  void  OS_InitRdyList(void);
static  void  OS_InitTaskIdle(void);
static  void  OS_InitTaskStat(void);
static  void  OS_InitTCBList(void);

//��ʼ��:(INITIALIZATION)
//����: ��ʼ��uC/OS-II���������ĵ��ñ����ڵ���OSStart()����֮ǰ��
//OSStart()�����������ʼ���ж�����
void  OSInit (void)
{
#if OS_VERSION >= 204
    OSInitHookBegin();                                           /* Call port specific initialization code   */
#endif

    OS_InitMisc();                                               /* Initialize miscellaneous variables       */

    OS_InitRdyList();                                            /* Initialize the Ready List                */
    OS_InitTCBList();                                            /* Initialize the free list of OS_TCBs      */
    OS_InitEventList();                                          /* Initialize the free list of OS_EVENTs    */

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
    OS_FlagInit();                                               /* Initialize the event flag structures     */
#endif

#if (OS_MEM_EN > 0) && (OS_MAX_MEM_PART > 0)
    OS_MemInit();                                                /* Initialize the memory manager            */
#endif

#if (OS_Q_EN > 0) && (OS_MAX_QS > 0)
    OS_QInit();                                                  /* Initialize the message queue structures  */
#endif

    OS_InitTaskIdle();                                           /* Create the Idle Task                     */
#if OS_TASK_STAT_EN > 0
    OS_InitTaskStat();                                           /* Create the Statistic Task                */
#endif

#if OS_VERSION >= 204
    OSInitHookEnd();                                             /* Call port specific init. code            */
#endif
}
//�жϺ�������ִ�� ENTER ISR
//����: ֪ͨuC/OS-II��һ���жϴ��?�����ڽ��У���������uC/OS-II�����ж�Ƕ�����
//OSIntEnter ()��OSIntExit (����j��ʹ��)�������ߣ�ֻ�����жϳ����С�
//����: ��
//����: ��
//ע��: 1) �����񼶲��ܵ��øú���
//      2) ���ϵͳʹ�õĴ������ܹ�ִ���Զ��Ķ�bִ�ж�ȡ���޸ģ�д��Ĳ�����ô�Ϳ���ֱ�ӵ���
//         �ж�Ƕ�ײ���(OSIntNesting)��������Ա�����ú������4�Ķ��⿪�����жϷ����ӳ�����
//         ��OSIntNesting��1�ǲ���������ģ���Ϊ��OSIntNesting��1ʱ���ж��ǹرյ�
//      3) �ж�Ƕ����ȿɴ�255
void  OSIntEnter (void)
{
#if OS_CRITICAL_METHOD == 3                                //�жϺ����趨Ϊģʽ3
    OS_CPU_SR  cpu_sr;
#endif
	//OS_ENTER_CRITICAL(); //�ر��ж�
    if (OSRunning == TRUE) {
        if (OSIntNesting < 255) {
            OSIntNesting++;                      //�ж�Ƕ�׼������1
        }
    }
	//OS_EXIT_CRITICAL(); //���ж�
}

//�жϺ����Ѿ���� EXIT ISR
//����: ֪ͨuC/OS-II��һ���жϷ����Ѿ�ִ����ɣ���������uC/OS-II�����ж�Ƕ�׵����ͨ��
//OSIntExit()��OSIntEnter()j��ʹ�á������һ��Ƕ�׵��ж�ִ�����ʱ������и�����ȼ�����
//׼��������uC/OS-II����������Ⱥ�������������£��жϷ��ص�������ȼ������񣬶�
//�Ǳ��ж��˵����񡣵����ߣ�ֻ�����жϳ����С�
//����: ��
//����: ��
//ע��: 1) �����񼶲��ܵ��øú����Ҽ�ʹû�е���OSIntEnter()�������ʹ��ֱ�ӵ���OSIntNesting�ķ�����
//         Ҳ�������OSIntExit()��
//      2) �������������ڽ�ֹ������ (�鿴 OSSchedLock()����)
void  OSIntExit (void)
{
#if OS_CRITICAL_METHOD == 3                                //�жϺ����趨Ϊģʽ3
    OS_CPU_SR  cpu_sr;
#endif
    
    
    if (OSRunning == TRUE) {							   //�ر��ж�
//        OS_ENTER_CRITICAL();
        if (OSIntNesting > 0) {                            //����ж�Ƕ�״���0
            OSIntNesting--;								   //�ж�Ƕ�׼������1
        }
//1)�ж�Ƕ�ײ�����������Ƕ�׼�����(OSLockNesting)���߶���������
//2)OSRdyTbl[]����ļ���ֵY�Ǳ�����ȫ�̱�OSIntExitY��
//3)������������ȼ���ľ�����������ȼ��Ƿ����������е���������ȼ�
//4)��������ƿ����ȼ��?�浽ָ����߼����ȼ�����������ƿ��ָ��
//5)�������л��Ĵ���(ͳ�����������)
//6)���ж������л�
        if ((OSIntNesting == 0) && (OSLockNesting == 0)) { //1
            OSIntExitY    = OSUnMapTbl[OSRdyGrp];          //2
            OSPrioHighRdy = (INT8U)((OSIntExitY << 3) + OSUnMapTbl[OSRdyTbl[OSIntExitY]]);
            if (OSPrioHighRdy != OSPrioCur) {              //3
                OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy];//4
//                printf("switch to prio %d task\n", OSTCBHighRdy->OSTCBPrio);
                OSCtxSwCtr++;                              //5
                OSIntCtxSw();                              //6
            }
        }
//        OS_EXIT_CRITICAL();	//���ж�
    }
}

//���������� PREVENT SCHEDULING
//����: ���������ڽ�ֹ�����ȣ�ֱ��������ɺ���ø���������OSSchedUnlock()Ϊֹ������
//	  	OSSchedlock()�����񱣳ֶ�CPU�Ŀ���Ȩ�������и����ȼ���ߵ���������˾���̬��Ȼ��,��ʱ
//		�ж��ǿ��Ա�ʶ���,�жϷ���Ҳ�ܵõ�(�����ж��ǿ��ŵ�)��OSSchedlock()��OSSchedUnlock()
//		����ɶ�ʹ��.��OSLockNesting����OSSchedLock()������õĴ���,������Ƕ�׵ĺ������
//		��δ���,��δ����������񲻵ø�Ԥ.uC/OS-II����Ƕ����ȴ�255��.��OSLockNesting������ʱ��
//		������µõ�����.����OSSchedLock()��OSSchedUnlock()��ʹ��Ҫ�ǳ�����,��Ϊ����Ӱ��uC/OS-II
//		�����������?
//˵���OSLockNesting�����ʱ��OSSchedUnlock()����OSSched��OSSchedUnlock()�Ǳ�ĳ������õģ�
//		�ڵ����������ڼ䣬������ʲô�¼������˲�ʹһ�������ȼ�������������̬��
//����: ��
//����: ��
//���棺���õ��ðѵ�ǰ�������ĳ���
//ע��: 1) ����OSSchedLock()�Ժ��û���Ӧ�ó��򲻵�ʹ���κ��ܽ�������������ϵͳ���á�Ҳ��˵��
//		�û����򲻵õ���OSMboxPend()��OSQPend()��OSSemPend()��OSTaskSuspend(OS_PR1O_SELF)��
//		OSTimeDly()��OSTimeDlyHMSM(),ֱ��OSLockNesting����Ϊֹ����Ϊ������������û�����ס
//		��ϵͳ���κ��������񶼲������С�
//		2) �������ȼ�������Ҫ����Ϣ�����������䡢��Ϣ���С��ź�ʱ���û���ϣ������ȼ�����
//		�������䡢���к��ź�û�еõ���Ϣ֮ǰ��ȡ����CPU�Ŀ���Ȩ����ʱ���û�����ʹ�ý�ֹ
//		�������
#if OS_SCHED_LOCK_EN > 0   //�������OSSchedLock()����
void  OSSchedLock (void)
{
#if OS_CRITICAL_METHOD == 3                      //�жϺ����趨Ϊģʽ3
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSRunning == TRUE) {                      //����ж������������
        OS_ENTER_CRITICAL();
        if (OSLockNesting < 255) {               //����Ƕ���Ƿ����255
            OSLockNesting++;                     //������Ƕ�׼�1
        }
        OS_EXIT_CRITICAL();					     //���ж�
    }
}
#endif    

//��������� ENABLE SCHEDULING
//����: ���������ڽ��������
//����: ��
//����: ��
//ע��: 1) OSSchedlock()��OSSchedUnlock()����ɶ�ʹ�ã���ʹ��OSSchedUnlock()����֮ǰ����ʹ
//		   ��OSSchedLock()����
#if OS_SCHED_LOCK_EN > 0
void  OSSchedUnlock (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSRunning == TRUE) {                                   /* Make sure multitasking is running    */
        OS_ENTER_CRITICAL();
        if (OSLockNesting > 0) {                               /* Do not decrement if already 0        */
            OSLockNesting--;                                   /* Decrement lock nesting level         */
            if ((OSLockNesting == 0) && (OSIntNesting == 0)) { /* See if sched. enabled and not an ISR */
                OS_EXIT_CRITICAL();
                OS_Sched();                                    /* See if a HPT is ready                */
            } else {
                OS_EXIT_CRITICAL();
            }
        } else {
            OS_EXIT_CRITICAL();
        }
    }
}
#endif  
  
//�������� START MULTITASKING
//����: ������OSStart()ʱ��OSStart()�������������ҳ��Ǹ��û���b�����ȼ����������������
//		�顣Ȼ��OSStart()���ø����ȼ��������������OSStartHighRdy()��(���������ļ�
//		OS_CPU_A.ASM)������ļ���ѡ���΢�������йء�ʵ���ϣ�����OSStartHighRdy()�ǽ�����ջ��
//		�����ֵ���ص�CPU�Ĵ����У�Ȼ��ִ��һ���жϷ���ָ��жϷ���ָ��ǿ��ִ�и�������롣
//		�����ȼ��������������OSStartHighRdy()��
//����: ��
//����: ��
//ע��: OSStartHighRdy() ����:
//		a) OSRunningΪ�棬ָ��������Ѿ���ʼ
//		b) ��uC/OS-II֮ǰ�����ٱ��뽨bһ��Ӧ������
//	    c) OSStartHighRdy()����Զ�����ص�OSStart()
void  OSStart (void)
{
    INT8U y;
    INT8U x;
    if (OSRunning == FALSE) {//OSRunning����Ϊ"��"��ָ��������Ѿ���ʼ
        y             = OSUnMapTbl[OSRdyGrp];        //����������ȼ����������
        x             = OSUnMapTbl[OSRdyTbl[y]];
        OSPrioHighRdy = (INT8U)((y << 3) + x);		 //�ҳ����̬��߼�������ƿ�
        OSPrioCur     = OSPrioHighRdy;				 //OSPrioCur��OSPrioHighRdy��ŵ����û�Ӧ����������ȼ�
        OSTCBHighRdy  = OSTCBPrioTbl[OSPrioHighRdy]; /* Point to highest priority task ready to run    */
        OSTCBCur      = OSTCBHighRdy;
        OSStartHighRdy();                            //���ø����ȼ��������������
    }
}

//ͳ�������ʼ�� STATISTICS INITIALIZATION
//����: ͳ�Ƴ�ʼ������OSStatInit()����û������Ӧ����������ʱ�����м�����(OSIdleCtr)�ļ���
//		�ж�졣�������ÿ��ִ��һ�Σ���ȷ������Ӧ�ó����е���������˶���CPUʱ�䡣���û���
//		Ӧ�ó����������Ժ����п��������CPUʱ������ˣ�OSIdleCtr�Ͳ�����ԭ4ʲô���񶼲�
//		����ʱ����ô�����Ҫ֪�#�OSIdleCtr��������ֵ��OSStatInit()�ڳ�ʼ��ʱ�����ڼ���
//		�����ֵOSIdleCtrMax�еġ�CPU�����ʣ�
//		                                        ���м���ֵOSIdleCtr
//      CPU ʹ����Usage (%) = 100 * (1 - --------------------------------)
//                                        �趨�����м���ֵOSIdleCtrMax
//����: ��
//����: ��
#if OS_TASK_STAT_EN > 0
void  OSStatInit (void)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    OSTimeDly(2);                                //�����ӳٺ���OSTimeDly()��������ʱ2��ʱ�ӽ�����ֹͣ���������
												 //����Ϊ��ʹOSStatInit()��ʱ�ӽ���ͬ��
    OS_ENTER_CRITICAL();
    OSIdleCtr    = 0L;                           //ִ��OSStartInit()ʱ�����м�����OSIdleCtr������                                */
    OS_EXIT_CRITICAL();
    OSTimeDly(OS_TICKS_PER_SEC);                 //��������ʱ����һ��
	                                             //(��Ϊû������������̬������OSTaskIdle()�ֻ����CPU�Ŀ���Ȩ)
    OS_ENTER_CRITICAL();
    OSIdleCtrMax = OSIdleCtr;                    //���м�����1�����ڼ����ֵ������м��������ֵ
    OSStatRdy    = TRUE;						 //��ͳ����������־OSStatRdy��Ϊ"��"���Դ�4����}��ʱ�ӽ���
    OS_EXIT_CRITICAL();
}
#endif
//ʱ�ӽ��ĺ��� PROCESS SYSTEM TICK
//����: uC/OS��Ҫ�û��ṩ�������ź�Դ������ʵ��ʱ����ʱ��ȷ�ϳ�ʱ��������Ӧ��ÿ��10�ε�100��
//		֮�䣬����˵10��100Hz��ʱ�ӽ�����Խ�ߣ�ϵͳ�Ķ��⸺�ɾ�Խ��.ʱ�ӽ��ĵ�ʵ��Ƶ��ȡ����
//		�û�Ӧ�ó���ľ��ȡ�ʱ�ӽ���Դ������ר�ŵ�Ӳ����ʱ��Ҳ������4��50/60Hz�����Դ��
//		�ź�
//����: ��
//����: ��
void  OSTimeTick (void)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_TCB    *ptcb; //����������ƿ����ȼ����


    OSTimeTickHook();                                      //���û����Զ��庯��(���Ӻ���)
#if OS_TIME_GET_SET_EN > 0   
//    OS_ENTER_CRITICAL();                                   //�ۼӴӿ�����4��ʱ�䣬�õ���һ���޷��32λ��
    OSTime++;
//    OS_EXIT_CRITICAL();
#endif
    if (OSRunning == TRUE) {    
        ptcb = OSTCBList;                                  //����������ƿ��б��׵�ַ
														   //��OSTCBList��ʼ������OS_TCBt����һֱ���������
        while (ptcb->OSTCBPrio != OS_IDLE_PRIO) {          /* Go through all TCBs in TCB list          */
//            OS_ENTER_CRITICAL();
            if (ptcb->OSTCBDly != 0) {                     //�������ȴ�ʱ����������Ϊ0
										   				//��ȷ�б��������ĺ���OSTaskSuspend()����������򲻻�������̬
														//ִ��ʱ��ֱ����Ӧ�ó����н�b�˶��ٸ�������
             if (--ptcb->OSTCBDly == 0) {               //�������ȴ�ʱ����������Ϊ0
														//��ĳ�����������ƿ��е�ʱ����ʱ��OSTCBDly�����㣬�������ͽ����˾���̬
                    if ((ptcb->OSTCBStat & OS_STAT_SUSPEND) == OS_STAT_RDY) { /* Is task suspended?    */
                        OSRdyGrp               |= ptcb->OSTCBBitY; /* No,  Make task R-to-R (timed out)*/
                        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
                    } else {                               /* Yes, Leave 1 tick to prevent ...         */
                        ptcb->OSTCBDly = 1;                /* ... loosing the task when the ...        */
                    }                                      /* ... suspension is removed.               */
                }
            }
            ptcb = ptcb->OSTCBNext;                        //ָ�������˫��t�ӱ�ĺ�t��
//            OS_EXIT_CRITICAL();
        }
    }
}		 
//��ð汾��GET VERSION
//����: ������Ƿ���һ��uC/OS-II�İ汾ֵ. ����ֵ����100��uC/OS-II�İ汾��.
//����: ��
//����: uC/OS-II�İ汾�ų���100.
INT16U  OSVersion (void)
{
    return (OS_VERSION);
}

//���⺯�� DUMMY FUNCTION
//����: �������������. ����������OSTaskDel()����.
//����: ��
//����: uC/OS-II�İ汾�ų���100.
#if OS_TASK_DEL_EN > 0
void  OS_Dummy (void)
{
}
#endif

//ʹһ������������̬ MAKE TASK READY TO RUN BASED ON EVENT OCCURING
//����: ��������ĳ���¼������¼��ȴ������б��е�������ȼ�����(HPT)Ҫ���ھ���̬ʱ�����¼���Ӧ
//		��OSSemPost()��OSMboxPost()��OSQPost()����OSQPostFront()�������OSEventTaskRdy()ʵ��
//		�ò�����仰˵���ú���ӵȴ����������ɾ��HPT���񣬲��Ѹ��������ھ���̬
//����: pevent    is a pointer to the event control block corresponding to the event.
//      msg       is a pointer to a message.  This pointer is used by message oriented services
//                such as MAILBOXEs and QUEUEs.  The pointer is not used when called by other
//                service functions.
//      msk       is a mask that is used to clear the status byte of the TCB.  For example,
//                OSSemPost() will pass OS_STAT_SEM, OSMboxPost() will pass OS_STAT_MBOX etc.//����: ��
//ע��: �������uC/OS-II�ڲ������㲻������Ӧ�ó��������,���ô˺���ҲӦ���ر��ж�
#if OS_EVENT_EN > 0
INT8U  OS_EventTaskRdy (OS_EVENT *pevent, void *msg, INT8U msk)
{
    OS_TCB *ptcb;
    INT8U   x;
    INT8U   y;
    INT8U   bitx;
    INT8U   bity;
    INT8U   prio;

//1)���ȼ���HPT������.OSEventTbl[]�е��ֽ�����������һ���0��OS_LOWEST_PRIO/8+1֮�����
//2)�����ø�����õ������ȼ�������.OSEventGrp�е�λ�q���
//3)�ж�HPT������.OSEventTbl[]����Ӧλ��λ��
//4)������һ���0��OS_LOWEST_PRIO/8+1֮������Լ���Ӧ��λ�q���
//5)������Ͻ��OSEventTaskRdy()��������HPT��������ȼ�
//6)Ȼ��Ϳ��Դӵȴ������б���ɾ���������
    y    = OSUnMapTbl[pevent->OSEventGrp];            /* Find highest prio. task waiting for message   */
    bity = OSMapTbl[y];
    x    = OSUnMapTbl[pevent->OSEventTbl[y]];
    bitx = OSMapTbl[x];
    prio = (INT8U)((y << 3) + x);                     /* Find priority of task getting the msg         */
    if ((pevent->OSEventTbl[y] &= ~bitx) == 0x00) {   /* Remove this task from the waiting list        */
        pevent->OSEventGrp &= ~bity;                  /* Clr group bit if this was only task pending   */
    }
//7)�����TCB�а�����Ҫ�ı����Ϣ��֪����HPT��������ȼ����Ϳɵõ�ָ��������TCB��ָ��
//8)��Ϊ������ȼ�������������Ѿ��õ����㣬����ֹͣOSTimeTick()�����.OSTCBDly��ĵݼ����
// ����OSEventTaskRdy()ֱ�ӽ������峺0
//9)��Ϊ�������ٵȴ���¼��ķ������Ա�������������ƿ���ָ���¼����ƿ��ָ��ָ��NULL
//10)���OSEventTaskRdy()����OSMboxPost()����OSQPost()���õģ��ú���Ҫ����Ӧ����Ϣ���ݸ�
// HPT���������������ƿ���
    ptcb                 =  OSTCBPrioTbl[prio];       /* Point to this task's OS_TCB                   */
    ptcb->OSTCBDly       =  0;                        /* Prevent OSTimeTick() from readying task       */
    ptcb->OSTCBEventPtr  = (OS_EVENT *)0;             /* Unlink ECB from this task                     */
#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0)
    ptcb->OSTCBMsg       = msg;                       /* Send message directly to waiting task         */
#else
    msg                  = msg;                       /* Prevent compiler warning if not used          */
#endif
//11)��OSEventTaskRdy()������ʱ��λ�q���msk��Ϊ����ݸ���ò��������ڶ�������ƿ��е�
// λ�����λ�q��룬�������¼����������Ӧ
//12)���.OSTCBStat�жϸ������Ƿ��Ѵ��ھ���״̬
//13)�����, ��HPT���뵽uC/OS-II�ľ��������б��С�ע�⣬HPT����õ����¼���һ���������
// ״̬��Ҳ��������Ѿ���������ԭ�������
    ptcb->OSTCBStat     &= ~msk;                      /* Clear bit associated with event type          */
    if (ptcb->OSTCBStat == OS_STAT_RDY) {             /* See if task is ready (could be susp'd)        */
        OSRdyGrp        |=  bity;                     /* Put task in the ready to run list             */
        OSRdyTbl[y]     |=  bitx;
    }
    return (prio);
}
#endif
//ʹһ���������ȴ�ĳ�¼�����״̬ MAKE TASK WAIT FOR EVENT TO OCCUR
//����: ��ĳ��������ȴ�һ���¼��ķ���ʱ���ź����������ź��������Լ���Ϣ���л�ͨ����Ӧ��
//		PEND������ñ�����ʹ��ǰ����Ӿ�����������������̬�����ŵ���Ӧ���¼����ƿ�ECB�ĵ�
// 		���������
//����: pevent ������¼����ƿ��ָ�룬Ϊ�ȴ�ĳ�¼����������
//����: ��
//ע��: �������uC/OS-II�ڲ������㲻����Ӧ�ó����е��������OS_EventTO()ҲӦ���ر��ж�
#if OS_EVENT_EN > 0
void  OS_EventTaskWait (OS_EVENT *pevent)
{	//��ָ���¼����ƿ�ECB��ָ��ŵ������������ƿ�TCB�У���b�������¼����ƿ�ECB֮���t��
    OSTCBCur->OSTCBEventPtr = pevent;             //������Ӿ����������ɾ��
    if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0x00) {   /* Task no longer ready      */
        OSRdyGrp &= ~OSTCBCur->OSTCBBitY;        /* Clear event grp bit if this was only task pending  */
    }
    //�Ѹ�����ŵ��¼����ƿ�ECB�ĵȴ��¼��������б���
	pevent->OSEventTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;          /* Put task in waiting list  */
    pevent->OSEventGrp                   |= OSTCBCur->OSTCBBitY;
}
#endif

//���ڳ�ʱ��������Ϊ����̬ MAKE TASK READY TO RUN BASED ON EVENT TIMEOUT
//����: �����Ԥ��ָ���ĵȴ�ʱ��������ȴ���¼�û�з�����ô���������Ϊ�ȴ�ʱ�������
//		״̬��Ϊ����̬������������£��ź����������ź��������Լ���Ϣ���л�ͨ��PEND�����
//		�ñ���������������
//����: pevent ������¼����ƿ��ָ�룬Ϊ��ʱ����̬������
//����: ��
//ע��: �������uC/OS-II�ڲ������㲻������Ӧ�ó����е��������OS_EventTO()ҲӦ���ر��ж�
#if OS_EVENT_EN > 0
void  OS_EventTO (OS_EVENT *pevent)
{
    if ((pevent->OSEventTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0x00) {
        pevent->OSEventGrp &= ~OSTCBCur->OSTCBBitY;
    }
    OSTCBCur->OSTCBStat     = OS_STAT_RDY;       /* Set status to ready                                */
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;     /* No longer waiting for event                        */
}
#endif
//�¼����ƿ��б��ʼ�� INITIALIZE EVENT CONTROL BLOCK'S WAIT LIST
//����: ����bһ���ź������������Ϣ����ʱ����Ӧ�Ľ�b����OSSemInit()��OSMboxCreate()������
//		OSQCreate()ͨ�����OSEventWaitListInit()���¼����ƿ��еĵȴ������б���г�ʼ�����ú���
//		��ʼ��һ��յĵȴ������б?����û���κ����񡣸ú���ĵ��ò���ֻ��һ�����ָ����Ҫ��
// 		ʼ�����¼����ƿ��ָ��pevent��
//����: pevent ����һ��ָ����¼����ƿ飬��ָ������Ǵ����ź����������ź����������Ϣ��
// 		��ʱ������¼����ƿ��ָ��
//����: ��
//ע��: �������uC/OS-II�ڲ������㲻���Ե�����
#if ((OS_Q_EN > 0) && (OS_MAX_QS > 0)) || (OS_MBOX_EN > 0) || (OS_SEM_EN > 0) || (OS_MUTEX_EN > 0)
void  OS_EventWaitListInit (OS_EVENT *pevent)
{
    INT8U  *ptbl;


    pevent->OSEventGrp = 0x00;                   /* No task waiting on event                           */
    ptbl               = &pevent->OSEventTbl[0];

#if OS_EVENT_TBL_SIZE > 0
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 1
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 2
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 3
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 4
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 5
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 6
    *ptbl++            = 0x00;
#endif

#if OS_EVENT_TBL_SIZE > 7
    *ptbl              = 0x00;
#endif
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                           INITIALIZE THE FREE LIST OF EVENT CONTROL BLOCKS
*
* Description: This function is called by OSInit() to initialize the free list of event control blocks.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitEventList (void)
{
#if (OS_EVENT_EN > 0) && (OS_MAX_EVENTS > 0)
#if (OS_MAX_EVENTS > 1)
    INT16U     i;
    OS_EVENT  *pevent1;
    OS_EVENT  *pevent2;


    pevent1 = &OSEventTbl[0];
    pevent2 = &OSEventTbl[1];
    for (i = 0; i < (OS_MAX_EVENTS - 1); i++) {                  /* Init. list of free EVENT control blocks  */
        pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
        pevent1->OSEventPtr  = pevent2;
        pevent1++;
        pevent2++;
    }
    pevent1->OSEventType = OS_EVENT_TYPE_UNUSED;
    pevent1->OSEventPtr  = (OS_EVENT *)0;
    OSEventFreeList      = &OSEventTbl[0];
#else
    OSEventFreeList              = &OSEventTbl[0];               /* Only have ONE event control block        */
    OSEventFreeList->OSEventType = OS_EVENT_TYPE_UNUSED;
    OSEventFreeList->OSEventPtr  = (OS_EVENT *)0;
#endif
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                    INITIALIZE MISCELLANEOUS VARIABLES
*
* Description: This function is called by OSInit() to initialize miscellaneous variables.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitMisc (void)
{
#if OS_TIME_GET_SET_EN > 0   
    OSTime        = 0L;                                          /* Clear the 32-bit system clock            */
#endif

    OSIntNesting  = 0;                                           /* Clear the interrupt nesting counter      */
    OSLockNesting = 0;                                           /* Clear the scheduling lock counter        */

    OSTaskCtr     = 0;                                           /* Clear the number of tasks                */

    OSRunning     = FALSE;                                       /* Indicate that multitasking not started   */
    
    OSCtxSwCtr    = 0;                                           /* Clear the context switch counter         */
    OSIdleCtr     = 0L;                                          /* Clear the 32-bit idle counter            */

#if (OS_TASK_STAT_EN > 0) && (OS_TASK_CREATE_EXT_EN > 0)
    OSIdleCtrRun  = 0L;
    OSIdleCtrMax  = 0L;
    OSStatRdy     = FALSE;                                       /* Statistic task is not ready              */
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                       INITIALIZE THE READY LIST
*
* Description: This function is called by OSInit() to initialize the Ready List.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitRdyList (void)
{
    INT16U   i;
    INT8U   *prdytbl;


    OSRdyGrp      = 0x00;                                        /* Clear the ready list                     */
    prdytbl       = &OSRdyTbl[0];
    for (i = 0; i < OS_RDY_TBL_SIZE; i++) {
        *prdytbl++ = 0x00;
    }

    OSPrioCur     = 0;
    OSPrioHighRdy = 0;

    OSTCBHighRdy  = (OS_TCB *)0;                                 
    OSTCBCur      = (OS_TCB *)0;
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                         CREATING THE IDLE TASK
*
* Description: This function creates the Idle Task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitTaskIdle (void)
{
#if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt(OS_TaskIdle,
                          (void *)0,                                 /* No arguments passed to OS_TaskIdle() */
                          &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1], /* Set Top-Of-Stack                     */
                          OS_IDLE_PRIO,                              /* Lowest priority level                */
                          OS_TASK_IDLE_ID,
                          &OSTaskIdleStk[0],                         /* Set Bottom-Of-Stack                  */
                          OS_TASK_IDLE_STK_SIZE,
                          (void *)0,                                 /* No TCB extension                     */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);/* Enable stack checking + clear stack  */
    #else
    (void)OSTaskCreateExt(OS_TaskIdle,
                          (void *)0,                                 /* No arguments passed to OS_TaskIdle() */
                          &OSTaskIdleStk[0],                         /* Set Top-Of-Stack                     */
                          OS_IDLE_PRIO,                              /* Lowest priority level                */
                          OS_TASK_IDLE_ID,
                          &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1], /* Set Bottom-Of-Stack                  */
                          OS_TASK_IDLE_STK_SIZE,
                          (void *)0,                                 /* No TCB extension                     */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);/* Enable stack checking + clear stack  */
    #endif
#else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate(OS_TaskIdle,
                       (void *)0,
                       &OSTaskIdleStk[OS_TASK_IDLE_STK_SIZE - 1],
                       OS_IDLE_PRIO);
    #else
    (void)OSTaskCreate(OS_TaskIdle,
                       (void *)0,
                       &OSTaskIdleStk[0],
                       OS_IDLE_PRIO);
    #endif
#endif
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                                      CREATING THE STATISTIC TASK
*
* Description: This function creates the Statistic Task.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

#if OS_TASK_STAT_EN > 0
static  void  OS_InitTaskStat (void)
{
#if OS_TASK_CREATE_EXT_EN > 0
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreateExt(OS_TaskStat,
                          (void *)0,                                   /* No args passed to OS_TaskStat()*/
                          &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],   /* Set Top-Of-Stack               */
                          OS_STAT_PRIO,                                /* One higher than the idle task  */
                          OS_TASK_STAT_ID,
                          &OSTaskStatStk[0],                           /* Set Bottom-Of-Stack            */
                          OS_TASK_STAT_STK_SIZE,
                          (void *)0,                                   /* No TCB extension               */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  /* Enable stack checking + clear  */
    #else
    (void)OSTaskCreateExt(OS_TaskStat,
                          (void *)0,                                   /* No args passed to OS_TaskStat()*/
                          &OSTaskStatStk[0],                           /* Set Top-Of-Stack               */
                          OS_STAT_PRIO,                                /* One higher than the idle task  */
                          OS_TASK_STAT_ID,
                          &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],   /* Set Bottom-Of-Stack            */
                          OS_TASK_STAT_STK_SIZE,
                          (void *)0,                                   /* No TCB extension               */
                          OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);  /* Enable stack checking + clear  */
    #endif
#else
    #if OS_STK_GROWTH == 1
    (void)OSTaskCreate(OS_TaskStat,
                       (void *)0,                                      /* No args passed to OS_TaskStat()*/
                       &OSTaskStatStk[OS_TASK_STAT_STK_SIZE - 1],      /* Set Top-Of-Stack               */
                       OS_STAT_PRIO);                                  /* One higher than the idle task  */
    #else
    (void)OSTaskCreate(OS_TaskStat,
                       (void *)0,                                      /* No args passed to OS_TaskStat()*/
                       &OSTaskStatStk[0],                              /* Set Top-Of-Stack               */
                       OS_STAT_PRIO);                                  /* One higher than the idle task  */
    #endif
#endif
}
#endif
/*$PAGE*/
/*
*********************************************************************************************************
*                                             INITIALIZATION
*                            INITIALIZE THE FREE LIST OF TASK CONTROL BLOCKS
*
* Description: This function is called by OSInit() to initialize the free list of OS_TCBs.
*
* Arguments  : none
*
* Returns    : none
*********************************************************************************************************
*/

static  void  OS_InitTCBList (void)
{
    INT8U    i;
    OS_TCB  *ptcb1;
    OS_TCB  *ptcb2;


    OSTCBList     = (OS_TCB *)0;                                 /* TCB Initialization                       */
    for (i = 0; i < (OS_LOWEST_PRIO + 1); i++) {                 /* Clear the priority table                 */
        OSTCBPrioTbl[i] = (OS_TCB *)0;
    }
    ptcb1 = &OSTCBTbl[0];
    ptcb2 = &OSTCBTbl[1];
    for (i = 0; i < (OS_MAX_TASKS + OS_N_SYS_TASKS - 1); i++) {  /* Init. list of free TCBs                  */
        ptcb1->OSTCBNext = ptcb2;
        ptcb1++;
        ptcb2++;
    }
    ptcb1->OSTCBNext = (OS_TCB *)0;                              /* Last OS_TCB                              */
    OSTCBFreeList    = &OSTCBTbl[0];
}

//������
//����: uC/OS-II�������н������̬���������ȼ���ߵ���һ��ȷ���ĸ��������ȼ���ߣ��������
// 		�����������˵Ĺ������ɵ����(Scheduler)��ɵġ����񼶵ĵ�����ɺ���OSSched()��ɵġ�
//		�жϼ��ĵ��������һ����OSIntExt()��ɵ�eduling).
//����: none
//����: none
//ע��: 1) ����һ��uC/OS-II�ڲ������㲻����Ӧ�ó�����ʹ����
//		2) �������������ڽ�ֹ������ (�鿴 OSSchedLock()����)
//˵��: 1)�����л��ܼ򵥣�������}����ɣ��������������΢������Ĵ��������ջ��Ȼ�󽫽ϸ�����
//		��������ļĴ���ֵ��ջ�лָ����Ĵ����С���uC/OS-II�У����������ջ�ṹ���ǿ���4���
//	 	�շ�����ж�һ������΢������ļĴ��������ջ�С����仰˵��uC/OS-II���о���̬����
//		����Ҫ���һ�У�ֻ�ǻָ����е�CPU�Ĵ��������жϷ���ָ�Ϊ���������л�������
//		OS_TASK_SW(),��Ϊģ����һ���жϡ�����΢�����������ж�ָ���������ָ��TRAP4ʵ�������
//		���жϷ����ӳ�������崦��(Trap hardler)��Ҳ�����¹ʴ���(exception handler)��������
//		���ж����������Ժ���OSCtxSw()��OSCtxSw()������ҪOS_TCBHighRdyָ�򼴽������������
//		����Ҫ�õ�ǰ������ƿ�OSTCBCurָ�򼴽�����������񣬲μ��8�£���ֲuC/OS-II���й���
//		OSCtxSw()�ĸ��꾡�Ľ��͡�
//		2) OSSched()�����д��붼���ٽ�δ��롣��Ѱ�ҽ������̬�����ȼ���ߵ��������У�Ϊ��ֹ��
//		�Ϸ����ӳ����һ��򼸸�����ľ���λ��λ���ж��Ǳ��ص�ġ�Ϊ����л�ʱ�䣬OSSched()ȫ
//		�����붼�����û������д��Ϊ��ӿɶ��ԣ�����ֲ�Ժͽ�������Դ������ٻ���OSSched()����
//		Cд�ġ�


void  OS_Sched (void)
{
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U      y;


    OS_ENTER_CRITICAL();
	//Ϊʵ�������л���OSTCBHighRdy����ָ�����ȼ���ߵ��Ǹ�������ƿ�OS_TCB������ͨ��
    if ((OSIntNesting == 0) && (OSLockNesting == 0)) { /* Sched. only if all ISRs done & not locked    */
	//����������жϷ����ӳ����е��õģ��ҵ������ģ��������Ⱥ����ҳ�������̬��
	//������ȼ����񣬽������̬�������ھ������OSRdyTbl[ ]����Ӧλ��λ.
        y             = OSUnMapTbl[OSRdyGrp];          /* Get pointer to HPT ready to run              */
        OSPrioHighRdy = (INT8U)((y << 3) + OSUnMapTbl[OSRdyTbl[y]]);
 		//�ҵ�������ȼ�����󣬺�����������ȼ���ߵ������Ƿ��ǵ�ǰ�������е������Ա��ⲻ
		//��Ҫ�������ȣ��໨ʱ��
       if (OSPrioHighRdy != OSPrioCur) {              /* No Ctx Sw if current task is highest rdy     */
		//Ϊʵ�������л���OSTCBHighRdy����ָ�����ȼ���ߵ��Ǹ�������ƿ�OS_TCB������ͨ��
		//��OSPrioHighRdyΪ�±��OSTCBPrioTbl[]�����е��Ǹ�Ԫ�ظ���OSTCBHighRdy4ʵ�ֵ�
            OSTCBHighRdy = OSTCBPrioTbl[OSPrioHighRdy];
//            printf("switch to prio %d task\n", OSTCBHighRdy->OSTCBPrio);
            OSCtxSwCtr++;                              //ͳ�Ƽ�����OSCtxSwCtr��1���Ը��������л�����
            OS_TASK_SW();                              //�������OS_TASK_SW()4���ʵ���ϵ������л�
        }
    }
    OS_EXIT_CRITICAL();
}
//�������� IDLE TASK
//����: �������uC/OS-II�ڲ�����uC/OS-II��Ҫ��bһ������������������û�������������
//		����̬ʱͶ�����С�������������Զ��Ϊ������ȼ�����OS_LOWEST_PRIO.�������񲻿��ܱ�Ӧ
//		�����ɾ��
//����: ��
//����: ��
//ע��: 1) OSTaskIdleHook()���������û��ں�����д���Լ��Ĵ��룬���Խ���OSTaskIdleHook()����
//		CPUִ��STOPָ��Ӷ����͹���ģʽ����Ӧ��ϵͳ�ɵ�ع���ʱ�����ַ�ʽ�ر����á�
//		2) �������Զ���ھ���̬�����Բ�Ҫ��OSTaskIdleHook()�е��ÿ���ʹ��������PEND����
//		OSTineDly???()�����OSTaskSuspend()����
void  OS_TaskIdle (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    for (;;) {
        OS_ENTER_CRITICAL();
        OSIdleCtr++;
        OS_EXIT_CRITICAL();
        OSTaskIdleHook();                        /* Call user definable HOOK                           */
    }
}
//ͳ������ STATISTICS TASK
//����: uC/OS-II��һ���ṩ����ʱ��ͳ�Ƶ���������������OSTaskStat(),����û���ϵͳ���峣
//		��OS_TASK_STAT_EN(���ļ�OS_CFG.H)��Ϊ1���������ͻὨb��һ���õ������?OSTaskStat()
//		ÿ��������һ��(���ļ�OS_CORE.C)�����㵱ǰ��CPU�����ʡ����仰˵��OSTaskStat()�����û�
//		Ӧ�ó���ʹ���˶���CPUʱ�䣬�ðٷֱȱ�ʾ�����ֵ����һ���з��8λ����OSCPUsage�У�����
//		����1��ٷֵ㡣
//		����û�Ӧ�ó������ʹ��ͳ�������û������ڳ�ʼ��ʱ��bһ��Ψһ�����������������
//		����OSStatInit()(���ļ�OS_CORE.C)�����仰˵���ڵ���ϵͳ�����OSStart()֮ǰ���û���
//		ʼ��������Ƚ�bһ����������������е���ϵͳͳ�Ƴ�ʼ������OSStatInit()��Ȼ���ٽ�b
//		Ӧ�ó����е���������
//								 OSIdleCtr
//		OSCPUUsage = 100 * (1 - ------------) (units are in %)
//								OSIdleCtrMax
//����: pdata ָ��һ����ݽṹ���ýṹ��4�ڽ�bͳ������ʱ�����񴫵ݲ���
//����: ��
//ע��: 1) uC/OS-II�Ѿ���������������ȼ���Ϊ��ͣ���OS_LOWEST_PR10��ͳ����������ȼ���Ϊ��
//		�ͣ�OS_LOWEST_PR10-1.
//		2) ��Ϊ�û���Ӧ�ó�������Ƚ�bһ����ʼ����TaskStart()����ʹ��ͳ������ǰ���û�������
//		�ȵ��õ���uC/OS-II�е�ϵͳ��ʼ������OSInit()��
//		3) �ڴ���ͳ������֮ǰ��Ϊ�˱���ϵͳ�ﵽ�ȶ�״̬����Ҫ�ӳ�5���ӣ������������ʱ2����
//		���趨�����м���ֵ
#if OS_TASK_STAT_EN > 0
void  OS_TaskStat (void *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT32U     run;
    INT32U     max;
    INT8S      usage;


    pdata = pdata;                               /* Prevent compiler warning for not using 'pdata'     */
    while (OSStatRdy == FALSE) {
        OSTimeDly(2 * OS_TICKS_PER_SEC);         /* Wait until statistic task is ready                 */
    }
    max = OSIdleCtrMax / 100L;
    for (;;) {
        OS_ENTER_CRITICAL();
        OSIdleCtrRun = OSIdleCtr;                /* Obtain the of the idle counter for the past second */
        run          = OSIdleCtr;
        OSIdleCtr    = 0L;                       /* Reset the idle counter for the next second         */
        OS_EXIT_CRITICAL();
        if (max > 0L) {
            usage = (INT8S)(100L - run / max);
            if (usage >= 0) {                    /* Make sure we don't have a negative percentage      */
                OSCPUUsage = usage;
            } else {
                OSCPUUsage = 0;
            }
        } else {
            OSCPUUsage = 0;
            max        = OSIdleCtrMax / 100L;
        }
        OSTaskStatHook();                        /* Invoke user definable hook                         */
        OSTimeDly(OS_TICKS_PER_SEC);             /* Accumulate OSIdleCtr for the next second           */
    }
}
#endif
//������ƿ��ʼ�� INITIALIZE TCB
//����: �������uC/OS-II�ڲ������ڽ�b����ʱ���õĳ�ʼ��������ƿ�OS_TCB����,��7�����
//		(�鿴 OSTaskCreate() �� OSTaskCreateExt()).
//����: prio ��������ȼ�
//		ptos OSTaskInit()��bջ�ṹ�Ժ�,ptos��ָ��ջ����ָ��,�ұ�����OS_TCB��OSTCBStkPrt��
//		pbos ָ��ջ�׵�ָ�룬������OSTCBStkBottom��Ԫ��
//		id �����־��(0..65535)��������.OSTCBId��
//		stk_size ��ջ������������OS_TCB��OSTABStkSize��
//		pext OS_TCB�е�)չָ�룬.OSTCBExtPtr��ֵ
//		opt OS_TCB��ѡ���������.OSTCBOpt��
//����: OS_NO_ERR ���óɹ�
//		OS_NO_MORE_TCB û�и���������ƿ鱻���䣬���޷������µ�����
//ע��: �������uC/OS-II�ڲ������㲻���Ե�����
INT8U  OS_TCBInit (INT8U prio, OS_STK *ptos, OS_STK *pbos, INT16U id, INT32U stk_size, void *pext, INT16U opt)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_TCB    *ptcb;


    OS_ENTER_CRITICAL();
    ptcb = OSTCBFreeList;                                  /* Get a free TCB from the free TCB list    */
    if (ptcb != (OS_TCB *)0) {
        OSTCBFreeList        = ptcb->OSTCBNext;            /* Update pointer to free TCB list          */
        OS_EXIT_CRITICAL();
        ptcb->OSTCBStkPtr    = ptos;                       /* Load Stack pointer in TCB                */
        ptcb->OSTCBPrio      = (INT8U)prio;                /* Load task priority into TCB              */
        ptcb->OSTCBStat      = OS_STAT_RDY;                /* Task is ready to run                     */
        ptcb->OSTCBDly       = 0;                          /* Task is not delayed                      */

#if OS_TASK_CREATE_EXT_EN > 0
        ptcb->OSTCBExtPtr    = pext;                       /* Store pointer to TCB extension           */
        ptcb->OSTCBStkSize   = stk_size;                   /* Store stack size                         */
        ptcb->OSTCBStkBottom = pbos;                       /* Store pointer to bottom of stack         */
        ptcb->OSTCBOpt       = opt;                        /* Store task options                       */
        ptcb->OSTCBId        = id;                         /* Store task ID                            */
#else
        pext                 = pext;                       /* Prevent compiler warning if not used     */
        stk_size             = stk_size;
        pbos                 = pbos;
        opt                  = opt;
        id                   = id;
#endif

#if OS_TASK_DEL_EN > 0
        ptcb->OSTCBDelReq    = OS_NO_ERR;
#endif

        ptcb->OSTCBY         = prio >> 3;                  /* Pre-compute X, Y, BitX and BitY          */
        ptcb->OSTCBBitY      = OSMapTbl[ptcb->OSTCBY];
        ptcb->OSTCBX         = prio & 0x07;
        ptcb->OSTCBBitX      = OSMapTbl[ptcb->OSTCBX];

#if OS_EVENT_EN > 0
        ptcb->OSTCBEventPtr  = (OS_EVENT *)0;              /* Task is not pending on an event          */
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0) && (OS_TASK_DEL_EN > 0)
        ptcb->OSTCBFlagNode  = (OS_FLAG_NODE *)0;          /* Task is not pending on an event flag     */
#endif

#if (OS_MBOX_EN > 0) || ((OS_Q_EN > 0) && (OS_MAX_QS > 0))
        ptcb->OSTCBMsg       = (void *)0;                  /* No message received                      */
#endif

#if OS_VERSION >= 204
        OSTCBInitHook(ptcb);
#endif

        OSTaskCreateHook(ptcb);                            /* Call user defined hook                   */
        
        OS_ENTER_CRITICAL();
        OSTCBPrioTbl[prio] = ptcb;
        ptcb->OSTCBNext    = OSTCBList;                    /* Link into TCB chain                      */
        ptcb->OSTCBPrev    = (OS_TCB *)0;
        if (OSTCBList != (OS_TCB *)0) {
            OSTCBList->OSTCBPrev = ptcb;
        }
        OSTCBList               = ptcb;
        OSRdyGrp               |= ptcb->OSTCBBitY;         /* Make task ready to run                   */
        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
        OS_EXIT_CRITICAL();
        return (OS_NO_ERR);
    }
    OS_EXIT_CRITICAL();
    return (OS_NO_MORE_TCB);
}
