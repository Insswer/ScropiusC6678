;/*********************** (C) COPYRIGHT 2010 Libraworks *************************
;* File Name	: os_cpu_a.asm 
;* Author		: Librae 
;* Version		: V1.0
;* Date			: 06/10/2010
;* Description	: ��COS-II asm port
;*******************************************************************************/

		.ref  OSRunning               ; External references
        .ref  OSPrioCur
        .ref  OSPrioHighRdy
        .ref  OSTCBCur
        .ref  OSTCBHighRdy
        .ref  OSIntNesting
        .ref  OSIntExit
        .ref  OSTaskSwHook
		.ref  OSInterrputSum
         
            
        .global  OSCtxSw
        .global  OSIntCtxSw
            
        	
;
;NVIC_INT_CTRL   	EQU     0xE000ED04  ; �жϿ��ƼĴ���
;NVIC_SYSPRI2    	EQU     0xE000ED20  ; ϵͳ���ȼ��Ĵ���(2)
;NVIC_PENDSV_PRI 	EQU     0xFFFF0000  ; ����жϺ�ϵͳ�����ж�
                                        ; (��Ϊ��ͣ�0xff).
;NVIC_PENDSVSET  	EQU     0x10000000  ; ��������жϵ�ֵ.


;		PRESERVE8
		
;		AREA    |.text|, CODE, READONLY
 ;       THUMB
    
           

;/***************************************************************************************
;* �������: OS_ENTER_CRITICAL
;*
;* ��������: �����ٽ���
;*            
;* ��    ��: None
;*
;* �� �� ֵ: None
;*****************************************************************************************/ 

;OS_ENTER_CRITICAL
 
;		CPSID   I                       ; Disable all the interrupts
                                                                        
;		PUSH 	{R1,R2}

;		LDR 	R1, =OSInterrputSum	    ; OSInterrputSum++
;       LDRB 	R2, [R1]
;       ADD   	R2, R2, #1
;       STRB 	R2, [R1]
;		POP     {R1,R2}
;  		BX LR

;/***************************************************************************************
;* �������: OS_EXIT_CRITICAL
;*
;* ��������: �˳��ٽ���
;*            
;* ��    ��: None
;*
;* �� �� ֵ: None
;*****************************************************************************************/


;		For C66XX save PC, B3, B0 - B2, B4 - B14, A0 - 15, CSR, IER, IRP
;		Restore and rint
;
;		STACK Frame When Saving
;		CSR															<--- High Memory
;		PC(B3)
;		B3(LR)
;		B0
;		B1
;		B2
;		B4
;		B5
;		B6
;		B7
;		B8
;		B9
;		B10
;		B11
;		B12
;		B13
;		B14
;		A0
;		A1
;		A2
;		A3
;		A4
;		A5
;		A6
;		A7
;		A8
;		A9
;		A10
;		A11
;		A12
;		A13
;		A14
;		A15													<--- Low Memory

; arg: unsigned long sp
	.global OSRestoreHighRdy
OSRestoreHighRdy:
	MV		A4,		B15

;	Load A15
	LDW		*++B15[2], A15
	NOP		4
;	Load A14
	LDW		*++B15[2], A14
	NOP		4
;	Load A13
	LDW		*++B15[2], A13
	NOP		4
;	Load A12
	LDW		*++B15[2], A12
	NOP		4
;	Load A11
	LDW		*++B15[2], A11
	NOP		4
;	Load A10
	LDW		*++B15[2], A10
	NOP		4
;	Load A9
	LDW		*++B15[2], A9
	NOP		4
;	Load A8
	LDW		*++B15[2], A8
	NOP		4
;	Load A7
	LDW		*++B15[2], A7
	NOP		4
;	Load A6
	LDW		*++B15[2], A6
	NOP		4
;	Load A5
	LDW		*++B15[2], A5
	NOP		4
;	Load A4
	LDW		*++B15[2], A4
	NOP		4
;	Load A3
	LDW		*++B15[2], A3
	NOP		4
;	Load A2
	LDW		*++B15[2], A2
	NOP		4
;	Load A1
	LDW		*++B15[2], A1
	NOP		4
;	Load A0
	LDW		*++B15[2], A0
	NOP		4
;	Load B14
	LDW		*++B15[2], B14
	NOP		4
;	Load B13
	LDW		*++B15[2], B13
	NOP		4
;	Load B12
	LDW		*++B15[2], B12
	NOP		4
;	Load B11
	LDW		*++B15[2], B11
	NOP		4
