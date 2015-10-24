/*
 * watchdog.c
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */
#include "watchdog.h"
/*
WDTIS 		----Watchdog timer interval select
	00b = Watchdog clock source /32768 from
	01b = Watchdog clock source /8192
	10b = Watchdog clock source /512		from16Mhz = 2s
	11b = Watchdog clock source /64			from16Mhz = 262mS
WDTSSEL 	----Watchdog timer clock source select
	0b = SMCLK
	1b = ACLK
WDTTMSEL 	---- Watchdog timer mode select
	0b = Watchdog mode
	1b = Interval timer mode
WDTIE		----Watchdog timer interrupt enable.
	0b = Interrupt not enabled
	1b = Interrupt enabled

*/
void watchdog_config(uint8 state)
{
	if (state == stop) 	{WDTCTL=WDTPW+WDTHOLD;}
	if (state == start)
	{
	//WDTCTL &= ~WDTSSEL;	//SMCLK
	WDTCTL = WDTPW + WDTTMSEL + WDTIS0 + WDTIS1 + WDTCNTCL;//Watchdog timer password + Interval timer mode + div 64 + Watchdog timer counter clear
	IE1 = WDTIE; 			//Watchdog timer interrupt enable
	//WDTCTL = WDT_MDLY_0_5;              // WDT as interval timer (period 0,5 ms)
	//IE1 |= WDTIE;                       // Enable WDT interrupt
	}

}
// Watchdog Interval Timer interrupt service
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	static uint8 WDreset = 0;
	if (fallingFlag) WDreset++;
	else WDreset = 0;
	if (WDreset == 30)
	{
		WDreset = 0;
		WDTCTL = 0;//provoke reset
	}
}
