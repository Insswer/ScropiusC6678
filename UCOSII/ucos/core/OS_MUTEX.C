/*
*********************************************************************************************************
// uC/OS-II实时控制内核
//互斥型信号量项管理
//文 件: OS_MUTEX.C 包含主要互斥型信号量代码
//作 者: Jean J. Labrosse
*********************************************************************************************************
*/

#ifndef  OS_MASTER_FILE
#include "includes.h"
#endif

// 局部变量 (LOCAL CONSTANTS)
#define  OS_MUTEX_KEEP_LOWER_8   0x00FF
#define  OS_MUTEX_KEEP_UPPER_8   0xFF00

#define  OS_MUTEX_AVAILABLE      0x00FF


#if OS_MUTEX_EN > 0	  //条件编译：当OS_SEM_EN允许产生信号量程序代码
//无等待地获取互斥型信号量(ACCEPT MUTUAL EXCLUSION SEMAPHORE)
//描述: 检查互斥型信号量，以判断某资源是否可以使用，与 OSMutexPend()不同的是，若资源不能使用，
//		则调用 OSMutexAccept()函数的任务并不被挂起， OSMutexAccept()仅查询状态。
//参数: pevent 指向管理某资源的互斥型信号量。程序在建立mutex时，得到该指针(参见 OSMutexCreate())
//		err 指向出错代码的指针，为以下值之一:
//		OS_NO_ERR 调用成功；
//		OS_ERR_EVENT_TYPE 'pevent'不是指向mutex类型的指针；
//		OS_ERR_PEVENT_NULL 'pevent'是空指针；
//		OS_ERR_PEND_ISR 在中断服务子程序中调用 OSMutexAccept().
//返回: == 1 如果mutex有效， OSMutexAccept()函数返回1；
// 		== 0 如果mutex被其他任务占用，OSMutexAccept()则返回0。
//警告: 1、必须先建立mutex,然后才能使用；
// 		2、在中断服务子程序中不能调用 OSMutexAccept()函数；
//		3、如使用 OSMutexAccept()获取mutex的状态，那么使用完共享资源后，必须调用 OSMutexPost()
//		   函数释放mutex 	    
#if OS_MUTEX_ACCEPT_EN > 0
INT8U  OSMutexAccept (OS_EVENT *pevent, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                            /* Allocate storage for CPU status register     */
    OS_CPU_SR  cpu_sr;
#endif    
    
    
    if (OSIntNesting > 0) {                            /* Make sure it's not called from an ISR        */
        *err = OS_ERR_PEND_ISR;
        return (0);
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                     /* Validate 'pevent'                            */
        *err = OS_ERR_PEVENT_NULL;
        return (0);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {  /* Validate event block type                    */
        *err = OS_ERR_EVENT_TYPE;
        return (0);
    }
#endif                                                     
    OS_ENTER_CRITICAL();							   /* Get value (0 or 1) of Mutex                  */
    if ((pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8) == OS_MUTEX_AVAILABLE) {     
        pevent->OSEventCnt &= OS_MUTEX_KEEP_UPPER_8;   /*      Mask off LSByte (Acquire Mutex)         */
        pevent->OSEventCnt |= OSTCBCur->OSTCBPrio;     /*      Save current task priority in LSByte    */
        pevent->OSEventPtr  = (void *)OSTCBCur;        /*      Link TCB of task owning Mutex           */
        OS_EXIT_CRITICAL();
        *err = OS_NO_ERR;
        return (1);
    }
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
    return (0);
}
#endif                                                     

//建立和初始化互斥型信号量(CREATE A MUTUAL EXCLUSION SEMAPHORE)
//描述: 互斥型信号量mutual的建立和初始化. 在与共享资源打交道时, 使用mutex可以保证满足互斥条件.
//参数: prio 优先级继承优先级(PIP).当一个高优先级的任务想要得到某mutex,而此时这个mutex却被
//			 一个低优先级的任务占用时,低优先级任务的优先级可以提升到PIP,知道其释放共享资源。
// 		err 指向出错代码的指针，为以下值之一:
//		OS_NO_ERR 调用成功mutex已被成功的建立；
//		OS_ERR_CREATE_ISR 试图在中断服务子程序中建立mutex；
// 		OS_PRIO_EXIST 优先级为PIP的任务已经存在；
// 		OS_ERR_PEVENT_NULL 已经没有OS_EVENT结构可以使用的了；
//		OS_PRIO_INVALID 定义的优先级非法，其值大于OS_LOWEST_PRIO.
//返回: 返回一个指针,该指针指向分配给mutex的事件控制块.如果得不到事件控制块,则返回一个空指针.
//注意: 1) 必须先建立mutex,然后才能使用;
// 		2) 必须确保优先级继承优先级.即prio高于可能与相应共享资源打交道的任务中优先级最高的任
// 		   务的优先级.例如有3个优先级分别为20，25，30的任务会使用mutex，那么prio的值必须小于
// 		   20；并且，已经建立了任务没有占用这个优先级。	
    
//建立并初始化一个互斥型信号量(优先级继承优先级(PIP)、出错代码指针)
OS_EVENT  *OSMutexCreate (INT8U prio, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    OS_EVENT  *pevent;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_CREATE_ISR;                          /* ... can't CREATE mutex from an ISR       */
        return ((OS_EVENT *)0);
    }
#if OS_ARG_CHK_EN > 0
    if (prio >= OS_LOWEST_PRIO) {                          /* Validate PIP                             */
        *err = OS_PRIO_INVALID;
        return ((OS_EVENT *)0);
    }
#endif
    OS_ENTER_CRITICAL();
    if (OSTCBPrioTbl[prio] != (OS_TCB *)0) {               /* Mutex priority must not already exist    */
        OS_EXIT_CRITICAL();                                /* Task already exist at priority ...       */
        *err = OS_PRIO_EXIST;                              /* ... inheritance priority                 */
        return ((OS_EVENT *)0);                            
    }
    OSTCBPrioTbl[prio] = (OS_TCB *)1;                      /* Reserve the table entry                  */
    pevent             = OSEventFreeList;                  /* Get next free event control block        */
    if (pevent == (OS_EVENT *)0) {                         /* See if an ECB was available              */
        OSTCBPrioTbl[prio] = (OS_TCB *)0;                  /* No, Release the table entry              */
        OS_EXIT_CRITICAL();
        *err               = OS_ERR_PEVENT_NULL;           /* No more event control blocks             */
        return (pevent);
    }
    OSEventFreeList     = (OS_EVENT *)OSEventFreeList->OSEventPtr;   /* Adjust the free list           */
    OS_EXIT_CRITICAL();
    pevent->OSEventType = OS_EVENT_TYPE_MUTEX;
    pevent->OSEventCnt  = (prio << 8) | OS_MUTEX_AVAILABLE;/* Resource is available                    */
    pevent->OSEventPtr  = (void *)0;                       /* No task owning the mutex                 */
    OS_EventWaitListInit(pevent);
    *err                = OS_NO_ERR;
    return (pevent);
}
	    
