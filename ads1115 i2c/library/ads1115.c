/*
 * adc1115.c
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */
#include "ads1115.h"
#include <msp430.h>
#include "i2c.h"
void TransmitADC1115()
{
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
}
void ReceiveADC1115(void)
{
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 &= ~UCTR ;                     // Clear UCTR
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
	__bis_SR_register(CPUOFF + GIE);        // Enter LPM0 w/ interrupts
}
void configADS1115()
{
	_DINT();
//	void init_I2C(void) {
//	  _DINT();
//	  while (UCB0CTL1 & UCTXSTP);               		// Ensure stop condition got sent
//	  UCB0CTL1 |= UCSWRST;                      		// Enable SW reset
//	  UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode, MSB first
//	  UCB0CTL1 = UCSSEL_2 + UCSWRST;            		// Use SMCLK, keep SW reset
//	  UCB0BR0 = 0x50;                             		// fSCL = SMCLK/0xFF = ???
//	  UCB0BR1 = 0;
//	  UCB0I2CSA = 0x49;                         		// Slave Address is 069h
//	  UCB0CTL1 &= ~UCSWRST;                     		// Clear SW reset, resume operation
//	  IE2 |= UCB0RXIE;             						//Enable RX interrupt
//	  IE2 |= UCB0TXIE;                         			// Enable TX interrupt
//	}
}
