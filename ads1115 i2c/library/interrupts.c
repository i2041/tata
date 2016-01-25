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

	case (BIT3):	//Button 3
	P1IFG 	&= ~ BIT3;
	P3OUT ^= (CIOCAN_PWM1 + CIOCAN_PWM2);
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
	P1IFG 	&=0;
	break;
	}
	__enable_interrupt();
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void) {

	if (P2IFG & BIT0)	//encoder 2.1
	{
	P2IFG &= ~ BIT0;
	if (P2IN & BIT1) tmp2 -= 1;
	else tmp2 += 1;
	}
	if (P2IFG & BIT2)	//button2
	{
	P2IFG &= ~ BIT2;
	P3OUT ^= BIT0;
	}
	if (P2IFG & BIT3)	//encoder 1.1
	{
	P2IFG &= ~ BIT3;
	if (P2IN & BIT4) tmp1 -= 1;
	else tmp1 += 1;
	}
	if (P2IFG & BIT5)	//button1
	{
	P2IFG &= ~ BIT5;
	P3OUT   ^= V220_PWM;
	}
	if (P2IFG & BIT6)	//encoder 3.1
	{
	P2IFG &= ~ BIT6;
	if (P2IN & BIT7) tmp3 -= 1;
	else tmp3 += 1;
	}
	__enable_interrupt();
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	if (IFG2 & UCB0RXIFG) {I2C_RX_Interrupt();IFG2 &= ~UCB0RXIFG;}
	if (IFG2 & UCB0TXIFG) {I2C_TX_Interrupt();IFG2 &= ~UCB0TXIFG;}
	if (IFG2 & UCA0TXIFG) {Uart_TX_Interrupt();IFG2 &= ~UCA0TXIFG;}
	if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
	__bis_SR_register(GIE);        // interrupts
}
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
	__bis_SR_register(GIE);        // interrupts
}
