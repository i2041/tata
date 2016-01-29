/*
 * pid.h
 *
 *  Created on: 25 џэт. 2016 у.
 *      Author: Serghei
 */

#ifndef LIBRARY_PID_H_
#define LIBRARY_PID_H_

#include "commontypes.h"
#define iMax  100
#define iMin -100

/* constants for PID */
const float Kp = 0.01;
const float Ki = 0.01;
const float Kd = 0.001;

/* Global Variables for PID */
float d_Temp = 0;
float i_Temp = 0;

uint16 pid_calculation(int16 temperature_error, uint16 pwm_register);


#endif /* LIBRARY_PID_H_ */
