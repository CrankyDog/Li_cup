#ifndef _servo_H
#define _servo_H

#include "stm32f10x.h"
#include "sys.h"

void Servo_Init(u16 per,u16 psc);
void Drive_Servo(u16 i);

#endif
