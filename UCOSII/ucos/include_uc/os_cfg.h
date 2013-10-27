/*
*********************************************************************************************************
uC/OS-IIʵʱ�����ں�
��Ҫ�İ��ļ�

�� ��: OS_CFG.H ucos�ں˹����ļ�
�� ��: Jean J. Labrosse
*********************************************************************************************************
*/


#ifndef __OS_CFG_H__
#define __OS_CFG_H__

//*********************************************************************************************************
//uC/OS-II ���ں˹���

#define OS_MAX_EVENTS             2    //Ӧ��������¼����ƿ����Ŀ �������0
#define OS_MAX_FLAGS              5    //Ӧ��������¼���־�����Ŀ	������� 0
#define OS_MAX_MEM_PART           5    //����ڴ�����Ŀ           MUST be > 0
#define OS_MAX_QS                 2    //Ӧ���������п��ƿ����Ŀ	MUST be > 0
#define OS_MAX_TASKS             11    //Ӧ�������������Ŀ			MUST be >= 2
#define OS_LOWEST_PRIO           31    //���������������ȼ�		���ô��� 63
#define OS_TASK_IDLE_STK_SIZE   512    //ͳ�������ջ��( # ����OS_STK�Ŀ����Ŀ)

#define OS_TASK_STAT_EN           1    //���� (1) ���߽�ֹ (0) ͳ������
#define OS_TASK_STAT_STK_SIZE   512    //���������ջ�� (#����OS_STK�Ŀ����Ŀ)

#define OS_ARG_CHK_EN             1    //���� (1) ���߽�ֹ (0) �����
#define OS_CPU_HOOKS_EN           1    //�ڴ�������ֲ�ļ�������ʹ�� uC/OS-II �Ľӿں���


                                       // ----------------------- �¼���־���� ------------------------
#define OS_FLAG_EN                1    //���� (1) ���߽�ֹ (0) �����¼���־��ش���
#define OS_FLAG_WAIT_CLR_EN       1    //������� Wait on Clear �¼���־����
#define OS_FLAG_ACCEPT_EN         1    //������� OSFlagAccept()
#define OS_FLAG_DEL_EN            1    //������� OSFlagDel()
#define OS_FLAG_QUERY_EN          1    //������� OSFlagQuery()


                                       // -------------------- ��Ϣ�������---------------------
#define OS_MBOX_EN                1    //���� (1) ���߽�ֹ (0) ������Ϣ������ش���
#define OS_MBOX_ACCEPT_EN         1    //������� OSMboxAccept()
#define OS_MBOX_DEL_EN            1    //������� OSMboxDel()
#define OS_MBOX_POST_EN           1    //������� OSMboxPost()
#define OS_MBOX_POST_OPT_EN       1    //������� OSMboxPostOpt()
#define OS_MBOX_QUERY_EN          1    //������� OSMboxQuery()


                                       // ---------------------�ڴ����  --------------------
#define OS_MEM_EN                 1    //���� (1) ���߽�ֹ (0) �����ڴ���ش���
#define OS_MEM_QUERY_EN           1    //������� OSMemQuery()


                                       // ---------------- �������ź����� ---------------
#define OS_MUTEX_EN               1    //���� (1) ���߽�ֹ (0) ��������ź���ش���
#define OS_MUTEX_ACCEPT_EN        1    //������� OSMutexAccept()
#define OS_MUTEX_DEL_EN           1    //������� OSMutexDel()
#define OS_MUTEX_QUERY_EN         1    //������� OSMutexQuery()


                                       // ---------------------- ��Ϣ���кŹ��� ----------------------
#define OS_Q_EN                   1    //���� (1) ���߽�ֹ (0) ������Ϣ������ش���
#define OS_Q_ACCEPT_EN            1    //������� OSQAccept()
#define OS_Q_DEL_EN               1    //������� OSQDel()
#define OS_Q_FLUSH_EN             1    //������� OSQFlush()
#define OS_Q_POST_EN              1    //������� OSQPost()
#define OS_Q_POST_FRONT_EN        1    //������� OSQPostFront()
#define OS_Q_POST_OPT_EN          1    //�������OSQPostOpt()
#define OS_Q_QUERY_EN             1    //�������OSQQuery()


                                       // ------------------------ �źŹ��� ------------------------
#define OS_SEM_EN                 1    //���� (1) ���߽�ֹ (0) �����ź���ش���
#define OS_SEM_ACCEPT_EN          1    //�������OSSemAccept()
#define OS_SEM_DEL_EN             1    //�������OSSemDel()
#define OS_SEM_QUERY_EN           1    //�������OSSemQuery()


                                       // ---------------------�������  ----------------------
#define OS_TASK_CHANGE_PRIO_EN    1    //�������OSTaskChangePrio()
#define OS_TASK_CREATE_EN         1    //�������OSTaskCreate()
#define OS_TASK_CREATE_EXT_EN     1    //�������OSTaskCreateExt()
#define OS_TASK_DEL_EN            1    //�������OSTaskDel()
#define OS_TASK_SUSPEND_EN        1    //�������OSTaskSuspend() and OSTaskResume()
#define OS_TASK_QUERY_EN          1    //�������OSTaskQuery()


                                       // --------------------- -ʱ����� ----------------------
#define OS_TIME_DLY_HMSM_EN       1    //�������OSTimeDlyHMSM()
#define OS_TIME_DLY_RESUME_EN     1    //�������OSTimeDlyResume()
#define OS_TIME_GET_SET_EN        1    //�������OSTimeGet() and OSTimeSet()


                                       // ---------------------- ��Ϲ��� -----------------------
#define OS_SCHED_LOCK_EN          1    //�������OSSchedLock() and OSSchedUnlock()


#define OS_TICKS_PER_SEC        1000   //����ÿ��Ľ�����Ŀ


//typedef INT16U             OS_FLAGS;   // Date type for event flag bits (8, 16 or 32 bits)             
typedef unsigned short            OS_FLAGS;   //�¼���־��������� (8λ, 16λ �� 32 λ)

#endif



















