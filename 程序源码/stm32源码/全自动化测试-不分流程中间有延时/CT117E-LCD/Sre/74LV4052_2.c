#include "74LV4052.h"

/*Initialization function*/
void _74LV4052_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin =  _74LV4052_A0_Pin|_74LV4052_A1_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(_74LV4052_Port, &GPIO_InitStructure);

    GPIO_ResetBits(_74LV4052_Port,_74LV4052_A0_Pin|_74LV4052_A1_Pin);	//Ä¬ÈÏÑ¡Í¨Channel_0
}

/*application layer functions*/
void _74LV4052_SelectChannel(u8 _74LV4052_Channel)
{
    if(_74LV4052_Channel == _74LV4052_Channel_0)
        GPIO_ResetBits(_74LV4052_Port,_74LV4052_A0_Pin|_74LV4052_A1_Pin);
    else if(_74LV4052_Channel == _74LV4052_Channel_1)
    {
        GPIO_SetBits(_74LV4052_Port,_74LV4052_A0_Pin);
        GPIO_ResetBits(_74LV4052_Port,_74LV4052_A1_Pin);
    }
    else if(_74LV4052_Channel == _74LV4052_Channel_2)
    {
        GPIO_ResetBits(_74LV4052_Port,_74LV4052_A0_Pin);
        GPIO_SetBits(_74LV4052_Port,_74LV4052_A1_Pin);
    }
    else if(_74LV4052_Channel == _74LV4052_Channel_3)
    {
        GPIO_SetBits(_74LV4052_Port,_74LV4052_A0_Pin|_74LV4052_A1_Pin);
    }
}












