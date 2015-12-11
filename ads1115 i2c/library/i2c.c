/*
 * i2c.c
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */
#include "i2c.h"
#include "commontypes.h"
void transmit()
{
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
	//isBusy = true;
	//__bis_SR_register(GIE);        // interrupts
}
//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
//-------------------------------------------------------------------------------
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
  static uint8 count=0;
  if(IFG2 &= UCB0RXIFG)
  {                              				// Master Recieve?
	 //if (count < length)                     // Check RX byte counter
	 {
		//IFG2 &= ~UCB0RXIFG;               // Clear USCI_B0 RX int flag
		RxBuffer[count] = UCB0RXBUF; 		// Get RX data
		//IFG2 &= ~UCB0RXIFG;               // Clear USCI_B0 TX int flag
		count++;
	 }
	 if (count >= length)                     // Check RX byte counter
	 {
		UCB0CTL1 |= UCTXSTP;                // I2C stop condition
		 //IFG2 &= ~UCB0RXIFG;              // Clear USCI_B0 TX int flag
		//Rxbuffer[count] = UCB0RXBUF;
		count = 0;
	 }
	 IFG2 &= ~UCB0RXIFG;                     	// Clear USCI_B0 TX int flag
  }
  else											// Master Transmit
	{
		if (count < length)                  	// Check TX byte counter
		{
			//IFG2 &= ~UCB0TXIFG;               // Clear USCI_B0 TX int flag
			UCB0TXBUF = TxBuffer[count];        // Load TX buffer
			count++;                            // Decrement TX byte counter
		}
		else
		{
		  UCB0CTL1 |= UCTXSTP;                  // I2C stop condition
		  //IFG2 &= ~UCB0TXIFG;                 // Clear USCI_B0 TX int flag
		  count=0;
		}
		IFG2 &= ~UCB0TXIFG;                     // Clear USCI_B0 TX int flag
	}
	__bis_SR_register(GIE);        // interrupts
}
