/*
 * max31855.c
 *
 *  Created on: 04 февр. 2015 г.
 *      Author: Serghei
 */
#include "spi.h"

#include "max31855.h"

float ThermoCoupleTemperature()
{
	float retVal = 3000;
	uint16 Tcouple_temperature;
	Tcouple_temperature = spi_read(0);//read from max 31..24 bites
	Tcouple_temperature = Tcouple_temperature<<8;
	Tcouple_temperature += spi_read(1);//read from max 23..16 bites
	if ((Tcouple_temperature & 0x1) != 1)	//Verify if present fault BIT (verify 16bit )
	{
		  if (Tcouple_temperature & 0x8000) // check sign bit!
		  {
			  int16 tmpValue = Tcouple_temperature;
			  tmpValue >>= 4; // ignore bottom 4 bits - they're just thermocouple data
			  retVal  = (float)tmpValue;
		  }
		  else
		  {
			  uint16 tmpValue = Tcouple_temperature;
			  tmpValue >>=4;
			  retVal = tmpValue;
		  }
		  //add 1/((x*2)^1+(x*2)^2); 19 and 18 bit.
		  if(Tcouple_temperature& 0x8)  retVal+=0.5;
		  if(Tcouple_temperature& 0x4)  retVal+=0.25;
	}
	else if(VERIFY_FAULT_COUPLE)
	{
		Tcouple_temperature = spi_read(2);//read from max 15..8 bites
		Tcouple_temperature = Tcouple_temperature<<8;
		Tcouple_temperature += spi_read(3);//read from max 7..0 bites
		if (Tcouple_temperature & 0x4) retVal = SCV_ERROR;
		else if (Tcouple_temperature & 0x2) retVal = SCG_ERROR;
		else retVal = OC_ERROR;
	}
	return retVal;
}
float InternalTemperature(uint8 readCoupleTemerature)
{
	float retVal;
	uint16 Internal_temperature;
	if (readCoupleTemerature)
	{
		Internal_temperature = spi_read(0);
		Internal_temperature = spi_read(1);
	}
	Internal_temperature = spi_read(2);//read from max 15..8 bites
	Internal_temperature = Internal_temperature<<8;
	Internal_temperature += spi_read(3);//read from max 7..0 bites


	  if (Internal_temperature & 0x8000) // check sign bit!
	  {//result temperature is negative
		  int16 tmpValue = Internal_temperature;
		  tmpValue >>= 8;// ignore bottom 8 bits - they're just thermocouple data
		  retVal = tmpValue;
	  }
	  else
	  {
		  uint16 tmpValue = Internal_temperature;
		  tmpValue >>= 8;// ignore bottom 8 bits - they're just thermocouple data
		  retVal = tmpValue;
	  }
	  //add 1/((x*2)^1+(x*2)^2); 19 and 18 bit.
	  if(Internal_temperature& 0x80)  retVal+=0.5;
	  if(Internal_temperature& 0x40)  retVal+=0.25;
	  if(Internal_temperature& 0x40)  retVal+=0.125;
	  if(Internal_temperature& 0x40)  retVal+=0.0625;
	return retVal;
}
