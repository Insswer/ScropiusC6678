
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
