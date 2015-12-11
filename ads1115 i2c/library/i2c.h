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

uint8* TxBuffer;
uint8 length;
uint8* RxBuffer;
bool isBusy;
#endif /* I2C_H_ */
