/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

#ifndef OSEKMTARGET_H_
#define OSEKMTARGET_H_

#include "osekOccConfig.h"
/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/

// 内部函数
OSDWORD osekTarget_SaveContext( void * savedContext );
void osekTarget_RestoreContext( void * restoredContext );

// 下面的函数用于第一次启动扩展任务和基本任务时调整任务的栈
void osekTarget_LoadETSP(void* sp) ;

#if 0
//加载基本任务堆栈指针.多核扩展中将由OSEK_TARGET_LoadBTSP(core)宏完成这个函数的功能
void osekTarget_LoadBTSP( void );
//保存基本任务堆栈的指针.多核扩展中将由OSEK_TARGET_SaveBTSP(core,sp)宏完成这个函数的功能
void osekTarget_SaveBTSP(OSDWORD sp );
#endif

extern void osekTarget_LoadBTSP_do(void *sp);

#if 0
// 变量声明.在多核扩展中,已经将这些变量放在OsKernel中,下面的变量不再使用
extern OSDWORD osekTarget_AllIntMask;
extern OSDWORD osekTarget_NestedAllIntMask;
extern OSDWORD osekTarget_NestedOsIntMask;
#if defined(CONFIG_OSEK_TARGET_DSP)
extern OSDWORD osekTarget_SavedBTSP;
#endif
#endif


/**************************** 定义部分 *********************************/
#if defined(CONFIG_OSEK_TARGET_TMS320C6000)


/// PPC的任务上下文结构，在配置了PPC目标机时该结构才存在
struct T_OSEK_TARGET_TaskContext_struct
{
    OSDWORD A[16];  /* A0 ~ A15 */
    OSDWORD B[15];  /* B0 ~ B14 */
    OSDWORD sp;     /* B15 */
    OSDWORD AMR;
    OSDWORD CSR;
    OSDWORD IER;
    OSDWORD IRP;
};

//- (OSWORD)1024
#define OSEK_TARGET_SaveBTSP(kernelPtr,sp)	(kernelPtr->osekTarget_SavedBTSP = (sp)-0x10)

#define OSEK_TARGET_LoadBTSP(osekTarget_SavedBTSP)      \
do {\
    osekTarget_LoadBTSP_do((void *)(osekTarget_SavedBTSP)); \
}while(0)


#if defined(CONFIG_OSEK_COMPILER_CCS)

#if defined(CONFIG_OSEK_TARGET_TMS320C6000)

#define OSEK_TARGET_OSIntSave( osIntSave )  	OSDWORD osIntSave
#define OSEK_TARGET_DisableOSInt(osIntSave) 	DisableOSInt(&osIntSave)
#define OSEK_TARGET_EnableOSInt(osIntSave) 		EnableOSInt(osIntSave)

#define OSEK_TARGET_DisableOSIntWithoutSave()  	BSP_DisableInt()
#define OSEK_TARGET_EnableOSIntWithoutSave()   	BSP_EnableInt()

#define OSEK_TARGET_DisableAllInt(core) 		DisableOSInt(&(osekKernel_osKernel[core].osekTarget_AllIntMask))
#define OSEK_TARGET_EnableAllInt(core)			EnableOSInt(osekKernel_osKernel[core].osekTarget_AllIntMask)

#define OSEK_TARGET_DisableNestedAllInt(core)  	DisableOSInt(&(osekKernel_osKernel[core].osekTarget_NestedAllIntMask))
#define OSEK_TARGET_EnableNestedAllInt(core)    EnableOSInt(osekKernel_osKernel[core].osekTarget_NestedAllIntMask)

#define OSEK_TARGET_DisableNestedOsInt(core) 	DisableOSInt(&(osekKernel_osKernel[core].osekTarget_NestedOsIntMask))
#define OSEK_TARGET_EnableNestedOsInt(core)  	EnableOSInt(osekKernel_osKernel[core].osekTarget_NestedOsIntMask)

#endif 	//#if defined(CONFIG_OSEK_TARGET_TMS320C6000)

#endif	//#if defined(CONFIG_OSEK_COMPILER_CCS)

#endif	//#if defined(CONFIG_OSEK_TARGET_TMS320C6000)

#endif /* OSEKMTARGET_H_ */
