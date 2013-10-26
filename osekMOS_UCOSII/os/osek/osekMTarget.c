/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/


#include  "osapi.h"

/*************************** 前向声明部分 ******************************/
asm("   .global $bss");

asm("_A0  .set  0*4     ");
asm("_A1  .set  1*4     ");
asm("_A2  .set  2*4     ");
asm("_A3  .set  3*4     ");
asm("_A4  .set  4*4     ");
asm("_A5  .set  5*4     ");
asm("_A6  .set  6*4     ");
asm("_A7  .set  7*4     ");
asm("_A8  .set  8*4     ");
asm("_A9  .set  9*4     ");
asm("_A10 .set  10*4    ");
asm("_A11 .set  11*4    ");
asm("_A12 .set  12*4    ");
asm("_A13 .set  13*4    ");
asm("_A14 .set  14*4    ");
asm("_A15 .set  15*4    ");
asm("_B0  .set  16*4    ");
asm("_B1  .set  17*4    ");
asm("_B2  .set  18*4    ");
asm("_B3  .set  19*4    ");
asm("_B4  .set  20*4    ");
asm("_B5  .set  21*4    ");
asm("_B6  .set  22*4    ");
asm("_B7  .set  23*4    ");
asm("_B8  .set  24*4    ");
asm("_B9  .set  25*4    ");
asm("_B10 .set  26*4    ");
asm("_B11 .set  27*4    ");
asm("_B12 .set  28*4    ");
asm("_B13 .set  29*4    ");
asm("_B14 .set  30*4    ");
asm("_B15 .set  31*4    ");
asm("_AMR .set  32*4    ");
asm("_CSR .set  33*4    ");
asm("_IER .set  34*4    ");
asm("_IRP .set  35*4    ");


/**************************** 定义部分 *********************************/

#if defined(CONFIG_OSEK_TARGET_DSP)

// 全局变量



#if 0
/*下面的变量声明已经放在OsKernel中*/
// OS相关中断掩码
OSDWORD osekTarget_OSIntMask = 0;
// 所有中断掩码
OSDWORD osekTarget_AllIntMask = 0;
// 嵌套的所有中断的掩码
OSDWORD osekTarget_NestedAllIntMask = 0;
// 嵌套的OS相关中断的掩码
OSDWORD osekTarget_NestedOsIntMask = 0;


asm("   .global osekTarget_SavedBTSP");
asm("osekTarget_SavedBTSP_Label:  .long  osekTarget_SavedBTSP");

#endif
/**************************** 实现部分 *********************************/

#if defined(CONFIG_OSEK_COMPILER_CCS)//#if defined(CONFIG_OSEK_COMPILER_CCS)

/*
 * OSDWORD osekTarget_SaveContext( void * savedContext)
 */
asm("   .text");
asm("   .align  4");
asm("   .global osekTarget_SaveContext");

/* A4 used as first param and return value */
asm("osekTarget_SaveContext:  ");
asm("   STW A0, *+A4(_A0)");
asm("   STW A1, *+A4(_A1)");
asm("   STW A2, *+A4(_A2)");
asm("   STW A3, *+A4(_A3)");
asm("   STW A4, *+A4(_A4)");
asm("   STW A5, *+A4(_A5)");
asm("   STW A6, *+A4(_A6)");
asm("   STW A7, *+A4(_A7)");
asm("   STW A8, *+A4(_A8)");
asm("   STW A9, *+A4(_A9)");
asm("   STW A10, *+A4(_A10)");
asm("   STW A11, *+A4(_A11)");
asm("   STW A12, *+A4(_A12)");
asm("   STW A13, *+A4(_A13)");
asm("   STW A14, *+A4(_A14)");
asm("   STW A15, *+A4(_A15)");
asm("   STW B0, *+A4(_B0)");
asm("   STW B1, *+A4(_B1)");
asm("   STW B2, *+A4(_B2)");
asm("   STW B3, *+A4(_B3)");
asm("   STW B4, *+A4(_B4)");
asm("   STW B5, *+A4(_B5)");

asm("   MVC AMR, B1");
asm("   MVC CSR, B2");
asm("   MVC IER, B4");
asm("   MVC IRP, B5");

