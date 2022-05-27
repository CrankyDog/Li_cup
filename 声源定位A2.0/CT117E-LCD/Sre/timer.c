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

u8 lem;
u8 flag='0';
u16 c_angle = 0;
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
					adcx=Get_Adc_Average(ADC_Channel_1,10);
//					printf("adcx:%d\r\n",adcx);
					temp = (float)adcx*(3.3/4096);
					adcx = temp;
//					printf("temp:%d\r\n",adcx);
					temp = temp*(360/3.3);
					adcx = temp;
				printf("当前角度:%d\r\n",adcx);
				err_abs = moku_go_err(co_angle,adcx);
				printf("err_abs:%d\r\n",err_abs);
				v_dir = moku_go_dir(co_angle,adcx);
				printf("dir:%d\r\n",v_dir);
				err_v = err_abs*3300/180;
				Dac1_Set_Vol(err_v);
				printf("err_v:%d\r\n",err_v);
				
					adcy=Get_Adc2_Average(ADC_Channel_5,10);
					temy = (float)adcy*(6000/4096);
					adcy = temy;
				printf("adcy:%d\r\n",adcy);
					Drive_Servo(angle(moku_go_PID(adcy,dir)));
					
//					printf("角度为：%d\r\n",adcx);
//					Drive_Servo(angle(Incremental_PID(adcx,co_angle)));
//					if (USART_RX_STA & 0x8000)
//						{
//								USART_RX_STA = 0;
//								line_cnt = USART_RX_BUF[0];
//								switch(line_cnt)
//								{
//									case '0':Line0=0;Line1=0;Line2=0;printf("0\r\n");break;
//									case '1':Line0=0;Line1=0;Line2=1;printf("1\r\n");break;
//									case '2':Line0=0;Line1=1;Line2=0;printf("2\r\n");break;
//									case '3':Line0=0;Line1=1;Line2=1;printf("3\r\n");break;
//									case '4':Line0=1;Line1=0;Line2=0;printf("4\r\n");break;
//									case '5':Line0=1;Line1=0;Line2=1;printf("5\r\n");break;
//									case '6':Line0=1;Line1=1;Line2=0;printf("6\r\n");break;
//									case '7':flag = '0';printf("va0.val=0\xff\xff\xff");break;
//								}
//						}
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