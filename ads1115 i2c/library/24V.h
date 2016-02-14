/*
 * 24V.h
 *
 *  Created on: 16.01.2016
 *      Author: uidg6243
 */

#ifndef LIBRARY_24V_H_
#define LIBRARY_24V_H_

#include <msp430.h>
#include "commontypes.h"

#define Button2 	BIT2	//PORT 2
#define Encoder2_1 	BIT0	//PORT 2
#define Encoder2_2 	BIT1	//PORT 2

#define V24_PWM 	BIT0	//PORT 3

void V24(States state);

uint8 _24V_State;
void V24V_recalculate_PWM();

uint16 	V24_Temperature_array[5];
uint8 	V24_Time_array[5];
uint8   V24_elementFromArray_write;
uint8   V24_elementFromArray_read;
uint8   V24_temperature;
uint16  V24_counterTime;

#endif /* 24V_H_ */
