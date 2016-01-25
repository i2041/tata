/*
 * 220V.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "220V.h"

void V220(States state)
{
	switch (state)
	{
		case init:
		{

		P2SEL 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2SEL2 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2DIR 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2OUT 	&=~(Encoder1_1 + Encoder1_2 + Button1);
		P2IES 	|= (Encoder1_1 + Encoder1_2 + Button1);		//Interrupt Edge Select on falling edge
		P2IFG 	&=~(Encoder1_1 + Button1);		//Encoder1_1+Encoder1_2+Button1 reset flags
		P2IE 	|= (Encoder1_1 + Button1);


		P3SEL 	&=~V220_PWM;
		P3SEL2 	&=~V220_PWM;
		P3DIR 	|= V220_PWM;
		P3OUT   &=~V220_PWM;
		_220V_State = init;
		break;
		}
		case stop:
		{
		P2OUT   &=~V220_PWM;
		_220V_State = stop;
		break;
		}
		case start:
		{
		P2OUT 	|= V220_PWM;
		_220V_State = start;
		break;
		}
	}
}
void V220_cyclic()
{
}
