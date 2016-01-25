#include <msp430.h>
#include "library/i2c.h"
#include "library/ads1115.h"
#include "library/uart.h"
#include "library/mlx90614.h"
#include "library/timer.h"
#include "library/printf.h"
#include "library/TLC5947.h"
#include "library/CiocaneLipit.h"
#include "library/220V.h"
#include "library/interrupts.h"
#include "library/24V.h"
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
bool buttonPressed=false;


/*
 * main.c
 */
int main(void) {
	tmp1 = 111;
	tmp2 = 111;
	tmp3 = 111;

	watchDogConfigure();
	configureFrequency();
	tlc5947_init();
	tlc5947_calculate_digits(0,111);
	tlc5947_calculate_digits(1,211);
	tlc5947_calculate_digits(2,220);
	tlc5947_update_displays();
	Init_I2C();
	Init_Uart();
	V220(init);
	ciocaneLipit(init);
	ciocaneLipit(start);
	V24(init);
	//ADS1115_Write_ConfigRegister(AIN0P_GND);
	//tmpValue = ADS1115_Read_ConfigRegister();
	_EINT();
	while(1)
	{
//		P1IFG 	&= ~ BIT3;	//Button3 */
//		P2IFG &= ~ BIT2;	//button2
//		P2IFG &= ~ BIT5;	//button1
//		P2IE |= BIT2;
//		P2IE |= BIT5;
//		P1IE |= BIT3;

		ADS1115_Write_ConfigRegister(AIN0P_AIN1N);
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

		tlc5947_calculate_digits(0,tmpValue);
		tlc5947_calculate_digits(1,tmp2);
		tlc5947_calculate_digits(2,tmp3);
		tlc5947_update_displays();
		//__delay_cycles(1000000);

		TxBuffer_Uart[0]=(tmpValue/10000)+48;tmpValue=tmpValue%10000;
		TxBuffer_Uart[1]=(tmpValue/1000)+48;tmpValue=tmpValue%1000;
		TxBuffer_Uart[2]=(tmpValue/100)+48;tmpValue=tmpValue%100;
		TxBuffer_Uart[3]=(tmpValue/10)+48;tmpValue=tmpValue%10;
		TxBuffer_Uart[4]=(tmpValue)+48;
		TxBuffer_Uart[5]='\n';
		Uart_send(6);	// 6 element, from 0 to 5

		__delay_cycles(1000000);


		//tmpValue88 = Mlx90614_read_Register(MLX90614_TOBJ1);
		//print("MLX90614=%f",10.2);
//		TxBuffer_Uart[0]=(tmpValue88/10000)+48;tmpValue88=tmpValue88%10000;
//		TxBuffer_Uart[1]=(tmpValue88/1000)+48;tmpValue88=tmpValue88%1000;
//		TxBuffer_Uart[2]=(tmpValue88/100)+48;tmpValue88=tmpValue88%100;
//		TxBuffer_Uart[3]=(tmpValue88/10)+48;tmpValue88=tmpValue88%10;
//		TxBuffer_Uart[4]=(tmpValue88)+48;
//		TxBuffer_Uart[5]='\n';
//		Uart_send(6);	// 6 element, from 0 to 5
		__delay_cycles(10000000);
	}
	return 0;
}

