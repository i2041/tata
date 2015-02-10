#include <msp430i2041.h>
//#include "library/adc.c"
//#include "library/io.c"
#include "library/max31855.h"
#include "library/watchdog.h"
/*
 * main.c
 */
float TCouple=0;
float TIntern=0;
uint32 Voltage=0;

void main(void) {
	watchdog_config(stop);

	go_to_sleep();

	watchdog_config(stop);
	init();
	while (1)
	{}
}

void task100ms()
{
	//WDTCTL = WDTPW + WDTCNTCL; //keep clearing the counter to prevent the reset
    TCouple = ThermoCoupleTemperature();
    TIntern = InternalTemperature();
}
void init()
{
	init_io();
	init_spi();
	TCouple = ThermoCoupleTemperature();
	TIntern = InternalTemperature();
	__enable_interrupt();
	init_adc();
	start_adc(2);
	while ((TCouple==0) || (TIntern==0) || (Voltage==0)){}	//white interrupt flag ADC to go next
	init_timer(1,16,0xC350);								//task 100ms 0x1388  1s -0xC350
}
void go_to_sleep()
{
	init_io();
	write_io(10,0);
	write_io(22,0);
	write_io(23,0);
	_bis_SR_register(GIE+CPUOFF+OSCOFF+SCG1);
}
void wakeUp()
{

	//_bic_SR_register(GIE+CPUOFF+OSCOFF+SCG1);
}
//#pragma vector=WDT_VECTOR
//__interrupt void wdttimer(void)
//{
//	IFG1&=~WDTIFG;
//}
//void config_wakeup()
//{
//
//}
