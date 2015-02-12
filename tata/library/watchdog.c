/*
 * watchdog.c
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */
#include "watchdog.h"

void watchdog_config(uint8 state)
{
	if (state == stop) 	{WDTCTL=WDTPW+WDTHOLD;}
	if (state == start)	{WDTCTL = WDTPW + WDTCNTCL + 0x0003;IE1=WDTIFG;}
}
