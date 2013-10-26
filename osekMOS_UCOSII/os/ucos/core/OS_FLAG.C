/*
*********************************************************************************************************
uC/OS-II实时控制内核
主要的包含文件
--事件标志组管理--
文 件: OS_FLAG.C 事件标志组代码
作 者: Jean J. Labrosse
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

#if (OS_VERSION >= 251) && (OS_FLAG_EN > 0) && (OS_MAX_FLAGS > 0)
//**********************************************************************************************************
//局部函数原型
static  void     OS_FlagBlock(OS_FLAG_GRP *pgrp, OS_FLAG_NODE *pnode, OS_FLAGS flags, INT8U wait_type, INT16U timeout);
static  BOOLEAN  OS_FlagTaskRdy(OS_FLAG_NODE *pnode, OS_FLAGS flags_rdy);

//检查事件标志组
//描述: 检查事件标志组中的事件标志位是置位还是清0。
//		应用程序可以检查任意一位是置位还是清0，也可以检查所有位是置位还是清0。
//		此函数于OSFlagPend()不同在于，如果需要的事件标志没有产生，那么调用该函数的任务
//		并不挂起。
//参数: pgrp 指向事件标志组的指针。建立事件标志组时(OSFlagCreate())得到该指针。
//		flags 指定需要检查的事件标志位。为1则检查对应位；为0则忽若对应位。
//		wait_type 定义等待事件标志位的方式。可以分为以下4种：
//		OS_FLAG_WAIT_CLR_ALL 所有指定事件标志位清 (0)；
//		OS_FLAG_WAIT_CLR_ANY 任意指定事件标志位清 (0)；
//		OS_FLAG_WAIT_SET_ALL 所有指定事件标志位置 (1)；
//		OS_FLAG_WAIT_SET_ANY 任意指定事件标志位置 (1)。
//注意:如果需要在得到期望的事件标志后，恢复该事件标志，则可以在调用函数时
//		将该参数加上一个常量OS_FLAG_CONSUME。例如，如果等待事件标志组中
//		任意指定事件标志位置位，并且在任意事件标志位置位后清除该位，则把参
//		数wait_type设置为：
//		OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
//		err 指向包含错误码的变量的指针。返回的错误码可能为以下几种：
//		OS_NO_ERR 调用成功；
//		OS_ERR_EVENT_TYPE pgrp指针不是指向事件标志组的指针；
//		OS_FLAG_ERR_WAIT_TYPE wait_type参数不是指定的4种方式之一；
//		OS_FLAG_INVALID_PGRP pgrp是一个空指针；
//		OS_FLAG_ERR_NOT_RDY 指定的事件标志没有发生。
//返回: 返回事件标志组的事件标志状态
//注意/警告：1、必须先建立事件标志组，然后使用；
//			 2、如果指定的事件标志没有发生，则调用任务并不挂起。 
#if OS_FLAG_ACCEPT_EN > 0
OS_FLAGS  OSFlagAccept (OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr;
#endif
    OS_FLAGS      flags_cur;
    OS_FLAGS      flags_rdy;
    BOOLEAN       consume;


#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                        /* Validate 'pgrp'                          */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);
    }
#endif
    if (wait_type & OS_FLAG_CONSUME) {                     /* See if we need to consume the flags      */
        wait_type &= ~OS_FLAG_CONSUME;
        consume    = TRUE;
    } else {
        consume    = FALSE;
    }			 
    *err = OS_NO_ERR;                                      /* Assume NO error until proven otherwise.  */
    OS_ENTER_CRITICAL();
    switch (wait_type) {
        case OS_FLAG_WAIT_SET_ALL:                         /* See if all required flags are set        */
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we wanted      */
                 }
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
             OS_EXIT_CRITICAL();
             break;

        case OS_FLAG_WAIT_SET_ANY:
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag set                      */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we got         */
                 }
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
             OS_EXIT_CRITICAL();
             break;

#if OS_FLAG_WAIT_CLR_EN > 0
        case OS_FLAG_WAIT_CLR_ALL:                         /* See if all required flags are cleared    */
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we wanted        */
                 }
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
             OS_EXIT_CRITICAL();
             break;

        case OS_FLAG_WAIT_CLR_ANY:
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag cleared                  */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we got           */
                 }
             } else {
                 *err  = OS_FLAG_ERR_NOT_RDY;
             }
             flags_cur = pgrp->OSFlagFlags;                /* Will return the state of the group       */
             OS_EXIT_CRITICAL();
             break;
