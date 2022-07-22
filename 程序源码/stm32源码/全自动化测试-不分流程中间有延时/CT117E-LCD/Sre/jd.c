#include "jd.h"
#include "delay.h"

u16 angle(u16 jiaodu)//返回值类型一定要是u16     因为arr的值大于一个字节
	{
		u16 zkb=0;
		//0.5/45=time/0.18   ==>   time=0.002ms  ==>每转动0.18角度走时0.002ms=2us
//    zkb=jiaodu*14+250;//错误的
		zkb = jiaodu>180 ? (250+(360-jiaodu)/0.18) : (250+jiaodu/0.18);//250个2us+（角度/精度）=占空比
		return zkb;  
	} 	