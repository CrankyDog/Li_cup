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
					adcx=Get_Adc_Average(ADC_Channel_1,10);
//					printf("adcx:%d\r\n",adcx);
					temp = (float)adcx*(3.3/4096);
					adcx = temp;
//					printf("temp:%d\r\n",adcx);
					temp = temp*(360/3.3);
					adcx = temp;
				printf("��ǰ�Ƕ�:%d\r\n",adcx);
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
					
//					printf("�Ƕ�Ϊ��%d\r\n",adcx);
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