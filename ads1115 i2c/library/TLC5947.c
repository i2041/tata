/*
 * TLC5947.c
 *
 *  Created on: 04 џэт. 2016 у.
 *      Author: Serghei
 */
#include "TLC5947.h"
void tlc5947_init()
{
	P3DIR |= _sin + _clk + _latch;
	displayIntensity = 0xFFF / 2;
}
void tlc5947_Set_Intensity(uint8 displayintensity)
{
	uint32 tmpValue;
	tmpValue =(displayintensity*0xFFF)/100;
	displayIntensity = (uint16)tmpValue;
}
uint8 tlc5947_get_Intensity()
{
	uint32 tmpValue;
	tmpValue = displayIntensity;
	tmpValue =(displayIntensity*100)/0xFFF;
	return (uint8)tmpValue;
}
void tlc5947_write(uint8 numDrivers,uint32 *displayData)//ThreeBytes *array
{
	  uint32 ch;
	  uint8  bit ;
	  P3OUT &= ~_latch;
	  while (numDrivers)
	  {
		  // 24 channels per TLC5974
		  for (ch=24; ch >= 1 ; ch--)
		  {
			if ( *displayData & ((uint32)1 << (ch-1)) )	//verify which channel need to activate 0->24 Ch
			{
				// 12 bits per channel, send MSB first
				for ( bit=12; bit>=1; bit--)
				{
				  P3OUT &= ~_clk;		//clock low

				  if (displayIntensity & (1 << bit) )//array->value
					  P3OUT |= _sin;	//data high
				  else
					  P3OUT &= ~_sin;	//data low

				  P3OUT |= _clk;		//clock high
				}
			}
			else										//In case if channel don't need to activate
			{											//leave 12bit 0(zero) for this channel
				P3OUT &= ~_sin;	//data low
				for ( bit=12; bit>=1; bit--)
				{
				  P3OUT &= ~_clk;		//clock low
				  P3OUT |= _clk;		//clock high
				}
			}
		  }
		  displayData++;
		  numDrivers--;
	  }
	  P3OUT &= ~_clk;			//clock low

	  P3OUT |= _latch;			//latch high
	  P3OUT &= ~_latch;			//latch low
}



