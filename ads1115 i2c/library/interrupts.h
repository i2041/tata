/*
 * interrupts.h
 *
 *  Created on: 15.01.2016
 *      Author: uidg6243
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "commontypes.h"
#include <msp430.h>
#include "220V.h"
#include "CiocaneLipit.h"
#include "TLC5947.h"
#include "i2c.h"
#include "uart.h"
#include "task.h"
#include "ads1115.h"

bool encoder1 ;
bool encoder2 ;
bool encoder3 ;
bool Flag33msOcured;
uint8 tmp1;
uint8 tmp2;
uint8 tmp3;

#endif /* INTERRUPTS_H_ */
