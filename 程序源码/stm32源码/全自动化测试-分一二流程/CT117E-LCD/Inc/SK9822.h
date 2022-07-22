#include "stm32f10x.h"

/*Initialization function*/
void SK9822_init(void);

/*application layer functions*/
void SK9822_disp(u8 led_num,u8 led_data[][4]);
void SK9822_clear(u8 led_num);

/*call layer functions*/
void SK9822_start(void);
void SK9822_over(void);
void SK9822_writedata(u8 *led_data);

void spi1_sendbyte(u8 data);




