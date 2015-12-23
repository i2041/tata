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
//uint16 ADS1115_Read_ConfigRegister()
//{
//	uint16 returnValue;
//	TxBuffer[0] = Conversion_Register;
//	length = 1;
//	//Start_I2C_Reception();
//}
