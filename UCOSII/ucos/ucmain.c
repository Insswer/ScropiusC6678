/*
 * ucmain.c
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */


#include "includes.h"
#include "sc_includes.h"
#include "sc_int.h"
#include "sc_os.h"

#define TASK0_Prio       	14
#define TASK1_Prio      	13
#define START_TASK_Prio     10

#define STK_SIZE     1024
#define START_STK_SIZE   1024

OS_STK  TASK1_STK[STK_SIZE];
OS_STK  TASK0_STK[STK_SIZE];
OS_STK  TASK_START_STK[START_STK_SIZE];

void TaskStart(void *pdata);
void Task0(void *pdata);
void Task1(void *pdata);

int sys_tick_handler(int core, unsigned int vect);

int ucmain(int onum)
{
	sc_request_irq(sys_tick_handler, BSP_GetCoreId(), 14);
	OSInit();
	OSTaskCreate(TaskStart,	//task pointer
				(void *)0,	//parameter
				(OS_STK *)&TASK_START_STK[START_STK_SIZE-1],	//task stack top pointer
				START_TASK_Prio );	//task priority

	OSStart();
	return 0;
}

void TaskStart(void * pdata)
{
	pdata = pdata;
	sc_enable_timer();
	OSTaskCreate(Task0, (void * )0, (OS_STK *)&TASK0_STK[STK_SIZE-1], TASK0_Prio);
	OSTaskCreate(Task1, (void * )0, (OS_STK *)&TASK1_STK[STK_SIZE-1], TASK1_Prio);

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
		OSTimeDly(5);
	}
}


void Task1(void *pdata)
{
	while(1)
	{
		printf("Hello from UCOSII task1 \n");
		OSTimeDly(5);
	}
}

int sys_tick_handler(int core, unsigned int vect)
{
	printf("system tick...\n");
	OSIntEnter();
	sc_timer_tick();
    OSTimeTick();        /* Call uC/OS-II's OSTimeTick()               */
    OSIntExit();         /* Tell uC/OS-II that we are leaving the ISR  */
    return 0;
}
