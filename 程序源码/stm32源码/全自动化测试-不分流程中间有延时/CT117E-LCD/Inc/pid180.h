#ifndef _PID180_H_
#define _PID180_H_
#include "delay.h"

extern float want_value,current_value,err,last_err,intergal,output,Kp,Ki,Kd;
extern u16 PID_angle;

u16 PID_operation(u16 value);

#endif