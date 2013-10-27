
#ifndef _C66XX_EXCEPTION_H
#define _C66XX_EXCEPTION_H


void BSP_EnableDspInterrupt(unsigned int dspInt);
void BSP_DisableDspInterrupt(unsigned int);
int BSP_AckDspInterrupt(unsigned int);
void BSP_SetDspInterrupt(unsigned int);
void BSP_ClearDspInterrupt(unsigned int);
void BSP_IsrHandlerEntry(unsigned int);
void BSP_ClearDspInterruptAll(void);
int BSP_GetExecutionState(void);


#define INT_TO_PRI(vecNum) (16 - vecNum)
#define PRI_TO_INT(pri)    (16 - pri)




#endif /* _C66XX_EXCEPTION_H */
