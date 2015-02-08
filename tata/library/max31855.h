/*
 * max31855.h
 *
 *  Created on: 04 ����. 2015 �.
 *      Author: Serghei
 */

#ifndef MAX31855_H_
#define MAX31855_H_

#include "commontypes.h"

#define SCV_ERROR 	3333
#define SCG_ERROR 	3222
#define OC_ERROR	3111
#define VERIFY_FAULT_COUPLE 1
float ThermoCoupleTemperature();
float InternalTemperature(uint8 readCoupleTemerature);


#endif /* MAX31855_H_ */