/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae 
* Date			: 08/10/2010
* Description	: ��COS-II��STM32�ϵ���ֲ����C���Բ��֣�
*				  ��(�����ջ��ʼ������͹��Ӻ����

******************************************************************************/

#define  OS_CPU_GLOBALS
#include "includes.h"

/******************************************************************************
** �������: OSTaskStkInit
** ��������: �����ջ��ʼ�����룬���������ʧ�ܻ�ʹϵͳ��#
** �䡡��: task  : ����ʼִ�еĵ�ַ
**         pdata �����ݸ�����Ĳ���
**         ptos  ������Ķ�ջ��ʼλ��
**         opt   �����Ӳ���ǰ�汾���ڱ��������ã���������μ�OSTaskCreateExt()��opt����
** �䡡��: ջ��ָ��λ��
** ȫ�ֱ�:
** ����ģ��:

******************************************************************************/

OS_STK *OSTaskStkInit (void  (*task)(void  *parg), void  *parg, OS_STK  *ptos, INT16U  opt)
{
    OS_STK *stk;
    INT32U csr_value;
    INT32U dp_value;

    (void)opt;                                        /*  'opt' is not used, prevent  */
                                                      /*  warning û����'opt'��       */
                                                      /*  ��ֹ���뾯��                */
                                                                            
    stk = ptos;                                       /*  Load stack pointer          */
                                                      /*  װ�ض�ջָ��                */
    csr_value = BSP_ReadCSR();
    dp_value = BSP_GetCurDp();

                                                      /*  Registers stacked as if     */
                                                      /*  auto-saved on exception     */             
	                                                  /*  ģ����쳣���Զ��ѼĴ���ѹջ*/
    /* For C66XX save CSR, IER, IRP  Entry Point, B3, B0 - B2, B4 - B14, A0 - 15 */
    *stk = (INT32U)(csr_value|0x1);	/* CSR */
    stk -= 2;
    *stk = (INT32U)task;		/* Entry Point */
    stk -= 2;
    *stk = (INT32U)0xffffffffL;	/* B3 */
    stk -= 2;
    *stk = (INT32U)parg;		/* B0 */
    stk -= 2;
    *stk = (INT32U)0xb1b1b1b1L; /* B1 */
    stk -= 2;
    *stk = (INT32U)0xb2b2b2b2L; /* B2 */
    stk -= 2;
    *stk = (INT32U)0xb4b4b4b4L; /* B4 */
    stk -= 2;
    *stk = (INT32U)0xb5b5b5b5L; /* B5 */
    stk -= 2;
    *stk = (INT32U)0xb6b6b6b6L; /* B6 */
    stk -= 2;
    *stk = (INT32U)0xb7b7b7b7L; /* B7 */
    stk -= 2;
    *stk = (INT32U)0xb8b8b8b8L; /* B8 */
    stk -= 2;
    *stk = (INT32U)0xb9b9b9b9L; /* B9 */
    stk -= 2;
    *stk = (INT32U)0xbabababaL; /* B10 */
    stk -= 2;
    *stk = (INT32U)0xbbbbbbbbL; /* B11 */
    stk -= 2;
    *stk = (INT32U)0xbcbcbcbcL; /* B12 */
    stk -= 2;
    *stk = (INT32U)0xbdbdbdbdL; /* B13 */
    stk -= 2;
    *stk = (INT32U)dp_value; /* B14 */
    stk -= 2;
    *stk = (INT32U)0xa0a0a0a0L; /* A0 */
    stk -= 2;
    *stk = (INT32U)0xa1a1a1a1L; /* A1 */
    stk -= 2;
    *stk = (INT32U)0xa2a2a2a2L; /* A2 */
    stk -= 2;
    *stk = (INT32U)0xa3a3a3a3L; /* A3 */
    stk -= 2;
    *stk = (INT32U)0xa4a4a4a4L; /* A4 */
    stk -= 2;
    *stk = (INT32U)0xa5a5a5a5L; /* A5 */
    stk -= 2;
    *stk = (INT32U)0xa6a6a6a6L; /* A6 */
    stk -= 2;
    *stk = (INT32U)0xa7a7a7a7L; /* A7 */
    stk -= 2;
    *stk = (INT32U)0xa8a8a8a8L; /* A8 */
    stk -= 2;
    *stk = (INT32U)0xa8a8a8a9L; /* A9 */
    stk -= 2;
    *stk = (INT32U)0xaaaaaaaaL; /* A10 */
    stk -= 2;
    *stk = (INT32U)0xababababL; /* A11 */
    stk -= 2;
    *stk = (INT32U)0xacacacacL; /* A12 */
    stk -= 2;
    *stk = (INT32U)0xadadadadL; /* A13 */
    stk -= 2;
    *stk = (INT32U)0xaeaeaeaeL; /* A14 */
    stk -= 2;
    *stk = (INT32U)0xafafafafL; /* A15 */
    stk -= 2;

    return(stk);
}

OS_EXT  BOOLEAN   OSRunning;
OS_EXT  OS_TCB    *OSTCBHighRdy;
OS_EXT  OS_TCB           *OSTCBCur;
OS_EXT  INT8U             OSPrioCur;
OS_EXT  INT8U             OSPrioHighRdy;

void OS_ENTER_CRITICAL(void)
{
	BSP_DisableInt();
}

void OS_EXIT_CRITICAL(void)
{
		BSP_EnableInt();
}

void ItDisAll(void)
{
	BSP_DisableInt();
}

void OSRestoreHighRdy(unsigned long sp);
unsigned long OSTaskSwHook (void);
void OSIntCtxSwRestore(unsigned long sp);

void OSStartHighRdy(void)
{
	unsigned long sp;
	OSTaskSwHook();
	OSRunning = 1;
	sp = (unsigned long)OSTCBHighRdy->OSTCBStkPtr;
//	printf("start high rdy task with sp 0x%08x\n", sp);
	OSRestoreHighRdy(sp);
}

void          OSIntCtxSw(void);
void OSIntCtxSw(void)
{
	unsigned long sp;
	OSTaskSwHook();
	OSTCBCur = OSTCBHighRdy;
	OSPrioCur = OSPrioHighRdy;
	sp = (unsigned long)OSTCBHighRdy->OSTCBStkPtr;
//	printf("switch to sp 0x%08x\n", sp);
	OSIntCtxSwRestore(sp);
}

/* ����ΪһЩ���Ӻ���ȫ��Ϊ�պ������˵���뿴������� */

#if OS_CPU_HOOKS_EN
/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                            (BEGINNING)
*
* Description: This function is called by OSInit() at the beginning of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookBegin (void)
{
}
#endif

/*
*********************************************************************************************************
*                                       OS INITIALIZATION HOOK
*                                               (END)
*
* Description: This function is called by OSInit() at the end of OSInit().
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts should be disabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSInitHookEnd (void)
{
}
#endif


/*
*********************************************************************************************************
*                                          TASK CREATION HOOK
*
* Description: This function is called when a task is created.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskCreateHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}


/*
*********************************************************************************************************
*                                           TASK DELETION HOOK
*
* Description: This function is called when a task is deleted.
*
* Arguments  : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)    : 1) Interrupts are disabled during this call.
*********************************************************************************************************
*/
void OSTaskDelHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                       /* Prevent compiler warning                                     */
}

