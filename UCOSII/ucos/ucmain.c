/*
 * ucmain.c
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */


#include "includes.h"


void SysTick_Handler(void)
{
#if 0
	struct C6xTimerDev *pTimer;
	printf("system tick...\n");
	OSIntEnter();
    pTimer = BSP_GetC6xTimer(BSP_GetCoreId());
    pTimer->ack_timer_int(pTimer);
    OSTimeTick();        /* Call uC/OS-II's OSTimeTick()               */
    OSIntExit();         /* Tell uC/OS-II that we are leaving the ISR  */
#endif
}