;	Load B10
	LDW		*++B15[2], B10
	NOP		4
;	Load B9
	LDW		*++B15[2], B9
	NOP		4
;	Load B8
	LDW		*++B15[2], B8
	NOP		4
;	Load B7
	LDW		*++B15[2], B7
	NOP		4
;	Load B6
	LDW		*++B15[2], B6
	NOP		4
;	Load B5
	LDW		*++B15[2], B5
	NOP		4
;	Load B4
	LDW		*++B15[2], B4
	NOP		4
;	Load B2
	LDW		*++B15[2], B2
	NOP		4
;	Load B1
	LDW		*++B15[2], B1
	NOP		4
;	Load B0
	LDW		*++B15[2], B0
	NOP		4
;	Load B3
	LDW		*++B15[2], B3
	NOP		4
;	Load PC
	LDW		*++B15[2], B16
	NOP		4
;	Load CSR
	LDW		*++B15[2], B17
	NOP		4
	MVC		B17,		CSR
	B		B16
	NOP		5



;/**************************************************************************************
;* �������: OSStartHighRdy
;*
;* ��������: ʹ�õ�������е�һ������
;* 
;* ��    ��: None
;*
;* �� �� ֵ: None
;**************************************************************************************/  

;OSStartHighRdy
;        LDR     R4, =NVIC_SYSPRI2      ; set the PendSV exception priority
;        LDR     R5, =NVIC_PENDSV_PRI
;        STR     R5, [R4]

;        MOV     R4, #0                 ; set the PSP to 0 for initial context switch call
;        MSR     PSP, R4

;        LDR     R4, =OSRunning         ; OSRunning = TRUE
;        MOV     R5, #1
;        STRB    R5, [R4]

                                       ;�л���������ȼ�������
;        LDR     R4, =NVIC_INT_CTRL     ;rigger the PendSV exception (causes context switch)
;        LDR     R5, =NVIC_PENDSVSET
;        STR     R5, [R4]

;        CPSIE   I                      ;enable interrupts at processor level
;OSStartHang
;        B       OSStartHang            ;should never get here

;/**************************************************************************************
;* �������: OSCtxSw
;*
;* ��������: �����������л�
;*
;* ��    ��: None
;*
;* �� �� ֵ: None
;***************************************************************************************/
		.global OSCtxSw
OSCtxSw:
;		Save Registers into sp
		MVC		CSR,  B17
		STW		B17, *B15--[2]
		STW		B3,	 *B15--[2]
		STW		B3,	 *B15--[2]
		STW		B0,  *B15--[2]
		STW		B1,	 *B15--[2]
		STW		B2,	 *B15--[2]
        STW		B4,  *B15--[2]
		STW		B5,	 *B15--[2]
		STW		B6,	 *B15--[2]
		STW		B7,	 *B15--[2]
		STW		B8,	 *B15--[2]
		STW		B9,	 *B15--[2]
		STW		B10, *B15--[2]
		STW		B11, *B15--[2]
		STW		B12, *B15--[2]
		STW		B13, *B15--[2]
		STW		B14, *B15--[2]
		STW		A0,	 *B15--[2]
		STW		A1,	 *B15--[2]
		STW		A2,	 *B15--[2]
		STW		A3,	 *B15--[2]
		STW		A4,	 *B15--[2]
		STW		A5,	 *B15--[2]
		STW		A6,	 *B15--[2]
		STW		A7,	 *B15--[2]
		STW		A8,	 *B15--[2]
		STW		A9,	 *B15--[2]
 		STW		A10, *B15--[2]
		STW		A11, *B15--[2]
		STW		A12, *B15--[2]
		STW		A13, *B15--[2]
		STW		A14, *B15--[2]
		STW		A15, *B15--[2]
		MVC		CSR,  B4
		STW		B4,	 *B15--[2]

		MVKL	OSTCBCur, A4
		MVKH	OSTCBCur, A4
		LDW		*A4,	A5
		NOP		4
		STW		B15,	*A5		; Store SP into OSTCBStkPtr


		CALLP	OSTaskSwHook,B3

;		MVKL	OSTCBHighRdy,	A4
;		MVKH	OSTCBHighRdy,	A4

;		MVKL	OSTCBCur,		A5
;		MVKH	OSTCBCur,		A5

;		LDW		*A4,			A6	;
;		NOP		4
;		STW		A6,				*A5 ;

