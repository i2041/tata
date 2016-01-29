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
#include "library/task.h"



float tmpValue88;




uint16 tmpValue22222;

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
	Init_I2C();
	Init_Uart();
	V220(init);
	ciocaneLipit(init);
	//ciocaneLipit(start);
	V24(init);
	//ADS1115_Write_ConfigRegister(AIN0P_GND);
	//tmpValue = ADS1115_Read_ConfigRegister();
	_EINT();
	while(1)
	{
		tmpValue22222 = readAdc(CIOCAN_ADC2);
		print("ciocan1=%d",tmpValue22222);
		//tmpValue88 = Mlx90614_read_Register(MLX90614_TOBJ1);
		//print("MLX90614=%f",10.2);
		__delay_cycles(10000000);
	}
	return 0;
}

