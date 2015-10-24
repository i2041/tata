/*
 * watchdog.h
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */

#ifndef WATCHDOG_H_
#define WATCHDOG_H_
#define stop 0
#define start 1
#include <msp430i2041.h>
#include "commontypes.h"
#include "programTata.h"
/*
15 		14 		13 		12 		11 		10 		9 		8
						WDTPW
7 			6 		5 		4 		3 		2 		1	0
WDTHOLD WDTNMIES WDTNMI WDTTMSEL WDTCNTCL WDTSSEL 	WDTIS

WDTPW		Watchdog timer password. Always reads as 69h. Must be written as 5Ah; writing any other value generates a PUC.
WDTHOLD		Watchdog timer hold. This bit stops the watchdog timer. Setting WDTHOLD = 1 when the WDT is not in use conserves power.
				0b = Watchdog timer is not stopped.
				1b = Watchdog timer is stopped.
WDTNMIES	Watchdog timer NMI edge select. This bit selects the interrupt edge for the NMI interrupt when WDTNMI = 1. Modifying this bit can trigger an NMI. Modify this bit
			when WDTIE = 0 to avoid triggering an accidental NMI.
				0b = NMI on rising edge
				1b = NMI on falling edge
WDTNMI		Watchdog timer NMI select. This bit selects the function for the RST/NMI pin.
				0b = Reset function
				1b = NMI function
WDTTMSEL	Watchdog timer mode select
				0b = Watchdog mode
				1b = Interval timer mode
WDTCNTCL	Watchdog timer counter clear. Setting WDTCNTCL = 1 clears the count value to 0000h. WDTCNTCL is automatically reset.
				0b = No action
				1b = Sets WDTCNT = 0000h
WDTSSEL		Watchdog timer clock source select
				0b = SMCLK
				1b = ACLK
WDTIS		Watchdog timer interval select. These bits select the watchdog timer interval to set the WDTIFG flag or generate a PUC.
				00b = Watchdog clock source /32768
				01b = Watchdog clock source /8192
				10b = Watchdog clock source /512
				11b = Watchdog clock source /64
*/
void watchdog_config(uint8 state);

#endif /* WATCHDOG_H_ */
