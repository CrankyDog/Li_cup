#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 

extern u16 adcx,adcy;
extern float temp,temy;
void Adc_Init(void); 				//ADCͨ����ʼ��
void Adc2_Init(void);
u16  Get_Adc(u8 ch); 				//���ĳ��ͨ��ֵ 
u16  Get_Adc2(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);//�õ�ĳ��ͨ����������������ƽ��ֵ  
u16 Get_Adc2_Average(u8 ch,u8 times);
#endif 















