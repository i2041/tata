/*
 * watchdog.c
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */
#include "watchdog.h"

void watchdog_config(uint8 state)
{
	if (state == 0) {WDTCTL=WDTPW+WDTHOLD;}
	if (state == 1)	{WDTCTL = WDTPW + WDTCNTCL;IE1=WDTIFG;}
}
