/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/
/*
 * osekMSpinlock.c
 *
 *  Created on: 2013-6-26
 *      Author: JackayChang
 */

#include "osapi.h"
#include "cfg.h"
#include "c66xx_spinlock.h"

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  GetSpinlock
 *  Description:
 *
 *  获取硬件自旋锁之前允许调度,获取硬件自旋锁之后关闭本地核心的任务调度
 * -------------------------------------------
 */
StatusType GetSpinlock(SpinlockIdType SpinlockId)
{
	/* 当前核心ID */
	CoreIDType core = GetCoreID();
	T_OSEK_TASK_ControlBlock *curTaskTCB = (osekKernel_osKernel[core].osekTask_RunningTask)->taskControlBlock;
	//声明保存中断变量
	OSEK_TARGET_OSIntSave( osIntSave );


	/* 检查自旋锁ID是否有效 */
	if(SpinlockId > CONFIG_OSEK_SPINLOCK_NUMS)
		return E_OS_ID;

	/* 检查任务是否已经占用自旋锁 :目前不支持自旋锁的嵌套,因此在获取自旋锁之后不能再去获取其他自旋锁*/
	if(curTaskTCB->spinLockList != NULL)
	{
		/* ***************TBD******************* */
		return E_OS_STATE;
	}
	/* 在获取自旋锁期间,任务循环等待,直到自旋锁可用.在等待期间应关闭中断 */
	OSEK_TARGET_DisableOSInt(osIntSave);

	/* 调用底层获取自旋锁函数,获得对应的硬件自旋锁 */
	BSP_GetSpinlock(osekSpin_LockTable[SpinlockId].semID);
	/* 底层返回后,表示已经获得底层的硬件自旋锁,保存 占用自旋锁的任务ID及核心*/
	/* ***************TBD******************* */
	/* 保存自旋锁到当前任务的控制结构中 */
	curTaskTCB->spinLockList = &osekSpin_LockTable[SpinlockId];
	/* 任务获取自旋锁之后,在此期间应该是不能被抢占的,因此关闭系统调度 */
	osekKernel_osKernel[core].osekTask_SchedulerLockLevel++;

	/* 打开中断 */
	OSEK_TARGET_EnableOSInt(osIntSave);
	/* 返回E_OK */
	return E_OK;

}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  ReleaseSpinlock
 *  Description:
 * -------------------------------------------
 */
StatusType ReleaseSpinlock(SpinlockIdType SpinlockId)
{
	CoreIDType core = GetCoreID();
	T_OSEK_TASK_ControlBlock *curTaskTCB = (osekKernel_osKernel[core].osekTask_RunningTask)->taskControlBlock;
	OSEK_TARGET_OSIntSave( osIntSave );

	/* 检查自旋锁ID是否有效 */
	if(SpinlockId > CONFIG_OSEK_SPINLOCK_NUMS)
		return E_OS_ID;

	/* 判断当前任务是否占用自旋锁,并且等于要释放的自旋锁ID */
	if(curTaskTCB->spinLockList == NULL)
		return E_OS_STATE;

	if(curTaskTCB->spinLockList->lockId != SpinlockId)
	{
		return E_OS_NOFUNC;
	}

	/* 关闭本地核心中断 */
	OSEK_TARGET_DisableOSInt(osIntSave);
	/* 释放任务占用的自旋锁 */
	curTaskTCB->spinLockList = NULL;

	/* 释放底层硬件自旋锁 */
	BSP_ReleaseSpinlock(osekSpin_LockTable[SpinlockId].semID);

	/* 释放调度器锁 */
	osekKernel_osKernel[core].osekTask_SchedulerLockLevel--;
	/* 如果当前任务允许抢占,则进行一次重调度检查 */
	if((curTaskTCB->configTable.property & OSEK_TASK_NONPREEMPT)== 0)
		osekTask_Dispatch();

	/* 打开中断 */
	OSEK_TARGET_EnableOSInt(osIntSave);
	return E_OK;

}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  TryToGetSpinlock
 *  Description:
 * -------------------------------------------
 */
StatusType TryToGetSpinlock(SpinlockIdType SpinlockId,TryToGetSpinlockType* Success)
{
	/* 返回结果 */
	OSWORD result;
	/* 当前核心ID */
	CoreIDType core = GetCoreID();
	T_OSEK_TASK_ControlBlock *curTaskTCB = (osekKernel_osKernel[core].osekTask_RunningTask)->taskControlBlock;
	//声明保存中断变量
	OSEK_TARGET_OSIntSave( osIntSave );

	/* 检查自旋锁ID是否有效 */
	if (SpinlockId > CONFIG_OSEK_SPINLOCK_NUMS)
		return E_OS_ID;

	/* 检查任务是否已经占用自旋锁 :目前不支持自旋锁的嵌套,因此在获取自旋锁之后不能再去获取其他自旋锁*/
	if(curTaskTCB->spinLockList != NULL)
	{
		/* ***************TBD******************* */
		return E_OS_STATE;
	}

	/* 在获取自旋锁期间,任务循环等待,直到自旋锁可用.在等待期间应关闭中断 */
	OSEK_TARGET_DisableOSInt(osIntSave);

	/* 调用底层获取自旋锁函数,获得对应的硬件自旋锁 */
	result = BSP_TryToGeSpinlock(osekSpin_LockTable[SpinlockId].semID);

	if(result == 0)
	{

		/* ***************TBD******************* */
		/* 保存自旋锁到当前任务的控制结构中 */
		curTaskTCB->spinLockList = &osekSpin_LockTable[SpinlockId];

		/* 任务获取自旋锁之后,在此期间应该是不能被抢占的,因此关闭系统调度 */
		osekKernel_osKernel[core].osekTask_SchedulerLockLevel++;
		*Success = TRYTOGETSPINLOCK_SUCCESS;
	}
	else
	{
		*Success = TRYTOGETSPINLOCK_NOSUCCESS;
	}


	/* 打开中断 */
	OSEK_TARGET_EnableOSInt(osIntSave);
	/* 返回E_OK */
	return E_OK;

}

/*
 * ---  FUNCTION  ----------------------------
 *         Name:  osekSpinLock_Init
 *  Description:  init SpinLock Module. this function should be called only once by the master core!
 * -------------------------------------------
 */
void osekSpinLock_Init(void)
{
	OSWORD i;
	OSWORD semHwId = CONFIG_OSEK_SPINLOCK_RESERVERD;
	/* get core id */
	CoreIDType core = GetCoreID();
	/* check if it is master core */
	if(core != OS_CORE_ID_MASTER)
		return;

	/* Do spinlock init */
	for(i = 0;i < CONFIG_OSEK_SPINLOCK_NUMS;i++)
	{
		/* set spinlock var */
		osekSpin_LockTable[i].lockId = i;
		osekSpin_LockTable[i].semID = semHwId;

		semHwId++;
	}
}
