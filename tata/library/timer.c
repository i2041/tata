/*
 * timer.c
 *
 *  Created on: 04 џэт. 2015 у.
 *      Author: Serghei
 */
#include <msp430i2041.h>
void init_timer(unsigned int MCLK_devider,unsigned int SMCLK_devider,unsigned int buff_ccr0)
{
	CSCTL0 &= !(0x02);	//DCO bypass mode. When this bit is set, the DCO is bypassed. An external
							//digital clock can be input on the CLKIN pin.
							//0b = DCO bypass mode not selected
							//1b = DCO bypass mode selected
		CSCTL0 &= !(0x01);	//DCO resistor select. This bit selects internal or external resistor for DCO
							//operation.
							//0b = DCO operation with internal resistor
							//1b = DCO operation with external resistor
	//		DCOF			//DCO fault flag. This bit is set when the fault is detected when DCO operates in
							//the external resistor mode. DCO switches to internal resistor mode as fail-safe
							//feature.
							//0b = No DCO fault
							//1b = DCO fault detected
		switch (MCLK_devider)
			{//CSCTL1 &= !(0x07);
				case 1:  {CSCTL1 |= (0x00);break;}		//000b = Divide by 1
				case 2:  {CSCTL1 |= (0x01);break;}		//001b = Divide by 1
				case 4:  {CSCTL1 |= (0x02);break;}		//010b = Divide by 1
				case 8:  {CSCTL1 |= (0x03);break;}		//011b = Divide by 1
				case 16: {CSCTL1 |= (0x04);break;}		//100b = Divide by 1
			default: break;
			}
			switch (SMCLK_devider)
			{//CSCTL1 &= (!0x70);
				case 1:  {CSCTL1 |= (0x00);break;}		//000b = Divide by 1
				case 2:  {CSCTL1 |= (0x10);break;}		//001b = Divide by 1
				case 4:  {CSCTL1 |= (0x20);break;}		//010b = Divide by 1
				case 8:  {CSCTL1 |= (0x30);break;}		//011b = Divide by 1
				case 16: {CSCTL1 |= (0x40);break;}		//100b = Divide by 1
			default: break;
			}
			/*	TAxCTL Register
			15 	14 	13 	12 	11 	10 	|	9 	8
					Reserved 		|	TASSEL

			7 	6 	|	5 		4 	|	3 		|	2  | 1 	 |  0
			ID 		|		MC 		| Reserved 	|TACLR |TAIE | TAIFG
			TASSEL 	=> Timer_A clock source select
						00b = TAxCLK
						01b = ACLK
						10b = SMCLK
						11b = Inverted TAxCLK
			ID		=> Input divider. These bits select the divider for the input clock.
						00b = /1
						01b = /2
						10b = /4
						11b = /8
			MC		=> Mode control. Setting MCx = 00h when Timer_A is not in use conserves power.
						00b = Stop mode: Timer is halted
						01b = Up mode: Timer counts up to TAxCCR0
						10b = Continuous mode: Timer counts up to 0FFFFh
						11b = Up/down mode: Timer counts up to TAxCCR0 then down to 0000h
			TAIE	=> Timer_A interrupt enable. This bit enables the TAIFG interrupt request.
						0b = Interrupt disabled
						1b = Interrupt enabled
			TAIFG	=> Timer_A interrupt flag
						0b = No interrupt pending
						1b = Interrupt pending
			*/
			TA0CTL = TASSEL_2 + ID_1 + TAIE + MC_1;  //0x0290;	//0,SMCLK,UPMODE,InteruptEnebled
			TA0CCR0 = buff_ccr0;
			TA0CCTL0|= CCIE;		//interupt enable
}
// Timer A0 interrupt service routine
// Interrupt handler for TA0CCR0 CCIFG.
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
	static unsigned int a=0;
	if (a % 2 == 0)
	{
		write_io(10,1);
		write_io(22,1);
		write_io(23,1);
	}
	else
	{
		write_io(10,0);
		write_io(22,0);
		write_io(23,0);
	}
//	P1OUT ^= 0x01;
//	P2OUT ^= 0x04;
//	P2OUT ^= 0x08;
	start_adc(2);
	task100ms();
	a++;//In this case, it will loop until the variable i increments to 20000.
	if (a>1) a=0;
	TA0CCTL0 	&= ~CCIFG;
	TA0CTL 		&= ~TAIFG;
	__enable_interrupt();
}
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
	switch( TA0IV )
	{
	 case 2: break; // CCR1 not used
	 case 4: break; // CCR2 not used
	 case 10:
     {
	 __enable_interrupt(); // overflow
	 break;
     }
	 case 14:
	 {
	 __enable_interrupt(); // overflow
	 break;
	 }
	}

}
