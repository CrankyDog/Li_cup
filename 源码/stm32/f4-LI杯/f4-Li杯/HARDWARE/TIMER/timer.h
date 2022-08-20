#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"

extern u8 flag,len,angle_flag,moku_flag,stop_flag,Draw_PID_flag,time_flag;
extern u16 c_angle,m_angle,a_angle;
void TIM3_Int_Init(u16 arr,u16 psc);
void TIM4_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
#endif