/*
*********************************************************************************************************
*                                           TASK SWITCH HOOK
*
* Description: This function is called when a task switch is performed.  This allows you to perform other
*              operations during a context switch.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are disabled during this call.
*              2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                 will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the 
*                 task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/
unsigned long OSTaskSwHook (void)
{
	unsigned long sp;
	/*
	 * 	MVKL	OSTCBHighRdy,	A4
		MVKH	OSTCBHighRdy,	A4

		MVKL	OSTCBCur,		A5
		MVKH	OSTCBCur,		A5

		LDW		*A4,			A6	;
		NOP		4
		STW		A6,				*A5 ;

		MVKL	OSPrioHighRdy,	A4
		MVKH	OSPrioHighRdy,	A4

		MVKL	OSPrioCur,		A5
		MVKH	OSPrioCur,		A5

		LDBU	*A4,			A6
		NOP		4
		STB		A6,				*A5

		MVKL	OSTCBHighRdy,	A4
		MVKH	OSTCBHighRdy,	A4
		LDW		*A4,			A6
		NOP		4
	 * */

	OSTCBCur = OSTCBHighRdy;
	OSPrioCur = OSPrioHighRdy;
	sp = (unsigned long)OSTCBHighRdy->OSTCBStkPtr;
//	printf("switch to sp 0x%08x\n", sp);
	return sp;
}

/*
*********************************************************************************************************
*                                           STATISTIC TASK HOOK
*
* Description: This function is called every second by uC/OS-II's statistics task.  This allows your 
*              application to add functionality to the statistics task.
*
* Arguments  : none
*********************************************************************************************************
*/
void OSTaskStatHook (void)
{
}

/*
*********************************************************************************************************
*                                           OSTCBInit() HOOK
*
* Description: This function is called by OSTCBInit() after setting up most of the TCB.
*
* Arguments  : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
#if OS_VERSION > 203
void OSTCBInitHook (OS_TCB *ptcb)
{
    ptcb = ptcb;                                           /* Prevent Compiler warning                 */
}
#endif


/*
*********************************************************************************************************
*                                               TICK HOOK
*
* Description: This function is called every tick.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/
void OSTimeTickHook (void)
{
}


/*
*********************************************************************************************************
*                                             IDLE TASK HOOK
*
* Description: This function is called by the idle task.  This hook has been added to allow you to do  
*              such things as STOP the CPU to conserve power.
*
* Arguments  : none
*
* Note(s)    : 1) Interrupts are enabled during this call.
*********************************************************************************************************
*/
#if OS_VERSION >= 251
void OSTaskIdleHook (void)
{
}

#endif

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
