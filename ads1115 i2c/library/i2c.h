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
void Init_I2C();
void Start_I2C_Transmition();
void Start_I2C_Reception();
void Stop_I2C();
#endif /* I2C_H_ */
