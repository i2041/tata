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



/*
 * main.c
 */
int main(void) {

	uint8 loop = 1;
	watchDogConfigure();
	configureFrequency();
	tlc5947_init();
	Init_I2C();
	Init_Uart();
	V220(init);
	ciocaneLipit(init);
	ciocaneLipit(start);
	V24(init);
	//ADS1115_Write_ConfigRegister(AIN0P_GND);
	//tmpValue = ADS1115_Read_ConfigRegister();
	__enable_interrupt();
	P3DIR |= BIT6;
	while(loop)
	{
		if (Flag33msOcured)
		{
			task33ms();

		}
		//tmpValue88 = Mlx90614_read_Register(MLX90614_TOBJ1);
		//print("MLX90614=%f",10.2);
	}
	return 0;
}

