/*
 * adc.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include <msp430i2041.h>
#include "adc.h"

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

	Voltage = 0;
	RunTime = 0;
}
uint16 read_adc(int pin)
{
	switch (pin)
	{
		case 0:{return SD24MEM0;}
		case 1:{return SD24MEM1;}	//for MinTemperature
		case 2:{return SD24MEM2;}
		case 3:{return SD24MEM3;}
		default:{return 0;}
	}
}
void start_adc()
{
	SD24CCTL0 |= SD24SC;
	SD24CCTL1 |= SD24SC;
	SD24CCTL2 |= SD24SC;
	SD24CCTL3 |= SD24SC;
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
	    	//SD24CCTL1 &= ~SD24IFG;
	    	min_Temperature =(read_adc(1));
	    	min_Temperature = min_Temperature>>4; //32768 / 128 = maximum 256 grade Celsius
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
	    	//SD24CCTL3 &= ~SD24IFG; 				  // SD24MEM3 IFG
	    	RunTime = read_adc(3);
	    	break;
	    }
}
