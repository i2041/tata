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

//typedef struct ThreeBytes
//{
//    uint32 value:24;
//}ThreeBytes;

uint16 displayIntensity;
void tlc5947_init();
void tlc5947_write(uint8 numOFdrivers,uint32 *displayData);//ThreeBytes *array
void tlc5947_Set_Intensity(uint8 displayintensity);
uint8 tlc5947_get_Intensity();
#endif /* LIBRARY_TLC5947_H_ */
