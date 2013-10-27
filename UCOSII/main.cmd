-c
-heap 		0x8000
-stack 		0x40000
-l 		rts6400.lib


MEMORY
{
	MSMC_SHARED: 	origin = 0x0c000000 length = 0x00400000
	DDR_SHARED: origin = 80000000h length = 20000000h 		/* intended for initialization */
	L2_SHARED:	origin = 800000h length = 0x00080000
	CORE0_L2SRAM: origin = 0x10800000  length = 0x00080000 /* intended for initialization */
	CORE1_L2SRAM: origin = 0x11800000  length = 0x00080000
	CORE2_L2SRAM: origin = 0x12800000  length = 0x00080000
	CORE3_L2SRAM: origin = 0x13800000  length = 0x00080000
	CORE4_L2SRAM: origin = 0x14800000  length = 0x00080000
	CORE5_L2SRAM: origin = 0x15800000  length = 0x00080000
	CORE6_L2SRAM: origin = 0x16800000  length = 0x00080000
	CORE7_L2SRAM: origin = 0x17800000  length = 0x00080000
}
SECTIONS
{
	.text:vecs 	> DDR_SHARED
	.text 		> DDR_SHARED	
	.data 		> DDR_SHARED
	.stack		> L2_SHARED
	.bss 		> DDR_SHARED
	.sysmem 	> DDR_SHARED
	.cinit 		> DDR_SHARED
	.cio		> L2_SHARED
	.const 		> DDR_SHARED
	.far 		> DDR_SHARED
}