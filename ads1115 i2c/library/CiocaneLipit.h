/*
 * CiocanLipit.h
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */

#ifndef CIOCANELIPIT_H_
#define CIOCANELIPIT_H_
#include <msp430.h>

#define Button3 BIT3	//PORT1
#define Encoder3_1 BIT6	//PORT 2
#define Encoder3_2 BIT7	//PORT 2

#define CIOCAN_PWM1 BIT2	//PORT 3 TIMER A1.1
#define CIOCAN_PWM2 BIT3	//PORT 3 TIMER A1.2

#define MAX_DUTY_CYCLE 0xEFFF
#define MIN_DUTY_CYCLE 0x00FF
typedef enum
{
	init=0,
	stop,
	stop_statie1,
	stop_statie2,
	start,
	start_statie1,
	start_statie2
}CiocanLipitStates;
typedef enum
{
	rotireStanga,
	rotireDreapta,
	apasare
}EncodeStates;

void ciocaneLipit_Init(CiocanLipitStates state);
void ciocaneLipit_Encoder(EncodeStates state);
void ciocaneLipit_cyclic();
#endif /* CIOCANELIPIT_H_ */
