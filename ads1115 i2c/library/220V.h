/*
 * 220V.h
 *
 *  Created on: 15 џэт. 2016 у.
 *      Author: Serghei
 */

#ifndef LIBRARY_220V_H_
#define LIBRARY_220V_H_

#include <msp430.h>
#include "commontypes.h"
#include "mlx90614.h"
#include "24V.h"

#define Button1 BIT5	//PORT 2
#define Encoder1_1 BIT3	//PORT 2
#define Encoder1_2 BIT4	//PORT 2
#define DEFAULT_TEMPERATURE_STEEP 1.8
#define V220_PWM BIT1	//PORT 3
#define DEFAULT_220V_TEMPERATURE_MAXIMUM 180
//======================PID_220V===============
#define Kp_220V 					0.1
#define Ki_220V 					0.01
#define Kd_220V 					0.5
#define Max_acumulated_error_220V	200
#define Min_acumulated_error_220V  -200
#define MAX_220V_COUNTERS			30
//=============================================

void V220(States state);
void V220_cyclic_recalculate_PWM();

uint8 _220V_State;
float temperature_220V_steep;
uint8 temperature_220V_maximum;
int16 temperature_220V_counter;//se poate de pus si un int8
uint8 temperature_220V;
float temperature_220V_required;

#endif /* LIBRARY_220V_H_ */
