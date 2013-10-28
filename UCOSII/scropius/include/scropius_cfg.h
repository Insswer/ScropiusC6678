/*
 * scropius_cfg.h
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#ifndef SCROPIUS_CFG_H_
#define SCROPIUS_CFG_H_

#define CCS5
#define USING_CORE	2
#define OS_NUM		2
#define TMS320C6678

/* All Kinds of OSs */
#define UCOSII		0
#define UCOSIII		1
#define DeOS		2
#define OsekMOS		3
#define	VXWORKS5.5	4
#define VXWORKS6.X	5
#define UCLINUX		6

/* define Cores */

#define CORE0_OS	UCOSII
#define CORE1_OS	UCOSII

/* exceptions */
#ifdef TMS320C6678
#define VECTORS_NUM	16
#endif

#endif /* SCROPIUS_CFG_H_ */