#endif

        default:
             OS_EXIT_CRITICAL();
             flags_cur = (OS_FLAGS)0;
             *err      = OS_FLAG_ERR_WAIT_TYPE;
             break;
    }
    return (flags_cur);
}
#endif

//创建一个事件标志组
//描述: 创建并初始化一个事件标志组
//参数: flags 事件标志组的事件标志初值
//		err 指向包含错误码的变量的指针。返回的错误码可能为以下几种：
//		OS_NO_ERR 成功创建事件标志组
//		OS_ERR_CREATE_ISR 从中断中调用OSFlagCreate()函数
//		OS_FLAG_GRP_DEPLETED 系统没有剩余的空闲事件标志组，需要更改OS_CFG.H中
//		的事件标志组数目配置
//返回: 如果成功创建事件标志组，则返回该事件标志组的指针；
//		若系统没有剩余的空闲事件标志组，则返回空指针。
//注意/警告: 在使用任何事件标志组功能之前，必须使用该函数创建事件标志组。 
OS_FLAG_GRP  *OSFlagCreate (OS_FLAGS flags, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                         /* Allocate storage for CPU status register        */
    OS_CPU_SR    cpu_sr;
#endif
    OS_FLAG_GRP *pgrp;


    if (OSIntNesting > 0) {                         /* See if called from ISR ...                      */
        *err = OS_ERR_CREATE_ISR;                   /* ... can't CREATE from an ISR                    */
        return ((OS_FLAG_GRP *)0);
    }
    OS_ENTER_CRITICAL();
    pgrp = OSFlagFreeList;                          /* Get next free event flag                        */
    if (pgrp != (OS_FLAG_GRP *)0) {                 /* See if we have event flag groups available      */
                                                    /* Adjust free list                                */
        OSFlagFreeList       = (OS_FLAG_GRP *)OSFlagFreeList->OSFlagWaitList;
        pgrp->OSFlagType     = OS_EVENT_TYPE_FLAG;  /* Set to event flag group type                    */
        pgrp->OSFlagFlags    = flags;               /* Set to desired initial value                    */
        pgrp->OSFlagWaitList = (void *)0;           /* Clear list of tasks waiting on flags            */
        OS_EXIT_CRITICAL();
        *err                 = OS_NO_ERR;
    } else {
        OS_EXIT_CRITICAL();
        *err                 = OS_FLAG_GRP_DEPLETED;
    }
    return (pgrp);                                  /* Return pointer to event flag group              */
}

//删除一个事件标志组
//描述: 用于删除一个事件标志组。因为多任务可能会试图继续使用已经删除了的事件标志组，故调
//		用本函数有风险，需小心。一般在删除事件标志组之前，应该首先删除与本事件有关任务。
//参数: pgrp 指向事件标志组的指针。建立事件标志组时(OSFlagCreate())得到该指针。
//		opt 确定删除一个事件的条件值:
//		opt == OS_DEL_NO_PEND 指明是仅在没有任务等待事件标志组时删除该事件标志组
//		opt == OS_DEL_ALWAYS 指明不管是否有任务等待事件标志组都删除该事件标志组
//		如果是后者，所有等待该事件标志组的任务都被置位就绪。
//		err 指向包含错误码的变量的指针。返回的错误码可能为以下几种之一：
//		OS_NO_ERR 成功删除该事件标志组；
//		OS_ERR_DEL_ISR 从中断中调用OSFlagDel()函数；
//		OS_FLAG_INVALID_PGRP pgrp是一个空指针；
//		OS_ERR_EVENT_TYPE pgrp不是指向事件标志组的指针；
//		OS_ERR_INVALID_OPT opt参数不是指定的值；
//		OS_ERR_TASK_WAITING 如果opt参数为OS_DEL_NO_PEND,那么此时有任务等待
//		事件标志组
//返回: 如果事件标志组被删除，组则返回空指针；
//		如果没有删除，则仍然返回指向该事件标志组的指针。
//		后一种情况需要检查出错代码，找出事件标志的失败的原因
//注意: 1) 需要小心，可能有其它任务正在等待该事件标志组的事件标志
//		2) 该函数有可能长时间关闭中断，其时间长短决定于标志组的任务个数	  

