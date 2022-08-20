#ifndef _PID_H
#define _PID_H
#include "delay.h"

extern float Kp,Ki,Kd;
extern float integral,add;
extern int v,err,last_err,former_err,pwm;
extern u16 pid_angle;
//extern u16 v0,v1,v2,v3,v4,v5;
//extern u16 v_1,v_2,v_3,v_4,v_5;
extern int dir;

u16 moku_go_err(u16 v_co_angle,u16 v_adcx);
int moku_go_dir(u16 v_co_angle,u16 v_adcx);
u16 moku_go_PID(u16 moku_go_angle,int dir);

u16 Incremental_PID(u16 current_angle,u16 target_angle);
u16 Position_PID(u16 current_angle,u16 target_angle);

#endif

