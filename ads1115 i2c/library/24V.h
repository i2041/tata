/*
 * 24V.h
 *
 *  Created on: 16.01.2016
 *      Author: uidg6243
 */

#ifndef LIBRARY_24V_H_
#define LIBRARY_24V_H_

#include <msp430.h>
#include "commontypes.h"
#include "mlx90614.h"
#include "220V.h"
#include "printf.h"
#define Button2 	BIT2	//PORT 2
#define Encoder2_1 	BIT0	//PORT 2
#define Encoder2_2 	BIT1	//PORT 2

#define V24_PWM 	BIT0	//PORT 3
#define V24_DEFAULT_TEMPERATURE 180
#define V24_ARRAY_ELEMENTS 5
//======================PID_24V===============
#define Kp_24V 					1
#define Ki_24V 					0.01
#define Kd_24V 					0.05
#define Max_acumulated_error_24V	200
#define Min_acumulated_error_24V  -200
#define TA0CCR0_FREQUENCE 		0x7FFF	//16.5ms = 0xFFFF  result 0x7FFF = 8.238ms
//=============================================


void V24(States state);

uint8 _24V_State;
void V24V_recalculate_PWM();
float map(float x, float in_min, float in_max, float out_min, float out_max);

uint8 	V24_Time_array[V24_ARRAY_ELEMENTS];
uint16 	V24_Temperature_array[V24_ARRAY_ELEMENTS];
bool 	V24_TimeSet;
uint8   V24_elementFromArray_write;
uint8   V24_elementFromArray_read;
float  V24_temperature;
float  V24_counterTime;

#endif /* 24V_H_ */