//删除互斥型信号量 (DELETE A MUTEX)
//描述: 删除一个mutex。使用这个函数有风险，因为多任务中其他任务可能还想用这个实际上已经被删除
//		了的mutex。使用这个函数时必须十分小心，一般地说，要删除一个mutex，首先应删除可能会用到
//		这个mutex的所有任务。
//参数: pevent 指向mutex的指针。应用程序建立mutex时得到该指针(参见OSMutexCreate()
//		opt 该参数定义删除mutex的条件。:
//		opt == OS_DEL_NO_PEND 只能在已经没有任何任务在等待该mutex时，才能删除；
//		opt == OS_DEL_ALWAYS 不管有没有任务在等待这个mutex，立刻删除mutex。
//		-->在第二种情况下，所有等待mutex的任务都立即进入就绪态.
// 		err 指向出错代码的指针，为以下值之一:
//		OS_NO_ERR 调用成功，mutex删除成功；
//		OS_ERR_DEL_ISR 试图在中断服务子程序中删除mutex。
//		OS_ERR_INVALID_OPT 定义的opt参数无效，不是上面提到的2个参数之一；
//		OS_ERR_TASK_WAITING 定义了OS_DEL_NO_PEND,而有一个或一个以上的任务在等这个mutex.
//		OS_ERR_EVENT_TYPE 'pevent'不是指向mutex的指针；
//		OS_ERR_PEVENT_NULL 已经没有可以使用的OS_EVENT数据结构了。			   
//返回: pevent 如果mutex已经删除，则返回空指针；如果mutex没能删除，则返回pevent.
// 		在后一种情况下，程序应检查出错代码，以查出原因。 
//注意: 1) 使用这个函数时必须十分小心，因为其他任务可能会用到mutex。  
#if OS_MUTEX_DEL_EN
OS_EVENT  *OSMutexDel (OS_EVENT *pevent, INT8U opt, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    BOOLEAN    tasks_waiting;
    INT8U      pip;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_DEL_ISR;                             /* ... can't DELETE from an ISR             */
        return (pevent);
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return ((OS_EVENT *)0);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {      /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return (pevent);
    }
#endif
    OS_ENTER_CRITICAL();
    if (pevent->OSEventGrp != 0x00) {                      /* See if any tasks waiting on mutex        */
        tasks_waiting = TRUE;                              /* Yes                                      */
    } else {
        tasks_waiting = FALSE;                             /* No                                       */
    }
    switch (opt) {
        case OS_DEL_NO_PEND:                               /* Delete mutex only if no task waiting     */
             if (tasks_waiting == FALSE) {
                 pip                 = (INT8U)(pevent->OSEventCnt >> 8);
                 OSTCBPrioTbl[pip]   = (OS_TCB *)0;        /* Free up the PIP                          */
                 pevent->OSEventType = OS_EVENT_TYPE_UNUSED;
                 pevent->OSEventPtr  = OSEventFreeList;    /* Return Event Control Block to free list  */
                 OSEventFreeList     = pevent;
                 OS_EXIT_CRITICAL();
                 *err = OS_NO_ERR;
                 return ((OS_EVENT *)0);                   /* Mutex has been deleted                   */
             } else {
                 OS_EXIT_CRITICAL();
                 *err = OS_ERR_TASK_WAITING;
                 return (pevent);
             }

        case OS_DEL_ALWAYS:                                /* Always delete the mutex                  */
             while (pevent->OSEventGrp != 0x00) {          /* Ready ALL tasks waiting for mutex        */
                 OS_EventTaskRdy(pevent, (void *)0, OS_STAT_MUTEX);
             }
             pip                 = (INT8U)(pevent->OSEventCnt >> 8);
             OSTCBPrioTbl[pip]   = (OS_TCB *)0;            /* Free up the PIP                          */
             pevent->OSEventType = OS_EVENT_TYPE_UNUSED;
             pevent->OSEventPtr  = OSEventFreeList;        /* Return Event Control Block to free list  */
             OSEventFreeList     = pevent;                 /* Get next free event control block        */
             OS_EXIT_CRITICAL();
             if (tasks_waiting == TRUE) {                  /* Reschedule only if task(s) were waiting  */
                 OS_Sched();                               /* Find highest priority task ready to run  */
             }
             *err = OS_NO_ERR;
             return ((OS_EVENT *)0);                       /* Mutex has been deleted                   */

        default:
             OS_EXIT_CRITICAL();
             *err = OS_ERR_INVALID_OPT;
             return (pevent);
    }
}
#endif
				 
