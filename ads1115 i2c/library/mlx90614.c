/*
 * mlx90614.c
 *
 *  Created on: 22.12.2015
 *      Author: uidg6243
 */
#include <msp430.h>
#include "mlx90614.h"
float Mlx90614_read_Register(MLX90614_ADRESS adress)
{
	uint16 tmpValue;
	float returnValue;
	UCB0I2CSA = MLX90614_I2CADDR;
	TxBuffer[0] = adress;
	Start_I2C_Transmition(1, WITHOUT_STOP);

	Start_I2C_Reception(3);

	tmpValue =  RxBuffer[1];
	tmpValue =  tmpValue<<8;
	tmpValue += RxBuffer[0];

	returnValue = tmpValue;
	returnValue /= 50;	//Kelvin
	//returnValue -= 273;	//273.15;Celsius
	returnValue  -= 273.15;
	return returnValue;
}
