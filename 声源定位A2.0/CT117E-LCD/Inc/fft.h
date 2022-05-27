#include "stm32f10x.h"


#define NPT 256
#define PI2 6.28318530717959
//采样率计算
//分辨率：Fs/NPT 
//#define Fs	10000
#define Fs	10240



void Creat_Single(long *dac_out_data,int vmax1,int vmax2,int vmax3,float fs1,float fs2,float fs3);
void Creat_Single_out(u16 *dac_out_data,int vmax,int vmax2,int vmax3,int fs1,int fs2,int fs3);
void GetPowerMag(long *lBufMagArray,long *lBufOutArray);
void SineWave_Data( u16 cycle ,u16 *D,float Um);

void fft_data_get(long *lBufOutArray,long *dma_data_32bit);
void fft_filter(u16 *dac_data,long *mag_data);
void InitBufInArray(void);
void max_value(long *mag_max_data,long *mag_data);
