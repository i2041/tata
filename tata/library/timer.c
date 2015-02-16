/*
 * timer.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include <msp430i2041.h>
#include "timer.h"
void init_timer(uint8 mode)
{
	if (mode == start)
	{
		CSCTL0 &= !(0x02);							//DCO bypass mode not selected
		CSCTL0 &= !(0x01);							//DCO operation with internal resistor
		CSCTL1 |= (0x00);							//Divide by 1 MCLK
		CSCTL1 |= (0x40);							//Divide by 1 SMCLK
		TA0CTL = TASSEL_2 + ID_3 + TAIE + MC_1;  	//SMCLK,UPMODE,InteruptEnebled
		TA0CCR0 = 0xACEA;							//500ms
		TA0CCTL0|= CCIE;							//interupt enable
	}
	if (mode == stop)
	{
		TA0CTL 		&= ~MC_3;							//Mode control. Setting MCx = 00h when Timer_A is not in use conserves power.	Stop mode: Timer is halted
		TA0CTL 		&= ~TAIE;							//Interrupt disabled
		TA0CTL 		&= ~TAIFG;							//cleare ifg
		TA0CCTL0 	&= ~CCIE;							//interupt enable
		activeMode = false;
		GlobalTimer=0;
		//comand_executed = false;
		//risingTime = 0;
		//comand_executed = false;
	}

}
// Timer A0 interrupt service routine
// Interrupt handler for TA0CCR0 CCIFG.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
	static uint8 tmpTimer=0;
	task500ms();
	if (tmpTimer == 0) {task500ms_impar();}
	tmpTimer++;
	if (tmpTimer>1) {tmpTimer=0;task1s();}
	TA0CCTL0 &= ~CCIFG;
	TA0CTL 	&= ~TAIFG;
	__enable_interrupt();
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
	switch( TA0IV )
	{
	 case 2: break; // CCR1 not used
	 case 4: break; // CCR2 not used
	 case 10:
     {
	 __enable_interrupt(); // overflow
	 break;
     }
	 case 14:
	 {
	 __enable_interrupt(); // overflow
	 break;
	 }
	}

}
void task500ms_impar()
{
	start_adc();
    ThermoCoupleTemperature();
    InternalTemperature();
}

void task500ms()
{

}
void task1s()
{
	static uint8 countError=0;
	GlobalTimer++;

	if (!comand_executed){verify_condition();}

	if (TCouple != couple_error)
	{
		countError=0;
		if (activeMode)
		{
			algortimul();
		}
	}
	else
	{
		if (countError >= NR_ERRORS)
		{
			countError=0;
			errorImplementaion(couple_error);
		}
		countError++;
	}
}
