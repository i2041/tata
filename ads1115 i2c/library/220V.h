/*
 * 220V.h
 *
 *  Created on: 15 џэт. 2016 у.
 *      Author: Serghei
 */

#ifndef LIBRARY_220V_H_
#define LIBRARY_220V_H_

#include <msp430.h>

#define Button1 BIT5	//PORT 2
#define Encoder1_1 BIT3	//PORT 2
#define Encoder1_2 BIT4	//PORT 2

#define V220_PWM BIT1	//PORT 3

typedef enum
{
	init=0,
	stop,
	start
}V220States;
typedef enum
{
	rotireStanga,
	rotireDreapta,
	apasare
}EncodeStates;

void V220_Init(V220States state);
void V220_Encoder(EncodeStates state);
void V220_cyclic();


#endif /* LIBRARY_220V_H_ */
