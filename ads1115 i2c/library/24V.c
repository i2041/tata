/*
 * 24V.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "24V.h"
void V24(States state)
{
	switch (state)
	{
		case init:
		{

		P2SEL 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P2SEL2 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P2DIR 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P1OUT 	&=~(Encoder2_1 + Encoder2_2 + Button2);
		P2IES 	|= (Encoder2_1 + Encoder2_2 + Button2);		//Interrupt Edge Select on falling edge
		P2IFG 	&=~(Encoder2_1 + Encoder2_2 + Button2);		//Encoder1_1+Encoder1_2+Button1 reset flags
		P2IE 	|= (Encoder2_1 + Button2);					//Encoder2_2


		P3SEL 	&=~V24_PWM;
		P3SEL2 	&=~V24_PWM;
		P3DIR 	|= V24_PWM;
		P3OUT   &=~V24_PWM;
		_24V_State = init;
		break;
		}
		case stop:
		{
		P3OUT   &=~V24_PWM;
		_24V_State = stop;
		break;
		}
		case start:
		{
		P3OUT 	|= V24_PWM;
		_24V_State = start;
		break;
		}
	}
}
