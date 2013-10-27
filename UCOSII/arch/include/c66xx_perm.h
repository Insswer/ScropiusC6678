/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_perm.h,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_perm.h
 * @brief
 *	<li>功能：声明了C66XX相关权限选项。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */



#ifndef C66XX_PERM_H_
#define C66XX_PERM_H_

#define PERM_NONE	(0x0)

/* user execution */
#define	PERM_UX		(0x1)
/* user write */
#define	PERM_UW		(0x2)
/* user read */
#define PERM_UR		(0x4)
/* supervisor execution */
#define PERM_SX		(0x8)
/* supervisor write */
#define PERM_SW		(0x10)
/* supervisor read */
#define PERM_SR		(0x20)
/* emulation debug access */
#define PERM_EMU	(0x40)
/* Non-secure access permission */
#define PERM_NS		(0x80)

#define	MAR_PREFETCH	(0x8)
#define	MAR_CACHE		(0x1)

#endif /* C66XX_PERM_H_ */
