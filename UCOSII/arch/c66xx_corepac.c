/************************************************************************
 *				鐢靛瓙绉戞妧澶у宓屽叆寮忚蒋浠跺伐绋嬩腑蹇� 鐗堟潈鎵�鏈�
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_corepac,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 鍒涘缓鏂囦欢
*/

/*
 * @file 	c66xx_corepac.c
 * @brief
 *	<li>鍔熻兘锛氬畾涔変簡C66XX浣撶郴DSP Corepac妯″潡銆�</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>閮ㄩ棬锛�
*/

/**************************** 寮曠敤閮ㄥ垎 *********************************/

#include "c66xx_corepac.h"
#include "c66xx_io.h"
#include "c66xx_debug.h"
#include "c66xx_mpu.h"

/**************************** 澹版槑閮ㄥ垎 *********************************/


/**************************** 瀹氫箟閮ㄥ垎 *********************************/

/*
 * @brief
 *    GetEventFlag锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝鑾峰緱鎸囧畾浜嬩欢鐨勭姸鎬併��
 * @param		efnum锛氫簨浠跺彿銆�
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:    0锛屼簨浠舵病鏈夊彂鐢熴��
 * <p>			1锛屼簨浠跺彂鐢熶簡銆�
 */
static int GetEventFlag(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_FLAG_REG(num));
	if (tmp & (1 << indx))
		return 1;
	else
		return 0;
}

/*
 * @brief
 *    SetEventFlag锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝浜哄伐璁剧疆缁欏畾浜嬩欢鐨勭姸鎬併��
 * @param		efnum锛氫簨浠跺彿銆�
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:    涓婁竴娆¤缃簨浠跺墠锛岃浜嬩欢鐨勭姸鎬併��
 */
static int SetEventFlag(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;
	int last;

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_FLAG_REG(num));
	if (tmp & (1 << indx))
		last = 1;
	else
		last = 0;

	tmp = readl(COREPAC_EVENT_SET_REG(num));
	tmp |= (1 << indx);
	writel(tmp, COREPAC_EVENT_SET_REG(num));

	return last;
}

/*
 * @brief
 *    ClrEventFlag锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝浜哄伐娓呴櫎缁欏畾浜嬩欢鐨勭姸鎬併��
 * @param		efnum锛氫簨浠跺彿銆�
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:    涓婁竴娆℃竻闄や簨浠跺墠锛岃浜嬩欢鐨勭姸鎬併��
 */
static int ClrEventFlag(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;
	int last;

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_FLAG_REG(num));
	if (tmp & (1 << indx))
		last = 1;
	else
		last = 0;

	tmp = readl(COREPAC_EVENT_CLEAR_REG(num));
	tmp |= (1 <<  indx);
	writel(tmp, COREPAC_EVENT_CLEAR_REG(num));

	return last;
}

/*
 * @brief
 *    UnmaskCombineEvent锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝瑙ｉ櫎缁欏畾缁勫悎浜嬩欢鐨勫睆钄界姸鎬併��
 * @param		efnum锛氱粍鍚堜簨浠跺彿銆�
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	鏃犮��
 */
static void UnmaskCombineEvent(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;

	if (efnum < 4) {
		DEBUG_DEV("bad efnum, you cannot combine 0-3 itself\n");
		return;
	}

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_MASK_REG(num));
	tmp &= ~(1 << indx);
	writel(tmp, COREPAC_EVENT_MASK_REG(num));

}

/*
 * @brief
 *    MaskCombineEvent锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝灞忚斀缁欏畾缁勫悎浜嬩欢銆�
 * @param		efnum锛氱粍鍚堜簨浠跺彿銆�
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	鏃犮��
 */
static void MaskCombineEvent(int efnum)
{
	int indx;
	int num;
	unsigned long tmp;

	if (efnum < 4) {
		DEBUG_DEV("bad efnum, you cannot combine 0-3 itself\n");
		return;
	}

	indx = efnum % 32;
	num = efnum / 32;

	tmp = readl(COREPAC_EVENT_MASK_REG(num));
	tmp |= (1 << indx);
	writel(tmp, COREPAC_EVENT_MASK_REG(num));
}

