/*
 * 220V.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "220V.h"

void V220(States state)
{
	switch (state)
	{
		case init:
		{

		P2SEL 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2SEL2 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2DIR 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2OUT 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2IES 	|= (Encoder1_1 + Encoder1_2 + Button1);		//Interrupt Edge Select on falling edge
		P2IFG 	&=~(Encoder1_1 + Button1);		//Encoder1_1+Encoder1_2+Button1 reset flags
		P2IE 	|= (Encoder1_1 + Button1);


		P3SEL 	&=~V220_PWM;
		P3SEL2 	&=~V220_PWM;
		P3DIR 	|= V220_PWM;
		P3OUT   &=~V220_PWM;
		temperature_220V_counter = 0;
		temperature_220V_maximum = DEFAULT_220V_TEMPERATURE_MAXIMUM;
		temperature_220V_steep = DEFAULT_TEMPERATURE_STEEP;
		_220V_State = init;
		break;
		}
		case stop:
		{
		P2OUT   &=~V220_PWM;
		temperature_220V_maximum = DEFAULT_220V_TEMPERATURE_MAXIMUM;
		_220V_State = stop;
		break;
		}
		case start:
		{
		//P2OUT 	|= V220_PWM;
		temperature_220V = 100;//need to remove only for simulate temperature
		temperature_220V_counter = 0;
		temperature_220V_required = 22;//Mlx90614_read_Register(MLX90614_TOBJ1);
		_220V_State = start;
		break;
		}
	}
}
void V220_cyclic_recalculate_PWM()
{
		static int16 i_Temp = 0;
		static int16 d_Temp = 0;

		float P_Term;
		float I_Term;
		float D_Term;

		float err_value;
		if (temperature_220V <= temperature_220V_maximum) //if (temperature_220V_steep <= temperature_220V_maximum) ?????????
		{
			temperature_220V += 2;//Mlx90614_read_Register(MLX90614_TOBJ1);

			temperature_220V_required += temperature_220V_steep;	//increment steep by steep temperature

			err_value = (temperature_220V_required - temperature_220V);
		P_Term = Kp_220V * err_value;

			i_Temp += err_value;
			if (i_Temp > Max_acumulated_error_220V) {i_Temp = Max_acumulated_error_220V;}
			else if (i_Temp < Min_acumulated_error_220V) {i_Temp = Min_acumulated_error_220V;}

		I_Term = Ki_220V * i_Temp;

		D_Term = Kd_220V * (d_Temp - err_value);
			d_Temp = err_value;

			temperature_220V_counter +=  (int16)(P_Term + I_Term + D_Term);

			if ( temperature_220V_counter > MAX_220V_COUNTERS ) temperature_220V_counter = MAX_220V_COUNTERS;
			else if ( temperature_220V_counter < 0 ) temperature_220V_counter = 0;
		}
		else	//temperature on 220V lamp ocured maximum
		{		//need to stop 220V lamp and start 24V lamp
			V220 (stop);
			V24  (start);
		}
		print("%f,%d\n",temperature_220V_required,temperature_220V_counter);
}
