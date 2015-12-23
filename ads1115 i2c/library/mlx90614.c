/*
 * mlx90614.c
 *
 *  Created on: 22.12.2015
 *      Author: uidg6243
 */
#include <msp430.h>
#include "mlx90614.h"
uint16 Mlx90614_readObjectTempC()
{
	uint16 returnValue;

	UCB0I2CSA = MLX90614_I2CADDR;
	TxBuffer[0] = MLX90614_TA;
	Start_I2C_Transmition(1, WITHOUT_STOP);

	Start_I2C_Reception(3);

	returnValue =  RxBuffer[1];
	returnValue =  returnValue<<8;
	returnValue += RxBuffer[0];

	returnValue /= 50;	//Kelvin
	returnValue -= 273;	//273.15;Celsius
	 //temp *= .02;
	 //temp  -= 273.15;
	return returnValue;
}
