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
	static float tmpTemperature=0;
	static uint8 countError=0;
	static uint8 retry = 0;//number of retry
	static uint8 retry_mode3 = 0;//numbers retry for mode3

	if (mode == 0)														//aerisirea camerii de ardere
	{
		write_io(10,1);write_io(22,0);write_io(23,0);					//aprindem ventilatorul
		tmpTimer = GlobalTimer;
		mode++;
		//retry = 0;
	}
	if (GlobalTimer <= RunTime)
	{
		if ((mode == 1) && ((GlobalTimer-tmpTimer) >= DELTA_TIME))		//aprinderea tuturor elementelor pentru a se aprinde cuptorul
		{
			write_io(10,1);write_io(22,1);write_io(23,1);				//se aprind toate numai dupa ce sa aerisit camera
			tmpTemperature = TCouple;
			mode++;
			//retry = 0;
			countError = 0;
		}
		if (mode == 2)													//aprindem numai benzina si turbina pentru a mentine focul aprins
		{
			if ((TCouple - tmpTemperature) > DELTA_TEMPERATURE)
			{

				write_io(10,1);write_io(22,0);write_io(23,1);			//se lasa aprins toate elememtele si se stinge bugia numai si
				mode++;													//	numai dupa ce sa ridicat temperatura cu 5 grade ceia ce inseamna ca cuptorul sa aprins
				countError=0;
				retry=0;
			}
			else if (countError > aprindereTimeError)					//daca cuptorul nu sa aprins timp de "aprindereTimeError" intra in eroare
			{															// se incearca de NrOfRetryers ori pina se valideaza eroarea.
				if (retry == NrOfRetryers)
				{
					retry=0;
					errorImplementaion(aprindere_error);
				}
				else
				{
				retry++;
				mode=0;
				//tmpTimer = GlobalTimer;
				}
			}
		countError++;
		}
		if ((mode == 3) )												//aprins sta numai ventilatorul
		{
			if (TCouple > max_Temperature)
			{
				write_io(10,1);write_io(22,0);write_io(23,0);			// daca o ajuns la temperatura MAX_TEMPERATURE se lasa numai turbina pentru a se raci
				mode++;
				retry_mode3=0;
				countError=0;
			}
			else if ((TCouple <= (tmpTemperature+1)) && (countError > aprindereTimeError))
			{
				if (retry_mode3 == NrOfRetryers)
				{
					retry_mode3 = 0;
					errorImplementaion(benzine_error);
				}
				else
				{
					retry_mode3++;
					mode=1;
				}
			}
			countError++;
		}
		if ((mode == 4) && ((uint16)TCouple < min_Temperature))			//se reaprind iarasi toate elemente cind ajunge la temperatura "min_Temperature"
		{
			mode = 2;
			tmpTemperature = TCouple;
			write_io(10,1);write_io(22,1);write_io(23,1);				//cind sa ajuns la "min_Temperature" aprindem toate elementele
		}
	}
	else
	{
		write_io(10,1);write_io(22,0);write_io(23,0);
		if ((uint16)TCouple < min_Temperature){go_to_sleep();}
	}
//	static uint8 a=0;
//		if (a % 2 == 0)
//		{
//			write_io(10,1);
//			write_io(22,1);
//			write_io(23,1);
//		}
//		else
//		{
//			write_io(10,0);
//			write_io(22,0);
//			write_io(23,0);
//		}
//		a++;
}
void verify_condition()
{
	if ((fallingFlag == true ) && ((GlobalTimer - fallingTime) > activeButtonForReset))
	{
		comand_executed=true;
		WDTCTL = 0;//provoke reset
	}

	if ((fallingFlag == false) && ((GlobalTimer - fallingTime) > activeButon))
	{
		if (activeMode)				// go in sleep mode
		{
			comand_executed=true;
			go_to_sleep();
		}
		else						//go in run mode
		{
			comand_executed=true;
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
}
void errorImplementaion(typeError Error)
{
	switch (Error)
	{
	case aprindere_error: 	{go_to_sleep();break;}
	case benzine_error	: 	{go_to_sleep();break;}
	case couple_error	: 	{go_to_sleep();break;}
	case voltage_error	: 	{go_to_sleep();break;}
	default:				{go_to_sleep();break;}
	}
}
