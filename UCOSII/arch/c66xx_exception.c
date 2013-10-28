
/**************************** 引用部分 *********************************/
#include <stdio.h>
#include "c66xx_exception.h"
#include "c66xx_itbl.h"
#include "c66xx_debug.h"
#include "c66xx_io.h"


void BSP_EnableDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}

	tmp = BSP_ReadIer();
	tmp |= (1 << dspInt);
	BSP_WriteIer(tmp);
}


void BSP_DisableDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}

	tmp = BSP_ReadIer();
	tmp &= ~(1 << dspInt);
	BSP_WriteIer(tmp);
}

int BSP_AckDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return -1;
	}

	tmp = BSP_ReadIfr();
	if (tmp & (1 << dspInt)) {
		tmp = 0;
		tmp |= (1 << dspInt);
		BSP_WriteIcr(tmp);
	}else {
		DEBUG_DEV("dsp_int%d is not exist\n",dspInt);
		return -1;
	}
	return 0;
}

void BSP_SetDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}
	tmp = 0;
	tmp |= (1 << dspInt);
	BSP_WriteIsr(tmp);
}

void BSP_ClearDspInterrupt(unsigned int dspInt)
{
	unsigned long tmp;
	if (dspInt > 15 || dspInt < 4) {
		DEBUG_DEV("bad dsp_int%d\n",dspInt);
		return;
	}
	tmp = 0;
	tmp |= (1 << dspInt);
	BSP_WriteIcr(tmp);
}

void BSP_ClearDspInterruptAll(void)
{
	unsigned long tmp;
	tmp = 0xfff0;
	BSP_WriteIcr(tmp);
}


int BSP_GetExecutionState(void)
{
	unsigned long tmp = BSP_ReadTSR();
	if (tmp & (1 << 6))
		return 0;
	else
		return 1;
}

extern int Scropius_ISRHandlerEntry(int, unsigned int);

static void BSP_OtherHandlerEntry(int core, unsigned int vectorNum)
{
	printf("unresolved interrupt request %d from core %d\n", vectorNum, core);
}


/*
 * @brief
 *    BSP_IsrHandlerEntry：中断处理总入口函数。
 * @param		vectorNum：发生的DSP中断号。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
void BSP_IsrHandlerEntry(unsigned int vectorNum)
{
	int core = BSP_GetCoreId();
	BSP_ClearDspInterrupt(vectorNum);
	if (Scropius_ISRHandlerEntry(core, vectorNum))
		BSP_OtherHandlerEntry(core, vectorNum);
}