//等待一个互斥型信号量(挂起) (PEND ON MUTUAL EXCLUSION SEMAPHORE)
//描述: 当任务需要独占共享资源时，应使用OSMutexPend()函数.如果任务在调用本函数时共享资源可
//		以使用，则OSMutexPend()函数返回，调用OSMutexPend()函数的任务得到了mutex。
//注意：OSMutexPend()实际上并没有"给"调用本函数的任务什么值，只不过参数err的值被置为
//		OS_NO_ERR，调用本函数的任务好像得到了mutex并继续运行。
// ---> 然而，如果nutex已经被别的任务占用了，那么OSMutexPend()函数就将调用该函数的任务放入
//		等待mutex的任务列表中，这个任务于是进入了等待状态，直到占有mutex的任务释放了mutex以
//		及共享资源，或者直到定义的等待时限超时。如果在等待时限内mutex得以释放,那么ucos_ii恢
//		复运行等待mutex的任务中优先级最高的任务。
//注意：如果mutex被优先级较低的任务占用了，那么OSMutexPend()会将占用mutex的任务的优先级提升
//		到优先级继承优先级PIP。PIP是在mutex建立时定义的(参见OSMutexCreate())
//参数: pevent 指向mutuex的指针。应用程序在建立mutuex时得到该指针的(参见OSMutexCreate())
// 		timeout 以时钟节拍数目的等待超时时限。如果在这一时限得不到mutex，任务将恢复执行。
//		        timeout的值为0，表示将无限期地等待mutex。timeout的最大值是65535个时钟节
//				拍。timeout的值并不与时钟节拍同步，timeout计数器在下一个时钟节拍到来时
//				开始递减。在这里，所谓下一个时钟节拍，也就是立刻就到来了。
//		err 指向出错代码的指针，为以下值之一:
//		OS_NO_ERR 调用成功，mutex可以使用；
//		OS_TIMEOUT 在定义的时间限内得不到mutex；
//		OS_ERR_EVENT_TYPE 用户没能向OSMutexPend()传递指向mutex的指针；
// 		OS_ERR_PEVENT_NULL 'pevent'是空指针
// 		OS_ERR_PEND_ISR 试图在中断服务子程序中获得mutex.
//返回: 无
//注意: 1) 必须先建立mutex,然后才能使用;
// 		2) 不要将占用mutex的任务挂起，也不要让占有mutex的任务等待usoc_ii提供的信号量、邮箱及消
//		   息队列等,不要将占用mutex的任务延迟.换言,用户代码应该抓紧时间,尽量快地释放共享资源。	   
void  OSMutexPend (OS_EVENT *pevent, INT16U timeout, INT8U *err)
{
#if OS_CRITICAL_METHOD == 3                                /* Allocate storage for CPU status register */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U      pip;                                        /* Priority Inheritance Priority (PIP)      */
    INT8U      mprio;                                      /* Mutex owner priority                     */
    BOOLEAN    rdy;                                        /* Flag indicating task was ready           */
    OS_TCB    *ptcb;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        *err = OS_ERR_PEND_ISR;                            /* ... can't PEND from an ISR               */
        return;
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        *err = OS_ERR_PEVENT_NULL;
        return;
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {      /* Validate event block type                */
        *err = OS_ERR_EVENT_TYPE;
        return;
    }
#endif
    OS_ENTER_CRITICAL();								   /* Is Mutex available?                      */
    if ((INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8) == OS_MUTEX_AVAILABLE) {
        pevent->OSEventCnt &= OS_MUTEX_KEEP_UPPER_8;       /* Yes, Acquire the resource                */
        pevent->OSEventCnt |= OSTCBCur->OSTCBPrio;         /*      Save priority of owning task        */
        pevent->OSEventPtr  = (void *)OSTCBCur;            /*      Point to owning task's OS_TCB       */
        OS_EXIT_CRITICAL();
        *err  = OS_NO_ERR;
        return;
    }
    pip   = (INT8U)(pevent->OSEventCnt >> 8);                     /* No, Get PIP from mutex            */
    mprio = (INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8);  /*     Get priority of mutex owner   */
    ptcb  = (OS_TCB *)(pevent->OSEventPtr);                       /*     Point to TCB of mutex owner   */
    if (ptcb->OSTCBPrio != pip && mprio > OSTCBCur->OSTCBPrio) {  /*     Need to promote prio of owner?*/
        if ((OSRdyTbl[ptcb->OSTCBY] & ptcb->OSTCBBitX) != 0x00) { /*     See if mutex owner is ready   */
                                                                  /*     Yes, Remove owner from Rdy ...*/
                                                                  /*          ... list at current prio */
            if ((OSRdyTbl[ptcb->OSTCBY] &= ~ptcb->OSTCBBitX) == 0x00) {
                OSRdyGrp &= ~ptcb->OSTCBBitY;
            }
            rdy = TRUE;
        } else {
            rdy = FALSE;                                          /* No                                */
        }
        ptcb->OSTCBPrio         = pip;                     /* Change owner task prio to PIP            */
        ptcb->OSTCBY            = ptcb->OSTCBPrio >> 3;
        ptcb->OSTCBBitY         = OSMapTbl[ptcb->OSTCBY];
        ptcb->OSTCBX            = ptcb->OSTCBPrio & 0x07;
        ptcb->OSTCBBitX         = OSMapTbl[ptcb->OSTCBX];
        if (rdy == TRUE) {                                 /* If task was ready at owner's priority ...*/
            OSRdyGrp               |= ptcb->OSTCBBitY;     /* ... make it ready at new priority.       */
            OSRdyTbl[ptcb->OSTCBY] |= ptcb->OSTCBBitX;
        }
        OSTCBPrioTbl[pip]       = (OS_TCB *)ptcb;
    }
    OSTCBCur->OSTCBStat |= OS_STAT_MUTEX;             /* Mutex not available, pend current task        */
    OSTCBCur->OSTCBDly   = timeout;                   /* Store timeout in current task's TCB           */
    OS_EventTaskWait(pevent);                         /* Suspend task until event or timeout occurs    */
    OS_EXIT_CRITICAL();
    OS_Sched();                                        /* Find next highest priority task ready         */
    OS_ENTER_CRITICAL();
    if (OSTCBCur->OSTCBStat & OS_STAT_MUTEX) {        /* Must have timed out if still waiting for event*/
        OS_EventTO(pevent);
        OS_EXIT_CRITICAL();
        *err = OS_TIMEOUT;                            /* Indicate that we didn't get mutex within TO   */
        return;
    }
    OSTCBCur->OSTCBEventPtr = (OS_EVENT *)0;
    OS_EXIT_CRITICAL();
    *err = OS_NO_ERR;
}
//释放一个互斥型信号量(POST TO A MUTUAL EXCLUSION SEMAPHORE)
//描述: 调用OSMutexPost()可以发出mutex。只是当用户程序已调用OSMutexAccept()或OSMutexPend()请
//		求得到mutex时，OSMutexPost()函数才起作用。当优先级较高的任务试图得到mutex时,如果占用
//		mutex的任务的优先级已经被升高，那么OSMutexPost()函数使优先级升高了的任务恢复原来的优
//		先级。如果有一个以上的任务在等待这个mutex，那么等待mutex的任务中优先级最高的任务将得
// 		得到mutex。然后本函数会调用调度函数,看被唤醒的任务是不是进入就绪态任务中优先级最高的
//		任务。如果是，则做任务切换，让这个任务运行。如果没有等待mutex的任务,那么本函数只不过
//		是将nutex的值设为OxFF，表示mutex可以使用。
//参数: pevent 指向mutuex的指针。应用程序在建立mutuex时得到该指针的(参见OSMutexCreate())
//返回: OS_NO_ERR 调用成功，mutex被释放；
//		OS_ERR_EVENT_TYPE OSMutexPost()传递的不是指向mutex的指针；
//		OS_ERR_PEVENT_NULL 'pevent'是空指针；
//		OS_ERR_POST_ISR 试图在中断服务子程序中调用OSMutexPost()函数；
//		OS_ERR_NOT_MUTEX_OWNER 发出mutex的任务实际上并不占用mutex。
//注意：1) 必须先建立mutex,然后才能使用;
//		2) 在中断服务子程序中不能调用OSMutexPost()函数  

