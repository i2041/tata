/*
 * i2c.c
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */
#include "i2c.h"
#include "commontypes.h"
void Init_I2C()
{
	_DINT();

	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0

	while (UCB0CTL1 & UCTXSTP);               		// Ensure stop condition got sent

	UCB0CTL1 |= UCSWRST;                      			// Enable SW reset

	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     			// I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            			// Use SMCLK, keep SW reset				????????? poate facem numai |=(sau egal) fara keep soft reset
	//UCB0BR0 = 0x50;                             		// 100kHz on 8Mhz
	//UCB0BR0 = 0x13;                             		// 400kHz on 8Mhz
	//UCB0BR0 = 0x9;                             		// 400kHz on 4Mhz
	UCB0BR0 = 0x27;                             		// 98.40kHz on 4Mhz
	UCB0BR1 = 0;
	//UCB0I2CSA = 0x49;                         		// Slave Address is 069h

	UCB0CTL1 &= ~UCSWRST;                     			// Clear SW reset, resume operation

	IE2 |= UCB0RXIE;             						//Enable RX interrupt
	IE2 |= UCB0TXIE;                         			// Enable TX interrupt
	_EINT();
}
void Start_I2C_Transmition(uint8 size, I2C_STATE withstop)
{
	length = size;
	while (isBusy);
	isBusy = true;
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 |= UCTR + UCTXSTT;             // I2C TX, start condition
	while (isBusy);
	if (withstop) Stop_I2C();                // I2C stop condition
}
void Start_I2C_Reception(uint8 size)
{
	length = size;
	while (isBusy);
	isBusy = true;
	while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
	UCB0CTL1 &= ~UCTR ;                     // Clear UCTR
	UCB0CTL1 |= UCTXSTT;                    // I2C start condition
	while (UCB0CTL1 & UCTXSTT);             // Start condition sent?
	if (length == 1) UCB0CTL1 |= UCTXSTP;   //only 1 byte send imediatly stop condition
	while (isBusy);
}
void Stop_I2C()
{
	UCB0CTL1 |= UCTXSTP;                // I2C stop condition
}
//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
//-------------------------------------------------------------------------------
void I2C_RX_Interrupt()
{
  static uint8 count1=0;

  {                              					// Master Recieve?
	  RxBuffer[count1]=UCB0RXBUF;
	  count1++;
	  if (count1 == (length-1) ) Stop_I2C();  		// I2C stop condition then length-1
	  if (count1 == length)
	  {
		  isBusy=false;
		  count1 = 0;
	  }
  }

}
void I2C_TX_Interrupt()										// Master Transmit
{
	static uint8 count2=0;
	{
	  if (count2 < length)                  	// Check TX byte counter
		{
			UCB0TXBUF = TxBuffer[count2];        // Load TX buffer
			count2++;                            // Decrement TX byte counter
		}
		else
		{
		  //Stop_I2C();                // I2C stop condition
		  isBusy=false;
		  count2 = 0;
		}
//	  UCB0TXBUF = 0x01;
//	  if (count2==1)
//	  {
//	 	Stop_I2C();              			// I2C stop condition
//	 	count2=0;
//	  }
//	  count2++;
	}
}
//-------------------------------------------------------------------------------
// The USCI_B0 data ISR is used to move received data from the I2C slave
// to the MSP430 memory. It is structured such that it can be used to receive
//-------------------------------------------------------------------------------
