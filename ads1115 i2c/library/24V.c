/*
 * 24V.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "24V.h"
void V24(States state)
{
	switch (state)
	{
		case init:
		{

		P2SEL 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P2SEL2 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P2DIR 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P1OUT 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P2IES 	|= (Encoder2_1 + Encoder2_2 + Button2);		//Interrupt Edge Select on falling edge
		P2IFG 	&=~(Encoder2_1 + Encoder2_2 + Button2);		//Encoder1_1+Encoder1_2+Button1 reset flags
		P2IE 	|= (Encoder2_1 + Button2);					//Encoder2_2


		P3SEL 	&=~V24_PWM;
		P3SEL2 	&=~V24_PWM;
		P3DIR 	|= V24_PWM;
		P3OUT   &=~V24_PWM;
		_24V_State = init;
		break;
		}
		case stop:
		{
		P3OUT   &=~V24_PWM;
		V24_elementFromArray_write=0;
		_24V_State = stop;
		break;
		}
		case start:
		{
		//P3OUT 	|= V24_PWM;
		V24_elementFromArray_read = 0;
		V24_counterTime = 0;
		_24V_State = start;
		break;
		}
	}
}
void V24V_recalculate_PWM()
{
	static float i_Temp = 0;
	static float d_Temp = 0;
	float P_Term;
	float I_Term;
	float D_Term;
	int16 err_value;
	float tmpTemperature;
	if ( V24_counterTime < V24_Time_array[V24_elementFromArray_read] )
	{
		V24_temperature = Mlx90614_read_Register(MLX90614_TOBJ1);
		if (V24_elementFromArray_read == 0 ) tmpTemperature = map(V24_counterTime,0,V24_Time_array[0],temperature_220V_maximum,V24_Temperature_array[1]);
		else tmpTemperature = map(,0,V24_Time_array[0],temperature_220V_maximum,V24_Temperature_array[1]);
		err_value = (_ciocanLipit_Temperature - current_tip_temperature);
		V24_counterTime++;
	}
	else V24(stop);
}
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
