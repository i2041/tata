/*
 * speaker.c
 *
 *  Created on: 12 feb. 2016
 *      Author: uidg6243
 */
/*
#include "speaker.h"
void speaker(States state)
{
	switch (state)
	{
		case init:
		{
			P3DIR 	|= V24_PWM;
			P3OUT   &=~V24_PWM;
			P3SEL 	&=~V24_PWM;
			P3SEL2 	&=~V24_PWM;
			TA0CCR0  = TA0CCR0_FREQUENCE;
			TA0CTL   = (TASSEL_2 + ID_0 + MC_1 + TACLR);	//SMCLK,div 1,Up Mode, 16.5ms = 0xFFFF
			TA0CCTL2 = OUTMOD_3;						//Set/Reset
			_speaker_State = init;
		break;
		}
		case stop:
		{
			P3SEL 	&=~V24_PWM;
			P3OUT   &=~V24_PWM;
			V24_elementFromArray_write=0;
			_speaker_State = stop;
		break;
		}
		case start:
		{
			TA0CCR2 = TA0CCR0-100;
			P3SEL 	|= V24_PWM;
			V24_elementFromArray_read = 0;
			V24_counterTime = 0;
			_24V_State = start;
		break;
		}
	}
}
*/