;		MVKL	OSPrioHighRdy,	A4
;		MVKH	OSPrioHighRdy,	A4
;
;		MVKL	OSPrioCur,		A5
;		MVKH	OSPrioCur,		A5
;
;		LDBU	*A4,			A6
;		NOP		4
;		STB		A6,				*A5
;
;		MVKL	OSTCBHighRdy,	A4
;		MVKH	OSTCBHighRdy,	A4
;		LDW		*A4,			A6
;		NOP		4

		MV		A4,				B15
		NOP		4

;		Restore	Context
;	Load A15
		LDW		*++B15[2], A15
		NOP		4
;	Load A14
		LDW		*++B15[2], A14
		NOP		4
;	Load A13
		LDW		*++B15[2], A13
		NOP		4
;	Load A12
		LDW		*++B15[2], A12
		NOP		4
;	Load A11
		LDW		*++B15[2], A11
		NOP		4
;	Load A10
		LDW		*++B15[2], A10
		NOP		4
;	Load A9
		LDW		*++B15[2], A9
		NOP		4
;	Load A8
		LDW		*++B15[2], A8
		NOP		4
;	Load A7
		LDW		*++B15[2], A7
		NOP		4
;	Load A6
		LDW		*++B15[2], A6
		NOP		4
;	Load A5
		LDW		*++B15[2], A5
		NOP		4
;	Load A4
		LDW		*++B15[2], A4
		NOP		4
;	Load A3
		LDW		*++B15[2], A3
		NOP		4
;	Load A2
		LDW		*++B15[2], A2
		NOP		4
;	Load A1
		LDW		*++B15[2], A1
		NOP		4
;	Load A0
		LDW		*++B15[2], A0
		NOP		4
;	Load B14
		LDW		*++B15[2], B14
		NOP		4
;	Load B13
		LDW		*++B15[2], B13
		NOP		4
;	Load B12
		LDW		*++B15[2], B12
		NOP		4
;	Load B11
		LDW		*++B15[2], B11
		NOP		4
;	Load B10
		LDW		*++B15[2], B10
		NOP		4
;	Load B9
		LDW		*++B15[2], B9
		NOP		4
;	Load B8
		LDW		*++B15[2], B8
		NOP		4
;	Load B7
		LDW		*++B15[2], B7
		NOP		4
;	Load B6
		LDW		*++B15[2], B6
		NOP		4
;	Load B5
		LDW		*++B15[2], B5
		NOP		4
;	Load B4
		LDW		*++B15[2], B4
		NOP		4
;	Load B2
		LDW		*++B15[2], B2
		NOP		4
;	Load B1
		LDW		*++B15[2], B1
		NOP		4
;	Load B0
		LDW		*++B15[2], B0
		NOP		4
;	Load B3
		LDW		*++B15[2], B3
		NOP		4
;	Load PC
		LDW		*++B15[2], B16
		NOP		4
;   Load CSR
		LDW		*++B15[2], B17
		NOP		4
		MVC		B17,	CSR
		B		B16
		NOP		5

;OSCtxSw
;		PUSH    {R4, R5}
;        LDR     R4, =NVIC_INT_CTRL  	;����PendSV�쳣 (causes context switch)
;        LDR     R5, =NVIC_PENDSVSET
;        STR     R5, [R4]
;		POP     {R4, R5}
;        BX      LR


;/**************************************************************************************
;* �������: OSIntCtxSw
;*
;* ��������: �жϼ������л�
;*
;* ��    ��: None
;*
;* �� �� ֵ: None
;***************************************************************************************/
	.global	OSIntCtxSwRestore
OSIntCtxSwRestore:
	MV		A4,		B15
;	Load A15
	LDW		*++B15[2], A15
	NOP		4
;	Load A14
	LDW		*++B15[2], A14
	NOP		4
;	Load A13
	LDW		*++B15[2], A13
	NOP		4
;	Load A12
	LDW		*++B15[2], A12
	NOP		4
;	Load A11
	LDW		*++B15[2], A11
	NOP		4
;	Load A10
	LDW		*++B15[2], A10
	NOP		4
;	Load A9
	LDW		*++B15[2], A9
	NOP		4
;	Load A8
	LDW		*++B15[2], A8
	NOP		4
;	Load A7
	LDW		*++B15[2], A7
	NOP		4
;	Load A6
	LDW		*++B15[2], A6
	NOP		4
;	Load A5
	LDW		*++B15[2], A5
	NOP		4
;	Load A4
	LDW		*++B15[2], A4
	NOP		4
;	Load A3
	LDW		*++B15[2], A3
	NOP		4
