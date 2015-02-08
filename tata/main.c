#include <msp430i2041.h>
//#include "library/adc.c"
//#include "library/io.c"
#include "library/max31855.h"
/*
 * main.c
 */
float TCouple;
float TIntern;

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	init();
	while(1)
    {

    }
}

void task100ms()
{
    TCouple = ThermoCoupleTemperature();
    TIntern = InternalTemperature();
}
void init()
{
	init_adc();
	init_io();
	init_timer(1,16,0xC350);			//tasc 100ms 0x1388  1s -0xC350
	init_spi();
	 __enable_interrupt();
	while (!(SD24CCTL0 & SD24IFG)){}	//white interrupt flag ADC to go next
}
void go_to_sleep()
{

	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	TA0CTL = MC_0;  			// Timer Halted
}
void wakeUp()
{

}
