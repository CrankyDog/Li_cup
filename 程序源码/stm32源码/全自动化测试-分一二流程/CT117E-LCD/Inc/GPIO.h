#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"

#define Line0 PEout(0)
#define Line1 PEout(1)
#define Line2 PEout(2)
#define Line3 PEout(3)
#define Line4 PEout(4)

#define Beep0 PDout(0)
#define Beep1 PDout(1)
#define Beep2 PDout(2)
#define Beep3 PDout(3)
#define Beep4 PDout(4)
#define Beep5 PDout(5)

#define Moku0 PCout(0)
#define Moku1 PCout(1)

void Line_Init(void);
void Beep_Init(void);
void Moku_Init(void);

#endif