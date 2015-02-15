/*
 * programTata.c
 *
 *  Created on: 11 февр. 2015 г.
 *      Author: Serghei
 */
#include "programTata.h"
#include "commontypes.h"
#include <msp430i2041.h>
void algortimul()
{
	static uint8 tmpTimer=0;
	static float tmpTemperature=0;
	static uint8 countError=0;
	static uint8 retry = 0;//number of retry
	if (mode == 0)
	{
		write_io(10,1);write_io(22,0);write_io(23,0);
		tmpTimer = GlobalTimer;
		mode++;
		//retry = 0;
	}
	if (GlobalTimer <= RunTime)
	{
		if ((mode == 1) && ((GlobalTimer-tmpTimer) >= DELTA_TIME))
		{
			write_io(10,1);write_io(22,1);write_io(23,1);
			tmpTemperature = TCouple;
			mode++;
			//retry = 0;
			countError = 0;
		}
		if (mode == 2)
		{
			if ((TCouple - tmpTemperature) > DELTA_TEMPERATURE)
			{

				write_io(10,1);write_io(22,0);write_io(23,1);
				mode++;countError=0;
			}
			else if (countError > aprindereTimeError)
			{
				if (retry == NrOfRetryers)
				{
					retry=0;
					errorImplementaion(aprindere_error);
				}
				retry++;
				mode=0;
				tmpTimer = GlobalTimer;
			}
		countError++;
		}
		if ((mode == 3) )
		{
			if (TCouple > MAX_TEMPERATURE)
			{
				write_io(10,1);write_io(22,0);write_io(23,0);
				mode++;
				retry=0;
				countError=0;
			}
			else if ((TCouple < tmpTemperature) && (countError > aprindereTimeError))
			{
				if (retry == NrOfRetryers)
				{
					retry = 0;
					errorImplementaion(benzine_error);
				}
				retry++;
				mode=1;
			}
			countError++;
		}
		if ((mode == 4) && ((uint16)TCouple < min_Temperature))
		{
			mode = 1;
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
	if ((risingFlag == true) && ((GlobalTimer - risingTime) > activeButtonForReset))
	{
		comand_executed=true;
		WDTCTL = 0;//provoke reset
	}

	if ((false == risingFlag)&&((GlobalTimer-risingTime) > activeButon))
	{
		if (activeMode)
		{
			comand_executed=true;
			go_to_sleep();
		}
		else
		{
			comand_executed=true;
			activeMode = true;
			watchdog_config(stop);
			init();
		}
	}

		if(!(activeMode) && ((GlobalTimer-risingTime) <= activeButon) && (risingFlag == false))
		{
			comand_executed=true;
			go_to_sleep();
		}
		if((activeMode) && ((GlobalTimer-risingTime) <= activeButon) && (risingFlag == false))
		{
			comand_executed=true;
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
	case benzine_error: 	{go_to_sleep();break;}
	default:				{go_to_sleep();break;}
	}
}
