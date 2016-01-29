/*
 * pid.c
 *
 *  Created on: 25 џэт. 2016 у.
 *      Author: Serghei
 */
#include "pid.h"
uint16 pid_calculation(int16 temperature_error, uint16 pwm_register)
{
	//Local variables for PID

	float P_Term;
	float I_Term;
	float D_Term;



	P_Term = Kp * temperature_error;
	i_Temp += temperature_error;

	if (i_Temp > iMax)
	{i_Temp = iMax;}
	else if (i_Temp < iMin)
	{i_Temp = iMin;}

	I_Term = Ki * i_Temp;

	D_Term = Kd * (d_Temp - temperature_error);
	d_Temp = temperature_error;

	pwm_register = pwm_register - (P_Term + I_Term + D_Term);
	return pwm_register;
}



