#include "timer.h"
#include "usart.h"
#include "adc.h"
#include "dac.h"
#include "pid.h"
#include "GPIO.h"
#include "servo.h"
#include "jd.h"
#include "draw.h"
#include "Speech.h"

float time_cnt;
u8 len;
u8 flag='0',moku_flag=0,angle_flag =  0,Draw_PID_flag=0,stop_flag=0,time_flag=0,time_flag2=0;
u16 c_angle = 0,m_angle = 0,a_angle = 0,cur_angle=0,tar_angle=0;
u8 line_cnt;
int v_dir;

void TIM2_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM2,ENABLE); //使能定时器6
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn; //定时器2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x04; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)==SET) //溢出中断
	{
		if(time_flag&(!time_flag2)){
			if(moku_go_err(tar_angle,cur_angle)<20){
				delay_ms(10);
				if(moku_go_err(tar_angle,cur_angle)<10){
					printf("t21.txt=\"%.2f s\"\xff\xff\xff",time_cnt);
					time_flag2 = 1;
					time_cnt=0;
				}
			}
			else{
				time_cnt+=0.01;
				if(time_cnt>5) time_cnt=0;
			}
		}
		else if(time_flag2&(moku_go_err(tar_angle,cur_angle)>9))
			time_flag2 = 0;
		
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);  //清除中断标志位
}

void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  ///使能TIM4时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM4,ENABLE); //使能定时器4
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; //定时器4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x03; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x04; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		Speech_SP();
		Speech_JG();
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);  //清除中断标志位
}
		
void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

u16 err_abs;
u16 err_v;
//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
			if (USART_RX_STA & 0x8000)
			{
					len=USART_RX_STA&0x3fff;
					line_cnt = USART_RX_BUF[0];
					switch(line_cnt)
					{
//						case 'a':Moku1='0';Moku0='0';play_flag=0;ori_flag=0;printf("t5.txt=\"On\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
//											printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");break;
//						case 'b':Moku1='0';Moku0='1';play_flag=1;ori_flag=1;printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"On\"\xff\xff\xff");
//											printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");break;
//						case 'c':Moku1='1';Moku0='0';play_flag=1;denoising_flag=0;ori_flag=0;printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
//											printf("t7.txt=\"On\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");break;
//						case 'd':Moku1='1';Moku0='1';play_flag=1;denoising_flag=1;ori_flag=0;printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
//											printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"On\"\xff\xff\xff");break;
//						case 'e':stop_flag=1;printf("page moku_go\xff\xff\xff");break;
//						case 'f':stop_flag=0;printf("page function_main\xff\xff\xff");break;
//						case 'g':stop_flag=1;printf("page equalization\xff\xff\xff");break;
//						case 'h':stop_flag=0;printf("page function_main\xff\xff\xff");break;
//						case 'A':Line4=0;Line3=0;Line2=0;Line1=0;Line0=1;printf("h0.val=h0.val+10\xff\xff\xff");break;
//						case 'B':Line4=0;Line3=0;Line2=0;Line1=1;Line0=0;printf("h0.val=h0.val-10\xff\xff\xff");break;
//						case 'C':Line4=0;Line3=0;Line2=0;Line1=1;Line0=1;printf("h1.val=h1.val+10\xff\xff\xff");break;
//						case 'D':Line4=0;Line3=0;Line2=1;Line1=0;Line0=0;printf("h1.val=h1.val-10\xff\xff\xff");break;
//						case 'E':Line4=0;Line3=0;Line2=1;Line1=1;Line0=1;printf("h2.val=h2.val+10\xff\xff\xff");break;
//						case 'F':Line4=0;Line3=1;Line2=0;Line1=0;Line0=0;printf("h2.val=h2.val-10\xff\xff\xff");break;
//						case 'G':Line4=0;Line3=1;Line2=0;Line1=0;Line0=1;printf("h3.val=h3.val+10\xff\xff\xff");break;
//						case 'H':Line4=0;Line3=1;Line2=0;Line1=1;Line0=0;printf("h3.val=h3.val-10\xff\xff\xff");break;
//						case 'I':Line4=0;Line3=1;Line2=0;Line1=1;Line0=1;printf("h4.val=h4.val+10\xff\xff\xff");break;
//						case 'J':Line4=0;Line3=1;Line2=1;Line1=0;Line0=0;printf("h4.val=h4.val-10\xff\xff\xff");break;
//						case 'K':Line4=0;Line3=1;Line2=1;Line1=0;Line0=1;printf("h5.val=h5.val+10\xff\xff\xff");break;
//						case 'L':Line4=0;Line3=1;Line2=1;Line1=1;Line0=0;printf("h5.val=h5.val-10\xff\xff\xff");break;
//						case 'M':Line4=0;Line3=1;Line2=1;Line1=1;Line0=1;printf("h6.val=h6.val+10\xff\xff\xff");break;
//						case 'N':Line4=1;Line3=0;Line2=0;Line1=0;Line0=0;printf("h6.val=h6.val-10\xff\xff\xff");break;
//						case 'O':Line4=1;Line3=0;Line2=0;Line1=0;Line0=1;printf("h7.val=h7.val+10\xff\xff\xff");break;
//						case 'P':Line4=1;Line3=0;Line2=0;Line1=1;Line0=0;printf("h7.val=h7.val-10\xff\xff\xff");break;
//						case 'X':angle_flag = 1;printf("page se_debug\xff\xff\xff");break;
//						case 'Y':angle_flag = 0;printf("page function_main\xff\xff\xff");break;
						case 'Z':flag = '1';m_angle = 180;printf("page time\xff\xff\xff");break;
//																			printf("t0.y=50\xff\xff\xff");printf("b2.y=160\xff\xff\xff");
//																			printf("b6.y=100\xff\xff\xff");printf("b7.y=100\xff\xff\xff");
//																			printf("b12.y=55\xff\xff\xff");printf("t0.y=50\xff\xff\xff");
//						case 'R':moku_flag=1;break;
//						case 'S':moku_flag=2;break;
						case 'V':Draw_PID_flag=1;printf("page PID_wave\xff\xff\xff");break;
						case 'W':Draw_PID_flag=0;time_flag=1;printf("page time\xff\xff\xff");break;
						case '0':flag = '0';stop_flag=0;moku_flag = 0;angle_flag=0;break;
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
		if(time_flag){
			printf("z0.val=%d\xff\xff\xff",cur_angle);
			printf("t22.txt=\"%d°\"\xff\xff\xff",tar_angle);
			printf("t23.txt=\"%d°\"\xff\xff\xff",cur_angle);
		}
		err_abs = moku_go_err(c_angle,adcx);
		v_dir = moku_go_dir(c_angle,adcx);
		err_v = err_abs*3300/180;
		Dac1_Set_Vol(err_v);
		
		adcy=Get_Adc2_Average(ADC_Channel_5,10);
		temy = (float)adcy*(6000/4096);
		adcy = temy;
		if(stop_flag|stop_flag2)Drive_Servo(angle(90));
		else Drive_Servo(angle(moku_go_PID(adcy,dir)));
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}

