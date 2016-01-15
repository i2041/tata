/*
 * 220V.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "220V.h"

void V220_Init(V220States state)
{
	switch (state)
	{
		case init:
		{

		P2SEL 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2SEL2 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2DIR 	&= ~(Encoder1_1 + Encoder1_2 + Button1);
		P2OUT 	&= ~(Encoder1_1 + Encoder1_2 + Button1);

		P2SEL 	&=~V220_PWM;
		P2SEL2 	&=~V220_PWM;
		P2DIR 	|= V220_PWM;
		P2OUT   &=~V220_PWM;

		break;
		}
		case stop:
		{
		P2OUT   &=~V220_PWM;
		break;
		}
		case start:
		{
		P2OUT 	|= V220_PWM;
		break;
		}
	}
}
void V220_Encoder(EncodeStates state)
{
	switch (state)
	{
	case rotireStanga:
	break;
	case rotireDreapta:
	break;
	case apasare:
	break;
	}
}
void V220_cyclic()
{

}
