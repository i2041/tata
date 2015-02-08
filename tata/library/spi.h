/*
 * spi.h
 *
 *  Created on: 04 февр. 2015 г.
 *      Author: Serghei
 */
#include "commontypes.h"
#ifndef SPI_H_
#define SPI_H_

void init_spi();
uint8 spi_read(uint8 NrOfBytes);

#endif /* SPI_H_ */
