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


#define STB_LENTH 2

u16 i=0,j=0;
u8 s1[100];
u8 buff_len=4;//�����ڿ��ƾ�ȷ��λ��׼�̶�����Ӧʱ�䣬ģʽ������֮һ
u8 *buff;
u8 buff_repeat[2],buff_repeat_num[2],buff_mov;
int tmp;
u8 mode = 0;//0:����ģʽ��1:���پ�׼ģʽ��2������������ģʽ��Ĭ�ϸ���ģʽ
/*74LV4052ȫ�ֱ���*/
u8 select_channel = 0;
/*MICȫ�ֱ���*/
u16 len = 1500;//һ��dmaɨ����������(shortΪ��λ),�����ڿ��Ʋɼ����ݳ��̣�ģʽ������֮һ
short *dma_data;
u8 DMA_FLAG_END = 0,MIC_MAX_DIR=13,MIC_S_DIR,LAST_MIC_S_DIR=13;//13Ϊ��Ĭ̬��12Ϊ����̬
u16 MIC_GROUP_MAX_DIR;
int max_voice;//������������(24b)
int threshold = 15000;//���ڿ��ƾ�Ĭ̬����ֵ��ģʽ������֮һ
/*LEDȫ�ֱ���*/
u8 led_data[12][4]= {0x1f,0x00,0x00,0x00,	//led�������ã�����(���0x1f)�������̣���
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00,
                     0x1f,0x00,0x00,0x00
                    };
//int level_set = 200000;
u8 LED_FLAG_LEVEL_ENABLE ;
u8 ang_stb_flag ;
u16 co_angle = 0;
float pre_angle = 0.0;
float co_pre_angle = 0.0;
u8 count10;
u8 ang_stb_cnt;

void start_dma_get(u8 channel)
{
    _74LV4052_SelectChannel(channel);//��ѡChannel
    MicArray_init(channel);//��ʼ��һ��MIC���ݲɼ�
}

void find_max_voice(void)
{
    for(i=0; i<len; i+=2)
    {
        tmp = ((int)(dma_data[i])<<8)+((unsigned short)dma_data[i+1]>>8);
        if(tmp > max_voice)
        {
            MIC_GROUP_MAX_DIR=i;
            max_voice = tmp;
        }
    }
}

void buff_proc(void)
{
    for(i=0; i<buff_len-1; i++)
        buff[i]=buff[i+1];
    buff[buff_len-1]=MIC_MAX_DIR;
}

void data_proc(void)
{
    /*step1:���ж���λ��*/
    if(max_voice < threshold)//�ж��Ƿ�Ϊ��Ĭ̬
        MIC_MAX_DIR = 13;//
    else if(MIC_GROUP_MAX_DIR<len/3)//������Ч���ֵλ�ã����Զ�λ��Դλ��
        MIC_MAX_DIR = 0;
    else if(MIC_GROUP_MAX_DIR<len*2/3)
        MIC_MAX_DIR = 4;
    else if(MIC_GROUP_MAX_DIR<len)
        MIC_MAX_DIR = 8;
//	printf("�֣�%d\r\n",MIC_S_DIR);
    /*step2:���ж���˷�λ��*/
    if(MIC_GROUP_MAX_DIR%4 && MIC_MAX_DIR != 13)//�Ǿ�Ĭ̬ʱ���ж���������
        MIC_MAX_DIR+=2;
//	printf("���Ҵ֣�%d\r\n",MIC_S_DIR);
    /*step3:��ȷ��λ*/
    buff_proc();	//��ʷ���ݸ���
    MIC_S_DIR = MIC_MAX_DIR;//�ٶ���ǰ���ֵΪ�ȶ�̬

    buff_repeat[0]=buff[0];
    for(i=1,buff_mov=0; i<buff_len; i++) //����buff��Ѱ���Ƿ�������м����
    {
        for(j=0; j<=buff_mov; j++)	//���Ѽ�¼��Ԫ���б���
        {
            if(buff[i]==buff_repeat[j])//����ظ�
            {
                //buff_repeat_num[j]++;
                break;
            }
        }
        if(j>buff_mov)//δ�ҵ���Ԫ��
        {
            buff_mov++;
            if(buff_mov>1)
            {
                MIC_S_DIR = LAST_MIC_S_DIR;//����̬�����������һ���ȶ�״̬
                break;
            }
            else
                buff_repeat[buff_mov] = buff[i];//����buff_repeat��
        }
    }
    //buff_repeat_num[0] = 0;
    //buff_repeat_num[1] = 0;

    if(i == buff_len)//ֻ������Ԫ�����
    {
        if(buff_mov)
        {
            MIC_S_DIR = ((buff_repeat[0]>buff_repeat[1]) ? (buff_repeat[0]-buff_repeat[1]):(buff_repeat[1]-buff_repeat[0]));
            if(MIC_S_DIR == 2)
                MIC_S_DIR = ((buff_repeat[0]>buff_repeat[1]) ? buff_repeat[1]:buff_repeat[0])+1;
            else if(MIC_S_DIR == 10)
                MIC_S_DIR = 11;
            else
                MIC_S_DIR = LAST_MIC_S_DIR;
        }
    }
}


