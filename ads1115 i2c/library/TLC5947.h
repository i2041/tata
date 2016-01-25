/*
 * TLC5947.h
 *
 *  Created on: 04 џэт. 2016 у.
 *      Author: Serghei
 */

#ifndef LIBRARY_TLC5947_H_
#define LIBRARY_TLC5947_H_
#include "commontypes.h"
#include <msp430.h>

#define _clk 	BIT4	//port3
#define _sin  	BIT7	//port3
#define _latch	BIT5	//port3
#define NumOfDrivers 3
#define NumOfDigits 3

typedef enum segmentsOfDisplay
{
	seg_A 	= 0x01,
	seg_B 	= 0x02,
	seg_C 	= 0x04,
	seg_DP 	= 0x08,
	seg_D 	= 0x10,
	seg_E 	= 0x20,
	seg_F 	= 0x40,
	seg_G 	= 0x80
}segments;
//	typedef union
//	{
//		struct
//		{
//			uint32 	ch	:24,
//					bit	:8;
//		}FourBytesUnion;
//	}tmpValue2223;
//	tmpValue2223 tmpValue;

uint16 displayIntensity;
uint32 display[NumOfDrivers];
void tlc5947_init();
void tlc5947_update_displays();//ThreeBytes *array
void tlc5947_Set_Intensity(uint8 displayintensity);
uint8 tlc5947_get_Intensity();
void tlc5947_calculate_digits(uint8 display, uint16 value);
#endif /* LIBRARY_TLC5947_H_ */
