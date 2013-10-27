
#ifndef _C66XX_CACHE_H
#define _C66XX_CACHE_H


/**************************** 引用部分 *********************************/
#include "c66xx_io.h"
#include "c66xx_perm.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

#define		L1D_DISABLE		0x0
#define		L1D_4K			0x1
#define		L1D_8K			0x2
#define		L1D_16K			0x3
#define		L1D_32K			0x4
#define		L1D_TOTAL		0x5

#define		L1DCFG			0x01840040
#define		L1DCC			0x01840044
#define		L1DWIBAR		0x01844030
#define		L1DWIWC			0x01844034
#define		L1DWBAR			0x01844040
#define		L1DWWC			0x01844044
#define		L1DIBAR			0x01844048
#define		L1DIWC			0x0184404c
#define		L1DWB			0x01845040
#define		L1DWBINV		0x01845044
#define		L1DINV			0x01845048

/* L1P Protection Registers */
#define		L1PMPPA_BASE	0x0184a640
#define		L1PMPPA_ID_REG(id)	(L1PMPPA_BASE + (id) * 0x4)

#define		L1DMPPA_BASE	0x0184ae40
#define		L1DMPPA_ID_REG(id)	(L1DMPPA_BASE + (id) * 0x4)

#define		L2MPPA_BASE		0x0184a200
#define		L2MPPA_ID_REG(id)	(L2MPPA_BASE + (id) * 0x4)

#define		L2CFG			0x01840000
#define		L2WBAR			0x01844000
#define		L2WWC			0x01844004
#define		L2WIBAR			0x01844010
#define		L2WIWC			0x01844014
#define		L2IBAR			0x01844018
#define		L2IWC			0x0184401c
#define		L2WB			0x01845000
#define		L2WBINV			0x01845004
#define		L2INV			0x01845008

#define		XPFCMD			0x08000300

#define 	DDR_MDR			0x21000000

#define		L1_DCACHE		0
#define		L1_PCACHE		1
#define		L2_CACHE		2

/* MEMORY ATTRIBUTE REGISTERS */


#define		PER_MAR_MEM_OFFSET		16777216	/* 0x01000000 */
#define		MAR_ATTR_ADDR_START(id)	(id * PER_MAR_MEM_OFFSET)

#define		MAR_BASE_ADDR			0x01848000
#define		MAR_ADDR(id)			(MAR_BASE_ADDR + 4 * (id))

/*
 * @brief
 *    BSP_DisableL1Dcache：关闭L1数据缓存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_DisableL1Dcache(void)
{
	unsigned long tmp;
	tmp = readl(L1DCFG);
	tmp &= ~(0x7 << 0);
	writel(tmp, L1DCFG);
	tmp = readl(L1DCFG);
	asm("	NOP 10");
}

/*
 * @brief
 *    BSP_PrefetchBufferInvalid：失效prefetch buffer中的所有entry，这个函数会导致所有预读失败。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_PrefetchBufferInvalid(void)
{
	unsigned long tmp;
	tmp = 0x1;
	writel(tmp, XPFCMD);
}

/*
 * @brief
 *    BSP_WriteMemBarrier：针对DDR的内存壁障，可以保证这个函数执行完后，之前的所有针对DDR的写操作都完成了。
 *    这个相当于ARM中的DMB，注意不是DSB，这个函数不保证任何在该函数之后的指令在该函数完成时还没有得到运行。
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */

static inline void BSP_WriteMemBarrier(void)
{
	unsigned long tmp;
	tmp = 0xdeadbeef;
	writel(tmp, DDR_MDR);
	tmp = readl(DDR_MDR);
}


