
; void BSP_InitializeVector(void);
	.global BSP_InitializeVector
BSP_InitializeVector:
	.ref		_vectors
	MVKL		_vectors,	B0
	MVKH		_vectors,	B0
	MVC			B0,			ISTP
	B			B3
	NOP			5



;	clear ISR,ICR,IRP and NRP
; void BSP_InitializeIntCtr(void);
	.global BSP_InitializeIntCtr
BSP_InitializeIntCtr:
	MVK 	0x0, B0
    MVC 	B0, AMR
    MVC 	B0, ISR
    MVC 	B0, ICR
    MVC 	B0, IRP
    MVC 	B0, NRP
	MVK	    0xfff0, B0
	MVC		B0, ICR
    MVK 	0x100, B0	; little endian
    MVC 	B0, CSR
    MVK 	2, B0		; enable NMI
    MVC 	B0, IER
    B		B3
    NOP		5


;	get current data page pointer
; unsigned long BSP_GetCurDp(void);
    .global BSP_GetCurDp
BSP_GetCurDp:
       MV		B14, A4
       B		B3
       NOP		5



;	get current stack pointer
; unsigned long BSP_GetCurSp(void);
       .global BSP_GetCurSp
BSP_GetCurSp:
       MV		B15,	A4
       B		B3
       NOP		5


;	get current core id
; unsigned int BSP_RawGetCoreId(void);
       .global BSP_RawGetCoreId
BSP_RawGetCoreId:
       MVC		DNUM,	B4
       MV		B4,		A4
       B		B3
       NOP		5


;	disable int by clearing CSR[0]
; void BSP_DisableInt(void);
       .global BSP_DisableInt
BSP_DisableInt:
       MVC		CSR,	B4
       CLR		B4,	0,	0,	B4
       MVC		B4,		CSR
       B		B3
       NOP		5


;	enable int by setting CSR[0]
; void BSP_EnableInt(void);
       .global BSP_EnableInt
BSP_EnableInt:
       MVC		CSR,	B4
       MVK		0x1,	B5
       OR		B5,	B4,	B4
       MVC		B4,		CSR
       B		B3
       NOP		5


;	save IER
; void BSP_IntMaskSave(unsigned long);
       .global BSP_IntMaskSave
BSP_IntMaskSave:
       MVC		IER,	B0
       STW		B0,		*A4
       B		B3
       NOP		5


;	load IER
; void BSP_IntMaskRestore(unsigned long);
       .global BSP_IntMaskRestore
BSP_IntMaskRestore:
       MV		A4,		B0
       MVC		B0,		IER
       B		B3
       NOP		5


;	enable IER
; void BSP_EnableIer(unsigned int);
       .global BSP_EnableIer
BSP_EnableIer:
       	MV		A4,		A0
       	MVK		0x1,	A2
       	MVC		IER,	B1
       	SHL		A2,		A0,		A0
       	OR		A0,		B1,		B1
       	MVC		B1,		IER
       	B		B3
       	NOP		5


;	disable IER
; void BSP_DisableIer(unsigned int dsp_int);
       .global BSP_DisableIer
BSP_DisableIer:
       	MV		A4,		A0
       	MVK		0x1,	A2
       	MVC		IER,	B1
       	SHL		A2,		A0,		A0
       	NOT		A0,		A0
       	AND		A0,		B1,		B1
       	MVC		B1,		IER
       	B		B3
      	NOP		5


;	get ier
; unsigned long BSP_ReadIer(void);
       .global BSP_ReadIer
BSP_ReadIer:
		MVC		IER,	B0
       	MV		B0,		A4
       	B		B3
       	NOP		5


;	write ier
; void BSP_WriteIer(unsigned long);
		.global BSP_WriteIer
BSP_WriteIer:
		MV		A4,		B0
		MVC		B0,		IER
		B		B3
		NOP		5



;	write isr
; void BSP_WriteIsr(unsigned long);
		.global BSP_WriteIsr
BSP_WriteIsr:
		MV		A4,		B0
		MVC		B0,		ISR
		B		B3
		NOP		5


;	write icr
; void BSP_WriteIcr(unsigned long);
		.global	BSP_WriteIcr
BSP_WriteIcr:
		MV		A4,		B0
		MVC		B0,		ICR
		B		B3
		NOP		5


;	read ifr
; unsigned long BSP_ReadIfr(void);
		.global BSP_ReadIfr
BSP_ReadIfr:
		MVC		IFR,	B0
		MV		B0,		A4
		B		B3
		NOP		5

; unsigned long	BSP_ReadTSR(void);
		.global BSP_ReadTSR
BSP_ReadTSR:
		MVC		TSR,	B0
		MV		B0,		A4
		B		B3
		NOP		5


; void BSP_MemoryFence(void);
	.global BSP_MemoryFence
BSP_MemoryFence:
	   MFENCE
	   B		B3
	   NOP		5

;
	.global BSP_LocalIntIrqSave
; void BSP_LocalIntIrqSave(unsigned long);
BSP_LocalIntIrqSave:
		MVC		CSR,	B0
		STW		B0,		*A4
        CLR		B0,	0,	0,	B0
        MVC		B0,		CSR
		B		B3
		NOP		5

	.global BSP_LocalIntIrqRestore
; void BSP_LocalIntIrqRestore(unsigned long);
BSP_LocalIntIrqRestore:
		LDW		*A4,	B0
		NOP		4
		MVC		B0,		CSR
		B		B3
		NOP		5

; void BSP_SetIdle(void);
	.global BSP_SetIdle
BSP_SetIdle:
		IDLE
		B		B3
		NOP		5

; unsigned long BSP_ReadCSR(void);
	.global BSP_ReadCSR
BSP_ReadCSR:
		MVC		CSR,	B0
		MV		B0,		A4
		B		B3
		NOP		5

; unsigned long BSP_ReadIRP(void);
	.global BSP_ReadIRP
BSP_ReadIRP:
		MVC		IRP,	B0
		MV		B0,		A4
		B		B3
		NOP		5
