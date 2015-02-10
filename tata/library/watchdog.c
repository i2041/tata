/*
 * watchdog.c
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */
#include "watchdog.h"
#include <msp430i2041.h>
void watchdog_config()
{
	WDTCTL=WDTPW+WDTHOLD;
	//WDTCTL = WDTPW + WDTCNTCL;
	//IE1=WDTIFG;
}
