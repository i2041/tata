#include <msp430i2041.h>
#include "library/adc.h"
#include "library/io.h"
#include "library/max31855.h"
#include "library/watchdog.h"
#include "library/programTata.h"
#include "library/commontypes.h"

float TCouple=0;
float TIntern=0;
uint32 Voltage=0;
bool activeMode = false;
bool comand_executed = false;
/*
 * main.c
 */

//uint32 Voltage=0;

void main(void) {
	watchdog_config(stop);
	go_to_sleep();
	//init();
	while (1)
	{}
}

void task500ms()
{
    TCouple = ThermoCoupleTemperature();
    TIntern = InternalTemperature();
}
void task1s()
{
	GlobalTimer++;

	if (!comand_executed){verify_condition();}

	if (activeMode){algortimul();}
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
	//verify if voltage <8V go to sleep again
	init_timer(start);
}
void go_to_sleep()
{
	P2DIR &= ~BIT1; // Set P2.1 to input direction
	P2IES &= ~BIT1;	//on raising
	P2IE |= BIT1; // enable P2.1 interrupt
	P2IFG &= ~BIT1;	// Clear Interrupt Flag
	activeMode = false;
	init_io();
	write_io(10,0);
	write_io(22,0);
	write_io(23,0);
	init_timer(stop);
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
