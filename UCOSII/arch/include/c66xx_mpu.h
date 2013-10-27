

#ifndef _C66XX_MPU_H
#define _C66XX_MPU_H
/**************************** 引用部分 *********************************/


/**************************** 声明部分 *********************************/


#include "c66xx_itbl.h"
#include "c66xx_io.h"
#include "c66xx_memarea.h"
#include "rb_tree.h"
#include "list.h"
/**************************** 定义部分 *********************************/
/*
 *  C6678 Distribute Memory Protection Unit
 *
 *  MPU0 -> Main CFG SCR    [0x01d00000, 0x026203ff]
 *  MPU1 -> QM_SS DATA PORT [0x34000000, 0x340bffff]
 *  MPU2 -> QM_SS CFG PORT  [0x02a00000, 0x02abffff]
 *  MPU3 -> Semaphore		[0x02640000, 0x026407ff]
 *
 *  Corepac.XMC -> DDR&MSMC SRAM
 *
 */


#define MPU_BASE_ADDR(id)				(0x02360000 + (id) * 0x8000)

#define MPU_REV_REG(id)					(MPU_BASE_ADDR(id) + 0x0)
#define MPU_CONF_REG(id)				(MPU_BASE_ADDR(id) + 0x4)
#define MPU_INT_RAW_STATE_REG(id)		(MPU_BASE_ADDR(id) + 0x10)
#define MPU_INT_ENABLED_STATE_REG(id)	(MPU_BASE_ADDR(id) + 0x14)
#define MPU_INT_ENABLE_REG(id)			(MPU_BASE_ADDR(id) + 0x18)
#define MPU_INT_ENABLE_CLEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x1c)
#define MPU_EOI_REG(id)					(MPU_BASE_ADDR(id) + 0x20)

#define MPU_PROG0_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x200)
#define MPU_PROG0_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x204)
#define MPU_PROG0_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x208)

#define MPU_012_PROG1_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x210)
#define MPU_012_PROG1_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x214)
#define MPU_012_PROG1_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x218)

#define MPU_012_PROG2_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x220)
#define MPU_012_PROG2_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x224)
#define MPU_012_PROG2_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x228)

#define MPU_012_PROG3_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x230)
#define MPU_012_PROG3_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x234)
#define MPU_012_PROG3_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x238)

#define MPU_012_PROG4_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x240)
#define MPU_012_PROG4_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x244)
#define MPU_012_PROG4_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x248)

#define MPU_02_PROG5_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x250)
#define MPU_02_PROG5_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x254)
#define MPU_02_PROG5_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x258)

#define MPU_02_PROG6_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x260)
#define MPU_02_PROG6_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x264)
#define MPU_02_PROG6_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x268)

#define MPU_02_PROG7_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x270)
#define MPU_02_PROG7_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x274)
#define MPU_02_PROG7_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x278)

#define MPU_02_PROG8_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x280)
#define MPU_02_PROG8_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x284)
#define MPU_02_PROG8_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x288)

#define MPU_02_PROG9_MPSAR_REG(id)		(MPU_BASE_ADDR(id) + 0x290)
#define MPU_02_PROG9_MPEAR_REG(id)		(MPU_BASE_ADDR(id) + 0x294)
#define MPU_02_PROG9_MPPA_REG(id)		(MPU_BASE_ADDR(id) + 0x298)

#define MPU_02_PROG10_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2a0)
#define MPU_02_PROG10_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2a4)
#define MPU_02_PROG10_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x2a8)

#define MPU_02_PROG11_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2b0)
#define MPU_02_PROG11_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2b4)
#define MPU_02_PROG11_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x2b8)

#define MPU_02_PROG12_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2c0)
#define MPU_02_PROG12_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2c4)
#define MPU_02_PROG12_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x2c8)

#define MPU_02_PROG13_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2d0)
#define MPU_02_PROG13_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2d4)
#define MPU_02_PROG13_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x2d8)

#define MPU_02_PROG14_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2e0)
#define MPU_02_PROG14_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2e4)
#define MPU_02_PROG14_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x2e8)

#define MPU_02_PROG15_MPSAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2f0)
#define MPU_02_PROG15_MPEAR_REG(id)	(MPU_BASE_ADDR(id) + 0x2f4)
#define MPU_02_PROG15_MPPA_REG(id)	(MPU_BASE_ADDR(id) + 0x2f8)


#define MPU_FLT_ADDR_REG(id)		(MPU_BASE_ADDR(id) + 0x300)
#define MPU_FLT_STAT_REG(id)		(MPU_BASE_ADDR(id) + 0x304)
#define MPU_FLT_CLR_REG(id)			(MPU_BASE_ADDR(id) + 0x308)


/* PID for all masters */

#define COREPAC0_PID	0
#define COREPAC1_PID	1
#define	COREPAC2_PID	2
#define	COREPAC3_PID	3
#define	COREPAC4_PID	4
#define	COREPAC5_PID	5
#define COREPAC6_PID	6
#define COREPAC7_PID	7
#define NETCORE_DMA_PID	8
#define	SRIO_M_PID		9
#define	QM_PKTDMA_PID	10
#define	PCIE_PID		11
#define	DAP_PID			12
#define HLINK0_PID		13
#define HLINK1_PID		14
#define TSIP_PID		15





