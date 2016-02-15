/*
 * printf.c
 *
 *  Created on: 01 ���. 2016 �.
 *      Author: Serghei
 */
#include <stdarg.h>
#include <stdio.h>
#include "commontypes.h"
#include "uart.h"
#include "printf.h"
void print(uint8 *string, ... )
{
	 va_list vl;
	 uint8 i;	//parsing the string
	 uint8 countIntegerNumber;
	 uint16 powValue=1;
	 uint32 tmpValue;	//pentru a calcula partea intreaga
	 //  szTypes is the last argument specified; you must access
	 //  all others using the variable-argument macros.
	 va_start( vl, string );
	 // Step through the list.
	 for(i = 0; string[i] != '\0'; ++i )
	 {
		 union Printable_t
	 	 {
		 uint16     i;
		 float   f;
		 char    c;
		 char   *s;
	 	 } Printable;

	 	 if (string[i] == '%' && string[i+1] != '\0')
	 	 {
	 		 switch( string[i+1])
	 		 {
			 case 'd'://decimal
				Printable.i = va_arg( vl, uint16);
				for (countIntegerNumber=1;( (Printable.i/10) / powValue ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				//if (Printable.i < 0) {TxBuffer_Uart[uartBuffer_lenght] = '-';uartBuffer_lenght++;}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
						TxBuffer_Uart[TxBuffer_Uart_Tail] = (uint8)Printable.i+48;
					} //in case if only 1 digit
					else
					{
					TxBuffer_Uart[TxBuffer_Uart_Tail]=(Printable.i / powValue)+48;
					Printable.i %= powValue;
					powValue/=10;
					}
					Uart_RecalculateTail_Buffer();
					countIntegerNumber--;
				}
			 break;

			 case 'f':
				Printable.f = va_arg( vl, double );
				tmpValue = (uint16)Printable.f;

				for (countIntegerNumber=1;( tmpValue / (10*powValue) ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				//if (Printable.i < 0) {TxBuffer_Uart[uartBuffer_lenght] = '-';uartBuffer_lenght++;}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
						TxBuffer_Uart[TxBuffer_Uart_Tail] = (uint8)tmpValue+48;
					} //in case if only 1 digit
					else
					{
					TxBuffer_Uart[TxBuffer_Uart_Tail]=(tmpValue / powValue)+48;
					tmpValue %= powValue;
					powValue/=10;
					}
					Uart_RecalculateTail_Buffer();
					countIntegerNumber--;
				}
				tmpValue = (uint32)Printable.f * 100;
				Printable.f *= 100;

				tmpValue = (uint32)Printable.f%tmpValue;
				TxBuffer_Uart[TxBuffer_Uart_Tail]='.';
				Uart_RecalculateTail_Buffer();
				TxBuffer_Uart[TxBuffer_Uart_Tail]=( (tmpValue / 10)+48 );
				Uart_RecalculateTail_Buffer();
				TxBuffer_Uart[TxBuffer_Uart_Tail]=(tmpValue % 10)+48;
				Uart_RecalculateTail_Buffer();
				 //printf_s( "%f\n", Printable.f );
			 break;

			 case 'c':
				 Printable.c = va_arg( vl, char );
				 //printf_s( "%c\n", Printable.c );
			 break;

			 case 's':
				 Printable.s = va_arg( vl, char * );
				// printf_s( "%s\n", Printable.s );
			 break;
			 default:
			 {
				 TxBuffer_Uart[TxBuffer_Uart_Tail] = string[i];
				 Uart_RecalculateTail_Buffer();
			 	 TxBuffer_Uart[TxBuffer_Uart_Tail] = string[i+1];
			 	 Uart_RecalculateTail_Buffer();
			 }
			 break;
	 		 }
	 		 i++;
	 	 }
		 else
		 {
			TxBuffer_Uart[TxBuffer_Uart_Tail] = string[i];
			Uart_RecalculateTail_Buffer();
		 }
	 }
	 Uart_TX_Interrupt();
}
