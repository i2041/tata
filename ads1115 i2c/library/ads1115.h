/*
 * ads1115.h
 *
 *  Created on: 11.12.2015
 *      Author: uidg6243
 */

#ifndef ADS1115_H_
#define ADS1115_H_
/*
=========================================================OS: Operational status/single-shot conversion start
This bit determines the operational status of the device.
This bit can only be written when in power-down mode.
For a write status:
0 : No effect
1 : Begin a single conversion (when in power-down mode)
For a read status:
0 : Device is currently performing a conversion
1 : Device is not currently performing a conversion
 */
typedef enum
{
	NoEfect					=0x0000,
	BeginSingleConversion	=0x8000
}OpStatus;
/*
=========================================================MUX[2:0]: Input multiplexer configuration (ADS1115 only)
These bits configure the input multiplexer. They serve no function on the ADS1113/4.
000 : AINP = AIN0 and AINN = AIN1 (default) 100 : AINP = AIN0 and AINN = GND
001 : AINP = AIN0 and AINN = AIN3 101 : AINP = AIN1 and AINN = GND
010 : AINP = AIN1 and AINN = AIN3 110 : AINP = AIN2 and AINN = GND
011 : AINP = AIN2 and AINN = AIN3 111 : AINP = AIN3 and AINN = GND
 */
typedef enum
{
	AIN0P_AIN1N	=0x0000,
	AIN0P_AIN3N	=0x1000,
	AIN1P_AIN3N	=0x2000,
	AIN2P_AIN3N	=0x3000,
	AIN0P_GND	=0x4000,
	AIN1P_GND	=0x5000,
	AIN2P_GND	=0x6000,
	AIN3P_GND	=0x7000
}MUX;
/*
=========================================================PGA[2:0]: Programmable gain amplifier configuration (ADS1114 and ADS1115 only)
These bits configure the programmable gain amplifier. They serve no function on the ADS1113.
000 : FS = ±6.144V(1) 		100 : FS = ±0.512V
001 : FS = ±4.096V(1) 		101 : FS = ±0.256V
010 : FS = ±2.048V(default)	110 : FS = ±0.256V
011 : FS = ±1.024V 			111 : FS = ±0.256V
 */
typedef enum
{
	V6_144	=0x0000,
	V4_096	=0x0200,
	V2_048	=0x0400,
	V1_024	=0x0600,
	V0_512	=0x0800,
	V0_256_1=0x0A00,
	V0_256_2=0x0C00,
	V0_256_3=0x0E00
}PGA;
/*
=========================================================MODE: Device operating mode
This bit controls the current operational mode of the ADS1113/4/5.
0 : Continuous conversion mode
1 : Power-down single-shot mode (default)
 */
typedef enum
{
	ContinuousConversion			=0x0000,
	PowerDownSingleShortConversion	=0x0100
}MODE;
/*
=========================================================DR[2:0]: Data rate
These bits control the data rate setting.
000 : 8SPS 			100 : 128SPS (default)
001 : 16SPS 		101 : 250SPS
010 : 32SPS 		110 : 475SPS
011 : 64SPS 		111 : 860SPS
 */
typedef enum
{
	SPS8	=0x0000,
	SPS16	=0x0020,
	SPS32	=0x0040,
	SPS64	=0x0060,
	SPS128	=0x0080,
	SPS250	=0x00A0,
	SPS475	=0x00C0,
	SPS860	=0x00E0
}DR;
/*
=========================================================COMP_MODE: Comparator mode (ADS1114 and ADS1115 only)
This bit controls the comparator mode of operation. It changes whether the comparator is implemented as a
traditional comparator (COMP_MODE = '0') or as a window comparator (COMP_MODE = '1'). It serves no
function on the ADS1113.
0 : Traditional comparator with hysteresis (default)
1 : Window comparator
 */
typedef enum
{
	TraditionalComparatorHysteresis	=0x0000,
	WindowsComparator	=0x0010
}COMP_MODE;
/*
=========================================================COMP_POL: Comparator polarity (ADS1114 and ADS1115 only)
This bit controls the polarity of the ALERT/RDY pin. When COMP_POL = '0' the comparator output is active
low. When COMP_POL='1' the ALERT/RDY pin is active high. It serves no function on the ADS1113.
0 : Active low (default)
1 : Active high
 */
typedef enum
{
	ActiveLow	=0x0000,
	ActiveHigh	=0x0008
}COMP_POL;
/*
=========================================================COMP_LAT: Latching comparator (ADS1114 and ADS1115 only)
This bit controls whether the ALERT/RDY pin latches once asserted or clears once conversions are within the
margin of the upper and lower threshold values. When COMP_LAT = '0', the ALERT/RDY pin does not latch
when asserted. When COMP_LAT = '1', the asserted ALERT/RDY pin remains latched until conversion data
are read by the master or an appropriate SMBus alert response is sent by the master, the device responds with
its address, and it is the lowest address currently asserting the ALERT/RDY bus line. This bit serves no
function on the ADS1113.
0 : Non-latching comparator (default)
1 : Latching comparator
 */
typedef enum
{
	Non_LatchingComparator	=0x0000,
		LatchingComparator	=0x0004
}COMP_LAT;
/*
=========================================================COMP_QUE: Comparator queue and disable (ADS1114 and ADS1115 only)
These bits perform two functions. When set to '11', they disable the comparator function and put the
ALERT/RDY pin into a high state. When set to any other value, they control the number of successive
conversions exceeding the upper or lower thresholds required before asserting the ALERT/RDY pin. They
serve no function on the ADS1113.
00 : Assert after one conversion
01 : Assert after two conversions
10 : Assert after four conversions
11 : Disable comparator (default)
 */
typedef enum
{
	AsertAfter_1_Conversion	=0x0000,
	AsertAfter_2_Conversion	=0x0001,
	AsertAfter_3_Conversion	=0x0002,
	DisableComparator		=0x0003
}COMP_QUE;

//typedef struct
//{
//	OpStatus	;
//	MUX			;
//	PGA			;
//	MODE		;
//	DR			;
//	COMP_MODE	;
//	COMP_POL	;
//	COMP_LAT	;
//	COMP_QUE	;
//}ConfigRegister;
void configADS1115();
void Transmit();
void Receive();
#endif /* ADS1115_H_ */
