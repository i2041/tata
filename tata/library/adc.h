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

uint32 Voltage;
uint16 RunTime;
uint16 min_Temperature;
uint16 read_adc(int pin);
void start_adc();
void init_adc();


#endif /* ADC_H_ */
