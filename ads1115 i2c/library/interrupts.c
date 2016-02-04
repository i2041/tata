/*
 * interrupts.c
 *
 *  Created on: 05 џэт. 2016 у.
 *      Author: Serghei
 */
#include "interrupts.h"

#pragma vector=TIMER1_A1_VECTOR	//timer 1
__interrupt void Timer_A1 (void)
{
	switch(TA1IV)
	{
	 case 2:	// CCR1
	 break;
	 case 4:	// CCR2
	 break;
	 case 10: 	Flag33msOcured = true;// overflow ? reserved
	 break;
	 case 14: 	// overflow ?
	 break;
	 default:	//default :)
	 break;
	}
 	__enable_interrupt();
}

#pragma vector = PORT1_VECTOR
__interrupt void port1_isr(void)
{

	if (P1IFG & BIT3)	//Button 3
	{
		P1IFG 	&= ~ BIT3;
		if(_ciocanLipit_Mode_temporar == temperatureMode)
		{
			_ciocanLipit_Mode_temporar = pwmMode;
		}
		else if (_ciocanLipit_Mode_temporar == pwmMode)
		{
			_ciocanLipit_Mode_temporar = temperatureMode;
		}
		_ciocanLipit_ButtonValidate = 0;
	}
	__enable_interrupt();
}

#pragma vector = PORT2_VECTOR
__interrupt void port2_isr(void) {

	if (P2IFG & BIT0)	//encoder 2.1
	{
		P2IFG &= ~ BIT0;
		if (P2IN & BIT1) tmp2 -= 1;
		else tmp2 += 1;
	}
	if (P2IFG & BIT2)	//button2
	{
		P2IFG &= ~ BIT2;

		//P3OUT ^= BIT0;
	}
	if (P2IFG & BIT3)	//encoder 1.1
	{
		P2IFG &= ~ BIT3;
		if (P2IN & BIT4) tmp1 -= 1;
		else tmp1 += 1;
	}
	if (P2IFG & BIT5)	//button1
	{
		P2IFG &= ~ BIT5;
		P3OUT   ^= V220_PWM;
	}
	if (P2IFG & BIT6)	//encoder 3.1
	{
		P2IFG &= ~ BIT6;

		if (P2IN & BIT7)
		{
			if (_ciocanLipit_Mode == pwmMode)
			{
				if ( _ciocanLipit_ProcentageValue > 1)
				{
					TA1CCR1 += (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE - 100)/100; // -1%
					TA1CCR2 = TA1CCR1;
					P3SEL 	|= (CIOCAN_PWM1 + CIOCAN_PWM2);
					_ciocanLipit_ProcentageValue --;
				}
				else
				{
					_ciocanLipit_ProcentageValue = 0;
					P3SEL &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
					P3OUT &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
					TA1CCR1 = MAX_DUTY_CYCLE;
					TA1CCR2 = TA1CCR1;
				}
			}
			else
			{
				_ciocanLipit_EncoderValidate = 0;
				_ciocanLipit_TemperatureTemporar-=1;
			}
		}
		else
		{

			if (_ciocanLipit_Mode == pwmMode)
			{
				if (_ciocanLipit_ProcentageValue < 99)
				{
					TA1CCR1 -= (MAX_DUTY_CYCLE - MIN_DUTY_CYCLE - 100)/100; // +1%
//					if (TA1CCR1 < MIN_DUTY_CYCLE
					TA1CCR2 = TA1CCR1;
					P3SEL 	|= (CIOCAN_PWM1 + CIOCAN_PWM2);
					_ciocanLipit_ProcentageValue ++;
				}
				else
				{
					_ciocanLipit_ProcentageValue = 100;
					P3SEL &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
					P3OUT |=  (CIOCAN_PWM1 + CIOCAN_PWM2);
					TA1CCR1 = MIN_DUTY_CYCLE;
					TA1CCR2 = TA1CCR1;
				}
			}
			else
			{
				_ciocanLipit_EncoderValidate = 0;
				_ciocanLipit_TemperatureTemporar+=1;
			}
		}
	}
	__enable_interrupt();
}

#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
	if (IFG2 & UCB0RXIFG) {I2C_RX_Interrupt();IFG2 &= ~UCB0RXIFG;}
	if (IFG2 & UCB0TXIFG) {I2C_TX_Interrupt();IFG2 &= ~UCB0TXIFG;}
	if (IFG2 & UCA0TXIFG) {Uart_TX_Interrupt();IFG2 &= ~UCA0TXIFG;}
	if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
	__enable_interrupt();//__bis_SR_register(GIE);        // interrupts
}
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void)
{
	if (IFG2 & UCA0RXIFG) {Uart_RX_Interrupt();IFG2 &= ~UCA0RXIFG;}
	__bis_SR_register(GIE);        // interrupts
}

#pragma vector = ADC10_VECTOR
__interrupt void ADC10_ISR(void)
{
	__enable_interrupt();
}
