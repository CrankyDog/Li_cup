#ifndef _beep_H
#define _beep_H
#include "stm32f10x.h"
#include "sys.h"

void TIM1_PWM_Init(u16 arr,u16 psc);
void Drive_Beep();

#endif