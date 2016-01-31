/*
 * CiocanLipit.h
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */

#ifndef CIOCANELIPIT_H_
#define CIOCANELIPIT_H_
#include <msp430.h>
#include "commontypes.h"
#include "printf.h"
#define Button3 BIT3	//PORT1
#define Encoder3_1 BIT6	//PORT 2
#define Encoder3_2 BIT7	//PORT 2
#define NrForValidateStates 30 //1sec

#define CIOCAN_PWM1 BIT2	//PORT 3 TIMER A1.1
#define CIOCAN_PWM2 BIT3	//PORT 3 TIMER A1.2

#define CIOCAN_ADC1 BIT4	//PORT 1
#define CIOCAN_ADC2 BIT5	//PORT 1


#define MAX_DUTY_CYCLE 0x583E	//FFFF=33ms, 7C1 = 1ms, F83E = 32ms
#define MIN_DUTY_CYCLE 0x37C1

#define AVERAGE  25

void ciocaneLipit(States state);
void recalculatePWM();
void newStateOcure();

typedef enum
{
	temperatureMode=0,
	pwmMode,
}ciocanLipitMode;

uint8 _ciocanLipit_State;
ciocanLipitMode _ciocanLipit_Mode;
ciocanLipitMode _ciocanLipit_Mode_temporar;

uint8 _ciocanLipit_ProcentageValue;

uint8 _ciocanLipit_ButtonValidate;
uint8 _ciocanLipit_EncoderValidate;

uint16 _ciocanLipit_Temperature;
uint16 _ciocanLipit_TemperatureTemporar;
uint16 _ciocanLipit_ADC_Buffer;
bool ADC_Is_Ready;
uint16 readAdc(uint8 channel);
#endif /* CIOCANELIPIT_H_ */
