

#ifndef _C66XX_ITBL_H
#define _C66XX_ITBL_H


/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/

/* 汇编导出的函数，不要使用这个，应该使用BSP_GetCoreId */
unsigned int 	BSP_RawGetCoreId(void);
void		 	BSP_EnableInt(void);
void 			BSP_DisableInt(void);

void			BSP_LocalIntIrqSave(unsigned long addr);
void			BSP_EnableIntIrqRestore(void);

void 			BSP_InitializeIntCtr(void);
void			BSP_initializeVector(void);

unsigned long	BSP_GetCurDp(void);
unsigned long	BSP_GetCurSp(void);

void			BSP_IntMaskSave(unsigned long *);
void			BSP_IntMaskRestore(unsigned long);

unsigned long 	BSP_ReadIer(void);
void			BSP_WriteIer(unsigned long);
void		    BSP_WriteIsr(unsigned long);
void			BSP_WriteIcr(unsigned long);
unsigned long	BSP_ReadIfr(void);
void			BSP_MemoryFence(void);
void			BSP_SetIdle(void);
unsigned long 	BSP_ReadTSR(void);
unsigned long 	BSP_ReadCSR(void);
unsigned long 	BSP_ReadIRP(void);


void			BSP_EnableIer(unsigned int dsp_int);
void			BSP_DisableIer(unsigned int dsp_int);

static inline int BSP_GetCoreId(void)
{
	return (BSP_RawGetCoreId() & 0xff);
}

/**************************** 定义部分 *********************************/



#endif /* _C66XX_ITBL_H */
