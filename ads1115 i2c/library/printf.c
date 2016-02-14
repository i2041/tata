/*
 * printf.c
 *
 *  Created on: 01 џэт. 2016 у.
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
	 uint8 uartBuffer_lenght = 0;
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
		 int     i;
		 float   f;
		 char    c;
		 char   *s;
	 	 } Printable;

	 	 if (string[i] == '%' && string[i+1] != '\0')
	 	 {
	 		 switch( string[i+1])
	 		 {
			 case 'd'://decimal
				Printable.i = va_arg( vl, int );
				for (countIntegerNumber=1;( (Printable.i/10) / powValue ) != 0; countIntegerNumber++)
				{
					powValue*=10;
				}
				//if (Printable.i < 0) {TxBuffer_Uart[uartBuffer_lenght] = '-';uartBuffer_lenght++;}
				while(countIntegerNumber)
				{
					if (countIntegerNumber == 1)
					{
						TxBuffer_Uart[uartBuffer_lenght] = (uint8)Printable.i+48;
					} //in case if only 1 digit
					else
					{
					TxBuffer_Uart[uartBuffer_lenght]=(Printable.i / powValue)+48;
					Printable.i %= powValue;
					powValue/=10;
					}
					uartBuffer_lenght++;
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
						TxBuffer_Uart[uartBuffer_lenght] = (uint8)tmpValue+48;
					} //in case if only 1 digit
					else
					{
					TxBuffer_Uart[uartBuffer_lenght]=(tmpValue / powValue)+48;
					tmpValue %= powValue;
					powValue/=10;
					}
					uartBuffer_lenght++;
					countIntegerNumber--;
				}
				//tmpValue = (uint32)Printable.f;
				tmpValue = (uint32)Printable.f * 100;
				Printable.f *= 100;

				tmpValue = (uint32)Printable.f%tmpValue;
				TxBuffer_Uart[uartBuffer_lenght]='.';
				uartBuffer_lenght++;
				TxBuffer_Uart[uartBuffer_lenght]=( (tmpValue / 10)+48 );
				uartBuffer_lenght++;
				TxBuffer_Uart[uartBuffer_lenght]=(tmpValue % 10)+48;
				uartBuffer_lenght++;

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
				 TxBuffer_Uart[uartBuffer_lenght] = string[i];
			 	 uartBuffer_lenght++;
			 	 TxBuffer_Uart[uartBuffer_lenght] = string[i+1];
				 uartBuffer_lenght++;
			 }
			 break;
	 		 }
	 		 i++;
	 	 }
		 else
		 {
			TxBuffer_Uart[uartBuffer_lenght] = string[i];
			uartBuffer_lenght++;
		 }
	 }
	 TxBuffer_Uart[uartBuffer_lenght] = '\n';
	 uartBuffer_lenght++;
	 Uart_send(uartBuffer_lenght);
}
void ShowVar(uint8 szTypes[], ... ) {
   va_list vl;
   uint8 i;
   uint8 uartBuffer_lenght = 0;
   //  szTypes is the last argument specified; you must access
   //  all others using the variable-argument macros.
   va_start( vl, szTypes );

   // Step through the list.
   for( i = 0; szTypes[i] != '\0'; ++i ) {
      union Printable_t
	  {
         int     i;
         float   f;
         char    c;
         char   *s;
      } Printable;

      switch( szTypes[i] ) {   // Type to expect.
         case 'i':
            Printable.i = va_arg( vl, int );
            TxBuffer_Uart[uartBuffer_lenght] = Printable.i+48;
         break;

         case 'f':
             Printable.f = va_arg( vl, double );
         break;

         case 'c':
             Printable.c = va_arg( vl, char );
         break;

         case 's':
             Printable.s = va_arg( vl, char * );
         break;

         default:TxBuffer_Uart[uartBuffer_lenght] = szTypes[i];
         break;
      }
      uartBuffer_lenght++;
   }
   va_end( vl );
}
//uint32 _power( uint8 val, uint8 _pow )
//{
//	uint32 tmpValue;
//	uint8 i=2;
//	if ( _pow == 0 ) tmpValue = 1;
//	else if (_pow == 1) tmpValue = val;
//	else
//	{
//		for (2;i<=_pow;i++)
//		{
//		tmpValue*=tmpValue;
//		}
//}
//}

