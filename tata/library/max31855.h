/*
 * max31855.h
 *
 *  Created on: 04 ����. 2015 �.
 *      Author: Serghei
 */

#ifndef MAX31855_H_
#define MAX31855_H_

#include "commontypes.h"

#define VERIFY_FAULT_COUPLE 0

void ThermoCoupleTemperature();
void InternalTemperature();
float TCouple;
float TIntern;
#endif /* MAX31855_H_ */
