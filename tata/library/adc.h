/*
 * adc.h
 *
 *  Created on: Feb 10, 2015
 *      Author: uidg6243
 */

#ifndef ADC_H_
#define ADC_H_

uint32 voltage(void);
void start_adc(uint8 chanal);
void init_adc();
uint16 read_adc(int pin);

#endif /* ADC_H_ */
