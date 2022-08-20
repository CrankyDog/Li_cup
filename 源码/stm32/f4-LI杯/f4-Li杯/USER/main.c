#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "servo.h"
#include "jd.h"
#include "timer.h"
#include "usart3.h"
#include "esp8266.h"
#include "stdio.h"
#include "adc.h"
#include "pid.h"
#include "GPIO.h"
#include "usart2.h"
#include "dac.h"
#include "Speech.h"

//ALIENTEK 探索者STM32F407开发板 实验4
//串口通信实验 -库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK


int main(void)
{ 
	
	u8 len,beep_flag=1;
	u32 beep_cnt=0;
	u8 beep_switch=0,q_100=1;
	u8 k,cnt_i,cnt_j;
	u8 angle_cnt[3];
	u16 u_angle = 0;
	u8 line_cnt;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);		//延时初始化 
	uart_init(115200);	//串口初始化波特率为115200
	usart2_init(115200);
	usart3_init(115200);
	Adc_Init();	
	Adc2_Init();
	Dac1_Init();
	Line_Init();
	Beep_Init();
	Moku_Init();
	Speech_Out();
	Speech_Init();
	TIM2_Int_Init(9999,83);
	TIM3_Int_Init(9999,4199);
	TIM4_Int_Init(9999,9999);
	Servo_Init(9999,167);
	Drive_Servo(180);
	flag = '0';	
	esp8266_start_AP();
/*	while(1){
		Open_Loc();
		delay_s(5);
		Open_Spe();
		delay_s(5);
		Spe_Org();
		delay_s(5);
		Spe_Equ();
		delay_s(5);
		Spe_Wit();
		delay_s(5);
		Open_Equ();
		delay_s(5);
	}
*/
	while(1)
	{
		if(flag == '0')
				{
						if(USART_RX_STA&0x8000)
						{
							len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
							USART_RX_STA=0;
							if(len == 1)
							{
								line_cnt = USART_RX_BUF[0];
								switch(line_cnt)
								{
									case 'a':Moku1='0';Moku0='0';play_flag=0;ori_flag=0;printf("t5.txt=\"On\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
														printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");break;
									case 'b':Moku1='0';Moku0='1';play_flag=1;ori_flag=1;printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"On\"\xff\xff\xff");
														printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");break;
									case 'c':Moku1='1';Moku0='0';play_flag=1;denoising_flag=0;ori_flag=0;printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
														printf("t7.txt=\"On\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");break;
									case 'd':Moku1='1';Moku0='1';play_flag=1;denoising_flag=1;ori_flag=0;printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
														printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"On\"\xff\xff\xff");break;
									case 'e':stop_flag=1;printf("page moku_go\xff\xff\xff");break;
									case 'f':stop_flag=0;printf("page function_main\xff\xff\xff");break;
									case 'g':stop_flag=1;printf("page equalization\xff\xff\xff");break;
									case 'h':stop_flag=0;printf("page function_main\xff\xff\xff");break;
									case 'A':Line4=0;Line3=0;Line2=0;Line1=0;Line0=1;printf("h0.val=h0.val+10\xff\xff\xff");break;
									case 'B':Line4=0;Line3=0;Line2=0;Line1=1;Line0=0;printf("h0.val=h0.val-10\xff\xff\xff");break;
									case 'C':Line4=0;Line3=0;Line2=0;Line1=1;Line0=1;printf("h1.val=h1.val+10\xff\xff\xff");break;
									case 'D':Line4=0;Line3=0;Line2=1;Line1=0;Line0=0;printf("h1.val=h1.val-10\xff\xff\xff");break;
									case 'E':Line4=0;Line3=0;Line2=1;Line1=1;Line0=1;printf("h2.val=h2.val+10\xff\xff\xff");break;
									case 'F':Line4=0;Line3=1;Line2=0;Line1=0;Line0=0;printf("h2.val=h2.val-10\xff\xff\xff");break;
									case 'G':Line4=0;Line3=1;Line2=0;Line1=0;Line0=1;printf("h3.val=h3.val+10\xff\xff\xff");break;
									case 'H':Line4=0;Line3=1;Line2=0;Line1=1;Line0=0;printf("h3.val=h3.val-10\xff\xff\xff");break;
									case 'I':Line4=0;Line3=1;Line2=0;Line1=1;Line0=1;printf("h4.val=h4.val+10\xff\xff\xff");break;
									case 'J':Line4=0;Line3=1;Line2=1;Line1=0;Line0=0;printf("h4.val=h4.val-10\xff\xff\xff");break;
									case 'K':Line4=0;Line3=1;Line2=1;Line1=0;Line0=1;printf("h5.val=h5.val+10\xff\xff\xff");break;
									case 'L':Line4=0;Line3=1;Line2=1;Line1=1;Line0=0;printf("h5.val=h5.val-10\xff\xff\xff");break;
									case 'M':Line4=0;Line3=1;Line2=1;Line1=1;Line0=1;printf("h6.val=h6.val+10\xff\xff\xff");break;
									case 'N':Line4=1;Line3=0;Line2=0;Line1=0;Line0=0;printf("h6.val=h6.val-10\xff\xff\xff");break;
									case 'O':Line4=1;Line3=0;Line2=0;Line1=0;Line0=1;printf("h7.val=h7.val+10\xff\xff\xff");break;
									case 'P':Line4=1;Line3=0;Line2=0;Line1=1;Line0=0;printf("h7.val=h7.val-10\xff\xff\xff");break;
									case 'V':Draw_PID_flag=1;printf("page PID_wave\xff\xff\xff");break;
									case 'W':Draw_PID_flag=0;time_flag=1;printf("page time\xff\xff\xff");break;
									case 'X':angle_flag = 1;printf("page se_debug\xff\xff\xff");break;
									case 'Y':angle_flag = 0;printf("page function_main\xff\xff\xff");break;
									case 'Z':flag = '1';m_angle = 180;printf("page time\xff\xff\xff");break;
								}
								delay_ms(10);
								Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
								delay_ms(10);
							}
							else if(len<4){
								for(k=0;k<len;k++)
								{
									angle_cnt[k] = USART_RX_BUF[k];
									while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
								}
								u_angle = atoi(angle_cnt);
								if(u_angle>360);
								else 
								{
									m_angle = u_angle;
								}
								angle_cnt[0]=0;
								angle_cnt[1]=0;
								angle_cnt[2]=0;
								u_angle = 0;
							}	
						}
						if(USART2_RX_STA&0x8000)
						{					   
							len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
							USART2_RX_STA=0;
							if(len<4){
							for(k=0;k<len;k++)
							{
								angle_cnt[k] = USART2_RX_BUF[k];
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
							}
							u_angle = atoi(angle_cnt);
							u2_printf("%d",u_angle);
							if(u_angle<0||u_angle>360);
							else 
							{
								a_angle = u_angle;
							}
							}
							angle_cnt[0]=0;
							angle_cnt[1]=0;
							angle_cnt[2]=0;
							u_angle = 0;
						}
				}
				