/*
 * @brief
 *    MaskCombineReadEvent锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝鎵惧埌鍙戠敓鐨勭粍鍚堜腑鏂��
 * @param		efnum锛氱粍鍚堜簨浠跺彿銆�
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	杩斿洖缁忚繃澶勭悊鐨勭粍鍚堜腑鏂姸鎬佸瘎瀛樺櫒鐨勫�硷紝鏌愪綅涓�1琛ㄧず璇ヤ綅瀵瑰簲鐨勭粍鍚堜簨浠跺凡缁忓彂鐢熴�傛煇浣嶄负0琛ㄧず
 * <p>			璇ヤ綅瀵瑰簲鐨勭粍鍚堜簨浠惰涔堟病鍙戠敓瑕佷箞琚睆钄戒簡銆�
 */
static unsigned long MaskCombineReadEvent(int efnum)
{
	unsigned long tmp, tmp1, tmp2;
	if (efnum > 3) {
		DEBUG_DEV("bad efnum\n");
		return 0;
	}
	tmp1 = readl(COREPAC_MASK_EVENT_FLAG_REG(efnum));
	tmp2 = readl(COREPAC_EVENT_MASK_REG(efnum));

	tmp = 0;
	tmp |= tmp1;
	tmp &= ~(tmp2);

	return tmp;
}

/*
 * @brief
 *    MaskCombineReadEvent锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝鎵惧埌鍙戠敓鐨勭粍鍚堜腑鏂��
 * @param		efnum锛氫簨浠跺彿銆�
 * 				dspInt: DSP涓柇鍙枫��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	杩斿洖涓婁竴娆spInt瀵瑰簲鐨別fnum銆�
 */
static int MapDspInt(int efnum, int dspInt)
{
	int indx;
	int num;
	unsigned long tmp;
	unsigned long last;

	if (dspInt < 4 || dspInt > 15) {
		DEBUG_DEV("bad dsp_int\n");
		return -1;
	}

	if (efnum > 128) {
		DEBUG_DEV("bad efnum \n");
		return -1;
	}

	indx = dspInt % 4;
	num = dspInt / 4 - 1;

	tmp = readl(COREPAC_INT_MUX_REG(num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));
	tmp &= ~(0xff << (indx * 8));
	tmp |= (efnum << (indx * 8));
	writel(tmp, COREPAC_INT_MUX_REG(num));
	DEBUG_DEV("map %d to dsp %d, in register 0x%08x\n",efnum, dspInt, COREPAC_INT_MUX_REG(num));
	DEBUG_DEV("COREPAC_INT_MUX_REG(%d) is now 0x%08x\n",num, readl(COREPAC_INT_MUX_REG(num)));
	return (int)last;
}

/*
 * @brief
 *    MappedEventGet锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝杩斿洖缁欏畾DSP涓柇鏄犲皠鐨勪簨浠跺彿銆�
 * @param		dspInt锛�    DSP涓柇鍙枫��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	缁欏畾DSP涓柇鎵�瀵瑰簲鐨勪簨浠跺彿銆�
 */
static int  MappedEventGet(int dspInt)
{
	int indx;
	int num;
	unsigned long tmp;
	unsigned long last;

	if (dspInt < 4 || dspInt > 15) {
		DEBUG_DEV("bad dsp_int\n");
		return -1;
	}

	indx = dspInt % 4;
	num = dspInt / 4 - 1;

	tmp = readl(COREPAC_INT_MUX_REG(num));
	last = ((tmp & (0xff << (indx * 8))) >> (indx * 8));
	return (int)last;
}


/*
 * @brief
 *    MappedDspIntsGet锛氬唴閮ㄤ娇鐢ㄥ嚱鏁帮紝杩斿洖鎵�鏈塂SP涓柇鎵�瀵瑰簲鐨勪簨浠跺彿銆�
 * @param		array锛�    涓�涓暟缁勶紝璇ユ暟缁勭敤浜庝繚瀛樿緭鍑恒��
 * @param[in]	鏃犮��
 * @param[out]	array:	涓�涓暟缁勶紝璇ユ暟缁勭敤浜庝繚瀛樿緭鍑恒��
 * @returns:	鏃犮��
 */
