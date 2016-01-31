/*
 * adc1115.c
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */
#include "ads1115.h"
#include "uart.h"

uint16 ADS1115_ConversionRegister(void)
{
	uint16 returnValue;
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Conversion_Register;
	Start_I2C_Transmition(1, WITH_STOP);

	Start_I2C_Reception(2);

	returnValue =  RxBuffer[0];
	returnValue =  returnValue<<8;
	returnValue += RxBuffer[1];

	return returnValue;
}
int16 	ADS1115_ConversionRegisterGradeCelsius(MUX mux_pin)
{
	int16 tmpValue;
	uint32 tmpValue2;
	float tmpValue3;
	float tmpValue4;
	float tmpValue5;
	const float T0	=	310.1897600;
	const uint16 V0	=	12631;
	const float p1	=	24.0619490;
	const float p2	=	4.0158622;
	const float p3	=	0.2685392;
	const float p4	=	-0.0097189;
	const float q1	=	0.1699587;
	const float q2	=	0.0114131;
	const float q3	=	-0.0003928;

	ADS1115_Write_ConfigRegister(mux_pin);
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
	return tmpValue;
}

void ADS1115_Write_ConfigRegister(MUX mux_pin)
{
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	TxBuffer[1] = ((BeginSingleConversion 	| mux_pin 							| V0_256_1 	| PowerDownSingleShortConversion)>>8 );
	TxBuffer[2] = ( SPS128 					| TraditionalComparatorHysteresis	| ActiveLow | Non_LatchingComparator | DisableComparator);
	Start_I2C_Transmition(3, WITH_STOP);
}
uint16 ADS1115_Read_ConfigRegister()
{
	uint16 returnValue;
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	Start_I2C_Transmition(1, WITH_STOP);

	Start_I2C_Reception(2);
	returnValue =  RxBuffer[0];
	returnValue =  returnValue<<8;
	returnValue += RxBuffer[1];
	return returnValue;
}
