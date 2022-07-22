#ifndef _SYDW_H_
#define _SYDW_H_
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

#define STB_LENTH 2

/*公共全局变量*/
extern u16 i,j;
extern u8 s1[100];
extern u8 buff_len;//可用于控制精确定位精准程度与响应时间，模式三变量之一
extern u8 *buff;
extern u8 buff_repeat[2],buff_repeat_num[2],buff_mov;
extern int tmp;
extern u8 mode;//0:高速模式，1:中速精准模式，2：最大采样低速模式。默认高速模式
/*74LV4052全局变量*/
extern u8 select_channel;
/*MIC全局变量*/
extern u16 len;//一轮dma扫描数据总量(short为单位),可用于控制采集数据长短，模式三变量之一
extern short *dma_data;
extern u8 DMA_FLAG_END,MIC_MAX_DIR,MIC_S_DIR,LAST_MIC_S_DIR;//13为静默态，12为保留态
extern u16 MIC_GROUP_MAX_DIR;
extern int max_voice;//左右声道数据(24b)
extern int threshold;//用于控制静默态门限值，模式三变量之一
/*LED全局变量*/
extern u8 led_data[12][4];
//int level_set = 200000;
extern u8 LED_FLAG_LEVEL_ENABLE ;
extern u8 ang_stb_flag ;
extern u16 co_angle;
extern float pre_angle;
extern float co_pre_angle;
extern u8 count10;
extern u8 ang_stb_cnt;
										
void find_max_voice(void);
void data_proc(void);
void buff_proc(void);
void disp_proc(void);
void start_dma_get(u8 channel);
void position_pre_ang (void);
void position_co_ang (void);

#endif