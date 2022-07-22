#include "pid.h"
#include "delay.h"
#include "usart.h"

u16 pid_angle;
float Kp,Ki,Kd;
int v,err,last_err=0,former_err=0,pwm;
//u16 v0=0,v1=1,v2=2,v3=3,v4=4,v5=5;
//u16 v_1=-1,v_2=-2,v_3=-3,v_4=-4,v_5=-5;
float integral,add;
int dir = 0;

//角度判断
u16 moku_go_err(u16 v_co_angle,u16 v_adcx)
{
		err = v_co_angle - v_adcx;
		if(err>180)
		{
				err = 360-err;
				dir = 1;
		}
		else if(err>-181&err<0)
		{
				err = -err;
				dir = 1;
		}
		else if(err<-180)
		{
				err = 360+err;
				dir = -1;
		}
		else dir = -1;
		return err;
}

//方向
int moku_go_dir(u16 v_co_angle,u16 v_adcx)
{
		err = v_co_angle - v_adcx;
		if(err>180)
		{
				err = 360-err;
				dir = 1;
		}
		else if(err>-181&err<0)
		{
				err = -err;
				dir = 1;
		}
		else if(err<-180)
		{
				err = 360+err;
				dir = -1;
		}
		else dir = -1;
		return dir;
}

//moku_go PID控制
u16 moku_go_PID(u16 moku_go_angle,int dir)
{
	if(dir == -1)
		v	= (moku_go_angle > 5000) ? 5 : (moku_go_angle+600)/1000;
	else if(dir == 1)
		v	= (moku_go_angle > 5000) ? 5 : (moku_go_angle+800)/1200;
		v = v*dir;
		switch(v)
		{
				case 5:pid_angle = 0;break;
				case 4:pid_angle = 54;break;
				case 3:pid_angle = 63;break;
				case 2:pid_angle = 72;break;
				case 1:pid_angle = 81;break;
				case 0:pid_angle = 90;break;
				case -1:pid_angle = 99;break;
				case -2:pid_angle = 108;break;
				case -3:pid_angle = 117;break;
				case -4:pid_angle = 126;break;
				case -5:pid_angle = 180;break;
		}
		return pid_angle;
}

//增量式PID控制
u16 Incremental_PID(u16 current_angle,u16 target_angle)
{
		Kp=0,Ki=50,Kd=0;
		err = target_angle - current_angle;
		if(err>180)
		{
				err = 360-err;
				dir = 1;
		}
		else if(err>-181&err<0)
		{
				err = -err;
				dir = 1;
		}
		else if(err<-180)
		{
				err = 360+err;
				dir = -1;
		}
		else dir = -1;
//		printf("flag:%d\r\n",dir);
//		printf("err:%d\r\n",err);
//		printf("last_err:%d\r\n",last_err);
//		printf("former_err:%d\r\n",former_err);
		add = Kp*(err-last_err)+Ki*err+Kd*(err-2*last_err+former_err);
//		printf("add:%1f\r\n",add);
		pwm	= (add > 5000) ? 5 : add/1000;
		v = dir*pwm;
		former_err = last_err;
		last_err = err;
		switch(v)
		{
				case 5:pid_angle = 0;break;
				case 4:pid_angle = 54;break;
				case 3:pid_angle = 63;break;
				case 2:pid_angle = 72;break;
				case 1:pid_angle = 81;break;
				case 0:pid_angle = 90;break;
				case -1:pid_angle = 99;break;
				case -2:pid_angle = 108;break;
				case -3:pid_angle = 117;break;
				case -4:pid_angle = 126;break;
				case -5:pid_angle = 180;break;
		}
//		printf("pid_angle:%d\r\n",pid_angle);
		return pid_angle;
}


//位置式PID控制
u16 Position_PID(u16 current_angle,u16 target_angle)
{
		Kp=70,Ki=0,Kd=0;
//Kp不出现振荡的最大值为60，Ki不出现振荡的最大值为0.8
		err = target_angle - current_angle;
//		printf("err:%d\r\n",err);
		integral += err;
		if(integral>10000||integral<-10000)	integral=-integral;
//		printf("integral:%1f\r\n",integral);
		if(err>180)
		{
				err = 360-err;
				dir = 1;
		}
		else if(err>-181&err<0)
		{
				err = -err;
				dir = 1;
		}
		else if(err<-180)
		{
				err = 360+err;
				dir = -1;
		}
		else dir = -1;
//		printf("flag:%d\r\n",dir);
//		integral += err;
//		printf("integral:%1f\r\n",integral);
		add = Kp*err + Ki*integral + Kd*(err-last_err);
		if(add<0) add=-add;
//		printf("add:%1f\r\n",add);
		pwm	= (add > 5000) ? 5 : add/1000;
		v = dir*pwm;
		last_err = err;
		switch(v)
		{
				case 5:pid_angle = 0;break;
				case 4:pid_angle = 54;break;
				case 3:pid_angle = 63;break;
				case 2:pid_angle = 72;break;
				case 1:pid_angle = 81;break;
				case 0:pid_angle = 90;break;
				case -1:pid_angle = 99;break;
				case -2:pid_angle = 108;break;
				case -3:pid_angle = 117;break;
				case -4:pid_angle = 126;break;
				case -5:pid_angle = 180;break;
		}
//		printf("pid_angle:%d\r\n",pid_angle);
		return pid_angle;
}