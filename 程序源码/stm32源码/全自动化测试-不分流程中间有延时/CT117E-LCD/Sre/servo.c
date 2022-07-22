#include "servo.h"

//舵机函数初始化，通过定时器5通道1产生PWM脉冲控制舵机转到相应角度
void Servo_Init(u16 per,u16 psc)
{
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);

  GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period=per;   
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM5,&TIM_OCInitStructure); 
	
	TIM_OC1PreloadConfig(TIM5,TIM_OCPreload_Enable); 
	TIM_ARRPreloadConfig(TIM5,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
	
}

//舵机驱动函数，i取1对应舵机0°，i取200对应舵机取180°
void Drive_Servo(u16 i)
{
  TIM_SetCompare1(TIM5,i);
}
