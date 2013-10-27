/*
 * c66xx_arch_config.h
 *
 *  Created on: 2013-10-27
 *      Author: insswer
 */

#ifndef C66XX_ARCH_CONFIG_H_
#define C66XX_ARCH_CONFIG_H_

#ifdef OSEKMOS
#define ARCH_CORE_NUM	CONFIG_OSEK_KERNEL_CORE_NUMS
#define OSEK_TIMER_SRC          (TIMER_0 + get_coreid())
#define OSEK_TIMER_IRQ          IRQ_TINT
#else
#define ARCH_CORE_NUM	2
#endif

#define IRQ_TINT				  64
#define TIMER_0					  0



#endif /* C66XX_ARCH_CONFIG_H_ */
