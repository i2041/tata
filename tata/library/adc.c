/*
 * adc.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#define R1 49900
#define R2 2825
//#define Vref 1175	//mV
#define Vref_ADC 3550	//35ADC pear 0.001mV; 1.157.000uV / 32768
#include <msp430i2041.h>
#include "commontypes.h"
#include "adc.h"
extern uint32 Voltage;

void init_adc()
{
	SD24CCTL0 |= SD24SNGL + SD24OSR_256 + SD24IE + SD24DF;
	SD24CCTL1 |= SD24SNGL + SD24OSR_256 + SD24IE + SD24DF;
	SD24CCTL2 |= SD24SNGL + SD24OSR_256 + SD24IE + SD24DF;
	SD24CCTL3 |= SD24SNGL + SD24OSR_256 + SD24IE + SD24DF;
	SD24INCTL0|= SD24INCH_0; //SD24 channel differential pair input
	SD24INCTL1|= SD24INCH_0;
	SD24INCTL2|= SD24INCH_0;
	SD24INCTL3|= SD24INCH_0;
	SD24PRE0  |= 0x08;	//delay before first conversion after x cycles
	SD24PRE1  |= 0x08;
	SD24PRE2  |= 0x08;
	SD24PRE3  |= 0x08;
	SD24CTL   |= SD24REFS;	//INTERNAL REFERINCE
}
uint16 read_adc(int pin)
{
	switch (pin)
	{
		case 0:{return SD24MEM0;}
		case 1:{return SD24MEM1;}
		case 2:{return SD24MEM2;}
		case 3:{return SD24MEM3;}
		default:{return 0;}
	}
}
void start_adc(uint8 chanal)
{
	switch (chanal)
	{
	case 0: SD24CCTL0 |= SD24SC;break;
	case 1: SD24CCTL1 |= SD24SC;break;
	case 2: SD24CCTL2 |= SD24SC;break;
	case 3: SD24CCTL3 |= SD24SC;break;
	}
}
#pragma vector=SD24_VECTOR
  __interrupt void SD24AISR(void)
{
	  switch (SD24IV)
	    {
	    case 2:                                   // SD24MEM Overflow
	    	break;
	    case 4:                                   // SD24MEM0 IFG
	    	SD24CCTL0 &= ~SD24IFG;
	        //__enable_interrupt();
	        break;
	    case 6:                                   // SD24MEM1 IFG
	    	SD24CCTL1 &= ~SD24IFG;
	        //__enable_interrupt();
	    	break;
	    case 8:                                   // SD24MEM2 IFG
	    	//SD24CCTL2 &= ~SD24IFG;
	    	Voltage = read_adc(2);	//read RAW ADC
	    	Voltage = (Voltage * Vref_ADC)/100000;				//calculate Vout
	    	Voltage = (Voltage*1879)/100;	//calculate Vin in mV, 18.36 = (R1+R2))/(R2)
	    	//__enable_interrupt();
			break;
	    case 10:
	    	SD24CCTL3 &= ~SD24IFG; 				  // SD24MEM3 IFG
	    	break;
	    }
}
//uint32 voltage()
//{
//uint32 retVal=0;
//uint32 retVal1=0;
//uint32 retVal2=0;
//start_adc();
////while (!(SD24CCTL2 & SD24IFG)){}	//white interrupt flag ADC to go next
//retVal = read_adc(2);	//read RAW ADC
//retVal = (retVal * Vref_ADC)/100000;				//calculate Vout
//retVal = (retVal*1872)/100;	//calculate Vin in mV, 18.36 = (R1+R2))/(R2)
//return retVal2;
//}
