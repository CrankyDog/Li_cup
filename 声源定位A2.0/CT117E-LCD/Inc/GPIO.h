#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"

#define Line0 PEout(0)
#define Line1 PEout(1)
#define Line2 PEout(2)


void Line_Init(void);

#endif