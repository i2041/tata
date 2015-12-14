#include <msp430.h>
#include "library/i2c.h"
#include "library/ads1115.h"
uint16 tmpValue;
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    DCOCTL = CALDCO_16MHZ;     				// Set DCO step and modulation
	BCSCTL1 = CALBC1_16MHZ;     			// Set range
	BCSCTL2 = DIVS_1;
	Init_I2C();
	ADS1115_Write_ConfigRegister();
	tmpValue = ADS1115_Read_ConfigRegister();
	while(1)
	{
		tmpValue = ADS1115_ConversionRegister();
		//__delay_cycles(1000000);
	}
	return 0;
}