asm("   STW B6, *+A4(_B6)");
asm("   STW B7, *+A4(_B7)");
asm("   STW B8, *+A4(_B8)");
asm("   STW B9, *+A4(_B9)");
asm("   STW B10, *+A4(_B10)");
asm("   STW B11, *+A4(_B11)");
asm("   STW B12, *+A4(_B12)");
asm("   STW B13, *+A4(_B13)");
asm("   STW B14, *+A4(_B14)");
asm("   STW B15, *+A4(_B15)");

asm("   MV  SP, B0");
asm("   MV  A4, SP");
asm("   STW B1, *+SP(_AMR)");
asm("   STW B2, *+SP(_CSR)");
asm("   STW B4, *+SP(_IER)");
asm("   STW B5, *+SP(_IRP)");
asm("   MV  B0, SP");
asm("   MVK 0, A4");
asm("   B B3");
asm("   NOP 5");

/*
 * void osekTarget_RestoreContext( void * restoredContext )
 */
asm("   .text");
asm("   .align  4");
asm("   .global  osekTarget_RestoreContext");

asm("osekTarget_RestoreContext:");
asm("   MV  A4, SP");
asm("   LDW *+SP(_AMR), B1");
asm("   LDW *+SP(_CSR), B2");
asm("   LDW *+SP(_IER), B4");
asm("   LDW *+SP(_IRP), B5");

asm("   LDW *+A4(_A0), A0");
asm("   LDW *+A4(_A1), A1");
asm("   LDW *+A4(_A2), A2");
asm("   LDW *+A4(_A3), A3");
asm("   LDW *+A4(_A4), A4");
asm("   LDW *+A4(_A5), A5");
asm("   LDW *+A4(_A6), A6");
asm("   LDW *+A4(_A7), A7");
asm("   LDW *+A4(_A8), A8");
asm("   LDW *+A4(_A9), A9");
asm("   LDW *+A4(_A10), A10");
asm("   LDW *+A4(_A11), A11");
asm("   LDW *+A4(_A12), A12");
asm("   LDW *+A4(_A13), A13");
asm("   LDW *+A4(_A14), A14");
asm("   LDW *+A4(_A15), A15");
asm("   LDW *+A4(_B0), B0");
asm("   LDW *+A4(_B3), B3");
asm("   LDW *+A4(_B6), B6");
asm("   LDW *+A4(_B7), B7");
asm("   LDW *+A4(_B8), B8");
asm("   LDW *+A4(_B9), B9");
asm("   LDW *+A4(_B10), B10");
asm("   LDW *+A4(_B11), B11");
asm("   LDW *+A4(_B12), B12");
asm("   LDW *+A4(_B13), B13");
asm("   LDW *+A4(_B14), B14");

asm("   MVC B1, AMR");
asm("   MVC B2, CSR");
asm("   MVC B4, IER");
asm("   MVC B5, IRP");

asm("   LDW *+A4(_B1), B1");
asm("   LDW *+A4(_B2), B2");
asm("   LDW *+A4(_B4), B4");
asm("   LDW *+A4(_B5), B5");

asm("   LDW *+A4(_B15), B15");
asm("   MVK 1, A4");
asm("   B B3");
asm("   NOP 5");

/*
 * void osekTarget_LoadETSP(void* sp)
 */
asm("   .text");
asm("   .align  4");
asm("   .global osekTarget_LoadETSP");
asm("osekTarget_LoadETSP:");
asm("   MV A4, B15");
asm("   B B3");
asm("   NOP 5");

/*
 * void osekTarget_LoadBTSP_do(void *sp)
 */
asm("   .text");
asm("   .align  4");
asm("   .global osekTarget_LoadBTSP_do");
asm("osekTarget_LoadBTSP_do:");
asm("   MV A4, B15");
asm("   B B3");
asm("  NOP 5");



/*
 * void DisableOSInt(void* osIntSave)
 */
asm("   .text");
asm("   .align  32");
asm("   .global  DisableOSInt");

asm("DisableOSInt:");
asm("   MVC CSR, B4");
asm("   STW B4, *A4");
asm("   CLR B4, 0, 0, B4");
asm("   MVC B4,CSR");
asm("   B B3");
asm("   NOP 5");

/*
 * void EnableOSInt(OSDWORD osIntSave)
 */
asm("   .text");
asm("   .align  32");
asm("   .global  EnableOSInt");

asm("EnableOSInt:");
asm("   MV  A4, B4");
asm("   MVC B4, CSR");
asm("   B B3");
asm("   NOP 5");

#endif

#endif
