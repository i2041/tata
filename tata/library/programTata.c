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
	static uint8 retry_mode3 = 0;//numbers retry for mode3

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
				mode++;
				countError=0;
				retry=0;
			}
			else if (countError > aprindereTimeError)
			{
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
		if ((mode == 3) )
		{
			if (TCouple > MAX_TEMPERATURE)
			{
				write_io(10,1);write_io(22,0);write_io(23,0);
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
	if ((fallingFlag == true) && ((GlobalTimer - fallingTime) > activeButtonForReset))
	{
		comand_executed=true;
		WDTCTL = 0;//provoke reset
	}

	if ((false == fallingFlag)&&((GlobalTimer - fallingTime) > activeButon))
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

		if(!(activeMode) && ((GlobalTimer-fallingTime) <= activeButon) && (fallingFlag == false))
		{
			comand_executed=true;
			go_to_sleep();
		}
		if((activeMode) && ((GlobalTimer-fallingTime) <= activeButon) && (fallingFlag == false))
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
	case benzine_error	: 	{go_to_sleep();break;}
	case couple_error	: 	{go_to_sleep();break;}
	case voltage_error	: 	{go_to_sleep();break;}
	default:				{go_to_sleep();break;}
	}
}
