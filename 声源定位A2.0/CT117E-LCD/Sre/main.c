#include "math.h"
#include "stm32f10x.h"
#include "SK9822.h"
#include "key.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "MicArray.h"
#include "malloc.h"
#include "74LV4052.h"
#include <stdio.h>
#include "sydw.h"
#include "servo.h"
#include "jd.h"
#include "timer.h"
#include "usart3.h"
#include "esp8266.h"
#include "beep.h"
#include "adc.h"
#include "pid.h"
#include "GPIO.h"
#include "uart2.h"
#include "dac.h"

int main(void)
{
		u8 len;
		u8 k;
		u8* jj; 
		u8 angle_cnt[3];
		u16 u_angle = 0;
		u8 line_cnt;
		
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    key_init();
		delay_init();
    LCD_Init();
		Adc_Init();	
		Adc2_Init();
		Dac1_Init();
		Line_Init();
	
		uart_init(115200);
		uart2_init(115200);
//		usart3_init(115200);
		TIM3_Init(9999,1799);
		Servo_Init(9999,143);
		Drive_Beep();
		flag = '1';
	
//		esp8266_start_trans();							//esp8266进行初始化
//		esp8266_send_data("start",200);
//		esp8266_quit_trans();
    while(1)
    {
				
//				Drive_Servo(angle(81));
//				Dac1_Set_Vol(1000);
//				if(USART2_RX_STA & 0x8000)
//				{
//						len = USART2_RX_STA & 0x3fff; //得到此次接收到的数据长度
//						if(len==1)
//						{
//							flag = USART2_RX_BUF[0];
//							printf("\r\n您发送的消息为:\r\n\r\n");
//							USART2_SendByte(flag);
//						}
//						USART2_RX_STA = 0;
//						printf("flag:%c\r\n",flag);
//				}
				if(flag == '1')
				{
						if(USART2_RX_STA&0x8000)
						{					   
							len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
							USART2_RX_STA=0;
							if(len<4){
//							printf("\r\n您发送的消息为:\r\n\r\n");
							for(k=0;k<len;k++)
							{
								angle_cnt[k] = USART2_RX_BUF[k];
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
							}
							USART1_SendBytes(angle_cnt,len);
							u_angle = atoi(angle_cnt);
//							u_angle = 100*(angle_cnt[0]-48)+10*(angle_cnt[1]-48)+(angle_cnt[2]-48);
							printf("u_angle:%d\r\n",u_angle);
							if(u_angle%30==0)
							{
//							printf("u_angle:%d\r\n",u_angle);
							if(u_angle<0||u_angle>360)
								printf("\r\n角度超出范围\r\n");
							else if(u_angle == 361)
								flag = '0';
							else 
							{
								co_angle = u_angle;
								printf("目标角度：%d\r\n",co_angle);
							}
							}
							}
							angle_cnt[0]=0;
							angle_cnt[1]=0;
							angle_cnt[2]=0;
							u_angle = 0;
						}
//						if (USART2_RX_STA & 0x8000)
//						{
//								len=USART2_RX_STA&0x3fff;
//								switch(line_cnt)
//								{
//									case '0':Line0=0;Line1=0;Line2=0;printf("0\r\n");break;
//									case '1':Line0=0;Line1=0;Line2=1;printf("1\r\n");break;
//									case '2':Line0=0;Line1=1;Line2=0;printf("2\r\n");break;
//									case '3':Line0=0;Line1=1;Line2=1;printf("3\r\n");break;
//									case '4':Line0=1;Line1=0;Line2=0;printf("4\r\n");break;
//									case '5':Line0=1;Line1=0;Line2=1;printf("5\r\n");break;
//									case '6':Line0=1;Line1=1;Line2=0;printf("6\r\n");break;
//									case '7':flag = '0';break;
//								}
//								USART2_SendByte(1);
//								USART2_RX_STA = 0;
//						}
				}
//				else if(flag == '2')
//				{
//						if (USART_RX_STA & 0x8000)
//						{
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
//									case '7':flag = '0';break;
//								}
//								USART_RX_STA = 0;
//						}
//				}
    }
}
