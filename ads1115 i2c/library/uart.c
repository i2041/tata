/*
 * uart.c
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */
#include "uart.h"
/*
Clock			Baudrate		UCBRx		UCBRSx		UCBRFx		UCOS16
1,048,576 Hz	9600			109			2			0			0
8MHz			115200			69			4			0			1
16MHz			115200			8			0			11			1
*/
void Init_Uart()
{
	P1SEL |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
	P1SEL2 |= RXD + TXD ; // P1.1 = RXD, P1.2=TXD
	UCA0CTL1 |= UCSWRST;                      			// Enable SW reset
	UCA0CTL1 |= UCSSEL_2; // SMCLK
//	UCA0BR0 = 160; // 1MHz 115200
//	UCA0BR1 = 1; // 1MHz 115200
	UCA0BR0 = 34; // 4MHz 115200
	UCA0BR1 = 0;
	UCA0MCTL = UCBRS_6;
	UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**
	IFG2 &= ~UCA0TXIFG;
	IFG2 &= ~UCA0RXIFG;
	UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
	UC0IE |= UCA0TXIE; // Enable USCI_A0 RX interrupt
}
void Uart_send(uint8 size)
{
	TxBuffer_Uart_length = size;
	UCA0TXBUF = TxBuffer_Uart[0]; //send first element to provoke interrupt
}
void Uart_command(uint8 NrOfElements)
{

}
void Uart_RX_Interrupt(void)
{
	static uint8 count=0;
	uint8 i;

	if ( count == SizeOfBuffer )
	{
		for (i=0; i < SizeOfBuffer;i++)
		{
			RxBuffer_Uart[i] = RxBuffer_Uart[i++];
		}
		RxBuffer_Uart[count] = UCA0RXBUF;
		if (RxBuffer_Uart[count] == 10)	//new line
		{
			Uart_command(count);
			count=0;
		}
	}
	else
		{
		RxBuffer_Uart[count] = UCA0RXBUF;
		if (RxBuffer_Uart[count] == 10)	//new line
		{
			Uart_command(count);
			count=0;
		}
		count++;
		}
}
void Uart_TX_Interrupt(void)
{
	static uint8 count = 1;
	if ( count >= TxBuffer_Uart_length) // TX over?
    {
		count=1;
    }
	else
	{
		UCA0TXBUF = TxBuffer_Uart[count];	//send next byte
		count++;
	}
}
