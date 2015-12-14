/*
 * adc1115.c
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */
#include "ads1115.h"

uint16 ADS1115_ConversionRegister(void)
{
	uint16 returnValue;
	//UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Conversion_Register;
	length = 1;
	Start_I2C_Transmition();

	length = 2;
	Start_I2C_Reception();

	returnValue =  RxBuffer[0];
	returnValue =  returnValue<<8;
	returnValue += RxBuffer[1];
	return returnValue;
}

void ADS1115_Write_ConfigRegister()
{
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	TxBuffer[1] = ((NoEfect | AIN0P_AIN1N 						| V6_144 	| ContinuousConversion)>>8 );
	TxBuffer[2] = ( SPS860 	| TraditionalComparatorHysteresis 	| ActiveLow | Non_LatchingComparator | DisableComparator);
	length = 3;
	Start_I2C_Transmition();
}
void ADS1115_Write_ConfigRegister_AIN0P_GND()
{
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	TxBuffer[1] = ((NoEfect | AIN0P_GND 						| V6_144 	| ContinuousConversion)>>8 );
	TxBuffer[2] = ( SPS860 	| TraditionalComparatorHysteresis 	| ActiveLow | Non_LatchingComparator | DisableComparator);
	length = 3;
	Start_I2C_Transmition();
}
void ADS1115_Write_ConfigRegister_AIN1P_GND()
{
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	TxBuffer[1] = ((NoEfect | AIN1P_GND 						| V6_144 	| ContinuousConversion)>>8 );
	TxBuffer[2] = ( SPS860 	| TraditionalComparatorHysteresis 	| ActiveLow | Non_LatchingComparator | DisableComparator);
	length = 3;
	Start_I2C_Transmition();
}
void ADS1115_Write_ConfigRegister_AIN2P_GND()
{
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	TxBuffer[1] = ((NoEfect | AIN2P_GND 						| V6_144 	| ContinuousConversion)>>8 );
	TxBuffer[2] = ( SPS860 	| TraditionalComparatorHysteresis 	| ActiveLow | Non_LatchingComparator | DisableComparator);
	length = 3;
	Start_I2C_Transmition();
}
void ADS1115_Write_ConfigRegister_AIN3P_GND()
{
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	TxBuffer[1] = ((NoEfect | AIN3P_GND 						| V6_144 	| ContinuousConversion)>>8 );
	TxBuffer[2] = ( SPS860 	| TraditionalComparatorHysteresis 	| ActiveLow | Non_LatchingComparator | DisableComparator);
	length = 3;
	Start_I2C_Transmition();
}
uint16 ADS1115_Read_ConfigRegister()
{
	uint16 returnValue;
	UCB0I2CSA = ADS1115_ADRESS;                         		// Slave Address is 069h
	TxBuffer[0] = Config_Register;
	length = 1;
	Start_I2C_Transmition();

	length = 2;
	Start_I2C_Reception();
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
