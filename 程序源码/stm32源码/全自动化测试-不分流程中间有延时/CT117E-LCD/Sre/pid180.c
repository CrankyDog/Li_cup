#include "pid180.h"
#include "delay.h"

float	want_value=0.0,current_value=0.0,err=0.0,last_err=0.0,intergal=0.0,output=0.0;
float Kp=0.4,Ki=0.3,Kd=0.2;
u16 PID_angle = 0;

u16 PID_operation(u16 value)
{
		float index;
		want_value = value;
		err = want_value - value;
		if(__fabs(err)<value*0.01)
				return value;
		if(__fabs(err)>100)
				index = 0;
		else
		{
				index = 1;
				intergal += err;
		}
		output = Kp*err + index*Ki*intergal + Kd*(err - last_err);
		last_err = err;
		PID_angle = current_value + output;
		return PID_angle;
}