INT8U  OSMutexPost (OS_EVENT *pevent)
{
#if OS_CRITICAL_METHOD == 3                           /* Allocate storage for CPU status register      */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U      pip;                                   /* Priority inheritance priority                 */
    INT8U      prio;


    if (OSIntNesting > 0) {                           /* See if called from ISR ...                    */
        return (OS_ERR_POST_ISR);                     /* ... can't POST mutex from an ISR              */
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                    /* Validate 'pevent'                             */
        return (OS_ERR_PEVENT_NULL);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) { /* Validate event block type                     */
        return (OS_ERR_EVENT_TYPE);
    }                                                 
#endif
    OS_ENTER_CRITICAL();
    pip  = (INT8U)(pevent->OSEventCnt >> 8);          /* Get priority inheritance priority of mutex    */
    prio = (INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8);  /* Get owner's original priority      */
    if (OSTCBCur->OSTCBPrio != pip && 
        OSTCBCur->OSTCBPrio != prio) {                /* See if posting task owns the MUTEX            */
        OS_EXIT_CRITICAL();
        return (OS_ERR_NOT_MUTEX_OWNER);
    }
    if (OSTCBCur->OSTCBPrio == pip) {                 /* Did we have to raise current task's priority? */
                                                      /* Yes, Return to original priority              */
                                                      /*      Remove owner from ready list at 'pip'    */
        if ((OSRdyTbl[OSTCBCur->OSTCBY] &= ~OSTCBCur->OSTCBBitX) == 0) {
            OSRdyGrp &= ~OSTCBCur->OSTCBBitY;
        }
        OSTCBCur->OSTCBPrio         = prio;
        OSTCBCur->OSTCBY            = prio >> 3;
        OSTCBCur->OSTCBBitY         = OSMapTbl[OSTCBCur->OSTCBY];
        OSTCBCur->OSTCBX            = prio & 0x07;
        OSTCBCur->OSTCBBitX         = OSMapTbl[OSTCBCur->OSTCBX];
        OSRdyGrp                   |= OSTCBCur->OSTCBBitY;
        OSRdyTbl[OSTCBCur->OSTCBY] |= OSTCBCur->OSTCBBitX;
        OSTCBPrioTbl[prio]          = (OS_TCB *)OSTCBCur;
    }
    OSTCBPrioTbl[pip] = (OS_TCB *)1;                  /* Reserve table entry                           */
    if (pevent->OSEventGrp != 0x00) {                 /* Any task waiting for the mutex?               */
                                                      /* Yes, Make HPT waiting for mutex ready         */
        prio                = OS_EventTaskRdy(pevent, (void *)0, OS_STAT_MUTEX);
        pevent->OSEventCnt &= OS_MUTEX_KEEP_UPPER_8;  /*      Save priority of mutex's new owner       */
        pevent->OSEventCnt |= prio;
        pevent->OSEventPtr  = OSTCBPrioTbl[prio];     /*      Link to mutex owner's OS_TCB             */
        OS_EXIT_CRITICAL();
        OS_Sched();                                   /*      Find highest priority task ready to run  */
        return (OS_NO_ERR);
    }
    pevent->OSEventCnt |= OS_MUTEX_AVAILABLE;         /* No,  Mutex is now available                   */
    pevent->OSEventPtr  = (void *)0;
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
/*$PAGE*/
/*
*********************************************************************************************************
*                                     QUERY A MUTUAL EXCLUSION SEMAPHORE
*
* Description: This function obtains information about a mutex
*
* Arguments  : pevent        is a pointer to the event control block associated with the desired mutex
*
*              pdata         is a pointer to a structure that will contain information about the mutex
*
* Returns    : OS_NO_ERR            The call was successful and the message was sent
*              OS_ERR_QUERY_ISR     If you called this function from an ISR
*              OS_ERR_PEVENT_NULL   'pevent' is a NULL pointer
*              OS_ERR_EVENT_TYPE    If you are attempting to obtain data from a non mutex.
*********************************************************************************************************
*/

#if OS_MUTEX_QUERY_EN > 0
INT8U  OSMutexQuery (OS_EVENT *pevent, OS_MUTEX_DATA *pdata)
{
#if OS_CRITICAL_METHOD == 3                      /* Allocate storage for CPU status register           */
    OS_CPU_SR  cpu_sr;
#endif    
    INT8U     *psrc;
    INT8U     *pdest;


    if (OSIntNesting > 0) {                                /* See if called from ISR ...               */
        return (OS_ERR_QUERY_ISR);                         /* ... can't QUERY mutex from an ISR        */
    }
#if OS_ARG_CHK_EN > 0
    if (pevent == (OS_EVENT *)0) {                         /* Validate 'pevent'                        */
        return (OS_ERR_PEVENT_NULL);
    }
    if (pevent->OSEventType != OS_EVENT_TYPE_MUTEX) {      /* Validate event block type                */
        return (OS_ERR_EVENT_TYPE);
    }
#endif
    OS_ENTER_CRITICAL();
    pdata->OSMutexPIP  = (INT8U)(pevent->OSEventCnt >> 8);
    pdata->OSOwnerPrio = (INT8U)(pevent->OSEventCnt & OS_MUTEX_KEEP_LOWER_8);
    if (pdata->OSOwnerPrio == 0xFF) {
        pdata->OSValue = 1;
    } else {
        pdata->OSValue = 0;
    }
    pdata->OSEventGrp  = pevent->OSEventGrp;               /* Copy wait list                           */
    psrc               = &pevent->OSEventTbl[0];
    pdest              = &pdata->OSEventTbl[0];
#if OS_EVENT_TBL_SIZE > 0
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 1
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 2
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 3
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 4
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 5
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 6
    *pdest++           = *psrc++;
#endif

#if OS_EVENT_TBL_SIZE > 7
    *pdest             = *psrc;
#endif
    OS_EXIT_CRITICAL();
    return (OS_NO_ERR);
}
#endif                                                     /* OS_MUTEX_QUERY_EN                        */
#endif                                                     /* OS_MUTEX_EN                              */
