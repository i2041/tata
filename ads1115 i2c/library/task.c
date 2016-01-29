/*
 * task.c
 *
 *  Created on: 27 ian. 2016
 *      Author: uidg6243
 */
#include "task.h"
void task100ms()
{
	const float T0	=	310.1897600;
	const uint16 V0	=	12631;

	const float p1	=	24.0619490;
	const float p2	=	4.0158622;
	const float p3	=	0.2685392;
	const float p4	=	-0.0097189;
	const float q1	=	0.1699587;
	const float q2	=	0.0114131;
	const float q3	=	-0.0003928;

	uint32 tmpValue2;
	float tmpValue3;
	float tmpValue4;
	float tmpValue5;


	ADS1115_Write_ConfigRegister(AIN0P_AIN1N);
	while ( (ADS1115_Read_ConfigRegister() & BeginSingleConversion) == 0){}

	tmpValue = ADS1115_ConversionRegister();

	if (tmpValue > 0x7FFF) tmpValue = 0;


	tmpValue2 = tmpValue;
	tmpValue2 = ( (tmpValue2*125) / 16);
	tmpValue = tmpValue2;
	tmpValue = tmpValue2 - V0;
	tmpValue3 = tmpValue;
	tmpValue3 /= 1000;
	tmpValue4 = ( tmpValue3 * p4)+p3;
	tmpValue4 = ( tmpValue4 * tmpValue3 )+ p2;
	tmpValue4 = ( tmpValue4 * tmpValue3 )+ p1;
	tmpValue4 = ( tmpValue4 * tmpValue3 );
	tmpValue5 = ( tmpValue3 * q3) + q2;
	tmpValue5 = ( tmpValue3 * tmpValue5) + q1;
	tmpValue5 = ( tmpValue3 * tmpValue5) + 1;
	tmpValue4 /= tmpValue5;
	tmpValue4 += T0;
	tmpValue4+=30;
	tmpValue = (uint16)tmpValue4;

	tlc5947_calculate_digits(0,tmpValue);
	tlc5947_calculate_digits(1,tmp2);

	//update display for ciocan de lipit
	if ( (_ciocanLipit_ButtonValidate == (NrForValidateStates+1)) )
	{
		if ( (_ciocanLipit_Mode == temperatureMode) )
		{
			if (_ciocanLipit_EncoderValidate < NrForValidateStates) tlc5947_calculate_digits(2,_ciocanLipit_TemperatureTemporar);
			else tlc5947_calculate_digits(2,_ciocanLipit_Temperature);
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

void task500ms()
{

}

void task1s()
{

}

void task33ms()
{
	static uint8 countTmpValue=0;


	if (_ciocanLipit_Mode == temperatureMode) recalculatePWM();
	newStateOcure();

	if ( countTmpValue %  3 == 0 ) 	{task100ms();}
	if ( countTmpValue % 15 == 0 ) 	{task500ms();}
	if ( countTmpValue % 30 == 0 ) 	{countTmpValue=0;task1s();}
	countTmpValue++;
}

