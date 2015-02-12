/*
 * commontypes.h
 *
 *  Created on: 04 февр. 2015 г.
 *      Author: Serghei
 */

#ifndef COMMONTYPES_H_
#define COMMONTYPES_H_

typedef unsigned char                  	uint8;	//0-255
typedef char						  	int8;	//-128 - 127
typedef unsigned short                 	uint16;	//0-65535
typedef short                 			int16;	//-32768 - 32767
typedef unsigned long                   uint32;	//0 - 4 294 967 295
typedef long                   			int32;	//-2147483648 - 2147483647
typedef unsigned long long int         	uint64;
typedef long long int         			int64;	//-9223372036854775808 - 9223372036854775807
typedef uint8							bool;
#define stop	0
#define start	1
#define false 0
#define true 1
#endif /* COMMONTYPES_H_ */
