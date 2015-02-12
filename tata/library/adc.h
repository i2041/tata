/*
 * adc.h
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */

#ifndef ADC_H_
#define ADC_H_

#define R1 49900
#define R2 2825
//#define Vref 1175	//mV
#define Vref_ADC 3550	//35ADC pear 0.001mV; 1.157.000uV / 32768

#include "commontypes.h"

extern uint32 Voltage;

void start_adc(uint8 chanal);
void init_adc();
uint16 read_adc(int pin);

#endif /* ADC_H_ */
