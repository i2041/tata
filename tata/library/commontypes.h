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
typedef enum typesOfError
{
   aprindere_error 	= 1,
   benzine_error	= 2,
   couple_error		= 3000,
   voltage_error	= 3,
   SCV_ERROR 		= 3333,
   SCG_ERROR 		= 3222,
   OC_ERROR			= 3111
}typeError;
#endif /* COMMONTYPES_H_ */
