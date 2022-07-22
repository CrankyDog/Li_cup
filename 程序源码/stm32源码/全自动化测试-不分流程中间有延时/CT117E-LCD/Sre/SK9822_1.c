#include "SK9822.h"

#define SK9822_PORT GPIOA
#define CKI_Pin	GPIO_Pin_5
#define SDI_Pin GPIO_Pin_7

/*Initialization function*/
void SK9822_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    SPI_InitTypeDef SPI_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    GPIO_InitStruct.GPIO_Pin = CKI_Pin | SDI_Pin;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(SK9822_PORT, &GPIO_InitStruct);

    SPI_I2S_DeInit(SPI2);
    SPI_InitStruct.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
    SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
    SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStruct.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStruct);

    SPI_Cmd(SPI1, ENABLE);
}

/*application layer functions*/
void SK9822_disp(u8 led_num,u8 led_data[][4])
{
    u8 i;
    SK9822_start();
    for(i=0; i<led_num; i++)
    {
        SK9822_writedata(*(led_data+i));
    }
    SK9822_over();
}
void SK9822_clear(u8 led_num)
{
    u8 i,led_clear[4]= {0,0,0,0};
    SK9822_start();
    for(i=0; i<led_num; i++)
    {
        SK9822_writedata(led_clear);
    }
    SK9822_over();
}

/*call layer function*/
void SK9822_start(void)
{
    spi1_sendbyte(0x00);
    spi1_sendbyte(0x00);
    spi1_sendbyte(0x00);
    spi1_sendbyte(0x00);
}
void SK9822_over(void)
{
    spi1_sendbyte(0xFF);
    spi1_sendbyte(0xFF);
    spi1_sendbyte(0xFF);
    spi1_sendbyte(0xFF);
}
void SK9822_writedata(u8 *led_data)
{
    spi1_sendbyte((*led_data)|0xE0);
    spi1_sendbyte(*(led_data+1));
    spi1_sendbyte(*(led_data+2));
    spi1_sendbyte(*(led_data+3));
}

void spi1_sendbyte(u8 data)
{
    while(!SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI1,data);
}