#if OS_FLAG_DEL_EN > 0
OS_FLAG_GRP  *OSFlagDel (OS_FLAG_GRP *pgrp, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr;
#endif
    BOOLEAN       tasks_waiting;
    OS_FLAG_NODE *pnode;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pgrp);
    }
#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                        /* Validate 'pgrp'                          */
        *err = OS_FLAG_INVALID_PGRP;
        return (pgrp);
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {          /* Validate event group type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pgrp);
    }
#endif
    OS_ENTER_CRITICAL();
    if (pgrp->OSFlagWaitList != (void *)0) {               /* See if any tasks waiting on event flags  */
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete group if no task waiting          */
             if (tasks_waiting == FALSE) {
                 pgrp->OSFlagType     = OS_EVENT_TYPE_UNUSED;
                 pgrp->OSFlagWaitList = (void *)OSFlagFreeList; /* Return group to free list           */
                 OSFlagFreeList       = pgrp;
                 OS_EXIT_CRITICAL();
                 *err                 = OS_NO_ERR;
                 return ((OS_FLAG_GRP *)0);                /* Event Flag Group has been deleted        */
             } else {
                 OS_EXIT_CRITICAL();
                 *err                 = OS_ERR_TASK_WAITING;
                 return (pgrp);
             }

        case OS_DEL_ALWAYS:                                /* Always delete the event flag group       */
             pnode = (OS_FLAG_NODE *)pgrp->OSFlagWaitList;
             while (pnode != (OS_FLAG_NODE *)0) {          /* Ready ALL tasks waiting for flags        */
                 OS_FlagTaskRdy(pnode, (OS_FLAGS)0);
                 pnode = (OS_FLAG_NODE *)pnode->OSFlagNodeNext;
             }
             pgrp->OSFlagType     = OS_EVENT_TYPE_UNUSED;
             pgrp->OSFlagWaitList = (void *)OSFlagFreeList;/* Return group to free list                */
             OSFlagFreeList       = pgrp;
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err = OS_NO_ERR;
             return ((OS_FLAG_GRP *)0);                    /* Event Flag Group has been deleted        */

        default:
             OS_EXIT_CRITICAL();
             *err = OS_ERR_INVALID_OPT;
             return (pgrp);
    }
}
#endif
 
//等待事件标志组中的事件标志(WAIT ON AN EVENT FLAG GROUP)
//描述: 任务等待事件标志组中的事件标志，可以是多个事件标志的不同组合方式。可以等待任
//		意指定事件标志位置位或清0，也可以是全部指定事件标志位置位或清0。如果任务等待
//		的事件标志位条件尚不满足，则任务会被挂起，直到指定的事件标志组合发生或指定的
//		等待时间超时。
//参数: pgrp 指向事件标志组的指针。建立事件标志组时(OSFlagCreate())得到该指针。
//		flags 指定需要检查的事件标志位。置为1，则检查对应位；置为0，则忽略对应位。
//		wait_type 定义等待事件标志位的方式。可以定为以下几种：
//		OS_FLAG_WAIT_CLR_ALL 所有指定事件标志位清0 ；
//		OS_FLAG_WAIT_SET_ALL 任意指定事件标志位置1 ；
//		OS_FLAG_WAIT_CLR_ANY 所有指定事件标志位清0 ；
//		OS_FLAG_WAIT_SET_ANY 任意指定事件标志位置1 ；
//提示: 如果需要在得到期望的事件标志后恢复该事件标志，则可以在调用该函数时，将
//		该参数加上一个常量OS_FLAG_CONSUME。例如,如果等待事件标志组中任意指定事
//		件标志位置位,并且在任意事件标志位置位后清除该位，则可以把参数wait_type
//		设置为： OS_FLAG_WAIT_SET_ANY + OS_FLAG_CONSUME
//		timeout 以时钟节拍数目的等待超时时限。如果在这一时限得不到事件，任务将恢复执行。
//				timeout的值为0，表示将无限期地等待事件。timeout的最大值是65535个时钟节
//				拍。timeout的值并不与时钟节拍同步，timeout计数器在下一个时钟节拍到来时
//				开始递减。在这里，所谓下一个时钟节拍，也就是立刻就到来了。
//		err 指向错误代码的指针，出错代码为以下值之一：
//		OS_NO_ERR 成功调用；
//		OS_ERR_PEND_ISR 从中断中调用该函数，这是规则不允许的；
//		OS_FLAG_INVALID_PGRP 'pgrp'不是指向事件标志组的指针；
//		OS_ERR_EVENT_TYPE 'pgrp'是空指针
//		OS_TIMEOUT 等待事件标志组的事件标志超时；
//		OS_FLAG_ERR_WAIT_TYPE 'wait_type'不是指定的参数之一。
//		OS_FLAG_CONSUME 定义常量OS_FLAG_CONSUME为0x80
//返回: 如果使用了OS_FLAG_CONSUME选项，则返回清理后的事件标志组事件标志状态；否则返
//		回OSFlagPend()函数运行结束后的事件标志组事件标志状态；如果发生了超时,则返回0。
//注意：必须首先创建事件标志组，再使用。

