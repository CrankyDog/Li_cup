#include "jd.h"
#include "delay.h"

u16 angle(u16 jiaodu)//����ֵ����һ��Ҫ��u16     ��Ϊarr��ֵ����һ���ֽ�
	{
		u16 zkb=0;
		//0.5/45=time/0.18   ==>   time=0.002ms  ==>ÿת��0.18�Ƕ���ʱ0.002ms=2us
//    zkb=jiaodu*14+250;//�����
		zkb = jiaodu>180 ? (250+(360-jiaodu)/0.18) : (250+jiaodu/0.18);//250��2us+���Ƕ�/���ȣ�=ռ�ձ�
		return zkb;  
	} 	