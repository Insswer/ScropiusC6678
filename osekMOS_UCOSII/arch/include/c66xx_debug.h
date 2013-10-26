/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_debug,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_debug.h
 * @brief
 *	<li>功能：c66xx的debug相关的操作头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */
#ifndef _C66XX_DEBUG_H
#define _C66XX_DEBUG_H

#include <stdio.h>

#define DEBUG

#ifdef	DEBUG
#define DEBUG_DEV(fmt,args...) printf(fmt, ##args)
#else
#define DEBUG_DEV(fmt,args...) do{}while(0)
#endif


#endif /* _C66XX_DEBUG_H */
