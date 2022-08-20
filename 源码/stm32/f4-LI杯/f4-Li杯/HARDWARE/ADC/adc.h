#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 

extern u16 adcx,adcy;
extern float temp,temy;
void Adc_Init(void); 				//ADC通道初始化
void Adc2_Init(void);
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16  Get_Adc2(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
u16 Get_Adc2_Average(u8 ch,u8 times);
#endif 