/*
 * @brief
 *    BSP_DisableL2Cache：关闭L2缓存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_DisableL2Cache(void)
{
	unsigned long tmp;
	tmp = readl(L2CFG);
	tmp &= ~(0x7 << 0);
	writel(tmp, L2CFG);
	tmp = readl(L2CFG);
	asm("	NOP 10");
}

/*
 * @brief
 *    BSP_EnableL1Dcache：开启L1数据缓存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_EnableL1Dcache(int kind)
{
	unsigned long tmp;
	tmp = readl(L1DCFG);
	tmp &= ~(0x7 << 0);
	tmp |= (kind << 0);
	writel(tmp, L1DCFG);
	tmp = readl(L1DCFG);
	asm("	NOP 10");
}

/*
 * @brief
 *    BSP_FreezeL1Dcache：冻结L1数据缓存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_FreezeL1Dcache(void)
{
	unsigned long tmp;
	tmp = readl(L1DCC);
	tmp |=	0x1;
	writel(tmp, L1DCC);
	tmp = readl(L1DCC);
	asm ("	NOP 10");
}

/*
 * @brief
 *    BSP_WarmL1Dcache：将L1数据缓存从冻结状态中唤醒
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_WarmL1Dcache(void)
{
	unsigned long tmp,poper;
	tmp = readl(L1DCC);
	poper = (tmp & (0x1 << 16) ) >> 16;
	tmp |= (poper << 0);
	writel(tmp, L1DCC);
	tmp = readl(L1DCC);
	asm ("	NOP 10");
}

/*
 * @brief
 *    BSP_L1DInvalid：无效话L1数据缓存的所有Cache Line
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_L1DInvalid(void)
{
	unsigned long tmp;
	tmp = readl(L1DINV);
	tmp |= 0x1;
	writel(tmp, L1DINV);
}

/*
 * @brief
 *    BSP_L1DWriteback：将L1数据缓存的所有Cache Line写回内存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_L1DWriteback(void)
{
	unsigned long tmp;
	tmp = readl(L1DWB);
	tmp |= 0x1;
	writel(tmp, L1DWB);
}

/*
 * @brief
 *    BSP_L1DWritebackInvalidate：将L1数据缓存的所有Cache Line无效并写回内存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_L1DWritebackInvalidate(void)
{
	unsigned long tmp;
	tmp = readl(L1DWBINV);
	tmp |= 0x1;
	writel(tmp, L1DWBINV);
}

/*
 * @brief
 *    BSP_L1PSetProtection：设置L1P的内存保护
 * @param		aid, 允许的master id。
 * 				page_num, 页编号，L1P有16个page，每个page
 * 				perm, 权限类型
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_L1PSetProtection(unsigned int aid, unsigned int page_num, unsigned int perm)
{
	unsigned int tmp = 0;
	if (page_num > 15)
		return;
	if (aid > 5)
		tmp = (0x3 << 8)|(perm << 0);
	else
		tmp = (0x1 << (aid + 10))|(0x1 << 8)|((perm & 0x3f)<<0);

	writel(tmp, L1PMPPA_ID_REG(page_num));
}

/*
 * @brief
 *    BSP_L1DSetProtection：设置L1D的内存保护
 * @param		aid, 允许的master id。
 * 				page_num, 页编号，L1P有16个page，每个page
 * 				perm, 权限类型
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_L1DSetProtection(unsigned int aid, unsigned int page_num, unsigned int perm)
{
	unsigned int tmp = 0;
	if (page_num > 15)
		return;
	if (aid > 5)
		tmp = (0x3 << 8)|(perm << 0);
	else
		tmp = (0x1 << (aid + 10))|(0x1 << 8)|((perm & 0x3f)<<0);

	writel(tmp, L1DMPPA_ID_REG(page_num));
}

/*
 * @brief
 *    BSP_L2SetProtection：设置L2的内存保护
 * @param		aid, 允许的master id。
 * 				page_num, 页编号，L1P有16个page，每个page
 * 				perm, 权限类型
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static inline void BSP_L2SetProtection(unsigned int aid, unsigned int page_num, unsigned int perm)
{
	unsigned int tmp = 0;
	if (page_num > 31)
		return;
	if (aid > 5)
		tmp = (0x3 << 8)|(perm << 0);
	else
		tmp = (0x1 << (aid + 10))|(0x1 << 8)|((perm & 0x3f)<<0);

	writel(tmp, L2MPPA_ID_REG(page_num));
}

static inline unsigned long BSP_GetPageAttribute(unsigned long addr)
{
	return readl(MAR_ADDR(addr / PER_MAR_MEM_OFFSET));
}

static inline void BSP_SetPageAttribute(unsigned long addr, unsigned long attr)
{
	writel(attr, MAR_ADDR(addr/ PER_MAR_MEM_OFFSET));
}

/* L1&L2 */

#define 	CACHE_L1P	0
#define		CACHE_L1D	1
#define		CACHE_L2	2
#define		CACHE_SRAM	3

struct cache {
	unsigned int kind;
	unsigned char level;
	unsigned char isDataCache;
	unsigned char isSram;
	unsigned char pageNum;
	unsigned int pageSize;
	void (*disableCache)(void);
	void (*setProtection)(unsigned int, unsigned int, unsigned int);
};

void BSP_InitCacheController(struct cache *, struct cache *, struct cache *);

#endif /* CACHE_H_ */
