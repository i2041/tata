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
	init_adc();
    init_io();
    init_timer(1,16,0xC350);//tasc 100ms 0x1388  1s -0xC350
    //start_adc();
    init_spi();


    __enable_interrupt();
    while(1)
    {

    }


}

void task100ms()
{
    TCouple = ThermoCoupleTemperature();
    TIntern = InternalTemperature(1);
}
