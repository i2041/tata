/*
 * adc.c
 *
 *  Created on: 04 ���. 2015 �.
 *      Author: Serghei
 */
#define R1 47
#define R2 4.7
#include <msp430i2041.h>
void init_adc()
{
	SD24CCTL0 |= SD24SNGL + SD24OSR_256 + SD24IE;
	SD24CCTL1 |= SD24SNGL + SD24OSR_256 + SD24IE;
	SD24CCTL2 |= SD24SNGL + SD24OSR_256 + SD24IE;
	SD24CCTL3 |= SD24SNGL + SD24OSR_256 + SD24IE;
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
int read_adc(int pin)
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
	  volatile double tmp=0;
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
	    	SD24CCTL2 &= ~SD24IFG;
	        //__enable_interrupt();
			break;
	    case 10:
	    	SD24CCTL3 &= ~SD24IFG;
	    	break;
	    }
}
float voltage()
{
float retVal;

	retVal = read_adc(2) - 32767;	//read RAW ADC
	retVal = 1.154/32.767;//mV		//convert to mV (Vout)
	retVal = ((retVal*(R1+R2))/(R2*1000));	//calculate Vin in mV

return retVal;
}
