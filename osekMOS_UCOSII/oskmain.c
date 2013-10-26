/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/


/**
 * @file    main.c
 * @brief
 *  <li>功能：用户应用程序</li>
 * @date
 */
/**************************** 引用部分 *****************************************/
#include <stdio.h>
#include "osprop.h"
#include "osapi.h"
#include "cfg.h"
#include "c66xx_init.h"
#include "c66xx_spinlock.h"
#include "c66xx_ipc.h"
#include "osekMRpcFunc.h"

/**************************** 定义部分 *****************************************/
DeclareTask(StartTask);
DeclareTask(JobStatusTask);
DeclareTask(TenmsTask);
DeclareTask(J1939Task);
DeclareTask(OS_5ms_task);
DeclareTask(FuelMassCalTask);
DeclareTask(IntActivateTask);

/**************************** 任务实现部分 *****************************************/
int testVar1 = 0;
int testVar2 = 0;
void sleep(int s)
{
	int i,j;
	for(i=0;i<s;i++)
	{
		for(j=0;j<10000;j++)
			;
	}
}

int count=0;
/* TASK 1 run on core 0 */
TASK(StartTask)
{
    StatusType          status;
    TaskStateType state;

    GetTaskState(Task1,&state);
    printf("Task1 state = %d\n",state);

    GetTaskState(Task2,&state);
    printf("Task2 state = %d\n",state);

    GetTaskState(Task4,&state);
    printf("Task4 state = %d\n",state);

    GetTaskState(Task5,&state);
    printf("Task5 state = %d\n",state);

    /****end user code******/
    status = TerminateTask();
}

/*TASK 2 run on core0 */
TASK(IntActivateTask)
{
    StatusType          status;
    TaskType	taskID;
    CoreIDType 	core=GetCoreID();
    /****start user code******/


    printf("C0_Task 2 End\n");


    /****end user code******/
    status = TerminateTask();
}
/*TASK 3 run on core0 */
TASK(TenmsTask)
{
    StatusType          status;
    TryToGetSpinlockType ret;
	//获取核心ID
	CoreIDType core=GetCoreID();

#if 1

	printf("C0_Task3...\n");
	sleep(10000);
	ActivateTask(Task5);


#else

	while(1)
	{

		//func_test(msg,2,1);
		//ActivateTask(Task1);
		status = TryToGetSpinlock(1,&ret);
		printf("status = %d,ret = %d\n",status,ret);
		if(ret == TRYTOGETSPINLOCK_SUCCESS)
			break;
	}
	/* set event alarm */
	status = SetAbsAlarm(Alarm1,12,10);
	printf("Task 3:Set Alarm status %d...\n",status);
	/* activate task alarm */
	status = SetAbsAlarm(Alarm2,10,10);
	printf("Task 3:Set Alarm status %d...\n",status);

#endif

	printf("C0_Task3 Done\n");
    status = TerminateTask();
}


/* TASK 4 run on core1 */
TASK(OS_5ms_task)
{
    StatusType          status;
    /****start user code******/
    //获取核心ID
    CoreIDType core=GetCoreID();
# if 0
    if(core == 1 )
    {
	   //start Alarm
    	printf("***C1_Task1 Running***\n");
    	status = SetAbsAlarm(Alarm3,8,10);
    	status = SetAbsAlarm(Alarm4,15,10);
    }
#else

    printf("*****C1_Task1 Running*****\n");
    status = GetSpinlock(1);
    printf("status = %d\n",status);

#endif
    /****end user code******/
    sleep(60000);
	status = ReleaseSpinlock(1);
	printf("Release status = %d\n",status);

    status = TerminateTask();
}

/* TASK 5 run on core1*/
TASK(EventTask)
{
    StatusType          status;

    /****start user code******/
#if 1
    ActivateTask(Task1);

    printf("***C1_Task2 Running***\n");
    while(1)
    	;

#else
	status = SetAbsAlarm(Alarm3,8,10);
    while(1)
    {
        sleep(3000);
        printf("C1_Task2\n");
        //ActivateTask(Task4);
    }



#endif
    /****end user code******/
    status = TerminateTask();
}

/**
 * @brief
 *          工况转换任务,周期50ms。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
TASK(JobStatusTask)
{
    StatusType          status;
    TaskType taskId;
    TaskStateType state;

    /****start user code******/
    printf("JobStatusTask\n");

    /****end user code******/

    status = TerminateTask();
}



/**
 * @brief
 *          外部通信任务，10ms任务。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
TASK(J1939Task)
{
    StatusType          status;

    /****start user code******/

    printf("J1939Task\n");

    /****end user code******/

    status = TerminateTask();
}



/**
 * @brief
 *          FuelMassCalTask。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
TASK(FuelMassCalTask)
{
    StatusType          status;
    /****start user code******/
    printf("FuelMassCalTask \n");

    /****end user code******/
    status = TerminateTask();
}


/**************************** 实现部分 *****************************************/
/**
 * @brief
 *  程序主入口函数，启动内核。
 *
 * @param   无。
 *
 * @return   无。
 *
 */

void main(){


	//ddr_test();
	OSMain(AppMode);
}

/**
 * @brief
 *          定时器中断函数。
 *
 * @param   无。
 *
 * @return   无。
 *
 */
ISR(TimingInt)
{
	//获取核心ID
	CoreIDType core=GetCoreID();
	//获取核心数据指针
	//T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];

	CounterTrigger(SysCounter);

}
