/************************ (C) COPYLEFT 2010 Leafgrass *************************

* File Name		: os_cpu_c.c 
* Author		: Librae
* Date			: 06/10/2010
* Description	: ��COS-II��STM32�ϵ���ֲ����C���Բ��֣�
*				  ��(�����ջ��ʼ������͹��Ӻ����

******************************************************************************/

#ifndef	__OS_CPU_H__
#define	__OS_CPU_H__

#ifdef  OS_CPU_GLOBALS
#define OS_CPU_EXT
#else
#define OS_CPU_EXT  extern
#endif

/******************************************************************************
*                    ������������޹ص��������
******************************************************************************/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;			/* Unsigned  8 bit quantity       */
typedef signed   char  INT8S;			/* Signed    8 bit quantity       */
typedef unsigned short INT16U;			/* Unsigned 16 bit quantity       */
typedef signed   short INT16S;			/* Signed   16 bit quantity       */
typedef unsigned int   INT32U;			/* Unsigned 32 bit quantity       */
typedef signed   int   INT32S;			/* Signed   32 bit quantity       */
typedef float          FP32;			/* Single precision floating point*/
typedef double         FP64;			/* Double precision floating point*/

typedef unsigned int   OS_STK;			/* Each stack entry is 32-bit wide*/
typedef unsigned int   OS_CPU_SR;		/* Define size of CPU status register*/
#define	TRUE			1
#define	FALSE			0

/*
*******************************************************************************
*                             Cortex M3
*                     Critical Section Management
*******************************************************************************
*/

#define  OS_CRITICAL_METHOD   4


/*
*******************************************************************************
*                          ARM Miscellaneous
*******************************************************************************
*/

#define  OS_STK_GROWTH        1      /* Stack grows from HIGH to LOW memory on ARM    */

#define  OS_TASK_SW()         OSCtxSw()

/*
*******************************************************************************
*                               PROTOTYPES
*                           (see OS_CPU_A.ASM)
*******************************************************************************
*/

#if OS_CRITICAL_METHOD == 4
	 void OS_ENTER_CRITICAL (void);
     void OS_EXIT_CRITICAL (void);     
#endif

void       OSCtxSw(void);
void       OSIntCtxSw(void);
void       OSStartHighRdy(void);


OS_CPU_EXT INT32U OSInterrputSum;

#endif

/************************ (C) COPYLEFT 2010 Leafgrass ************************/
