/*
 * CorePacIrq.h
 *
 *  Created on: 2013-4-10
 *      Author: insswer
 */

#ifndef COREPACIRQ_H_
#define COREPACIRQ_H_



/* CorePac System IRQ */
#define IRQ_EVT0        0   /* combined events */
#define IRQ_EVT1        1
#define IRQ_EVT2        2
#define IRQ_EVT3        3
#define IRQ_TETBHFULL   4   /* TETB */
#define IRQ_TETBFULL    5
#define IRQ_TETBAXQ     6
#define IRQ_TETBOVFL    7
#define IRQ_TETBUNFL    8
#define IRQ_EMUDTDMA    9   /* emulator events */
#define IRQ_MSMC        10  /* MSMC */
#define IRQ_RTDXRX      11  /* RTDX */
#define IRQ_RTDXTX      12
#define IRQ_IDMAINT0    13  /* IDMA channel 0 interrupt */
#define IRQ_IDMAINT1    14  /* IDMA channel 1 interrupt */
#define IRQ_SEMERR      15  /* semaphores */
#define IRQ_SEM         16  /* semaphores */
#define IRQ_PCIEMSI0    17  /* PCIe MSI */
#define IRQ_TSIP0	18
#define IRQ_TSIP1	19
#define IRQ_RIOINT16    20  /* RapidIO interrupts */
#define IRQ_INTC0OUT    21  /* CP_INTC per-core outputs: 21 to 31  */
#define IRQ_QML         32  /* QM low: 32 to 47 */
#define IRQ_QMH         48  /* QM high: 48 to 55 */
#define IRQ_TSIP0RFS	52
#define IRQ_TSIP0RSF	53
#define IRQ_TSIP0XFS	54
#define IRQ_TSIP0XSF	55
#define IRQ_TSIP1RFS	56
#define IRQ_TSIP1RSF	57
#define IRQ_TSIP1XFS	58
#define IRQ_TSIP1XSF	59
#define IRQ_INTC0OUT2   62  /* CP_INTC outputs */
#define IRQ_INTC0OUT3   63
#define IRQ_TINT        64  /* timer interrupts */
#define IRQ_TINTLO      64
#define IRQ_TINTHI      65
#define IRQ_TINT8       66
#define IRQ_TINTLO8     66
#define IRQ_TINTHI8     67
#define IRQ_TINT9       68
#define IRQ_TINTLO9     68
#define IRQ_TINTHI9     69
#define IRQ_TINT10	70
#define IRQ_TINTLO10	70
#define IRQ_TINTHI10	71
#define IRQ_TINT11	72
#define IRQ_TINTLO11	72
#define IRQ_TINTHI11	73
#define IRQ_TINT12	74
#define IRQ_TINTLO12	74
#define IRQ_TINTHI12	75
#define IRQ_TINT13	76
#define IRQ_TINTLO13	76
#define IRQ_TINTHI13	77
#define IRQ_TINT14	78
#define IRQ_TINTLO14	78
#define IRQ_TINTHI14	79
#define IRQ_TINT15	80
#define IRQ_TINTLO15	80
#define IRQ_TINTHI15	81
#define IRQ_GPIO_START  82
#define IRQ_GPIO8       82
#define IRQ_GPIO9       83
#define IRQ_GPIO10      84
#define IRQ_GPIO11      85
#define IRQ_GPIO12      86
#define IRQ_GPIO13      87
#define IRQ_GPIO14      88
#define IRQ_GPIO15      89
#define IRQ_GPIOINT     90  /* local GPIO */
#define IRQ_IPCLOCAL    91  /* inter DSP interrupt from IPCGR */
#define IRQ_INTC0OUT4   92  /* INTC outputs */
#define IRQ_INTC0OUT5   93
#define IRQ_INTC0OUT6   94
#define IRQ_INTC0OUT7   95
#define IRQ_INTERR      96  /* irq controller dropped CPU interrupt event */
#define IRQ_EMCIDMAERR  97  /* EMC invalid IDMA parameters */
#define IRQ_EFINTA      100 /* EFI interrupt from side A */
#define IRQ_EFINTB      101 /* EFI interrupt from side B */
#define IRQ_INTC0OUT0   102 /* INTC outputs */
#define IRQ_INTC0OUT1   103
#define IRQ_INTC0OUT8   104
#define IRQ_INTC0OUT9   105
#define IRQ_INTC0OUT16	106
#define IRQ_INTC0OUT17	107
#define IRQ_INTC0OUT24	108
#define IRQ_INTC0OUT25	109
#define IRQ_MDMAERR     110 /* VbusM error event */
#define IRQ_TPCC0AET    112 /* TPCC0 AET */
#define IRQ_PMCED       113 /* single bit error detected during DMA read */
#define IRQ_TPCC1AET    114 /* TPCC1 AET */
#define IRQ_TPCC2AET    115 /* TPCC2 AET */
#define IRQ_UMCED1      116 /* corrected bit error detected */
#define IRQ_UMCED2      117 /* uncorrected bit error detected */
#define IRQ_PDCINT      118 /* PDC sleep interrupt */
#define IRQ_SYSCMPA     119 /* CPU memory protection fault */
#define IRQ_L1PCMPA     120 /* L1P CPU memory protection fault */
#define IRQ_L1PDMPA     121 /* L1P DMA memory protection fault */
#define IRQ_L1DCMPA     122 /* L1D CPU memory protection fault */
#define IRQ_L1DDMPA     123 /* L1D DMA memory protection fault */
#define IRQ_L2CMPA      124 /* L2 CPU memory protection fault */
#define IRQ_L2CDMPA     125 /* L2 DMA memory protection fault */
#define IRQ_EMCCMPA     126 /* external CPU memory protection fault */
#define IRQ_EMCBUSERR   127 /* bus error interrupt */


#endif /* COREPACIRQ_H_ */
