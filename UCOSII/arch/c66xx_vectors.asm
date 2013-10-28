;************************************************************************
;*				电子科技大学嵌入式软件工程中心 版权所有
;* 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
;***********************************************************************/
;
; $Log: c66xx_vectors,v $
; Revision 1.0  2013/06/13 13:13:00
; 创建文件

; @file 	c66xx_vectors.h
; @brief
;	<li>功能：定义了C66XX体系相关的中断向量表。</li>
; @author	Insswer
; @date 	2013-06-13
; <p>部门：


;------------------------------------------------------------------------------
; Global symbols defined here and exported out of this file
;------------------------------------------------------------------------------
    .global _vectors

;------------------------------------------------------------------------------
; Global symbols referenced in this file but defined somewhere else.
; Remember that your interrupt service routines need to be referenced here.
;------------------------------------------------------------------------------
    .ref _c_int00
    .ref c_int4
    .ref c_int5
    .ref c_int6
    .ref c_int7
    .ref c_int8
    .ref c_int9
    .ref c_int10
    .ref c_int11
    .ref c_int12
    .ref c_int13
    .ref c_int14
    .ref c_int15

;------------------------------------------------------------------------------
; This is a macro that instantiates one entry in the interrupt service table.
;------------------------------------------------------------------------------
VEC_ENTRY .macro addr
	STW	  B0,	*--B15
	MVKL  addr, B0
	MVKH  addr, B0
	B	  B0
	LDW	  *B15++,	B0
	NOP   2
    NOP
    NOP
   .endm


;------------------------------------------------------------------------------
; This is a dummy interrupt service routine used to initialize the IST.
;------------------------------------------------------------------------------
_vec_dummy:
    b _vec_dummy
    nop 4
    b _vec_dummy
    nop
    nop
    nop
    nop
    nop


;******************************************************************************
;                       Vector Table
;
; ====== vectors in use ========
;  Plug in the entry point at the ISR function in the interrupt vector table
;
; ====== unused ========
;  plug inifinite loop -- with nested branches to
;  disable interrupts -- for all undefined vectors
;
;******************************************************************************
        .sect ".text:vecs"
        .align 1024
_vectors:
_vector0:   VEC_ENTRY _c_int00    ;RESET
_vector1:   VEC_ENTRY _vec_dummy  ;NMI
_vector2:   VEC_ENTRY _vec_dummy  ;RSVD
_vector3:   VEC_ENTRY _vec_dummy
_vector4:   VEC_ENTRY c_int4
_vector5:   VEC_ENTRY c_int5
_vector6:   VEC_ENTRY c_int6
_vector7:   VEC_ENTRY c_int7
_vector8:   VEC_ENTRY c_int8
_vector9:   VEC_ENTRY c_int9
_vector10:  VEC_ENTRY c_int10
_vector11:  VEC_ENTRY c_int11
_vector12:  VEC_ENTRY c_int12
_vector13:  VEC_ENTRY c_int13
_vector14:  VEC_ENTRY c_int14
_vector15:  VEC_ENTRY c_int15

		.end