//等待事件标志组的事件标志位(事件组指针、需要检查的标志位、等待事件标志位的方式、允许等待
//的时钟节拍、出错代码的时钟节拍)	 
OS_FLAGS  OSFlagPend (OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U wait_type, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR     cpu_sr;
#endif
    OS_FLAG_NODE  node;
    OS_FLAGS      flags_cur;
    OS_FLAGS      flags_rdy;
    BOOLEAN       consume;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_PEND_ISR;                            /* ... can't PEND from an ISR               */
        return ((OS_FLAGS)0);
    }
#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                        /* Validate 'pgrp'                          */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {          /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);
    }
#endif
    if (wait_type & OS_FLAG_CONSUME) {                     /* See if we need to consume the flags      */
        wait_type &= ~OS_FLAG_CONSUME;
        consume    = TRUE;
    } else {
        consume    = FALSE;
    }
/*$PAGE*/
    OS_ENTER_CRITICAL();
    switch (wait_type) {
        case OS_FLAG_WAIT_SET_ALL:                         /* See if all required flags are set        */
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we wanted      */
                 }
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;

        case OS_FLAG_WAIT_SET_ANY:
             flags_rdy = pgrp->OSFlagFlags & flags;        /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag set                      */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags &= ~flags_rdy;      /* Clear ONLY the flags that we got         */
                 }
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;

#if OS_FLAG_WAIT_CLR_EN > 0
        case OS_FLAG_WAIT_CLR_ALL:                         /* See if all required flags are cleared    */
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy == flags) {                     /* Must match ALL the bits that we want     */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we wanted        */
                 }
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;

        case OS_FLAG_WAIT_CLR_ANY:
             flags_rdy = ~pgrp->OSFlagFlags & flags;       /* Extract only the bits we want            */
             if (flags_rdy != (OS_FLAGS)0) {               /* See if any flag cleared                  */
                 if (consume == TRUE) {                    /* See if we need to consume the flags      */
                     pgrp->OSFlagFlags |= flags_rdy;       /* Set ONLY the flags that we got           */
                 }
                 flags_cur = pgrp->OSFlagFlags;            /* Will return the state of the group       */
                 OS_EXIT_CRITICAL();                       /* Yes, condition met, return to caller     */
                 *err      = OS_NO_ERR;
                 return (flags_cur);
             } else {                                      /* Block task until events occur or timeout */
                 OS_FlagBlock(pgrp, &node, flags, wait_type, timeout);
                 OS_EXIT_CRITICAL();
             }
             break;
