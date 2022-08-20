#ifndef __GPIO_H
#define __GPIO_H	 
#include "sys.h"

#define Line0 PEout(0)
#define Line1 PEout(1)
#define Line2 PEout(2)
#define Line3 PEout(3)
#define Line4 PEout(4)

#define Beep0 PFout(0)
#define Beep1 PFout(1)
#define Beep2 PFout(2)
#define Beep3 PFout(3)
#define Beep4 PFout(4)
#define Beep5 PFout(5)

#define Moku0 PCout(1)
#define Moku1 PCout(2)

#define B0	PDout(0)
#define B1	PDout(2)
#define B2	PDout(4)
#define B7	PGout(9)
#define B3	PDin(7)
#define B6	PDin(6)
#define A25	PDin(1)
#define A26	PDin(3)
#define A27 PDin(5)

void Speech_Init(void);
void Speech_Out(void);
void Line_Init(void);
void Beep_Init(void);
void Moku_Init(void);

#endif

