/*
 * c6678_cache.c
 *
 *  Created on: 2013-10-7
 *      Author: insswer
 */
#include "c66xx_cache.h"
#include <stdlib.h>

static struct cache L1_Dcache, L1_Pcache, L2_Cache;

/*
 * @brief
 *    __BSP_DisableL2Cache：关闭L2缓存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void __BSP_DisableL2Cache(void)
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
 *    __BSP_DisableL1Dcache：关闭L1数据缓存
 * @param		无。
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void __BSP_DisableL1Dcache(void)
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
 *    __BSP_L1PSetProtection：设置L1P的内存保护
 * @param		aid, 允许的master id。
 * 				page_num, 页编号，L1P有16个page，每个page
 * 				perm, 权限类型
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void __BSP_L1PSetProtection(unsigned int aid, unsigned int page_num, unsigned int perm)
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
 *    __BSP_L1DSetProtection：设置L1D的内存保护
 * @param		aid, 允许的master id。
 * 				page_num, 页编号，L1P有16个page，每个page
 * 				perm, 权限类型
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void __BSP_L1DSetProtection(unsigned int aid, unsigned int page_num, unsigned int perm)
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
 *    __BSP_L2SetProtection：设置L2的内存保护
 * @param		aid, 允许的master id。
 * 				page_num, 页编号，L1P有16个page，每个page
 * 				perm, 权限类型
 * @param[in]	无。
 * @param[out]	无。
 * @returns:	无。
 */
static void __BSP_L2SetProtection(unsigned int aid, unsigned int page_num, unsigned int perm)
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


/*
 * @brief
 *    BSP_InitCacheController：软件初始化Cache控制器
 * @param		struct Cache *L1_d, struct Cache *L1_p, struct Cache *L2
 * @param[in]	无。
 * @param[out]	struct Cache *L1_d, struct Cache *L1_p, struct Cache *L2
 * @returns:	无。
 */
void BSP_InitCacheController(struct cache *L1_d, struct cache *L1_p, struct cache *L2)
{
	L1_Dcache.isSram = 0;
	L1_Dcache.isDataCache = 1;
	L1_Dcache.kind = L1_DCACHE;
	L1_Dcache.level = 1;
	L1_Dcache.pageNum = 16;
	L1_Dcache.pageSize = 2;
	L1_Dcache.disableCache = __BSP_DisableL1Dcache;
	L1_Dcache.setProtection = __BSP_L1DSetProtection;

	L1_Pcache.isSram = 0;
	L1_Pcache.isDataCache = 0;
	L1_Pcache.kind = L1_PCACHE;
	L1_Pcache.level = 1;
	L1_Pcache.pageNum = 16;
	L1_Pcache.pageSize = 2;
	L1_Pcache.disableCache = NULL;
	L1_Pcache.setProtection = __BSP_L1PSetProtection;

	L2_Cache.isSram = 0;
	L2_Cache.isDataCache = 1;
	L2_Cache.kind = L2_CACHE;
	L2_Cache.level = 2;
	L2_Cache.pageNum = 32;
	L2_Cache.pageSize = 16;
	L2_Cache.disableCache = __BSP_DisableL2Cache;
	L2_Cache.setProtection = __BSP_L2SetProtection;

	L1_d = &L1_Dcache;
	L1_p = &L1_Pcache;
	L2 = &L2_Cache;
}
