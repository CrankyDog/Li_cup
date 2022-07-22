#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK NANO STM32开发板
//通用定时器 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2018/3/27
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2018-2028
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
extern u8 flag,lem,angle_flag,moku_flag,stop_flag,Draw_PID_flag;
extern u16 c_angle,m_angle,a_angle;

void TIM3_Init(u16 arr,u16 psc); 
void TIM6_Int_Init(u16 arr,u16 psc);
void TIM1_PWM_Init(u16 arr,u16 psc);
 
#endif
