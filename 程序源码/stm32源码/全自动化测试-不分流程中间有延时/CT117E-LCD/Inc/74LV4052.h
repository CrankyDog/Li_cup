#include "stm32f10x.h"

#define _74LV4052_Port GPIOD
#define _74LV4052_A0_Pin GPIO_Pin_11	//µÍÎ»
#define _74LV4052_A1_Pin GPIO_Pin_13	//¸ßÎ»

#define _74LV4052_Channel_0 0x00
#define _74LV4052_Channel_1 0x01
#define _74LV4052_Channel_2 0x02
#define _74LV4052_Channel_3 0x03

/*Initialization function*/
void _74LV4052_init(void);

/*application layer functions*/
void _74LV4052_SelectChannel(u8 _74LV4052_Channel);






