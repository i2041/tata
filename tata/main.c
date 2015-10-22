#include <msp430i2041.h>
#include "library/adc.h"
#include "library/io.h"
#include "library/max31855.h"
#include "library/watchdog.h"
#include "library/programTata.h"
#include "library/commontypes.h"
#include "library/spi.h"
/*
 * main.c
 */
void main(void) {
	watchdog_config(stop);
	go_to_sleep();
	//init();
	while (1)
	{}
}
void init()
{
	init_io();
	init_spi();
	ThermoCoupleTemperature();
	InternalTemperature();
	__enable_interrupt();
	init_adc();
	start_adc();
	while ((TCouple==0) || (TIntern==0) || (Voltage==0)  ){}	//white interrupt flag ADC to go next
																//asteptam pina ce nu optinem valori valide pentru a putea merge mai departe
	//verify if voltage <8V go to sleep again
	init_timer(start);
}
void go_to_sleep()
{
	P2DIR &= ~BIT1; // Set P2.1 to input direction
	P2IES |= BIT1;	//on falling
	P2IE |= BIT1; // enable P2.1 interrupt
	P2IFG &= ~BIT1;	// Clear Interrupt Flag

	//init_io();

	write_io(10,0);	write_io(22,0);	write_io(23,0);

	init_timer(stop);
	initProgramTata();
	_bis_SR_register(GIE+CPUOFF+OSCOFF+SCG1);
}