#endif

        default:
             OS_EXIT_CRITICAL();
             flags_cur = (OS_FLAGS)0;
             *err      = OS_FLAG_ERR_WAIT_TYPE;
             return (flags_cur);
    }
    OS_Sched();                                            /* Find next HPT ready to run               */
    OS_ENTER_CRITICAL();
    if (OSTCBCur->OSTCBStat & OS_STAT_FLAG) {              /* Have we timed-out?                       */
        OS_FlagUnlink(&node);
        OSTCBCur->OSTCBStat = OS_STAT_RDY;                 /* Yes, make task ready-to-run              */
        OS_EXIT_CRITICAL();
        flags_cur           = (OS_FLAGS)0;
        *err                = OS_TIMEOUT;                  /* Indicate that we timed-out waiting       */
    } else {
        if (consume == TRUE) {                             /* See if we need to consume the flags      */
            switch (wait_type) {
                case OS_FLAG_WAIT_SET_ALL:
                case OS_FLAG_WAIT_SET_ANY:                 /* Clear ONLY the flags we got              */
                     pgrp->OSFlagFlags &= ~OSTCBCur->OSTCBFlagsRdy;
                     break;

#if OS_FLAG_WAIT_CLR_EN > 0
                case OS_FLAG_WAIT_CLR_ALL:
                case OS_FLAG_WAIT_CLR_ANY:                 /* Set   ONLY the flags we got              */
                     pgrp->OSFlagFlags |= OSTCBCur->OSTCBFlagsRdy;
                     break;
#endif
            }
        }
        flags_cur = pgrp->OSFlagFlags;
        OS_EXIT_CRITICAL();
        *err      = OS_NO_ERR;                             /* Event(s) must have occurred              */
    }
    return (flags_cur);
}	   
//给出设定的事件标志位(POST EVENT FLAG BIT(S))
//描述: 给出设定的事件标志位。指定的事件标志位可以设定为置位或清除。若OSFlagPost()设
//		置的事件标志位正好满足某个等待使劲标志组的任务,则OSFlagPost()将该任务设为就绪。
//参数: pgrp 指向事件标志组的指针。建立事件标志组时(OSFlagCreate())得到该指针。
//		flags 指定需要检查的事件标志位。如果opt参数位OS_FLAG_SET，那么事件标志组中对
//			  应的事件标志位置位。例如，如果置位事件标志组的事件标志0、4和5,则需要把
//			  FLAGS参数设置位ox31(bit 0 是最低位)。若opt参数为OS_FLAG_CLR,那么事件标
//			  志组中对应的事件标志为被清0。
//		opt 表明是置位指定事件标志位(OS_FLAG_SET)；
//			还是清0指定事件标志位(OS_FLAG_CLR)。
//		err 指向错误代码的指针，出错代码为以下值之一：
//		OS_NO_ERR 成功调用
//		OS_FLAG_INVALID_PGRP 'pgrp'指针为空指针
//		OS_ERR_EVENT_TYPE 'pgrp'指针没有指向事件标志组结构；
//		OS_FLAG_INVALID_OPT opt不是指定的参数之一。
//返回: 事件标志组的新的事件标志状态
//警告: 1) 必须先创建事件标志组，然后使用；
//		2) 这个函数的运行时间决定于等待事件标志组的任务的数目；
//		3) 关闭中断的时间也取决于等待事件标志组的任务的数目。 
OS_FLAGS  OSFlagPost (OS_FLAG_GRP *pgrp, OS_FLAGS flags, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                          /* Allocate storage for CPU status register       */
    OS_CPU_SR     cpu_sr;
#endif
    OS_FLAG_NODE *pnode;
    BOOLEAN       sched;
    OS_FLAGS      flags_cur;
    OS_FLAGS      flags_rdy;


#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {                  /* Validate 'pgrp'                                */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) {    /* Make sure we are pointing to an event flag grp */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);
    }