/* L1 L2 XMC Memory protection */
/* L1&L2 is defined in c66xx_cache */

#define MPLK0		0x0184ad00
#define	MPLK1		0x0184ad04
#define	MPLK2		0x0184ad08
#define	MPLK3		0x0184ad0c

#define	MPLKCMD		0x0184ad10
#define	MPLKSTAT	0x0184ad14
#define MAX_CORE_NUM 8

#define MPU0		0
#define MPU1		1
#define MPU2		2
#define MPU3		3
#define XMC			4

/* XMC registers */

#define XMC_REG_BASE 0x08000000
#define XMPAXL(id)	(XMC_REG_BASE + 8 * (id))
#define XMPAXH(id)	(XMC_REG_BASE + 8 * (id))
#define XMPFAR		(0x08000200)
#define XMPFSR		(0x08000204)
#define XMPFCR		(0x08000208)
#define MDMAARBX	(0x08000208)

#define XPFACS		(0x08000304)
#define XPFAC_BASE	(0x08000310)
#define XPFAC(id)	(XPFAC_BASE + 4 * (id))
#define XPFADDR_BASE	(0x08000400)
#define XPFADDR(id)	(XPFADDR_BASE + 4 * (id))


/* lock and unlock the Memory protection registers */
/*

 How To Lock The Key
1. Write a 1 to the KEYR field of the MPLKCMD register. This resets the internal
status for the MPLK0 through MPLK3 registers.
2. Write the key to MPLK0 through MPLK3.All four registers must be written
exactly once. They may be written in any order.
3. Write a 1 to the LOCK field of the MPLKCMD register. This engages the lock.

 How To Unlock The Key
1. Write a 1 to the KEYR field in the MPLKCMD register. This resets some internal
status for the MPLK0 through the MPLK3 registers.
2. Write the unlock key to MPLK0 through the MPLK3 registers. The hardware
compares the written value with the stored key value. Software must write to all
four registers exactly once. The writes can arrive in any order.
3. Write a 1 to the UNLOCK field in the MPLKCMD register. If the key written in
step 2 matches the stored key, the hardware disengages the lock. If the key written
in step 2 does not match, the hardware signals an exception. The hardware
reports the fault address as the address of the MPLKCMD register.


 */

static inline void __BSP_L1L2XMCLock(void)
{
	/* Write 1 to MPLKCMD.KEYR */
	writel((0x1 << 2), MPLKCMD);

	/* Write key */
	writel(0xdeadbeef, MPLK0);
	writel(0xdeadbeef, MPLK1);
	writel(0xdeadbeef, MPLK2);
	writel(0xdeadbeef, MPLK3);

	/* Write 1 to MPLKCMD.LOCK */
	writel((0x1 << 1), MPLKCMD);
}

static inline void __BSP_L1L2XMCUnlock(void)
{
	/* Write 1 to MPLKCMD.KEYR */
	writel((0x1 << 2), MPLKCMD);

	/* Write keys back to MPLK0 through MPLK3 */
	writel(0xdeadbeef, MPLK0);
	writel(0xdeadbeef, MPLK1);
	writel(0xdeadbeef, MPLK2);
	writel(0xdeadbeef, MPLK3);

	/* Unlock the key */
	writel((0x1 << 0), MPLKCMD);
}



static inline int BSP_GetSizeFromSeg(int seg_size)
{
	switch (seg_size) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		return 0;
	case 11:
		return 4;
	case 12:
		return 8;
	case 13:
		return 16;
	case 14:
		return 32;
	case 15:
		return 64;
	case 16:
		return 128;
	case 17:
		return 256;
	case 18:
		return 512;
	case 19:
		return 1024;
	case 20:
		return 2048;
	case 21:
		return 4096;
	case 22:
		return 8*1024;
	case 23:
		return 16*1024;
	case 24:
		return 32*1024;
	case 25:
		return 64*1024;
	case 26:
		return 128*1024;
	case 27:
		return 256*1024;
	case 28:
		return 512*1024;
	case 29:
		return 1024*1024;
	case 30:
		return 2048*1024;
	case 31:
		return 4096*1024;
	default:
		return -1;
	}
}

/* including mpu0 to mpu4 */
struct mpu {
	/* information of mpu */
	unsigned int id;
	/* information in CONFIG register */
	unsigned char address_alignment;	/* size in KB */
	unsigned char num_fix_address;
	unsigned char num_prog_address;
	unsigned char num_support_aid;
	unsigned int assume_allowed;

	/* link all the memory area that have set protection */
	struct rb_root area_root;

};

void BSP_InitMpu(void);
int BSP_SetMemAreaNewPerm(unsigned int addr, unsigned perm, unsigned int aid);
struct mem_area *BSP_GetXmpMemAreaByIndx(unsigned int indx);
int BSP_GetMemAreaPerm(unsigned int addr, unsigned long *aid, unsigned long *perm);
int BSP_CreatNewXMPArea(unsigned long baddr, unsigned long remap_addr, unsigned long seg_size,
		unsigned long seg_id, unsigned long aid, unsigned perm);
struct mem_area *BSP_GetFreeXmpMemArea(int *indx);
int BSP_FreeXmpMemArea(unsigned long indx);
int BSP_RemoveXMPArea(unsigned long indx);


#endif /* _C66XX_MPU_H */
