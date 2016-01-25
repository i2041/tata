/*
 * mlx90614.h
 *
 *  Created on: 22.12.2015
 *      Author: uidg6243
 */

#ifndef MLX90614_H_
#define MLX90614_H_

#include "i2c.h"
#include "commontypes.h"
#define MLX90614_I2CADDR 0x00

typedef enum
{
// RAM
MLX90614_RAWIR1 = 0x04,
MLX90614_RAWIR2 = 0x05,
MLX90614_TA 	= 0x06,
MLX90614_TOBJ1 	= 0x07,
MLX90614_TOBJ2 	= 0x08,
// EEPROM
MLX90614_TOMAX 	= 0x20,
MLX90614_TOMIN 	= 0x21,
MLX90614_PWMCTRL= 0x22,
MLX90614_TARANGE= 0x23,
MLX90614_EMISS 	= 0x24,
MLX90614_CONFIG = 0x25,
MLX90614_ADDR	= 0x0E,
MLX90614_ID1 	= 0x3C,
MLX90614_ID2 	= 0x3D,
MLX90614_ID3	= 0x3E,
MLX90614_ID4 	= 0x3F
}MLX90614_ADRESS;

float Mlx90614_read_Register(MLX90614_ADRESS adress);

#endif /* MLX90614_H_ */
