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
	static uint8 a=0;
		if (a % 2 == 0)
		{
			write_io(10,1);
			write_io(22,1);
			write_io(23,1);
		}
		else
		{
			write_io(10,0);
			write_io(22,0);
			write_io(23,0);
		}
		a++;
}
void verify_condition()
{
	if ((risingFlag == true) && ((GlobalTimer - risingTime) > 10))
	{
		WDTCTL = 0;//provoke reset
		comand_executed=true;
	}

	if ((risingFlag == false) && ((GlobalTimer-risingTime) > 3))
		{
			if (activeMode) {go_to_sleep();}
			else
			{
				activeMode = true;
				watchdog_config(stop);
				init();
			}
			comand_executed=true;
		}

		if(!(activeMode) && ((GlobalTimer-risingTime) <= 3) && (risingFlag == false))
		{
			comand_executed=true;
			go_to_sleep();
		}
		if((activeMode) && ((GlobalTimer-risingTime) <= 3) && (risingFlag == false))
		{
			comand_executed=true;
		}
}
