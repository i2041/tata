/*
 * programTata.c
 *
 *  Created on: 11 февр. 2015 г.
 *      Author: Serghei
 */
#include "programTata.h"
#include "commontypes.h"
#include <msp430i2041.h>
#include "watchdog.h"
void algortimul()
{
	static uint8 tmpTimer=0;
	static uint8 tmpTemperature=0;
	static uint8 retryAprindere=0;
	if (GlobalTimer <= RunTime)
	{
		if (mode == 0) 														//aerisirea camerii
		{
			if (onlyOnce == true)
			{
			write_io(10,1);write_io(22,0);write_io(23,0);
			onlyOnce = false;
			tmpTimer = GlobalTimer;
			}
			if  ((GlobalTimer-tmpTimer) >= DELTA_TIME)						//aerisirea camerii < DELTA_TIME
			{
				mode++;
				onlyOnce = true;
			}
		}
		if (mode == 1)														//aprinderea tuturor elementeleor
		{
			if (onlyOnce == true)
			{
				write_io(10,1);write_io(22,1);write_io(23,1);
				tmpTemperature = (uint8)TCouple;
				tmpTimer = GlobalTimer;
				//if (retryAprindere == 0)retryAprindere = 0;
				onlyOnce = false;
			}
			if ( ((uint8)TCouple >= tmpTemperature) && ((uint8)TCouple - tmpTemperature) > DELTA_TEMPERATURE )				//aprinderea tuturor elementelor < ((TCouple - tmpTemperature) > DELTA_TEMPERATURE)
			{
				mode++;
				//tmpTimer = GlobalTimer;
				onlyOnce = true;
				retryAprindere = 0;
			}
			else if ((GlobalTimer - tmpTimer) > aprindereTimeError)			//daca DELTA_TEMPERATURE nu a fost timp de aprindereTimeError setam eroarea
			{
				retryAprindere++;
				tmpTimer = GlobalTimer;
				if (retryAprindere == 2)										// a 2 incercare daca avem eroare atunci se seteaza DTC
				{
					retryAprindere = 0;
					errorImplementaion(aprindere_error);
				}
				else
				{
					//mode = 0;												// se poate de trecut prin modul 0 de la inceput
				}
			}
		}
		if ((mode == 2) )
		{
			if (onlyOnce == true)
			{
				write_io(10,1);write_io(22,0);write_io(23,1);
				onlyOnce = false;
			}
			if (TCouple > max_Temperature)
			{
				mode++;
				tmpTemperature = (uint8)TCouple;
				onlyOnce = true;
			}
			else
			{
				if ((uint8)TCouple > tmpTemperature ) tmpTemperature = TCouple;
				else if ( (tmpTemperature - (uint8)TCouple) > deltaTemperatureMode2 )
				{
					errorImplementaion(benzine_error);
				}
			}
		}
		if ((mode == 3) )
		{
			if (onlyOnce == true)
			{
				write_io(10,1);write_io(22,0);write_io(23,0);
				onlyOnce = false;
			}
			if ((uint16)TCouple < min_Temperature)
			{
				mode = 1;
				onlyOnce = true;
			}
		}
		if ( mode == 5 )												//racirea camerii de ardere
		{
			if (onlyOnce == true)
			{
				write_io(10,1);write_io(22,0);write_io(23,0);
				tmpTimer = GlobalTimer;
				onlyOnce = false;
			}
			if 	(
					( ((uint16)TCouple < TemperaturaMinRacire) && ((GlobalTimer - tmpTimer) > 30) 			)		// in cazul cind a fost aprinsa si sa dorit brusc oprirea lui
					||
					( ((uint16)TCouple > TemperaturaMinRacire) && ((GlobalTimer - tmpTimer) > TimpulRacire) )		// in cazul cind a avut o temperatura mare si sa e nevoie de racire
				)
			{
				go_to_sleep();
			}
		}
	}
	else
	{
		static bool onlyOnce2 = true;
		if (onlyOnce2 == true)
		{
			write_io(10,1);write_io(22,0);write_io(23,0);
			tmpTimer = GlobalTimer;
			onlyOnce2 = false;
		}
		if 	(
				( ((uint16)TCouple < TemperaturaMinRacire) && ((GlobalTimer - tmpTimer) > 30) 			)		// in cazul cind a fost aprinsa si sa dorit brusc oprirea lui
				||
				( ((uint16)TCouple > TemperaturaMinRacire) && ((GlobalTimer - tmpTimer) > TimpulRacire) )		// in cazul cind a avut o temperatura mare si sa e nevoie de racire
			)
		{
			go_to_sleep();
		}
	}
}
void verify_condition()
{
	if ((fallingFlag == true ) && ((GlobalTimer - fallingTime) > activeButtonForReset))				//tinem apasat mult timp
	{
		comand_executed = true;
		WDTCTL = 0;//provoke reset
	}
	if ((fallingFlag == false) && ((GlobalTimer - fallingTime) > activeButon))
	{
		if ( activeMode )				// In normal Mode preparing for cooling room
		{
			if ( mode == 5 )
			{
				//comand_executed = true;	// nu cred ca e nevoie caci se duce in sleep mode
				go_to_sleep();
			}
			else						// Now I am in cooling mode, resulting I need to go in sleep mode imediatly.
			{
				comand_executed = true;
				onlyOnce = true;
				mode = 5;
			}
		}
		else							//go in run mode
		{
			comand_executed = true;
			activeMode = true;
			watchdog_config(stop);
			init();
		}
	}
	if(!(activeMode) && ((GlobalTimer-fallingTime) <= activeButon) && (fallingFlag == false))
	{
		comand_executed = true;
		go_to_sleep();
	}
	if((activeMode) && ((GlobalTimer-fallingTime) <= activeButon) && (fallingFlag == false))
	{
		comand_executed = true;
	}
}
void initProgramTata()
{
mode = 0;
onlyOnce = true;
}
void errorImplementaion(typeError Error)
{
	switch (Error)
	{
	case aprindere_error: 	{go_to_sleep();break;}
	case benzine_error	: 	{go_to_sleep();break;}			// SE POATE DE SETAT SI PE MODUL 5 CARE MAI APOI SA SE DUCA LA SLEEP
	case couple_error	: 	{go_to_sleep();break;}
	case voltage_error	: 	{go_to_sleep();break;}
	default:				{go_to_sleep();break;}
	}
}
