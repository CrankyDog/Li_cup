#include "Speech.h"
#include "delay.h"
#include "timer.h"
#include "GPIO.h"
#include "usart.h"

u8 location_flag=0,improve_flag=0,play_flag=0,denoising_flag=0,stop_flag2=1,Fc_flag=1,nothing_flag=0,ori_flag=0;
u8 cnt_i,cnt_j;

void Open_Equ(void)
{
	B7 = 0;	B1 = 1;	B2 = 1;	B0 = 0;
}	
void Open_Loc(void)	
{	
	B7 = 1;	B1 = 0;	B2 = 0;	B0 = 1;
}	
void Open_Spe(void)	
{	
	B7 = 0;	B1 = 1;	B2 = 0;	B0 = 0;
}	
void Spe_Org(void)	
{	
	B7 = 1; B1 = 1;	B2 = 0;	B0 = 1;
}	
void Spe_Equ(void)	
{	
	B7 = 0; B1 = 0;	B2 = 1;	B0 = 0;
}	
void Spe_Wit(void)	
{	
	B7 = 1; B1 = 0;	B2 = 1;	B0 = 1;
}

void Speech_JG(void)
{
	if(location_flag==1)
		stop_flag2=0;
	else if(location_flag==0)
		stop_flag2=1;
	//开启/关闭声源定位

	if(Fc_flag){}
	else if(improve_flag==1){
		for(cnt_j=0;cnt_j<10;cnt_j++){
			Line4=0;Line3=1;Line2=0;Line1=0;Line0=0;
			delay_ms(50);
			Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
			delay_ms(50);
			Line4=0;Line3=1;Line2=0;Line1=0;Line0=1;
			delay_ms(50);
			Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
			delay_ms(50);
		}
		Fc_flag = 1;
	}
	else if(improve_flag==0){
		for(cnt_i=0;cnt_i<10;cnt_i++){
			Line4=0;Line3=0;Line2=1;Line1=1;Line0=1;
			delay_ms(50);
			Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
			delay_ms(50);
			Line4=0;Line3=1;Line2=0;Line1=1;Line0=0;
			delay_ms(50);
			Line4=0;Line3=0;Line2=0;Line1=0;Line0=0;
			delay_ms(50);
		}
		Fc_flag = 1;
	}
	//提高/恢复音色
	if(play_flag==1){
		if(ori_flag==1){
			Moku1 = '0';	Moku0 = '1';
		}
		else if(denoising_flag==1){
			Moku1 = '1';	Moku0 = '1';}
		else if(denoising_flag==0){
			Moku1 = '1';	Moku0 = '0';}
	}
	else if(play_flag==0){
		Moku1 = '0';	Moku0 = '0';
	}
}

void Speech_SP(void)
{
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==0)
		location_flag=1;
	//开启声源定位
	
	else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)==SET)
		location_flag=0;
	//关闭声源定位
	
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)==SET){
		if(improve_flag==0){
			Fc_flag=0;
			improve_flag=1;
		}
	}
	//提亮音色
	
	else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)==0)
		if(improve_flag==1){
			Fc_flag=0;
			improve_flag=0;
		}
	//恢复音色
	
	if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)==SET)&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==0)){
		play_flag=1;
		denoising_flag=0;
		ori_flag=0;
		printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
		printf("t7.txt=\"On\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");
	}
	//播放语音/关闭去噪
	
	else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)==0)&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==SET)){
		play_flag=1;
		denoising_flag=1;
		ori_flag=0;
		printf("t5.txt=\"\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
		printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"On\"\xff\xff\xff");
	}
	//开启去噪
	
	else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)==SET)&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==SET)){
		play_flag=0;
		ori_flag=0;
		printf("t5.txt=\"On\"\xff\xff\xff");printf("t6.txt=\"\"\xff\xff\xff");
		printf("t7.txt=\"\"\xff\xff\xff");printf("t8.txt=\"\"\xff\xff\xff");
	}
	//关闭语音	
	
	else if((GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)==0)&(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)==0))
		nothing_flag=1;
	//不操作
	
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)==SET){
		flag = '1';m_angle = 180;printf("page time\xff\xff\xff");time_flag=1;
	}
}