static void MappedDspIntsGet(int *array)
{
	int i;

	for (i = 4;i < 16; i++)
		array[i-4] = MappedEventGet(i);

}

/* C66XX鐨凜orepac鎺у埗缁撴瀯浣� */
static struct CorepacIntFuncSets CorepacOperations = {
		.corepac_get_event_flag = GetEventFlag,
		.corepac_set_event_flag = SetEventFlag,
		.corepac_clr_event_flag = ClrEventFlag,
		.corepac_mask_combine_event = MaskCombineEvent,
		.corepac_unmask_combine_event = UnmaskCombineEvent,
		.corepac_map_dsp_int = MapDspInt,
		.corepac_mapped_dsp_ints_get = MappedDspIntsGet,
		.corepac_mapped_event_get = MappedEventGet,
		.corepac_mask_combine_read_event = MaskCombineReadEvent,
};

/* 姣忎釜CPU閮芥湁鐨凜orepac涓柇鎺у埗鍣ㄧ殑瀹炰綋 */
static struct CorepacIntDev PerCpuCorepacIntDevices[MAX_CORE_NUM];
static struct Corepac PerCpuCorepac[MAX_CORE_NUM];


/*
 * @brief
 *    BSP_GetCurCorepacIntc锛氳繑鍥炴墽琛岃鍑芥暟瀵瑰簲鏍哥殑Corepac涓柇鎺у埗鍣ㄧ殑瀹炰緥銆�
 * @param		鏃犮��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	struct CorepacIntDev *锛氭墽琛岃鍑芥暟瀵瑰簲鏍哥殑Corepac涓柇鎺у埗鍣ㄧ殑瀹炰緥銆�
 */
struct CorepacIntDev *BSP_GetCurCorepacIntc(void)
{
	int id = BSP_GetCoreId();
	return &PerCpuCorepacIntDevices[id];
}

/*
 * @brief
 *    BSP_GetCurCorepac锛氳繑鍥炴墽琛岃鍑芥暟鐨凜orepac瀵硅薄鎸囬拡銆�
 * @param		鏃犮��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	struct Corepac *锛氭墽琛岃鍑芥暟瀵瑰簲鏍哥殑Corepac鎸囬拡銆�
 */
struct Corepac *BSP_GetCurCorepac(void)
{
	int id = BSP_GetCoreId();
	return &PerCpuCorepac[id];
}

/*
 * @brief
 *    BSP_InitCurCorepac锛氳蒋浠跺垵濮嬪寲鎵ц璇ュ嚱鏁板搴旂殑Corepac涓柇鎺у埗鍣ㄧ殑瀹炰緥銆�
 * @param		鏃犮��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	鏃犮��
 */
void	BSP_InitCurCorepac(void)
{
	int i = BSP_GetCoreId();
	PerCpuCorepacIntDevices[i].core_id = i;
	PerCpuCorepacIntDevices[i].pFunc_sets = &CorepacOperations;
	PerCpuCorepac[i].core_id = i;
	PerCpuCorepac[i].corepac_int_device = &PerCpuCorepacIntDevices[i];
	DEBUG_DEV("found corepac %d \n",i);
}

/*
 * @brief
 *    BSP_GetCurEvent锛氳繑鍥炵粰瀹欴SP涓柇鍙峰搴旂殑褰撳墠Corepac涓柇鎺у埗鍣ㄦ槧灏勭殑涓柇浜嬩欢鍙枫��
 * @param		dspInt锛氱粰瀹欴SP涓柇鍙枫��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	缁欏畾DSP涓柇褰撳墠鏄犲皠鐨勪腑鏂簨浠跺彿銆�
 */
int BSP_GetCurEvent(int dspInt)
{
	struct CorepacIntDev *pCur_corepac_int_dev = BSP_GetCurCorepacIntc();
	if (pCur_corepac_int_dev->pFunc_sets == NULL) {
		DEBUG_DEV("current corepac's operations set is NULL!\n");
		return -1;
	}

	int efnum = pCur_corepac_int_dev->pFunc_sets->corepac_mapped_event_get(dspInt);
	return efnum;

}

