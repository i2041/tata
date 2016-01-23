/*
 * 220V.h
 *
 *  Created on: 15 џэт. 2016 у.
 *      Author: Serghei
 */

#ifndef LIBRARY_220V_H_
#define LIBRARY_220V_H_

#include <msp430.h>
#include "commontypes.h"

#define Button1 BIT5	//PORT 2
#define Encoder1_1 BIT3	//PORT 2
#define Encoder1_2 BIT4	//PORT 2

#define V220_PWM BIT1	//PORT 3

void V220(States state);
void V220_Encoder(EncodeStates state);
void V220_cyclic();

uint8 _220V_State;

#endif /* LIBRARY_220V_H_ */
