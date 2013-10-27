;************************************************************************
;*				电子科技大学嵌入式软件工程中心 版权所有
;* 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
;***********************************************************************/
;
; $Log: c66xx_itbl,v $
; Revision 1.0  2013/06/13 13:13:00
; 创建文件

; @file 	c66xx_itbl.c
; @brief
; <li>功能：定义了C66XX体系相关的中断向量表。</li>
; @author	Insswer
; @date 	2013-06-13
; <p>部门：


	.global c_int4
    .global c_int5
    .global c_int6
    .global c_int7
    .global c_int8
    .global c_int9
    .global c_int10
    .global c_int11
    .global c_int12
    .global c_int13
    .global c_int14
    .global c_int15

	.ref BSP_IsrHandlerEntry
	.ref BSP_EnableInt
	.ref OSTCBCur
; For C66XX save CSR, IER, IRP Entry Point, B3, B0 - B2, B4 - B14, A0 - 15

isrSaveContext .macro
		MVC		CSR,  B17
		STW		B17, *B15--[2]
		MVC		IRP,  B16
		STW		B16, *B15--[2]
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

		MVKL	OSTCBCur, A4
		MVKH	OSTCBCur, A4
		LDW		*A4,	A5
		NOP		4
		STW		B15,	*A5		; Store SP into OSTCBStkPtr



        .endm





isrResumeContext	.macro
		; if no need to sw ctx restore all the register
		MVKL	OSTCBCur, A4
		MVKH	OSTCBCur, A4
		LDW		*A4,	A5
		NOP		4
		LDW		*A5,	B15
		NOP		4

		LDW     *++B15[2], A15
		NOP		4
		LDW     *++B15[2], A14
		NOP		4
		LDW     *++B15[2], A13
		NOP		4
		LDW     *++B15[2], A12
		NOP		4
		LDW     *++B15[2], A11
		NOP		4
		LDW     *++B15[2], A10
		NOP		4
		LDW     *++B15[2], A9
		NOP		4
		LDW     *++B15[2], A8
		NOP		4
		LDW     *++B15[2], A7
		NOP		4
		LDW     *++B15[2], A6
		NOP		4
		LDW     *++B15[2], A5
		NOP		4
		LDW     *++B15[2], A4
		NOP		4
		LDW     *++B15[2], A3
		NOP		4
		LDW     *++B15[2], A2
		NOP		4
		LDW     *++B15[2], A1
		NOP		4
		LDW     *++B15[2], A0
		NOP		4
		LDW     *++B15[2], B14
		NOP		4
	    LDW     *++B15[2], B13
	    NOP		4
		LDW     *++B15[2], B12
		NOP		4
		LDW     *++B15[2], B11
		NOP		4
		LDW     *++B15[2], B10
		NOP		4
		LDW     *++B15[2], B9
		NOP		4
		LDW     *++B15[2], B8
		NOP		4
		LDW     *++B15[2], B7
		NOP		4
		LDW     *++B15[2], B6
		NOP		4
		LDW     *++B15[2], B5
		NOP		4
		LDW     *++B15[2], B4
		NOP		4
		LDW     *++B15[2], B2
		NOP		4
		LDW     *++B15[2], B1
		NOP		4
		LDW     *++B15[2], B0
		NOP		4
		LDW     *++B15[2], B3
		NOP		4
		LDW     *++B15[2], B16
		NOP		4
		MVC		B16,	   IRP
		LDW		*++B15[2], B17
		NOP		4
		MVC		B17,	   CSR
		B		IRP
		NOP		5
        .endm



c_int4:
	isrSaveContext
	MVK		0x4,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int5:
	isrSaveContext
	MVK		0x5,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int6:
	isrSaveContext
	MVK		0x6,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int7:
	isrSaveContext
	MVK		0x7,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int8:
	isrSaveContext
	MVK		0x8,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int9:
	isrSaveContext
	MVK		0x9,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext


c_int10:
	isrSaveContext
	MVK		0xa,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int11:
	isrSaveContext
	MVK		0xb,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int12:
	isrSaveContext
	MVK		0xc,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int13:
	isrSaveContext
	MVK		0xd,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int14:
	isrSaveContext
	MVK		0xe,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext

c_int15:
	isrSaveContext
	MVK		0xf,	A4
	CALLP		BSP_IsrHandlerEntry,B3
	isrResumeContext



	.end
