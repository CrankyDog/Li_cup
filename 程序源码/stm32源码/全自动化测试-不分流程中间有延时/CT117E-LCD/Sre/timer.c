#include "timer.h"
#include "sydw.h"
#include "sys.h"
#include "delay.h"
#include "jd.h"
#include "servo.h"
#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "adc.h"
#include "pid.h"
#include "usart.h"
#include "GPIO.h"
#include "dac.h"
#include "Draw.h"

u8 lem;
u8 flag='0',angle_flag =  0,Draw_PID_flag=0,stop_flag=0;
u16 c_angle = 0,m_angle = 0,a_angle = 0,cur_angle=0,tar_angle=0;
u8 line_cnt;
int v_dir;

void TIM3_Init(u16 arr,u16 psc)
{
    RCC->APB1ENR|=1<<1;	//TIM3时钟使能    
 	TIM3->ARR=arr;  	//设定计数器自动重装值 
	TIM3->PSC=psc;  	//预分频器设置
	TIM3->DIER|=1<<0;   //允许更新中断				
	TIM3->CR1|=0x01;    //使能定时器3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//抢占0，子优先级3，组2		
							 
}

u16 err_abs;
u16 err_v;
//定时器3中断服务函数
//adcx=Get_Adc2_Average(ADC_Channel_5,10);

void TIM3_IRQHandler(void) //TIM3中断
{
			if(TIM3->SR&0X0001)//溢出中断
			{
					if (USART_RX_STA & 0x8000)
					{
							len=USART_RX_STA&0x3fff;
							line_cnt = USART_RX_BUF[0];
							switch(line_cnt)
							{
								case 'V':Draw_PID_flag=1;printf("page PID_wave\xff\xff\xff");break;
								case 'W':Draw_PID_flag=0;printf("page wave\xff\xff\xff");break;
								case '0':flag = '0';printf("t0.y=400\xff\xff\xff");printf("b2.y=510\xff\xff\xff");printf("b12.y=405\xff\xff\xff");printf("b5.y=55\xff\xff\xff");break;
							}
							USART_RX_STA=0;
					}
					
					adcx=Get_Adc_Average(ADC_Channel_1,10);
					temp = (float)adcx*(3.3/4096);
					adcx = temp;
					temp = temp*(360/3.3);
					adcx = temp;
					c_angle = angle_flag ? m_angle : a_angle;
					
					cur_angle = adcx;
					tar_angle = c_angle;
					if(Draw_PID_flag == 1){
							Draw_PID(cur_angle,tar_angle);
					}
					err_abs = moku_go_err(c_angle,adcx);
					v_dir = moku_go_dir(c_angle,adcx);
					err_v = err_abs*3300/180;
					Dac1_Set_Vol(err_v);
				
					adcy=Get_Adc2_Average(ADC_Channel_5,10);
					temy = (float)adcy*(6000/4096);
					adcy = temy;
					Drive_Servo(angle(moku_go_PID(adcy,dir)));
					if(stop_flag)Drive_Servo(angle(90));
					if(!stop_flag)Drive_Servo(angle(moku_go_PID(adcy,dir)));
			}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 
}

extern vu16 USART3_RX_STA;
 
//定时器7中断服务程序		    
void TIM6_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//是更新中断
	{	 			   
		USART3_RX_STA|=1<<15;	//标记接收完成
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //清除TIM7更新中断标志    
		TIM_Cmd(TIM6, DISABLE);  //关闭TIM7 
	}	    
}
 
//通用定时器4中断初始化，这里时钟选择为APB1的2倍
//arr：自动重装值 psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz 
//通用定时器中断初始化 
void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//TIM4时钟使能    
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	
	TIM_Cmd(TIM6,ENABLE);//开启定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器 			
	
}