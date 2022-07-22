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
    RCC->APB1ENR|=1<<1;	//TIM3ʱ��ʹ��    
 	TIM3->ARR=arr;  	//�趨�������Զ���װֵ 
	TIM3->PSC=psc;  	//Ԥ��Ƶ������
	TIM3->DIER|=1<<0;   //��������ж�				
	TIM3->CR1|=0x01;    //ʹ�ܶ�ʱ��3
  	MY_NVIC_Init(1,3,TIM3_IRQn,2);//��ռ0�������ȼ�3����2		
							 
}

u16 err_abs;
u16 err_v;
//��ʱ��3�жϷ�����
//adcx=Get_Adc2_Average(ADC_Channel_5,10);

void TIM3_IRQHandler(void) //TIM3�ж�
{
			if(TIM3->SR&0X0001)//����ж�
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
	TIM3->SR&=~(1<<0);//����жϱ�־λ 
}

extern vu16 USART3_RX_STA;
 
//��ʱ��7�жϷ������		    
void TIM6_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 			   
		USART3_RX_STA|=1<<15;	//��ǽ������
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update  );  //���TIM7�����жϱ�־    
		TIM_Cmd(TIM6, DISABLE);  //�ر�TIM7 
	}	    
}
 
//ͨ�ö�ʱ��4�жϳ�ʼ��������ʱ��ѡ��ΪAPB1��2��
//arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz 
//ͨ�ö�ʱ���жϳ�ʼ�� 
void TIM6_Int_Init(u16 arr,u16 psc)
{	
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);//TIM4ʱ��ʹ��    
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	
	TIM_Cmd(TIM6,ENABLE);//������ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 			
	
}