//------------------------------------------接收Wifi K210发送来的开始信号-----------------------------------------
			if(flag == '1')
			{
				angle_flag = 1;
				beep_flag = 1;
//				m_angle = 180;
				if(USART3_RX_STA&0x8000)
					{
						flag = '2';
						USART3_RX_STA = 0;
						Open_Loc();
//						u2_printf("\x7E\x05\x41\x01\x07\x42\xEF");
//						delay_s(5);
					}
			}
//-------------------------------------------整套系统开始运行------------------------------------------				
			if(flag == '2')
			{
				if(moku_flag == 0){
//					m_angle = 180;
					angle_flag = 0;
					beep_cnt++;
				//------------------------------------蜂鸣器测试-------------------------------
					if(beep_cnt==10000000&beep_flag==1)
					{
							beep_cnt = 0;
							switch(beep_switch){
								case 0:Beep0 = 1;Beep1 = 0;Beep2 = 0;Beep3 = 0;Beep4 = 0;Beep5 = 0;break;
								case 1:Beep0 = 0;Beep1 = 1;Beep2 = 0;Beep3 = 0;Beep4 = 0;Beep5 = 0;break;
								case 2:Beep0 = 0;Beep1 = 0;Beep2 = 0;Beep3 = 0;Beep4 = 1;Beep5 = 0;break;
								case 3:Beep0 = 0;Beep1 = 0;Beep2 = 1;Beep3 = 0;Beep4 = 0;Beep5 = 0;break;
								case 4:Beep0 = 0;Beep1 = 0;Beep2 = 0;Beep3 = 0;Beep4 = 0;Beep5 = 1;break;
								case 5:Beep0 = 1;Beep1 = 0;Beep2 = 0;Beep3 = 0;Beep4 = 0;Beep5 = 0;break;
								case 6:Beep0 = 0;Beep1 = 1;Beep2 = 0;Beep3 = 0;Beep4 = 0;Beep5 = 0;break;
								case 7:Beep0 = 0;Beep1 = 0;Beep2 = 0;Beep3 = 1;Beep4 = 0;Beep5 = 0;break;
								default:beep_switch = 0;stop_flag = 1;beep_flag = 0;moku_flag = 2;Beep0 = 0;Beep1 = 0;Beep2 = 0;Beep3 = 0;Beep4 = 0;Beep5 = 0;break;
							}
							beep_switch++;
					}
//				}
				//--------------------------------------舵机转动-----------------------------
					if(USART2_RX_STA&0x8000)
					{					   
						len=USART2_RX_STA&0x3fff;//得到此次接收到的数据长度
						USART2_RX_STA=0;
						if(len<4){
							for(k=0;k<len;k++)
							{
								angle_cnt[k] = USART2_RX_BUF[k];
								while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
							}
							u_angle = atoi(angle_cnt);
							if(u_angle>360);
							else 
							{
								a_angle = u_angle;
							}
						}
						angle_cnt[0]=0;
						angle_cnt[1]=0;
						angle_cnt[2]=0;
						u_angle = 0;
					}
//					moku_flag = 0;
				}
			//---------------------------------开始测试moku：go--------------------------------
				else if(moku_flag == 2){
					Open_Spe();
					if(q_100 == 1){
						for(cnt_i=0;cnt_i<5;cnt_i++){
							Line4=0;Line3=0;Line2=1;Line1=1;Line0=1;
							delay_ms(50);
							Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
							delay_ms(50);
							Line4=0;Line3=1;Line2=0;Line1=1;Line0=0;
							delay_ms(50);
							Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
							delay_ms(50);
							Line4=0;Line3=1;Line2=0;Line1=0;Line0=0;
							delay_ms(50);
							Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
							delay_ms(50);
							Line4=0;Line3=1;Line2=0;Line1=0;Line0=1;
							delay_ms(50);
							Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
							delay_ms(50);
						}
					}
					q_100 = 0;
					Moku1='0';Moku0='0';play_flag=0;ori_flag=0;
					delay_s(1);
			//----------------------------------第一部分原声----------------------------
						
					for(cnt_i=0;cnt_i<5;cnt_i++){
						Line4=0;Line3=0;Line2=1;Line1=1;Line0=1;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
						Line4=0;Line3=1;Line2=0;Line1=1;Line0=0;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
					}
					delay_s(1);
					Spe_Org();
//					u2_printf("\x7E\x05\x41\x01\x02\x47\xEF");
					delay_s(1);
					Moku1='0';Moku0='1';play_flag=1;ori_flag=1;
					delay_s(5);
					for(cnt_j=0;cnt_j<5;cnt_j++){
						Line4=0;Line3=1;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
						Line4=0;Line3=1;Line2=0;Line1=0;Line0=1;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
					}
					Moku1='0';Moku0='0';play_flag=0;ori_flag=0;	delay_s(1);
			//----------------------------------第二部分均衡器调节声音-------------------
						
					for(cnt_j=0;cnt_j<5;cnt_j++){
						Line4=0;Line3=1;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
						Line4=0;Line3=1;Line2=0;Line1=0;Line0=1;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
					}
					delay_s(1);
					Spe_Equ();
//					u2_printf("\x7E\x05\x41\x01\x03\x46\xEF");
					delay_s(2);
					Moku1='0';Moku0='1';play_flag=1;ori_flag=1;
					delay_s(5);
					Moku1='0';Moku0='0';play_flag=0;ori_flag=0;	
					for(cnt_i=0;cnt_i<5;cnt_i++){
						Line4=0;Line3=0;Line2=1;Line1=1;Line0=1;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
						Line4=0;Line3=1;Line2=0;Line1=1;Line0=0;
						delay_ms(50);
						Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
						delay_ms(50);
					}
					delay_s(2);
			//----------------------------------第三部分噪声加原声-----------------------
					Spe_Wit();
//					u2_printf("\x7E\x05\x41\x01\x04\x41\xEF");
					delay_s(2);
					Moku1='1';Moku0='0';play_flag=1;denoising_flag=0;ori_flag=0;	delay_s(5);
					Moku1='0';Moku0='0';play_flag=0;ori_flag=0;	delay_s(2);
			//--------------------------------第四部分滤波后声音-----------------------
					Open_Equ();
//					u2_printf("\x7E\x05\x41\x01\x05\x40\xEF");
					delay_s(2);
					Moku1='1';Moku0='1';play_flag=1;denoising_flag=1;ori_flag=0;	delay_s(5);
					Moku1='0';Moku0='0';play_flag=0;ori_flag=0;	delay_s(2);
					
//					u2_printf("\x7E\x05\x41\x01\x06\x43\xEF");
					delay_s(5);
					
			//-------------------------------结束后返回初始值----------------------------			
					moku_flag = 0;flag = '0';stop_flag=0;angle_flag=0;beep_cnt=0;beep_flag=1;
//					printf("t0.y=410\xff\xff\xff");printf("b2.y=410\xff\xff\xff");
//					printf("t0.y=410\xff\xff\xff");printf("b2.y=410\xff\xff\xff");
//					printf("b12.y=405\xff\xff\xff");printf("b6.y=410\xff\xff\xff");printf("b7.y=410\xff\xff\xff");
//					printf("t0.y=410\xff\xff\xff");printf("b2.y=410\xff\xff\xff");
				}
				if (USART_RX_STA & 0x8000)
				{
						len=USART_RX_STA&0x3fff;
						line_cnt = USART_RX_BUF[0];
						switch(line_cnt)
						{
//							case 'R':moku_flag=1;m_angle = 180;break;
//							case 'S':moku_flag=2;stop_flag=1;break;
							case 'V':Draw_PID_flag=1;printf("page PID_wave\xff\xff\xff");break;
							case 'W':Draw_PID_flag=0;time_flag =1;printf("page time\xff\xff\xff");break;
//							case '0':flag = '0';stop_flag=0;moku_flag = 0;angle_flag=0;beep_cnt=0;beep_flag=1;
//											printf("t0.y=400\xff\xff\xff");printf("b2.y=510\xff\xff\xff");
//											printf("b12.y=405\xff\xff\xff");printf("b6.y=410\xff\xff\xff");printf("b7.y=410\xff\xff\xff");break;
						}
						USART_RX_STA=0;
				}
			}
	}
}