;	Load A2
	LDW		*++B15[2], A2
	NOP		4
;	Load A1
	LDW		*++B15[2], A1
	NOP		4
;	Load A0
	LDW		*++B15[2], A0
	NOP		4
;	Load B14
	LDW		*++B15[2], B14
	NOP		4
;	Load B13
	LDW		*++B15[2], B13
	NOP		4
;	Load B12
	LDW		*++B15[2], B12
	NOP		4
;	Load B11
	LDW		*++B15[2], B11
	NOP		4
;	Load B10
	LDW		*++B15[2], B10
	NOP		4
;	Load B9
	LDW		*++B15[2], B9
	NOP		4
;	Load B8
	LDW		*++B15[2], B8
	NOP		4
;	Load B7
	LDW		*++B15[2], B7
	NOP		4
;	Load B6
	LDW		*++B15[2], B6
	NOP		4
;	Load B5
	LDW		*++B15[2], B5
	NOP		4
;	Load B4
	LDW		*++B15[2], B4
	NOP		4
;	Load B2
	LDW		*++B15[2], B2
	NOP		4
;	Load B1
	LDW		*++B15[2], B1
	NOP		4
;	Load B0
	LDW		*++B15[2], B0
	NOP		4
;	Load B3
	LDW		*++B15[2], B3
	NOP		4
;	Load PC
	LDW		*++B15[2], B16
	NOP		4
;   Load CSR
	LDW		*++B15[2], B17
	NOP		4
	MVC		B17,	CSR
	B		B16
	NOP		5



;OSIntCtxSw
;		 PUSH    {R4, R5}
;        LDR     R4, =NVIC_INT_CTRL      ;����PendSV�쳣 (causes context switch)
;        LDR     R5, =NVIC_PENDSVSET
;        STR     R5, [R4]
; 		 POP     {R4, R5}
;        BX      LR
;        NOP

;/**************************************************************************************
;* �������: OSPendSV
;*
;* ��������: OSPendSV is used to cause a context switch.
;*
;* ��    ��: None
;*
;* �� �� ֵ: None
;***************************************************************************************/

;PendSV_Handler
;    CPSID   I                                                   ; Prevent interruption during context switch
;    MRS     R0, PSP                                             ; PSP is process stack pointer �������PSP��ջ,����Ժ��Ա���Ĵ���,�ο�CM3Ȩ���е�˫��ջ-�ײ�ע
;    CBZ     R0, PendSV_Handler_Nosave		                    ; Skip register save the first time

;    SUBS    R0, R0, #0x20                                       ; Save remaining regs r4-11 on process stack
;    STM     R0, {R4-R11}

;    LDR     R1, =OSTCBCur                                       ; OSTCBCur->OSTCBStkPtr = SP;
;    LDR     R1, [R1]
;    STR     R0, [R1]                                            ; R0 is SP of process being switched out

                                                                ; At this point, entire context of process has been saved
;PendSV_Handler_Nosave
;    PUSH    {R14}                                               ; Save LR exc_return value
;    LDR     R0, =OSTaskSwHook                                   ; OSTaskSwHook();
;    BLX     R0
;    POP     {R14}

;    LDR     R0, =OSPrioCur                                      ; OSPrioCur = OSPrioHighRdy;
;    LDR     R1, =OSPrioHighRdy
;    LDRB    R2, [R1]
;    STRB    R2, [R0]

;    LDR     R0, =OSTCBCur                                       ; OSTCBCur  = OSTCBHighRdy;
;    LDR     R1, =OSTCBHighRdy
;    LDR     R2, [R1]
;    STR     R2, [R0]

;    LDR     R0, [R2]                                            ; R0 is new process SP; SP = OSTCBHighRdy->OSTCBStkPtr;
;    LDM     R0, {R4-R11}                                        ; Restore r4-11 from new process stack
;    ADDS    R0, R0, #0x20
;    MSR     PSP, R0                                             ; Load PSP with new process SP
;    ORR     LR, LR, #0x04                                       ; Ensure exception return uses process stack
;    CPSIE   I
;    BX      LR                                                  ; Exception return will restore remaining context


;/**************************************************************************************
;* �������: ItDisAll
;*
;* ��������: �ر��жϿ�����������ж�.
;*
;* ��    ��: None
;*
;* �� �� ֵ: None
;***************************************************************************************/

;ItDisAll
;    	CPSID   I
;    	BX      LR


;    	ALIGN
					
;        END
        
;/*********************** (C) COPYRIGHT 2010 Libraworks ************************/
