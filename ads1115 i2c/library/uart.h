/*
 * uart.h
 *
 *  Created on: 15.12.2015
 *      Author: uidg6243
 */

#ifndef UART_H_
#define UART_H_
#include <msp430.h>
#include "commontypes.h"

#define RXD BIT1
#define TXD BIT2
#define SizeOfBuffer 50
uint8 TxBuffer_Uart[SizeOfBuffer];
uint8 TxBuffer_Uart_length;
uint8 RxBuffer_Uart[SizeOfBuffer];
void Init_Uart();
void Uart_RX_Interrupt();
void Uart_TX_Interrupt();
void Uart_send(uint8 size);
#endif /* UART_H_ */
