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
	displayIntensity = 0x07F ;	//max 0x0FFF
}
void tlc5947_Set_Intensity(uint8 displayintensity)
{
	uint32 tmpValue;
	tmpValue =(displayintensity*0xFFF);
	displayIntensity = (uint16)tmpValue;
}
uint8 tlc5947_get_Intensity()
{
	uint32 tmpValue;
	tmpValue = displayIntensity;
	tmpValue =(displayIntensity*100)/0xFFF;
	return (uint8)tmpValue;
}
//#pragma FUNCTION_OPTIONS( tlc5947_update_displays,"--opt_level=0" )
void tlc5947_update_displays()//ThreeBytes *array
{
		uint8  bit;
		uint8  ch;
		uint8  numDrivers = NumOfDrivers;
		P3DIR |= _latch + _sin + _clk;
		P3OUT &= ~_latch;
		while (numDrivers)
		{
		  // 24 channels per TLC5974
		  for (ch=24; ch >= 1 ; ch--)
		  {
			if (display[NumOfDrivers - numDrivers] & ((uint32)1 << (ch-1)) )	//verify which channel need to activate 0->24 Ch
			//if(0)
			  {
				// 12 bits per channel, send MSB first
				for (bit=12; bit>=1; bit--)
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
				for (bit=12; bit>=1; bit--)
				{
				  P3OUT &= ~_clk;		//clock low
				  P3OUT |= _clk;		//clock high
				}
			}
		  }
		  numDrivers--;
		}
		P3OUT &= ~_clk;				//clock low
		P3OUT |= _latch;			//latch high
		__delay_cycles(1000);
		P3OUT &= ~_latch;			//latch low
}
void tlc5947_calculate_digits(uint8 displayNr, uint16 value)
{
	uint32 tmpValue2=0;
	uint8  digit[3];
	uint8  DigitNr;
	uint32 powValue=10;
	for (DigitNr=0;DigitNr < (NumOfDigits-1);DigitNr++)
	{
		powValue*=10;
	}
	value = value%powValue;
	digit[0]= (value/100);
	digit[1]= (value%100)/10;
	digit[2]= (value%10);

	for(DigitNr=0;DigitNr < NumOfDigits;DigitNr++)
	{
		if (DigitNr == 1)
		{
			switch (digit[(NumOfDigits-1)-DigitNr])
				{
				case 0: tmpValue2 |= ((seg_A + seg_B + seg_C)<<4) + (seg_D + seg_E + seg_F)>>4; break;
				case 1: tmpValue2 |= (seg_B + seg_C)<<4; break;
				case 2: tmpValue2 |= ((seg_A + seg_B)<<4) + ((seg_D + seg_E + seg_G)>>4); break;
				case 3: tmpValue2 |= ((seg_A + seg_B + seg_C)<<4) + ((seg_D + seg_G)>>4); break;
				case 4: tmpValue2 |= ((seg_B + seg_C)<<4)  + ((seg_F + seg_G)>>4); break;
				case 5: tmpValue2 |= ((seg_A + seg_C)<<4) + ((seg_F + seg_G  + seg_D)>>4); break;
				case 6: tmpValue2 |= ((seg_A + seg_C)<<4) + ((seg_F + seg_E + seg_D  + seg_G)>>4); break;
				case 7: tmpValue2 |= ((seg_A + seg_B + seg_C)<<4); break;
				case 8: tmpValue2 |= ((seg_A + seg_B + seg_C)<<4) + ((seg_D + seg_E + seg_F + seg_G)>>4);break;
				case 9: tmpValue2 |= ((seg_A + seg_C)<<4) + ((seg_F + seg_G + seg_B+ seg_D)>>4); break;
				default:break;
				}
			if (displayNr == 2 ) tmpValue2 |= (seg_DP<<4);//seg_A not work and change on DP;
		}
		else
		{
		switch (digit[(NumOfDigits-1)-DigitNr])
			{
				case 0: tmpValue2 |= seg_A + seg_B + seg_C + seg_D + seg_E + seg_F; break;
				case 1: tmpValue2 |= seg_B + seg_C; break;
				case 2: tmpValue2 |= seg_A + seg_B + seg_D + seg_E + seg_G; break;
				case 3: tmpValue2 |= seg_A + seg_B + seg_G + seg_C + seg_D; break;
				case 4: tmpValue2 |= seg_F + seg_G + seg_B + seg_C ; break;
				case 5: tmpValue2 |= seg_A + seg_F + seg_G + seg_C + seg_D; break;
				case 6: tmpValue2 |= seg_A + seg_F + seg_E + seg_D + seg_C + seg_G; break;
				case 7: tmpValue2 |= seg_A + seg_B + seg_C; break;
				case 8: tmpValue2 |= seg_A + seg_B + seg_C + seg_D + seg_E + seg_F + seg_G;break;
				case 9: tmpValue2 |= seg_A + seg_F + seg_G + seg_B + seg_C + seg_D; break;
				default:break;
			}
		}
		if ( DigitNr != (NumOfDigits-1) ) tmpValue2=tmpValue2<<8; //last time dont need Rshift
	}
	display[(NumOfDrivers-1)-displayNr] = tmpValue2;
}


