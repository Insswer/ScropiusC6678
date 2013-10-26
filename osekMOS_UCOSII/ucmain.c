
#include "includes.h"	 

#define STK_SIZE     512
#define START_STK_SIZE   512


#define TASK0_Prio       	14
#define TASK1_Prio      	13
#define START_TASK_Prio     10

//�����ջ
OS_STK  TASK1_STK[STK_SIZE];
OS_STK  TASK0_STK[STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];
//��������
void TaskStart(void *pdata);
void Task0(void *pdata);
void Task1(void *pdata);
void SysArch_Configuration(void);

int ucmain(void)
{			 	   
	SysArch_Configuration();
	OSInit();
	OSTaskCreate( TaskStart,	//task pointer
					(void *)0,	//parameter
					(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
					START_TASK_Prio );	//task priority
	
	OSStart();
	return 0;	   
}

static inline void enableTimer(void)
{
	BSP_InitC6xTimer(BSP_GetCoreId());
}

void TaskStart(void * pdata)
{
	pdata = pdata;
	OSTaskCreate(Task0, (void * )0, (OS_STK *)&TASK0_STK[STK_SIZE-1], TASK0_Prio);
	OSTaskCreate(Task1, (void * )0, (OS_STK *)&TASK1_STK[STK_SIZE-1], TASK1_Prio);
	enableTimer();
	 while(1)
	 {
		 printf("\nEnter Main Task\n");
		 OSTimeDly(5);
	 }

}


void Task0(void *pdata)
{
	while(1)
	{
		printf("Hello from UCOSII task0 \n");
		OSTimeDlyHMSM(0,0,0,500);	
	}
}


void Task1(void *pdata)
{
	while(1)
	{
		printf("Hello from UCOSII task1 \n");
		OSTimeDlyHMSM(0,0,0,200);	
	}
}	  

void SysTick_Handler(void)
{
	struct C6xTimerDev *pTimer;
	printf("system tick...\n");
	OSIntEnter();
    pTimer = BSP_GetC6xTimer(BSP_GetCoreId());
    pTimer->ack_timer_int(pTimer);
    OSTimeTick();        /* Call uC/OS-II's OSTimeTick()               */
    OSIntExit();         /* Tell uC/OS-II that we are leaving the ISR  */
}



void SysArch_Configuration(void)
{
	struct cic_device *pCic_dev0;
	struct cic_device *pCic_dev1;
	int core_num;

	core_num = BSP_GetCoreId();

	/* BP仅初始化CIC */
	if (core_num == 0)
		BSP_InitCic();

	BSP_InitCurCorepac();
	printf("Init Ctr...\n");
	BSP_InitializeIntCtr();
	printf("Init Vector...\n");
	BSP_InitializeVector();
//	BSP_DisableL1Dcache();

	if (core_num == 0) {
		pCic_dev0 = BSP_GetCicChip(0);
		pCic_dev0->cic_global_enable_interrupt(pCic_dev0);
		pCic_dev1 = BSP_GetCicChip(1);
		pCic_dev1->cic_global_enable_interrupt(pCic_dev1);
	}

	printf("Init IPC...\n");
	BSP_InitIPCModule(13);
	BSP_ClearAllSpinlock();
	printf("Init Timer...\n");
	if (BSP_GetExecutionState())
		printf("device runs in supervisor mode.\n");
	else
		printf("device runs in user mode.\n");
}




















