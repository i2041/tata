/*
 * programTata.h
 *
 *  Created on: 11 февр. 2015 г.
 *      Author: Serghei
 */

#ifndef PROGRAMTATA_H_
#define PROGRAMTATA_H_
#include "adc.h"
#include "max31855.h"
#include "timer.h"
#include "io.h"
#include "commontypes.h"

#define DELTA_TIME 10
#define DELTA_TEMPERATURE 5
#define aprindereTimeError 15	// 15seconds daca nu sa aprins
#define activeButon	3			// 3 second
#define preStart	10			// 10 second to clear fire room of gasoline
#define NrOfRetryers 1			//numbers of retryers 1 = 2 times
#define activeButtonForReset 7
#define RunTime	1800			//30 min
#define TemperaturaMinRacire 30	//30C temperatura de racire minima la care se poate duce la sleep
#define TimpulRacire 300		//5min timpul de racire inainte de a se duce la sleep
#define deltaTemperatureMode2 20// 20 grade, in caz ca sunt 20 de grade mai jos in cazul cind e aprins inseamna ca sa terminat benzina
bool onlyOnce;
void algortimul();
void initProgramTata();
void errorImplementaion(typeError Error);
void verify_condition();
bool fallingFlag;
uint8 mode;
extern void init();
extern void go_to_sleep();
#endif /* PROGRAMTATA_H_ */
