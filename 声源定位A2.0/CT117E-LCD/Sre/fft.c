#include "fft.h"
#include "math.h"
#include "stm32_dsp.h"
#include "table_fft.h"


extern u16 i,j;

void Creat_Single(long *dac_out_data,int vmax1,int vmax2,int vmax3,float fs1,float fs2,float fs3)
{
    u16 i = 0;
    float fx=0.0;

    for(i=0; i<NPT; i++)
    {
        fx = 2048+vmax1*sin(PI2 * i * fs1/ Fs)+
             vmax2*sin(PI2 * i * fs2 / Fs)+
             vmax3*sin(PI2 * i * fs3 / Fs);
        dac_out_data[i] = ((signed short)fx) << 16;
    }
}

void Creat_Single_out(u16 *dac_out_data,int vmax,int vmax2,int vmax3,int fs1,int fs2,int fs3)
{
    u16 i = 0;

    for(i=0; i<NPT; i++)
    {
        dac_out_data[i] = 2048+vmax*sin(PI2 * i * 1.0/(NPT-1))+vmax2*sin(PI2 * i * fs2 / (fs1*(NPT-1)))
                          +vmax3*sin(PI2 * i * fs3 / (fs1*(NPT-1)));
    }
}


//获取FFT后的频率分量
void GetPowerMag(long *lBufMagArray,long *lBufOutArray)
{
    signed short lX,lY;
    float X,Y,Mag;
    unsigned short i;
    for(i=0; i<NPT/2; i++)
    {
        lX  = (lBufOutArray[i] << 16) >> 16;
        lY  = (lBufOutArray[i] >> 16);

        //除以32768再乘65536是为了符合浮点数计算规律
        X = NPT * ((float)lX) / 32768;
        Y = NPT * ((float)lY) / 32768;
        Mag = sqrt(X * X + Y * Y)*1.0/ NPT;
        if(i == 0)
            lBufMagArray[i] = (unsigned long)(Mag * 32768);
        else
            lBufMagArray[i] = (unsigned long)(Mag * 65536);
    }
}

void fft_data_get(long *lBufOutArray,long *dma_data_32bit)
{
    long lBufInArray[NPT];
    for(i=0; i<NPT; i++)
        lBufInArray[i] = dma_data_32bit[i];
    //cr4_fft_1024_stm32(lBufOutArray, lBufInArray, NPT);							//FFT变换
    cr4_fft_256_stm32(lBufOutArray, lBufInArray, NPT);
}

void fft_filter(u16 *dac_data,long *mag_data)
{
    int vmax1,vmax2,vmax3;
    int f1,f2,f3;
    vmax1 = mag_data[1];
    vmax2 = mag_data[1];
    vmax3 = mag_data[1];
    f1 = 1*Fs/NPT;
    f2 = 1*Fs/NPT;
    f3 = 1*Fs/NPT;

    for(i = 1; i<NPT/2; i++)
    {

        if(mag_data[i] >= vmax1)
        {
            vmax3 = vmax2;
            vmax2 = vmax1;
            vmax1 = mag_data[i];
            f3 = f2;
            f2 = f1;
            f1 = i*Fs/NPT;
        }

        else	if(mag_data[i] >= vmax2)
        {
            vmax3 = vmax2;
            vmax2 = mag_data[i];
            f3 = f2;
            f2 = i*Fs/NPT;
        }

        else	if(mag_data[i] >= vmax3)
        {
            vmax3 = mag_data[i];
            f3 = i*Fs/NPT;
        }
    }
    Creat_Single_out(dac_data,vmax1,vmax2,vmax3,f1,f2,f3);
}

void max_value(long *mag_max_data,long *mag_data)
{
    for(i = 2; i<NPT/2; i++)
    {
        if(mag_data[i]>mag_data[i-1]&& mag_data[i]>mag_data[i+1] && ((mag_data[i]-mag_data[i-1])>50||(mag_data[i]-mag_data[i+1])>50))
            mag_max_data[i] = mag_data[i];
        else
            mag_max_data[i]=0;
    }



}

