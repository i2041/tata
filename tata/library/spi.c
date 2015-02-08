/*
 * spi.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include <msp430i2041.h>
#include "commontypes.h"

void init_spi()
{
UCA0CTLW0 |= UCSWRST;

P2OUT  |=  BIT1;	//Chip Select
P2DIR  |=  BIT1;	//Chip Select
P1SEL0 |= BIT1 + BIT2;
//UCA0CTLW0 |= UCCKPL; 	//The inactive state is high
UCA0CTLW0 |= UCMST;		//Master mode select
UCA0CTLW0 |= UCSYNC;	//syncron mode
UCA0CTLW0 |= UCMSB;		//MSB firs
//UCA0CTLW0 |= UCMODE1;	//10b = 4-pin SPI with UCxSTE active low: Slave enabled when UCxSTE = 0
UCA0CTLW0 |= UCSSEL1;	//10b = SMCLK

UCA0CTLW0 &= ~UCSWRST;
}

uint8 spi_read(uint8 NrOfBytes)
{

if (NrOfBytes == 0)
	{
		P2OUT  |=  (BIT1); //  Unselect    Device
		P2OUT  &=  (~BIT1);    //  Select  Device
	}

while  (!(UCA0IFG & UCTXIFG));    //  USCI_A0 TX  buffer  ready?
UCA0TXBUF  =   0xA1;   //  Send    0xAA    over    SPI to  Slave
while  (!(UCA0IFG & UCRXIFG));    //  USCI_A0 RX  Received?

if (NrOfBytes == 3)
	{
		P2OUT  |=  (BIT1); //  Unselect    Device
	}
return UCA0RXBUF;  //  Store   received    data
}

