#include "draw.h"
#include "usart.h"
#include "delay.h"

void Draw_PID(u16 cur_angle,u16 tar_angle)
{
		printf("add 1,0,%d\xff\xff\xff",cur_angle*255/360);
		printf("add 1,1,%d\xff\xff\xff",tar_angle*255/360);
}

void Draw_Wavr(u16 part1,u16 part2,u16 part3,u16 part4)
{
		printf("add 1,0,%d\xff\xff\xff",part1);
		printf("add 2,0,%d\xff\xff\xff",part2);
		printf("add 3,0,%d\xff\xff\xff",part3);
		printf("add 4,0,%d\xff\xff\xff",part4);
}

