/*
 * interrupts.c
 *
 *  Created on: 05 џэт. 2016 у.
 *      Author: Serghei
 */
#include "interrupts.h"

#pragma vector=TIMER1_A1_VECTOR	//timer 1
__interrupt void Timer_A1 (void)
{
	switch(TA1IV)
	{
	 case 2:	// CCR1
	 break;
	 case 4:	// CCR2
	 break;
	 case 10: 	// overflow ? reserved
	 break;
	 case 14: 	// overflow ?
	 break;
	 default:	//default :)
	 break;
	}
 	__enable_interrupt();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void) {

	switch(P1IFG)
	{
	case (BIT0):
	break;

	case (BIT1):
	break;

	case (BIT2):
	break;

	case (BIT3):
	P1IFG &= ~ BIT3;	//Button3 PRESSED
	break;

	case (BIT4):
	break;

	case (BIT5):
	break;

	case (BIT6):
	break;

	case (BIT7):
	break;

	default://warning apear interrupt, to do something
	__enable_interrupt();
	break;
	}
	__enable_interrupt();
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void) {

	switch(P2IFG)
	{
	case (BIT0):
	P2IFG &= ~ BIT0;	//encoder 2.1
	break;

	case (BIT1):
	P2IFG &= ~ BIT1;	//encoder 2.2
	break;

	case (BIT2):
	P2IFG &= ~ BIT2;	//button2
	break;

	case (BIT3):
	P2IFG &= ~ BIT3;	//encoder 1.1
	break;

	case (BIT4):
	P2IFG &= ~ BIT4;	//encoder 1.2
	break;

	case (BIT5):
	P2IFG &= ~ BIT5;	//button1
	break;

	case (BIT6):
	P2IFG &= ~ BIT6;	//encoder 3.1
	break;

	case (BIT7):
	P2IFG &= ~ BIT7;	//encoder 3.2
	break;

	default://warning apear interrupt, to do something
	__enable_interrupt();
	break;
	}
	__enable_interrupt();
}
