/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#include "stdTypes.h"
#include "osapi.h"
#include "c66xx_init.h"
#include "c66xx_cache.h"
#include "cfg.h"
#include "osekExTest.h"
#include "c66xx_spinlock.h"
/*************************** 前向声明部分 ******************************/
#ifdef DEV_DEBUG
#endif
/**************************** 定义部分 *********************************/

// 局部变量
static OSBYTE osekExecution_wait1_flag = 2;
static OSBYTE osekExecution_wait2_flag = 2;
static OSBYTE osekExecution_wait_down_flag = 2;

/**************************** 实现部分 *********************************/

/*
 * @brief
 *    StartOS：启动操作系统。
 * @param
 * @param[in]	mode：应用模式。
 * @param[out]  无。
 * @returns:    无。
 */

void StartOS( AppModeType mode )
{



/*

	//初始化资源管理
#if defined(CONFIG_OSEK_RESOURCE)
//    osekResource_Initialize();
#endif


	// 准备进入多任务运行

	// 将系统的第一个任务安排为IDLE任务
#if defined(CONFIG_OSEK_SYSTEM_EP)
    kernelPtr->osekTask_RunningTask = OSEK_TASK_GetReadyTask(kernelPtr,0);
#else
	kernelPtr->osekTask_RunningTask = OSEK_TASK_GetReadyTask(kernelPtr,OSEK_TASK_ID2PRI(OSEK_TASK_IDLE_ID));
#endif

    // 初始化通信管理
#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)
#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0
	InitCOM();
#endif
#endif

*/
	//获取核心ID
	CoreIDType core = GetCoreID();
	//获取核心数据指针
	T_OSEK_KERNEL_OsKernelRef kernelPtr = &osekKernel_osKernel[core];
	// 关闭中断
	OSEK_TARGET_DisableOSIntWithoutSave() ;

	printf("start os core %d\n",core);
	/* init Kernel data */
	osekKernel_Initialize();
	/* init spinlock */
	osekSpinLock_Init();

	//初始化应用模式
	kernelPtr->osekExecution_CurrentAppMode = mode;

	//初始化任务管理
	osekTask_Initialize();
	/* 初始化中断 */
	osekInterrupt_Initialize();
	//Counter 初始化管理
	osekCounter_Initialize();
	/*初始化Alarm */
	osekAlarm_Initialize();

	// 将系统的第一个任务安排为IDLE任务
#if defined(CONFIG_OSEK_SYSTEM_EP)
    kernelPtr->osekTask_RunningTask = OSEK_TASK_GetReadyTask(kernelPtr,0);
#else
    //IDELE_ID有问题
	kernelPtr->osekTask_RunningTask = OSEK_TASK_GetReadyTask(kernelPtr,OSEK_TASK_ID2PRI(GET_IDLE_TASK_ID(kernelPtr)));
#endif

	//进行第一阶段同步
	//加锁保护
	BSP_GetSpinlock(SPIN_LOCK_OS_SETUP);
	osekExecution_wait1_flag--;

	BSP_ReleaseSpinlock(SPIN_LOCK_OS_SETUP);
	//去锁

	while(osekExecution_wait1_flag!=0)
	{
		;
	}

	//同步完成

	// 调用系统启动HOOK函数,每个核心可能不同,因此考虑不同核心调用不同Hook的情况
	OSEK_HOOK_StartupHook();

	//进行第二阶段同步
	//加锁保护
	BSP_GetSpinlock(SPIN_LOCK_OS_SETUP);
	osekExecution_wait2_flag--;

	BSP_ReleaseSpinlock(SPIN_LOCK_OS_SETUP);

	while(osekExecution_wait2_flag!=0)
	{
		;
	}
	//第二阶段完成

	// 进行系统重调度
	osekTask_Dispatch();

	// 打开中断
	OSEK_TARGET_EnableOSIntWithoutSave();

	// IDLE任务主循环
	while(1)
	{

		OSEK_HOOK_IdleHook();

	}

}


/*
 * @brief
 *    ShutdownOS：关闭操作系统。
 * @param
 * @param[in]	error：错误信息。
 * @param[out]  无。
 * @returns:    无。
 */

void ShutdownOS( StatusType error )
{
	// 关闭中断
	OSEK_TARGET_DisableOSIntWithoutSave() ;
	osekExecution_wait_down_flag--;
	BSP_L1DWriteback();
	//发送核间中断给下一个核心
	//等待同步
		while(osekExecution_wait_down_flag!=0){
				//nop;等待同步
		}

		//同步完成
	// 调用系统关闭HOOK函数
	OSEK_HOOK_ShutdownHook(error);

	// 关闭通信模块
#if defined(CONFIG_OSEK_COM_LOCALMESSAGESONLY)
#if CONFIG_OSEK_COM_NUMBER_OF_MESSAGES > 0
	CloseCOM();
#endif
#endif

	// 进入无限循环
	while(1)
	{
	}

}

/**
 * @brief
 *    StartCore：
 * @param[in]	CoreID：核心id编号
 * @param[out]	Status：返回状态值
 * @returns     无。
 */

void StartCore(CoreIDType CoreID,StatusType * Status){
	//激活其他核心
	BSP_L1DWriteback();
	osekKernel_osKernel[CoreID].osState = CORE_STATUS_RUN;
	BSP_L1DWriteback();
	printf("core %d status %d\n",CoreID,osekKernel_osKernel[CoreID].osState);
}

/**
 * @brief
 *    SpinCode：局部函数，自悬代码，用于同步核心待 ,激活方式为单链式激活，m->0->1->2->3->4->5->6
 * @param[in]	AppModeType：应用模式
 * @returns     无。
 */
static void SpinCode(AppModeType mode){

	while(1){
//		BSP_L1DInvalid();
		volatile CoreIDType CoreID = GetCoreID();
		printf("core %d status %d\n",CoreID,osekKernel_osKernel[CoreID].osState);
		if(osekKernel_osKernel[CoreID].osState==CORE_STATUS_RUN){
			break;
		}
	}

	/*核心开始启动,执行硬件初始化代码*/
	BSP_ArchInit();

	/*执行激活其他核心操作*/
#if 0
	if(CoreID++ < CONFIG_OSEK_KERNEL_CORE_NUMS){
		StartCore(CoreID,&status);
	}
#endif
	//运行os
	StartOS(mode);
}


#define EX_TEST
int ucmain(void);


/**
 * @brief
 *    OSMain：进入操作系统的主函数，与用户中定义的main相区分,用户的应用需要调用
 *    		操作系统的OSMain来启动操作系统，该函数用来进行主核心启动，并且下发启动命令
 * @param[in]	mode：操作系统启动模式。
 * @returns     无。
 */
void OSMain(AppModeType mode){
#if 1
	/* 关闭2级缓存 */
	BSP_DisableL2Cache();
	volatile CoreIDType core=GetCoreID();
	//主核启动
	if(core==UC_CORE){
		StatusType status;
		osekKernel_osKernel[OS_CORE_ID_MASTER].osState = CORE_STATUS_RUN;
//		BSP_L1DWriteback();
		BSP_ArchInit();
		/* 激活其他核心 */
		if(CONFIG_OSEK_KERNEL_CORE_NUMS > 1){
			/* 多核模式下 */
			printf("start core 1...\n");
			StartCore(OS_CORE_ID_0,&status);
		}
		StartOS(mode);
	}else if (core == OS_CORE_ID_MASTER){
		ucmain();
	}else {
		/* 运行待激活函数 */
		SpinCode(mode);
	}
#else
	ex_main();
#endif
}

void ShutdownAllCores(){

}

