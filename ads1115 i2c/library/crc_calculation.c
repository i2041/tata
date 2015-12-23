/*
 * crc_calculation.c
 *
 *  Created on: 22.12.2015
 *      Author: uidg6243
 */

#ifndef CRC_CALCULATION_C_
#define CRC_CALCULATION_C_
#include "crc_calculation.h"

uint8 crc8(int *p, uint8 len)
{
		len--;
		uint16 i;
        uint16 crc = 0x0;
        while (len--)
		{
			i = (crc ^ *p++) & 0xFF;
			crc = (crc_table[i] ^ (crc << 8)) & 0xFF;
        }
        return crc & 0xFF;
}

#endif /* CRC_CALCULATION_C_ */
