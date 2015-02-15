/*
 * programTata.h
 *
 *  Created on: 11 февр. 2015 г.
 *      Author: Serghei
 */

#ifndef PROGRAMTATA_H_
#define PROGRAMTATA_H_

#define MAX_TEMPERATURE 150
#define DELTA_TIME 10
#define DELTA_TEMPERATURE 5

#include "commontypes.h"
extern bool activeMode;
extern bool comand_executed;
extern float TCouple;
extern uint16 GlobalTimer;
extern uint16 RunTime;
extern uint16 min_Temperature;
void algortimul();
void initProgramTata();
bool risingFlag;
uint8 mode;
extern uint16 risingTime;
#endif /* PROGRAMTATA_H_ */
