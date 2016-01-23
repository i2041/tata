/*
 * CiocaneLipit.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "CiocaneLipit.h"
void ciocaneLipit(States state)
{
	switch (state)
	{
		case(init):
		{
			/*** Encoder 3 statia lipit***/
			P2SEL 	&=~(Encoder3_1 + Encoder3_2);
			P2SEL2 	&=~(Encoder3_1 + Encoder3_2);

			P2DIR 	&=~(Encoder3_1 + Encoder3_2);
			//P1OUT = (Encoder3_1 + Encoder3_2); Pull UP resistor

			P2IES 	|= (Encoder3_1 + Encoder3_2);		//Interrupt Edge Select on falling edge
			P2IFG 	&=~(Encoder3_1 + Encoder3_2);		//Encoder3_1 + Encoder3_2 reset flags
			P2IE 	|= (Encoder3_1 );//+ Encoder3_2);

			/*** Button 3 statia lipit***/
			P1SEL 	&=~Button3;
			P1SEL2 	&=~Button3;

			P1DIR 	&=~Button3;

			P1IES 	|= Button3;							//Interrupt Edge Select on falling edge
			P1IFG 	&=~Button3;							//Button3 reset flags
			P1IE 	|= Button3;

//			/*** Timer_B Set-Up ***/
			P3DIR	|= (CIOCAN_PWM1 + CIOCAN_PWM2);
			P3OUT	&=~(CIOCAN_PWM1 + CIOCAN_PWM2);
//			P3SEL	|= (CIOCAN_PWM1 + CIOCAN_PWM2);
//			P3SEL2	&=~(CIOCAN_PWM1 + CIOCAN_PWM2);
//			TA1CTL  = (TASSEL_2 + ID_1 + MC_0  + TAIE);	//SMCLK,div 2,Stop Mode, Interrupt Enable
//
//			TA1CCTL1 |= OUTMOD_3;						//Set/Reset
//			TA1CCTL2 |= OUTMOD_3;
//
//			TA1CCR0 |= MAX_DUTY_CYCLE;
//			_ciocanLipit_State = init;

			break;
		}
		case(stop_statie1):
		{
			P3SEL &= ~CIOCAN_PWM1;
			P3OUT &= ~CIOCAN_PWM1;
			_ciocanLipit_State = stop_statie1;
			break;
		}
		case(stop_statie2):
		{
			P3SEL &= ~CIOCAN_PWM2;
			P3OUT &= ~CIOCAN_PWM2;
			_ciocanLipit_State = stop_statie2;
			break;
		}
		case(stop):
		{
			P3SEL &= ~(CIOCAN_PWM1 + CIOCAN_PWM1);
			TA1CTL  = MC_0;							//Stop Mode
			P3OUT &= ~(CIOCAN_PWM1 + CIOCAN_PWM1);
			_ciocanLipit_State = stop;
			//ShutDownDisplay
			break;
		}
		case(start_statie1):
		{
			TA1CCR1 |= MIN_DUTY_CYCLE;
			P3SEL |= CIOCAN_PWM1;
			TA1CTL  = MC_2 + TACLR;							//Continuous mode , reset
			_ciocanLipit_State = start_statie1;
			break;
		}
		case(start_statie2):
		{
			TA1CCR2 |= MIN_DUTY_CYCLE;
			P3SEL |= CIOCAN_PWM2;
			TA1CTL  = MC_2 + TACLR;							//Continuous mode , reset
			_ciocanLipit_State = start_statie2;
			break;
		}
		case(start):
		{
			P3SEL |= CIOCAN_PWM1 + CIOCAN_PWM2;
			TA1CTL  = MC_2 + TACLR;							//Continuous mode , reset
			TA1CCR1 |= MIN_DUTY_CYCLE;
			_ciocanLipit_State = start;
			break;
		}
	}
}
void ciocaneLipit_Encoder(EncodeStates state)
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
void ciocaneLipit_cyclic()
{

}
