/*
 * task.c
 *
 *  Created on: 27 ian. 2016
 *      Author: uidg6243
 */
#include "task.h"
void task100ms()
{

}

void task500ms()
{
	uint16 tmpValue = (uint16)ADS1115_ConversionRegisterGradeCelsius(AIN0P_AIN1N);
	tlc5947_calculate_digits(0,tmpValue);

	if ( _24V_State != start )
	{
		if (V24_TimeSet == true)  tlc5947_calculate_digits(1,V24_Time_array[V24_elementFromArray_write]);
		else tlc5947_calculate_digits(1,V24_Temperature_array[V24_elementFromArray_write]);
	}
	else tlc5947_calculate_digits(1,555);//tlc5947_calculate_digits(1,Mlx90614_read_Register(MLX90614_TOBJ1));
	//update display for ciocan de lipit
	if ( (_ciocanLipit_ButtonValidate == (NrForValidateStates+1)) )
	{
		if ( (_ciocanLipit_Mode == temperatureMode) )
		{
			if (_ciocanLipit_EncoderValidate < NrForValidateStates) tlc5947_calculate_digits(2,_ciocanLipit_TemperatureTemporar);
			else tlc5947_calculate_digits(2,_ciocanLipit_Temperature_Average);
		}
		if ( (_ciocanLipit_Mode == pwmMode) )
		{
			tlc5947_calculate_digits(2,_ciocanLipit_ProcentageValue);
		}
	}
	else
	{
		tlc5947_calculate_digits(2,_ciocanLipit_Mode_temporar);
	}
	tlc5947_update_displays();
}

void task1s()
{
//	if (_24V_State == start)
//	{
//	V24V_recalculate_PWM();
//	V24_counterTime++;
//	}
}

void task33ms()
{
	static uint8 countTmpValue=0;


	if (_ciocanLipit_Mode == temperatureMode) recalculatePWM();
	newStateOcure();

	if (countTmpValue < temperature_220V_counter && _220V_State == start) P2OUT |= V220_PWM;
	else if (countTmpValue >= temperature_220V_counter && _220V_State == start) P2OUT &=~V220_PWM;

	if (_24V_State == start)
	{
		V24V_recalculate_PWM();
	}

	if ( countTmpValue %  3 == 0 ) 	{task100ms();}
	if ( countTmpValue % 15 == 0 ) 	{task500ms();}
	if ( countTmpValue % 30 == 0 ) 	{countTmpValue=0;task1s();}
	countTmpValue++;

	Flag33msOcured = false;
}

