/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/
/*
 * osekMSpinlock.h
 *
 *  Created on: 2013-6-26
 *      Author: JackayChang
 */

#ifndef OSEKMSPINLOCK_H_
#define OSEKMSPINLOCK_H_

#include "stdTypes.h"



struct T_OSEK_SPINLOCK_Lock_struct
{
	/* 自旋锁对应的底层硬件编号,由系统初始化时指定 */
	OSWORD	semID;
	/* 上层应用使用的软件自旋锁ID */
	SpinlockIdType lockId;
};

extern StatusType  GetSpinlock( SpinlockIdType SpinlockId );
extern StatusType  ReleaseSpinlock( SpinlockIdType SpinlockId );
extern StatusType  TryToGetSpinlock( SpinlockIdType SpinlockId,TryToGetSpinlockType* Success );

/*自旋锁初始化,应该只被调用一次,且由主核来调用*/
void osekSpinLock_Init(void);



#endif /* OSEKMSPINLOCK_H_ */