#endif
/*$PAGE*/
    OS_ENTER_CRITICAL();
    switch (opt) {
        case OS_FLAG_CLR:
             pgrp->OSFlagFlags &= ~flags;            /* Clear the flags specified in the group         */
             break;

        case OS_FLAG_SET:
             pgrp->OSFlagFlags |=  flags;            /* Set   the flags specified in the group         */
             break;

        default:
             OS_EXIT_CRITICAL();                     /* INVALID option                                 */
             *err = OS_FLAG_INVALID_OPT;
             return ((OS_FLAGS)0);
    }
    sched = FALSE;                                   /* Indicate that we don't need rescheduling       */
    pnode = (OS_FLAG_NODE *)pgrp->OSFlagWaitList;
    while (pnode != (OS_FLAG_NODE *)0) {             /* Go through all tasks waiting on event flag(s)  */
        switch (pnode->OSFlagNodeWaitType) {
            case OS_FLAG_WAIT_SET_ALL:               /* See if all req. flags are set for current node */
                 flags_rdy = pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy == pnode->OSFlagNodeFlags) {
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;

            case OS_FLAG_WAIT_SET_ANY:               /* See if any flag set                            */
                 flags_rdy = pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy != (OS_FLAGS)0) {
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;

#if OS_FLAG_WAIT_CLR_EN > 0
            case OS_FLAG_WAIT_CLR_ALL:               /* See if all req. flags are set for current node */
                 flags_rdy = ~pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy == pnode->OSFlagNodeFlags) {
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;

            case OS_FLAG_WAIT_CLR_ANY:               /* See if any flag set                            */
                 flags_rdy = ~pgrp->OSFlagFlags & pnode->OSFlagNodeFlags;
                 if (flags_rdy != (OS_FLAGS)0) {
                     if (OS_FlagTaskRdy(pnode, flags_rdy) == TRUE) { /* Make task RTR, event(s) Rx'd   */
                         sched = TRUE;                               /* When done we will reschedule   */
                     }
                 }
                 break;
#endif
        }
        pnode = (OS_FLAG_NODE *)pnode->OSFlagNodeNext; /* Point to next task waiting for event flag(s) */
    }
    OS_EXIT_CRITICAL();
    if (sched == TRUE) {
        OS_Sched();
    }
    OS_ENTER_CRITICAL();
    flags_cur = pgrp->OSFlagFlags;
    OS_EXIT_CRITICAL();
    *err      = OS_NO_ERR;
    return (flags_cur);
}			  
//查询事件标志组的当前事件标志状态(QUERY EVENT FLAG)
//描述: 查询事件标志组的当前事件标志状态。在现在的版本中，该函数还不能返回等待该事件
//		标志组的任务列表
//参数: pgrp 指向事件标志组的指针。建立事件标志组时(OSFlagCreate())得到该指针。
//		err 指向错误代码的指针，出错代码为以下值之一:
//		OS_NO_ERR 成功调用
//		OS_FLAG_INVALID_PGRP 'pgrp'指针为空指针
//		OS_ERR_EVENT_TYPE 'pgrp'指针没有指向事件标志组结构。
//返回: 事件标志组的新的事件标志状态
//警告: 1) 必须先创建事件标志组，然后使用；
//		2) 可以从中断中调用该函数。				  

#if OS_FLAG_QUERY_EN > 0
OS_FLAGS  OSFlagQuery (OS_FLAG_GRP *pgrp, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                       /* Allocate storage for CPU status register          */
    OS_CPU_SR  cpu_sr;
#endif
    OS_FLAGS   flags;


#if OS_ARG_CHK_EN > 0
    if (pgrp == (OS_FLAG_GRP *)0) {               /* Validate 'pgrp'                                   */
        *err = OS_FLAG_INVALID_PGRP;
        return ((OS_FLAGS)0);
    }
    if (pgrp->OSFlagType != OS_EVENT_TYPE_FLAG) { /* Validate event block type                         */
        *err = OS_ERR_EVENT_TYPE;
        return ((OS_FLAGS)0);
    }
#endif
    OS_ENTER_CRITICAL();
    flags = pgrp->OSFlagFlags;
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
    return (flags);                               /* Return the current value of the event flags       */
}
#endif

