/*
 * io.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include <msp430i2041.h>
void init_io()
{
	P1DIR|= 0x01;	//For P1.0 	0000 0001
	P2DIR |= 0x04;	//For P2.2 	0000 0100
	P2DIR |= 0x08;	//For P2.3 	0000 1000

	P2DIR &= ~BIT1; // Set P2.1 to input direction
	P2IES &= ~BIT1;	//on raising
	P2IE |= BIT1; // enable P2.1 interrupt
}
void write_io(int pin,int state)
{
	switch(pin)
	{
	case 10:
		{
		if (state == 1){P1OUT |= 0x01;}//For P1.0 	0000 0001
		else {P1OUT &= ~0x01;}
		break;
		}
	case 22:
		{
		if (state == 1){P2OUT |= 0x04;}//For P2.2 	0000 0100
		else {P2OUT &= ~0x04;}
		break;
		}
	case 23:
		{
		if (state == 1){P2OUT |= 0x08;}//For P2.3 	0000 1000
		else {P2OUT &= ~0x08;}
		break;
		}
	default: break;
	}
}
// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	_BIC_SR(LPM4_EXIT); // wake up from low power mode
	 P2IFG &= ~BIT1;
}
