#include <msp430.h>
#include "library/i2c.h"
#include "library/ads1115.h"
#include "library/uart.h"
#include "library/mlx90614.h"
#include "library/timer.h"
#include "library/printf.h"
int16 tmpValue;
uint32 tmpValue2;
float tmpValue88;





const float T0	=	310.1897600;
const uint16 V0	=	12631;

const float p1	=	24.0619490;
const float p2	=	4.0158622;
const float p3	=	0.2685392;
const float p4	=	-0.0097189;
const float q1	=	0.1699587;
const float q2	=	0.0114131;
const float q3	=	-0.0003928;

float tmpValue3;
float tmpValue4;
float tmpValue5;
uint8 tmpCount = 0;
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    configureFrequency();
	Init_I2C();
	Init_Uart();
	//ADS1115_Write_ConfigRegister(AIN0P_GND);
	//tmpValue = ADS1115_Read_ConfigRegister();

	while(1)
	{
/*
		ADS1115_Write_ConfigRegister(AIN0P_GND);
		while ( (ADS1115_Read_ConfigRegister() & BeginSingleConversion) == 0){}

		tmpValue = ADS1115_ConversionRegister();

		if (tmpValue > 0x7FFF) tmpValue = 0;


		tmpValue2 = tmpValue;
		tmpValue2 = ( (tmpValue2*125) / 16);
		tmpValue = tmpValue2;

//		TxBuffer_Uart[0]=(tmpValue/10000)+48;tmpValue=tmpValue%10000;
//		TxBuffer_Uart[1]=(tmpValue/1000)+48;tmpValue=tmpValue%1000;
//		TxBuffer_Uart[2]=(tmpValue/100)+48;tmpValue=tmpValue%100;
//		TxBuffer_Uart[3]=(tmpValue/10)+48;tmpValue=tmpValue%10;
//		TxBuffer_Uart[4]=(tmpValue)+48;
//		TxBuffer_Uart[5]=' ';
//		TxBuffer_Uart_length = 6;
//		Uart_send();


		tmpValue = tmpValue2 - V0;
		tmpValue3 = tmpValue;
		tmpValue3 /= 1000;
		tmpValue4 = ( tmpValue3 * p4)+p3;
		tmpValue4 = ( tmpValue4 * tmpValue3 )+ p2;
		tmpValue4 = ( tmpValue4 * tmpValue3 )+ p1;
		tmpValue4 = ( tmpValue4 * tmpValue3 );
		tmpValue5 = ( tmpValue3 * q3) + q2;
		tmpValue5 = ( tmpValue3 * tmpValue5) + q1;
		tmpValue5 = ( tmpValue3 * tmpValue5) + 1;
		tmpValue4 /= tmpValue5;
		tmpValue4 += T0;
		tmpValue4+=30;
		tmpValue = (uint16)tmpValue4;

		//__delay_cycles(1000000);

		TxBuffer_Uart[0]=(tmpValue/10000)+48;tmpValue=tmpValue%10000;
		TxBuffer_Uart[1]=(tmpValue/1000)+48;tmpValue=tmpValue%1000;
		TxBuffer_Uart[2]=(tmpValue/100)+48;tmpValue=tmpValue%100;
		TxBuffer_Uart[3]=(tmpValue/10)+48;tmpValue=tmpValue%10;
		TxBuffer_Uart[4]=(tmpValue)+48;
		TxBuffer_Uart[5]='|';
		Uart_send(6);	// 6 element, from 0 to 5

		__delay_cycles(1000000);

		ADS1115_Write_ConfigRegister(AIN2P_AIN3N);
		while ( (ADS1115_Read_ConfigRegister() & BeginSingleConversion) == 0){}
		tmpValue = ADS1115_ConversionRegister();

		if (tmpValue > 0x7FFF) tmpValue = 0;


		tmpValue2 = tmpValue;
		tmpValue2 = ( (tmpValue2*125) / 16);
		tmpValue = tmpValue2;

//		TxBuffer_Uart[0]=(tmpValue/10000)+48;tmpValue=tmpValue%10000;
//		TxBuffer_Uart[1]=(tmpValue/1000)+48;tmpValue=tmpValue%1000;
//		TxBuffer_Uart[2]=(tmpValue/100)+48;tmpValue=tmpValue%100;
//		TxBuffer_Uart[3]=(tmpValue/10)+48;tmpValue=tmpValue%10;
//		TxBuffer_Uart[4]=(tmpValue)+48;
//		TxBuffer_Uart[5]=' ';
//		TxBuffer_Uart_length = 6;
//		Uart_send();


		tmpValue = tmpValue2 - V0;
		tmpValue3 = tmpValue;
		tmpValue3 /= 1000;
		tmpValue4 = ( tmpValue3 * p4)+p3;
		tmpValue4 = ( tmpValue4 * tmpValue3 )+ p2;
		tmpValue4 = ( tmpValue4 * tmpValue3 )+ p1;
		tmpValue4 = ( tmpValue4 * tmpValue3 );
		tmpValue5 = ( tmpValue3 * q3) + q2;
		tmpValue5 = ( tmpValue3 * tmpValue5) + q1;
		tmpValue5 = ( tmpValue3 * tmpValue5) + 1;
		tmpValue4 /= tmpValue5;
		tmpValue4 += T0;
		tmpValue4+=25;
		tmpValue = (uint16)tmpValue4;

		__delay_cycles(1000000);

		TxBuffer_Uart[0]=(tmpValue/10000)+48;tmpValue=tmpValue%10000;
		TxBuffer_Uart[1]=(tmpValue/1000)+48;tmpValue=tmpValue%1000;
		TxBuffer_Uart[2]=(tmpValue/100)+48;tmpValue=tmpValue%100;
		TxBuffer_Uart[3]=(tmpValue/10)+48;tmpValue=tmpValue%10;
		TxBuffer_Uart[4]=(tmpValue)+48;
		TxBuffer_Uart[5]='|';
		TxBuffer_Uart[6]='|';
		Uart_send(7);	// 6 element, from 0 to 5

		__delay_cycles(1000000);
*/
		tmpValue88 = Mlx90614_read_Register(MLX90614_TOBJ1);
		print("MLX90614=%f",tmpValue88);
//		TxBuffer_Uart[0]=(tmpValue88/10000)+48;tmpValue88=tmpValue88%10000;
//		TxBuffer_Uart[1]=(tmpValue88/1000)+48;tmpValue88=tmpValue88%1000;
//		TxBuffer_Uart[2]=(tmpValue88/100)+48;tmpValue88=tmpValue88%100;
//		TxBuffer_Uart[3]=(tmpValue88/10)+48;tmpValue88=tmpValue88%10;
//		TxBuffer_Uart[4]=(tmpValue88)+48;
//		TxBuffer_Uart[5]='\n';
//		Uart_send(6);	// 6 element, from 0 to 5
		__delay_cycles(1000000);
	}
	return 0;
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
