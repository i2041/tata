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
#define aprindereTimeError 15	// 15seconds if count error more execute error
#define activeButon	3			// 3 second
#define preStart	10			// 10 second to clear fire room of gasoline
#define NrOfRetryers 1			//numbers of retryers 0,1; 2-times
#define activeButtonForReset 7
#include "commontypes.h"
extern bool activeMode;
extern bool comand_executed;
extern float TCouple;
extern uint16 GlobalTimer;
extern uint16 RunTime;
extern uint16 min_Temperature;
void algortimul();
void initProgramTata();
bool fallingFlag;
uint8 mode;
extern uint16 fallingTime;
#endif /* PROGRAMTATA_H_ */
