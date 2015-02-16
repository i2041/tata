/*
 * timer.h
 *
 *  Created on: 11 февр. 2015 г.
 *      Author: Serghei
 */

#ifndef TIMER_H_
#define TIMER_H_
#define NR_ERRORS 5
#include "commontypes.h"

uint16 GlobalTimer;
bool activeMode;
bool comand_executed;
extern float TCouple;
/*	TAxCTL Register
15 	14 	13 	12 	11 	10 	|	9 	8
		Reserved 		|	TASSEL

7 	6 	|	5 		4 	|	3 		|	2  | 1 	 |  0
ID 		|		MC 		| Reserved 	|TACLR |TAIE | TAIFG
TASSEL 	=> Timer_A clock source select
			00b = TAxCLK
			01b = ACLK
			10b = SMCLK
			11b = Inverted TAxCLK
ID		=> Input divider. These bits select the divider for the input clock.
			00b = /1
			01b = /2
			10b = /4
			11b = /8
MC		=> Mode control. Setting MCx = 00h when Timer_A is not in use conserves power.
			00b = Stop mode: Timer is halted
			01b = Up mode: Timer counts up to TAxCCR0
			10b = Continuous mode: Timer counts up to 0FFFFh
			11b = Up/down mode: Timer counts up to TAxCCR0 then down to 0000h
TAIE	=> Timer_A interrupt enable. This bit enables the TAIFG interrupt request.
			0b = Interrupt disabled
			1b = Interrupt enabled
TAIFG	=> Timer_A interrupt flag
			0b = No interrupt pending
			1b = Interrupt pending
*/
void init_timer(uint8 mode);
void task500ms_impar();
void task1s();
void task500ms();

#endif /* TIMER_H_ */
