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


		P3DIR 	|= V24_PWM;
		P3OUT   &=~V24_PWM;
		P3SEL 	&=~V24_PWM;
		P3SEL2 	&=~V24_PWM;
		TA0CCR0  = TA0CCR0_FREQUENCE;
		TA0CTL   = (TASSEL_2 + ID_0 + MC_1 + TACLR);	//SMCLK,div 1,Up Mode, 16.5ms = 0xFFFF
		TA0CCTL2 = OUTMOD_3;						//Set/Reset

		V24_elementFromArray_write = 0;
		while ( V24_elementFromArray_write < V24_ARRAY_ELEMENTS )
		{
			V24_Time_array[V24_elementFromArray_write] = 0;
			V24_Temperature_array[V24_elementFromArray_write] = 0;
			V24_elementFromArray_write++;
		}
		V24_elementFromArray_write = 0;
		V24_Temperature_array[V24_elementFromArray_write] = V24_DEFAULT_TEMPERATURE;


		V24_TimeSet = true;
		_24V_State = init;
		break;
		}
		case stop:
		{
		P3SEL 	&=~V24_PWM;
		P3OUT   &=~V24_PWM;
		V24_elementFromArray_write = 0;
		while ( V24_elementFromArray_write < V24_ARRAY_ELEMENTS )
		{
			V24_Time_array[V24_elementFromArray_write] = 0;
			V24_Temperature_array[V24_elementFromArray_write] = 0;
			V24_elementFromArray_write++;
		}
		V24_elementFromArray_write = 0;
		V24_Temperature_array[V24_elementFromArray_write] = V24_DEFAULT_TEMPERATURE;
		V24_TimeSet = true;
		_24V_State = stop;
		break;
		}
		case start:
		{
		TA0CCR2 = TA0CCR0-100;
		P3SEL 	|= V24_PWM;
		V24_elementFromArray_read = 0;
		V24_counterTime = 0.99;
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
	int32 TA0CCRx_temporar;
	if ( V24_elementFromArray_read < V24_elementFromArray_write && (uint8)V24_counterTime <= V24_Time_array[V24_elementFromArray_write-1])
	{
		V24_temperature = 111;//Mlx90614_read_Register(MLX90614_TOBJ1);

		if (V24_elementFromArray_read == 0 )tmpTemperature = map(V24_counterTime,0,V24_Time_array[0],V24_DEFAULT_TEMPERATURE,V24_Temperature_array[0]);
		else tmpTemperature = map(V24_counterTime,V24_Time_array[V24_elementFromArray_read-1],V24_Time_array[V24_elementFromArray_read],V24_Temperature_array[V24_elementFromArray_read-1],V24_Temperature_array[V24_elementFromArray_read]);

		err_value = (V24_temperature - tmpTemperature);
		P_Term = Kp_24V * err_value;

			i_Temp += err_value;
			if (i_Temp > Max_acumulated_error_24V) {i_Temp = Max_acumulated_error_24V;}
			else if (i_Temp < Min_acumulated_error_24V) {i_Temp = Min_acumulated_error_24V;}

		I_Term = Ki_24V * i_Temp;

		D_Term = Kd_24V * (d_Temp - err_value);
			d_Temp = err_value;

			TA0CCRx_temporar = TA0CCR2 + (P_Term + I_Term + D_Term);

			if ( TA0CCRx_temporar > (TA0CCR0_FREQUENCE-100) )
			{TA0CCRx_temporar = (TA0CCR0_FREQUENCE-100);}
			else if ( TA0CCRx_temporar < (0+100) )
			{TA0CCRx_temporar = 0+100;}//(0+100)

			TA0CCR2 = TA0CCRx_temporar;

		print("%f,%f,%d,%d\n",V24_counterTime,tmpTemperature,V24_temperature,TA0CCR2);

		if ( (uint8)V24_counterTime >= V24_Time_array[V24_elementFromArray_read] ) {V24_elementFromArray_read++;}

		V24_counterTime+=0.033;
	}
	else V24(stop);
}
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
