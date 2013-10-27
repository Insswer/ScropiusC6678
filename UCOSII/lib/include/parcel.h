/************************************************************************
 *				电子科技大学嵌入式软件工程中心 版权所有
 * 	 Copyright (C) 2005-2013 ESEC UESTC. All Rights Reserved.
 ***********************************************************************/

/*
 * $Log: parcel,v $
 * Revision 1.0  2013/06/13 13:13:00
 * 创建文件
*/

/*
 * @file 	parcel.h
 * @brief
 *	<li>功能：字节化工具模块。</li>
 * @author	Insswer
 * @date 	2013-06-13
 * <p>部门：
 */

#ifndef PARCEL_H_
#define PARCEL_H_

#define PARCEL_DEFAULT_SIZE		64
#define PARCEL_ALIGN			4


/**************************** 引用部分 *********************************/
#include "string.h"

/**************************** 声明部分 *********************************/


/**************************** 定义部分 *********************************/

struct parcel {
	unsigned int offset;
	unsigned char buffer[PARCEL_DEFAULT_SIZE];
};

static inline void InitParcel(struct parcel *parcel)
{
	memset(parcel->buffer, 0, PARCEL_DEFAULT_SIZE);
	parcel->offset = 0;
}

static inline void PutSignedInt(struct parcel *par, int data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(int));
	par->offset += PARCEL_ALIGN;
}

static inline void PutUnsignedInt(struct parcel *par, unsigned int data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(unsigned int));
	par->offset += PARCEL_ALIGN;
}

static inline void PutSignedShort(struct parcel *par, short data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(short));
	par->offset += PARCEL_ALIGN;
}

static inline void PutUnsignedShort(struct parcel *par,unsigned short data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(unsigned short));
	par->offset += PARCEL_ALIGN;
}
static inline void PutSignedChar(struct parcel *par, char data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(char));
	par->offset += PARCEL_ALIGN;
}

static inline void PutUnsignedChar(struct parcel *par, unsigned char data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(unsigned char));
	par->offset += PARCEL_ALIGN;
}

static inline void PutSignedLong(struct parcel *par, long data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(long));
	par->offset += sizeof(long);
}

static inline void PutUnsignedLong(struct parcel *par, unsigned long data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(unsigned char));
	par->offset += sizeof(unsigned long);
}

static inline void PutPointer(struct parcel *par, unsigned long data)
{
	memcpy((unsigned char *)par->buffer + par->offset, &data, sizeof(void *));
	par->offset += PARCEL_ALIGN;
}

static inline int GetSignedInt(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(int *)((unsigned char *)par->buffer + par->offset);
}

static inline unsigned int GetUnsignedInt(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(unsigned int *)((unsigned char *)par->buffer + par->offset);
}

static inline short GetSignedShort(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(short *)((unsigned char *)par->buffer + par->offset);
}

static inline unsigned short GetUnsignedShort(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(unsigned short *)((unsigned char *)par->buffer + par->offset);
}

static inline char GetSignedChar(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(char *)((unsigned char *)par->buffer + par->offset);
}

static inline unsigned char GetUnsignedChar(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(unsigned char *)((unsigned char *)par->buffer + par->offset);
}

static inline long GetSignedLong(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(long *)((unsigned char *)par->buffer + par->offset);
}

static inline unsigned long GetUnsignedLong(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(unsigned long *)((unsigned char *)par->buffer + par->offset);
}

static inline unsigned long GetPointer(struct parcel *par)
{
	par->offset -= PARCEL_ALIGN;
	return *(unsigned long *)((unsigned char *)par->buffer + par->offset);
}



#endif /* PARCEL_H_ */