/*
 * @brief
 *    FindFirstBitZero32锛氬唴閮ㄥ嚱鏁帮紝璇ュ嚱鏁扮敤浜庤繑鍥炰竴涓�32浣嶆暟鐨勫墠瀵奸浂銆�
 * @param		tmp锛氱粰瀹�32浣嶆暟鐩��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	-1锛氳32浣嶆暟姣忎綅鍏ㄤ负0銆�
 * <p>			鍏朵粬锛氱涓�涓�1浣嶆墍鍦ㄧ殑浣嶆暟銆�
 */
static int FindFirstBitZero32(unsigned long tmp)
{
	int i;
	for (i = 0;i < 32;i++) {
		if (tmp & (1 << i)) {
			return i;
		} else
			continue;
	}
	return -1;

}



/*
 * @brief
 *    BSP_GetCurCombineState锛氳繑鍥炵涓�涓彂鐢熺殑缁勫悎涓柇鐨勪腑鏂彿銆�
 * @param		efnum锛氬摢涓�涓粍鍚堜腑鏂殑涓柇鍙枫��
 * @param[in]	鏃犮��
 * @param[out]	鏃犮��
 * @returns:	-1锛氳缁勫悎涓柇娌℃湁涓柇鍙戠敓鎴栧弬鏁伴敊璇��
 * <p>			鍏朵粬锛氬彂鐢熺殑绗竴涓粍鍚堜腑鏂殑涓柇鍙枫��
 */
int	 BSP_GetCurCombineState(int efnum)
{
	int num;
	unsigned long tmp;
	int kind;
	struct CorepacIntDev *pCur_corepac_int_dev = BSP_GetCurCorepacIntc();

	if (efnum > 3) {
		DEBUG_DEV("only event0-3 can be combined by other events\n");
		return -1;
	}

	if (pCur_corepac_int_dev->pFunc_sets == NULL) {
		DEBUG_DEV("current corepac's operations set is NULL!\n");
		return -1;
	}

	num = efnum / 32;

	tmp = pCur_corepac_int_dev->pFunc_sets->corepac_mask_combine_read_event(efnum);

	kind = FindFirstBitZero32(tmp);

	if (kind == -1)
		return -1;
	else
		return ((32 * num) + kind);

}

/*
 * @brief
 *    BSP_GetCurCombineAllState锛氳繑鍥炵粰瀹氱粍鍚堜腑鏂腑鍙戠敓鐨勬墍鏈変腑鏂儏鍐点��
 * @param		efnum锛氬摢涓�涓粍鍚堜腑鏂殑涓柇鍙枫��
 * 				array锛氳緭鍑哄弬鏁帮紝鐢ㄤ簬琛ㄧず璇ョ粍鍚堜腑鏂腑鍙戠敓鐨勬墍鏈変腑鏂儏鍐点��
 * @param[in]	鏃犮��
 * @param[out]	array锛氳緭鍑哄弬鏁帮紝鐢ㄤ簬琛ㄧず璇ョ粍鍚堜腑鏂腑鍙戠敓鐨勬墍鏈変腑鏂儏鍐点��
 * @returns:	-1锛氳缁勫悎涓柇娌℃湁涓柇鍙戠敓鎴栧弬鏁伴敊璇��
 * <p>			0锛氬嚱鏁版甯歌繑鍥炪��
 */
int	BSP_GetCurCombineAllState(int efnum, int *array)
{
	int num;
	unsigned long tmp;
	int i;
	struct CorepacIntDev *pCur_corepac_int_dev = BSP_GetCurCorepacIntc();

	if (efnum > 3) {
		DEBUG_DEV("only event0-3 can be combined by other events\n");
		return -1;
	}

	if (pCur_corepac_int_dev->pFunc_sets == NULL) {
		DEBUG_DEV("current corepac's operations set is NULL!\n");
		return -1;
	}

	num = efnum / 32;

	tmp = pCur_corepac_int_dev->pFunc_sets->corepac_mask_combine_read_event(efnum);

	for (i = 0;i < 32;i ++) {
		if (tmp & (1 << i))
			array[i] = i + num * 32;
		else
			array[i] = -1;
	}

	return 0;
}


