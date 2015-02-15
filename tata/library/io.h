/*
 * io.h
 *
 *  Created on: 11 февр. 2015 г.
 *      Author: Serghei
 */

#ifndef IO_H_
#define IO_H_

#include "timer.h"
#include "commontypes.h"

extern bool activeMode;
extern bool comand_executed;
uint16 risingTime;
extern uint16 GlobalTimer;
extern bool risingFlag;

void init_io();
void write_io(int pin,int state);

#endif /* IO_H_ */