void disp_proc(void)
{
    /*led_data���ݴ���*/
    for(i=0; i<12; i++)
    {
        if(MIC_S_DIR != i)
        {
            led_data[i][1] = 0x00;
            led_data[i][3] = 0x00;
        }
        else
        {
            led_data[i][1] = 0xff;
            led_data[i][3] = 0x00;

        }


//			if(LED_FLAG_LEVEL_ENABLE)
//			{
//				if(max_voice < level_set)
//					light_level = 1;
//				else
//					light_level = 2;
//			}
//			else
//				light_level = 1;
//			if(light_level == 1)//��ɫ
//			{
//				led_data[i][1] = 0xff;
//				led_data[i][3] = 0x00;
//			}
//			else if(light_level == 2)//��ɫ
//			{
//				led_data[i][1] = 0x00;
//				led_data[i][3] = 0xff;
//			}

    }
//    printf("%d\r\n",MIC_S_DIR);

    SK9822_disp(12,led_data);

}
//void dma_data_16to32bit()
//{
//    for(i=0,j=0; i<len-1; i+=2)
//    {
//        dma_data_32bit[j] = dma_data[i]+dma_data[i+1];
//        j++;
//    }
//}
//��λ�����
void position_pre_ang ()
{
    int 	sum=0;
//	int		valid_len=buff_len;
    float	ang=0.0;
    ang_stb_flag = 1;
	count10 = 0;
    for(i=0; i<buff_len; i++)
    {
        if(buff[i]==13)
        {
            ang_stb_flag = 0;
        }
		 if(buff[i]==10)
        {
            count10 ++;
        }
        sum+=buff[i];
//		printf("buff[%d]:%d\r\n",i,buff[i]);
    }
	if(count10 <buff_len/3)
		ang = sum*1.0/buff_len*1.0*30;
	else
		ang	= 330;
	
//	printf("sum:%d ang:%lf\r\n",sum,ang);
    co_pre_angle = ang;
    if(ang_stb_flag	 &&	((int)co_pre_angle%30==0))
    {
        pre_angle=co_pre_angle;
    }
//printf("pre_angle:%lf\r\n",pre_angle);

    //return ang;

}

void position_co_ang (void)
{
	if(MIC_S_DIR == LAST_MIC_S_DIR)
		ang_stb_cnt++;
	if(MIC_S_DIR != 13 && ang_stb_cnt >= STB_LENTH)
	{
		co_angle = MIC_S_DIR*30;
		ang_stb_cnt = 0;
	}
}


void EXTI0_IRQHandler(void)
{
    EXTI_ClearFlag(EXTI_Line0);
    delay_ms(5);
    if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))
    {
        LED_FLAG_LEVEL_ENABLE = !LED_FLAG_LEVEL_ENABLE;
//		if(LED_FLAG_LEVEL_ENABLE)
//			LCD_ShowString(8,90,240,16,16,"KEY_UP: On  -> LEVEL_ENABLE");
//		else
//			LCD_ShowString(8,90,240,16,16,"KEY_UP: Off -> LEVEL_ENABLE");
    }
}

void EXTI3_IRQHandler(void)//����ģʽ
{
    EXTI_ClearFlag(EXTI_Line3);
    delay_ms(5);
    if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3))
    {
        mode++;
        if(mode>2)
            mode = 0;
        if(mode == 0)//����ģʽ
        {
            len = 1500;
            buff_len = 2;
            threshold = 30000;
        }
        else if(mode == 1)//���پ�ȷģʽ
        {
            len = 6000;
            buff_len = 4;
            threshold = 15000;
        }
        else//����������ģʽ
        {
            len = 19500;
            buff_len = 10;
            threshold = 15000;
        }
//		sprintf((char*)s1,"KEY1:   %u   -> MODE        ",mode);
        LCD_ShowString(8,70,240,16,16,s1);
    }
}

void DMA1_Channel4_IRQHandler(void)//DMA��������жϺ���
{
    if(DMA_GetFlagStatus(DMA1_FLAG_TC4))
    {
        SPI_I2S_DeInit(SPI2);//��û��������SPI2�������DMA���մ�λ
        DMA_DeInit(DMA1_Channel4);//��û��DMA�ж������ã�DMA���޷������ж�

        select_channel++;//ѡ����һͨ��
        if(select_channel>_74LV4052_Channel_2)//ɨ�����һ��
        {
            select_channel = _74LV4052_Channel_0;
            DMA_FLAG_END = 1;
        }
        else
        {
            start_dma_get(select_channel);
        }
    }
}
