#include "MicArray.h"

#define MIC_PORT GPIOB
#define MIC_BCK GPIO_Pin_13
#define MIC_WS GPIO_Pin_12
#define MIC_SD GPIO_Pin_15

extern short *dma_data;
extern u16 len;
void MicArray_DMAConfig(u8 channel)
{
    DMA_InitTypeDef DMA_InitStruct;

    DMA_InitStruct.DMA_PeripheralBaseAddr = 0x4000380C;
    DMA_InitStruct.DMA_MemoryBaseAddr = (u32)(dma_data+channel*len/3);
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_BufferSize = len/3;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStruct.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel4,&DMA_InitStruct);
    DMA_Cmd(DMA1_Channel4,ENABLE);

    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);
}

void MicArray_init(u8 channel)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    I2S_InitTypeDef I2S_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStruct.GPIO_Pin = MIC_BCK | MIC_WS;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(MIC_PORT, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = MIC_SD;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(MIC_PORT, &GPIO_InitStruct);

    I2S_InitStruct.I2S_Mode = I2S_Mode_MasterRx;
    I2S_InitStruct.I2S_Standard = I2S_Standard_Phillips;
    I2S_InitStruct.I2S_DataFormat = I2S_DataFormat_24b;
    I2S_InitStruct.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    I2S_InitStruct.I2S_AudioFreq = I2S_AudioFreq_48k;
    I2S_InitStruct.I2S_CPOL = I2S_CPOL_Low;
    I2S_Init(SPI2,&I2S_InitStruct);

    MicArray_DMAConfig(channel);

    NVIC_InitStruct.NVIC_IRQChannel = DMA1_Channel4_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

    I2S_Cmd(SPI2,ENABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx, ENABLE);
}










