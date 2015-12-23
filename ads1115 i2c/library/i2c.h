/*
 * i2c.h
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */

#ifndef I2C_H_
#define I2C_H_

#include "commontypes.h"
#include <msp430.h>

uint8 TxBuffer[10];
uint8 length;
uint8 RxBuffer[10];
bool isBusy;
typedef enum
{
	WITHOUT_STOP 	= 0,
	WITH_STOP 		= 1
}I2C_STATE;
void Init_I2C();
void Start_I2C_Transmition(uint8 size, I2C_STATE withstop);
void Start_I2C_Reception(uint8 size);
void Stop_I2C();
void I2C_TX_Interrupt();
void I2C_RX_Interrupt();

#endif /* I2C_H_ */
