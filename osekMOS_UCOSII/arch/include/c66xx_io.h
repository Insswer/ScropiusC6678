/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: c66xx_io,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	c66xx_io.h
 * @brief
 *	<li>功能：c66xx的I/O相关操作头文件。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

#ifndef		_C66XX_IO_H_
#define		_C66XX_IO_H_


/**************************** 引用部分 *********************************/

#include "stdio.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/
#define __arch_getb(a)			(*(volatile unsigned char *)(a))
#define __arch_getw(a)			(*(volatile unsigned short *)(a))
#define __arch_getl(a)			(*(volatile unsigned long *)(a))

#define __arch_putb(v,a)		(*(volatile unsigned char *)(a) = (v))
#define __arch_putw(v,a)		(*(volatile unsigned short *)(a) = (v))
#define __arch_putl(v,a)		(*(volatile unsigned long *)(a) = (v))


#define readb(c)	({ unsigned char  __v = __arch_getb(c); __v; })
#define readw(c)	({ unsigned short __v = __arch_getw(c); __v; })
#define readl(c)	({ unsigned long __v = __arch_getl(c);  __v; })


#define writeb(v,c)	({ unsigned char  __v = v; __arch_putb(__v,c); __v; })
#define writew(v,c)	({ unsigned short __v = v; __arch_putw(__v,c); __v; })
#define writel(v,c)	({ unsigned long __v = v; __arch_putl(__v,c); __v; })

#define offsetof(type,member)		\
	((unsigned int) &((type *)0)->member)

#define	container_of(ptr,type,member)	({					\
	const typeof(((type *)0)->member) *__mptr = (ptr); 	\
	(type *)((char *)__mptr - offsetof(type,member));	})

#define ERROR()  printf("error at (%s)%d-%s....\n", __FILE__, __LINE__, __FUNCTION__)

#endif