//SUSPEND TASK UNTIL EVENT FLAG(s) RECEIVED OR TIMEOUT OCCURS
//描述: 这个函数是uC/OS-II的内部函数,如果期望的事件标志位没有置位，任务将被挂起，直到事件
//		标志位置位或者等待超时。
//		此程序完成操作并将调用OSFlagBlock()的任务添加到事件标志组的等待列表中
//参数: pgrp 指向事件标志组的指针。建立事件标志组时(OSFlagCreate())得到该指针。
//		pnode is a pointer to a structure which contains data about the task waiting for
//		event flag bit(s) to be set.
//		flags 指定需要检查的事件标志位。如果opt参数位OS_FLAG_SET，那么事件标志组中对
//			  应的事件标志位置位。例如，如果置位事件标志组的事件标志0、4和5,则需要把
// 			  FLAGS参数设置位ox31(bit 0 是最低位)。若opt参数为OS_FLAG_CLR,那么事件标
//			  志组中对应的事件标志为被清0。
//		wait_type 定义等待事件标志位的方式。可以分为以下4种：
//		OS_FLAG_WAIT_CLR_ALL 所有指定事件标志位清 (0)；
//		OS_FLAG_WAIT_CLR_ANY 任意指定事件标志位清 (0)；
//		OS_FLAG_WAIT_SET_ALL 所有指定事件标志位置 (1)；
//		OS_FLAG_WAIT_SET_ANY 任意指定事件标志位置 (1)。
//		timeout 以时钟节拍数目的等待超时时限。如果在这一时限得不到事件，任务将恢复执行。
//			    timeout的值为0，表示将无限期地等待事件。timeout的最大值是65535个时钟节
//				拍。timeout的值并不与时钟节拍同步，timeout计数器在下一个时钟节拍到来时
//				开始递减。在这里，所谓下一个时钟节拍，也就是立刻就到来了。
//返回: 无
//程序在: OS_FLAG.C中OSFlagPend()
//注意: 这个程序是uC/OS-II内部的，请不要调用它.
static  void  OS_FlagBlock (OS_FLAG_GRP *pgrp, OS_FLAG_NODE *pnode, OS_FLAGS flags, INT8U wait_type, INT16U timeout)
{
    OS_FLAG_NODE  *pnode_next;


    OSTCBCur->OSTCBStat      |= OS_STAT_FLAG;
    OSTCBCur->OSTCBDly        = timeout;              /* Store timeout in task's TCB                   */
#if OS_TASK_DEL_EN > 0
    OSTCBCur->OSTCBFlagNode   = pnode;                /* TCB to link to node                           */
#endif
    pnode->OSFlagNodeFlags    = flags;                /* Save the flags that we need to wait for       */
    pnode->OSFlagNodeWaitType = wait_type;            /* Save the type of wait we are doing            */
    pnode->OSFlagNodeTCB      = (void *)OSTCBCur;     /* Link to task's TCB                            */
    pnode->OSFlagNodeNext     = pgrp->OSFlagWaitList; /* Add node at beginning of event flag wait list */
    pnode->OSFlagNodePrev     = (void *)0;
    pnode->OSFlagNodeFlagGrp  = (void *)pgrp;         /* Link to Event Flag Group                      */
    pnode_next                = (OS_FLAG_NODE *)pgrp->OSFlagWaitList;
    if (pnode_next != (void *)0) {                    /* Is this the first NODE to insert?             */
        pnode_next->OSFlagNodePrev = pnode;           /* No, link in doubly linked list                */
    }
    pgrp->OSFlagWaitList = (void *)pnode;
                                                      /* Suspend current task until flag(s) received   */
    if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0) {
        OSRdyGrp &= ~OSTCBCur->OSTCBBitY;
    }
}

/*$PAGE*/
/*
*********************************************************************************************************
*                                    INITIALIZE THE EVENT FLAG MODULE
*
* Description: This function is called by uC/OS-II to initialize the event flag module.  Your application
*              MUST NOT call this function.  In other words, this function is internal to uC/OS-II.
*
* Arguments  : none
*
* Returns    : none
*
* WARNING    : You MUST NOT call this function from your code.  This is an INTERNAL function to uC/OS-II.
*********************************************************************************************************
*/

void  OS_FlagInit (void)
{
#if OS_MAX_FLAGS == 1
    OSFlagFreeList                 = (OS_FLAG_GRP *)&OSFlagTbl[0];  /* Only ONE event flag group!      */
    OSFlagFreeList->OSFlagType     = OS_EVENT_TYPE_UNUSED;
    OSFlagFreeList->OSFlagWaitList = (void *)0;
#endif

#if OS_MAX_FLAGS >= 2
    INT8U        i;
    OS_FLAG_GRP *pgrp1;
    OS_FLAG_GRP *pgrp2;


    pgrp1 = &OSFlagTbl[0];
    pgrp2 = &OSFlagTbl[1];
    for (i = 0; i < (OS_MAX_FLAGS - 1); i++) {                      /* Init. list of free EVENT FLAGS  */
        pgrp1->OSFlagType     = OS_EVENT_TYPE_UNUSED;
        pgrp1->OSFlagWaitList = (void *)pgrp2;
        pgrp1++;
        pgrp2++;
    }
    pgrp1->OSFlagWaitList = (void *)0;
    OSFlagFreeList        = (OS_FLAG_GRP *)&OSFlagTbl[0];
#endif
}

//使等待事件标志的任务进入就绪态 MAKE TASK READY-TO-RUN, EVENT(s) OCCURRED
//描述: 这个函数是 uC/OS-II内部函数.
//		该处理在uC/OS-II中是一个标准过程，这个惟一的不同在于，事件标志组中当一个任务等待的
//		事件标志发生后，为该任务建立的OS_FLAG_NODE数据结构就没有用处了；所以这里把这个任务
//		的OS_FLAG_NODE数据结构从等待任务链表中删除掉，同时还会把这个OS_FLAG_NODE数据结构指
//		针，从该任务的事件控制块中删除掉。
//参数: pnode 标志节点is a pointer to a structure which contains data about the task
//		waiting forevent flag bit(s) to be set.
//		flags_rdy contains the bit pattern of the event flags that cause the task to become
//		ready-to-run.			   
//返回: 无
//访问: 本函数在OS_FLAG.C的OSFlagsPost() 中
//注意: 1) 即使任务等待的事件标志都发生了，任务已经从事件标志组的等待任务链表中被删除了，但
//		   是这个任务可能由于其它的原因而不能进入就绪态;
//		2) 这个函数是uC/OS-II内部函数，你应用的时候不要调用它.			  
static  BOOLEAN  OS_FlagTaskRdy (OS_FLAG_NODE *pnode, OS_FLAGS flags_rdy)
{
    OS_TCB   *ptcb;
    BOOLEAN   sched;


    ptcb                = (OS_TCB *)pnode->OSFlagNodeTCB;  /* Point to TCB of waiting task             */
    ptcb->OSTCBDly      = 0;
    ptcb->OSTCBFlagsRdy = flags_rdy;
    ptcb->OSTCBStat    &= ~OS_STAT_FLAG;
    if (ptcb->OSTCBStat == OS_STAT_RDY) {                  /* Put task into ready list                 */
        OSRdyGrp               |= ptcb->OSTCBBitY;
        OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
        sched                   = TRUE;
    } else {
        sched                   = FALSE;
    }
    OS_FlagUnlink(pnode);
    return (sched);
}			   
//从任务等待链表中删除 UNLINK EVENT FLAG NODE FROM WAITING LIST
//描述: 这个函数是uC/OS-II内部函数，从任务等待链表中删除。
//参数: pnode is a pointer to a structure which contains data about the task waiting for
//		event flag bit(s) to be set.
//返回: 无
//访问: OS_FlagTaskRdy() OS_FLAG.C
//		OSFlagPend() OS_FLAG.C
// 		OSTaskDel() OS_TASK.C
//注意: 1) This function assumes that interrupts are disabled.
//		2) 这个函数是uC/OS-II内部函数，你应用的时候不要调用它.  
//在这种情况下，调用OS_FlagUnlink()函数，把这个OS_FLAG_NODE从事件标志组的等待任务链表中
//删除，并且返回一个出错代码，说明发生了等待超时。这段代码只是简单的将一个OS_FLAG_NODE
//从一个双向链表中删除
void  OS_FlagUnlink (OS_FLAG_NODE *pnode)
{
#if OS_TASK_DEL_EN > 0
    OS_TCB       *ptcb;
#endif
    OS_FLAG_GRP  *pgrp;
    OS_FLAG_NODE *pnode_prev;
    OS_FLAG_NODE *pnode_next;


    pnode_prev = (OS_FLAG_NODE *)pnode->OSFlagNodePrev;
    pnode_next = (OS_FLAG_NODE *)pnode->OSFlagNodeNext;
    if (pnode_prev == (OS_FLAG_NODE *)0) {                      /* Is it first node in wait list?      */
        pgrp                 = (OS_FLAG_GRP *)pnode->OSFlagNodeFlagGrp;
        pgrp->OSFlagWaitList = (void *)pnode_next;              /*      Update list for new 1st node   */
        if (pnode_next != (OS_FLAG_NODE *)0) {
            pnode_next->OSFlagNodePrev = (OS_FLAG_NODE *)0;     /*      Link new 1st node PREV to NULL */
        }
    } else {                                                    /* No,  A node somewhere in the list   */
        pnode_prev->OSFlagNodeNext = pnode_next;                /*      Link around the node to unlink */
        if (pnode_next != (OS_FLAG_NODE *)0) {                  /*      Was this the LAST node?        */
            pnode_next->OSFlagNodePrev = pnode_prev;            /*      No, Link around current node   */
        }
    }
#if OS_TASK_DEL_EN > 0
    ptcb                = (OS_TCB *)pnode->OSFlagNodeTCB;
    ptcb->OSTCBFlagNode = (OS_FLAG_NODE *)0;
#endif
}
#endif
