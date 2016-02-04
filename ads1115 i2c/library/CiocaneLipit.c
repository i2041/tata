/*
 * CiocaneLipit.c
 *
 *  Created on: 14.01.2016
 *      Author: uidg6243
 */
#include "CiocaneLipit.h"
void ciocaneLipit(States state)
{
	switch (state)
	{
		case(init):
		{
			/*** Encoder 3 statia lipit***/
			P2SEL 	&=~(Encoder3_1 + Encoder3_2);
			P2SEL2 	&=~(Encoder3_1 + Encoder3_2);

			P2DIR 	&=~(Encoder3_1 + Encoder3_2);
			//P1OUT = (Encoder3_1 + Encoder3_2); Pull UP resistor

			P2IES 	|= (Encoder3_1 + Encoder3_2);		//Interrupt Edge Select on falling edge
			P2IFG 	&=~(Encoder3_1 + Encoder3_2);		//Encoder3_1 + Encoder3_2 reset flags
			P2IE 	|= (Encoder3_1 );//+ Encoder3_2);

			/*** Button 3 statia lipit***/
			P1SEL 	&=~Button3;
			P1SEL2 	&=~Button3;

			P1DIR 	&=~Button3;

			P1IES 	|= Button3;							//Interrupt Edge Select on falling edge
			P1IFG 	&=~Button3;							//Button3 reset flags
			P1IE 	|= Button3;

			/*** Timer_B Set-Up ***/
			P3DIR	|= (CIOCAN_PWM1 + CIOCAN_PWM2);
			P3OUT	&=~(CIOCAN_PWM1 + CIOCAN_PWM2);

			P3SEL	&=~(CIOCAN_PWM1 + CIOCAN_PWM2);
			P3SEL2	&=~(CIOCAN_PWM1 + CIOCAN_PWM2);
			//ADC Configure ADC10
			P1SEL |= (CIOCAN_ADC1 + CIOCAN_ADC2);						// ADC input pin P1.4 P1.5
			ADC10CTL0 = SREF_1 + ADC10SHT_3 + REF2_5V + REFON + ADC10ON;				// no interrupt
			ADC10AE0 |= (CIOCAN_ADC1 + CIOCAN_ADC2);		// Port ADC option select

			//TA1CTL  = (TASSEL_2 + ID_0 + MC_2  + TAIE);	//SMCLK,div 1,Stop Mode, Interrupt Enable 16.56ms = 0xFFFF
			TA1CTL  = (TASSEL_2 + ID_1 + MC_2  + TAIE + TACLR);	//SMCLK,div 2,Stop Mode, Interrupt Enable 33ms = 0xFFFF

			TA1CCTL1 |= OUTMOD_3;						//Set/Reset
			TA1CCTL2 |= OUTMOD_3;

			TA1CCR0 |= MAX_DUTY_CYCLE;
			TA1CCR1 |= MAX_DUTY_CYCLE-100;
			TA1CCR2 |=TA1CCR1;
			_ciocanLipit_Temperature = 260;
			_ciocanLipit_TemperatureTemporar = _ciocanLipit_Temperature;
			_ciocanLipit_State = init;
			_ciocanLipit_Mode = temperatureMode;
			break;
		}
		case(stop_statie1):
		{
			P3SEL &= ~CIOCAN_PWM1;
			P3OUT &= ~CIOCAN_PWM1;
			_ciocanLipit_State = stop_statie1;
			break;
		}
		case(stop_statie2):
		{
			P3SEL &= ~CIOCAN_PWM2;
			P3OUT &= ~CIOCAN_PWM2;
			_ciocanLipit_State = stop_statie2;
			break;
		}
		case(stop):
		{
			P3SEL &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
			P3OUT &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
			_ciocanLipit_State = stop;
			break;
		}
		case(start):
		{
			P3SEL 	|= (CIOCAN_PWM1 + CIOCAN_PWM2);
			_ciocanLipit_State = start;
			break;
		}
	}
}
void recalculatePWM()
{
			uint16 tip_temp_ADC1 = readAdc(CIOCAN_ADC1);
			uint16 tip_temp_ADC2 = readAdc(CIOCAN_ADC2);
			uint16 current_tip_temperature = 0;
			static float i_Temp = 0;
			static float d_Temp = 0;
			static float i_Temp2 = 0;
			static float d_Temp2 = 0;

			int16 err_value;
			uint16 TA1CCRx_temporar;
			float P_Term;
			float I_Term;
			float D_Term;
			float ADC_TO_Temperature;
			_ciocanLipit_Temperature = 0;
			if (tip_temp_ADC1 != 1023)
			{
				P3SEL 	|= (CIOCAN_PWM1);
				ADC_TO_Temperature = (9.712843528*tip_temp_ADC1*tip_temp_ADC1*tip_temp_ADC1)/100000000 - (2.361429586*tip_temp_ADC1*tip_temp_ADC1)/10000 + 0.7141578774*tip_temp_ADC1 + 1.586170803;
				//float ADC_TO_Temperature2 = (9.712843528*tip_temp_ADC2*tip_temp_ADC2*tip_temp_ADC2)/100000000 - (2.361429586*tip_temp_ADC2*tip_temp_ADC2)/10000 + 0.7141578774*tip_temp_ADC2 + 1.586170803;
				//from measurements we determined that the following linearization is necessary:
				if (ADC_TO_Temperature < 310)
				{
					ADC_TO_Temperature = ADC_TO_Temperature * 0.68 + 21;
				}
				else
				{
					ADC_TO_Temperature = ADC_TO_Temperature * 0.88 -42;
				}
				current_tip_temperature = (uint16)ADC_TO_Temperature;	// after calibration this seems more acurate

				err_value = (_ciocanLipit_TemperatureTemporar - current_tip_temperature);

			P_Term = Kp * err_value;

				i_Temp += err_value;
				if (i_Temp > Max_acumulated_error) {i_Temp = Max_acumulated_error;}
				else if (i_Temp < Min_acumulated_error) {i_Temp = Min_acumulated_error;}

			I_Term = Ki * i_Temp;

			D_Term = Kd * (d_Temp - err_value);
				d_Temp = err_value;

				TA1CCRx_temporar = TA1CCR1 - (P_Term + I_Term + D_Term);

				if ( TA1CCRx_temporar > (MAX_DUTY_CYCLE-100) )
				{TA1CCRx_temporar = (MAX_DUTY_CYCLE-100);}
				else if ( TA1CCRx_temporar < (MIN_DUTY_CYCLE+100) )
				{TA1CCRx_temporar = (MIN_DUTY_CYCLE+100);}

				TA1CCR1 = TA1CCRx_temporar;
				_ciocanLipit_Temperature = current_tip_temperature;

			}
			//second iron
			else
			{/*iron1 is not connected*/
				P3SEL 	&= ~(CIOCAN_PWM1);
				P3OUT &= ~(CIOCAN_PWM1);
				TA1CCR1 = (MAX_DUTY_CYCLE-100);
			}
			if (tip_temp_ADC2 != 1023)
			{
				P3SEL 	|= (CIOCAN_PWM2);
				ADC_TO_Temperature = (9.712843528*tip_temp_ADC2*tip_temp_ADC2*tip_temp_ADC2)/100000000 - (2.361429586*tip_temp_ADC2*tip_temp_ADC2)/10000 + 0.7141578774*tip_temp_ADC2 + 1.586170803;

				P_Term=0;
				I_Term=0;
				D_Term=0;
				if (ADC_TO_Temperature < 310)
				{
					ADC_TO_Temperature = ADC_TO_Temperature * 0.68 + 21;
				}
				else
				{
					ADC_TO_Temperature = ADC_TO_Temperature * 0.88 -42;
				}
				current_tip_temperature = (uint16)ADC_TO_Temperature;	// after calibration this seems more acurate

				err_value = (_ciocanLipit_TemperatureTemporar - current_tip_temperature);

			P_Term = Kp * err_value;

				i_Temp2 += err_value;
				if (i_Temp2 > Max_acumulated_error) {i_Temp2 = Max_acumulated_error;}
				else if (i_Temp2 < Min_acumulated_error) {i_Temp2 = Min_acumulated_error;}

			I_Term = Ki * i_Temp2;

			D_Term = Kd * (d_Temp2 - err_value);
				d_Temp2 = err_value;

				TA1CCRx_temporar = TA1CCR2 - (P_Term + I_Term + D_Term);

				if ( TA1CCRx_temporar > (MAX_DUTY_CYCLE-100) )
				{TA1CCRx_temporar = (MAX_DUTY_CYCLE-100);}
				else if ( TA1CCRx_temporar < (MIN_DUTY_CYCLE+100) )
				{TA1CCRx_temporar = (MIN_DUTY_CYCLE+100);}

				TA1CCR2 = TA1CCRx_temporar;

				_ciocanLipit_Temperature += current_tip_temperature;
				if (tip_temp_ADC1 != 1023)_ciocanLipit_Temperature /=2;//avg virful 1 si virful2
			}
			else
			{/*iron2 is not connected*/
				P3SEL 	&= ~(CIOCAN_PWM2);
				P3OUT &= ~(CIOCAN_PWM1);
				TA1CCR2 = (MAX_DUTY_CYCLE-100);
			}
}
void newStateOcure()
{
	if ( (_ciocanLipit_ButtonValidate != (NrForValidateStates+1)) || (_ciocanLipit_EncoderValidate != (NrForValidateStates+1)) )
	{
		if (_ciocanLipit_ButtonValidate < NrForValidateStates) _ciocanLipit_ButtonValidate++;
		else if (_ciocanLipit_ButtonValidate == NrForValidateStates)
		{

			if ( (_ciocanLipit_Mode_temporar == pwmMode) && (_ciocanLipit_Mode != _ciocanLipit_Mode_temporar) )
			{
			_ciocanLipit_ProcentageValue = 0;
			P3SEL &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
			P3OUT &= ~(CIOCAN_PWM1 + CIOCAN_PWM2);
			TA1CCR1 = MAX_DUTY_CYCLE-100;
			TA1CCR2 = MAX_DUTY_CYCLE-100;
			}
			if ( (_ciocanLipit_Mode_temporar == temperatureMode) && (_ciocanLipit_Mode != _ciocanLipit_Mode_temporar) )
			{
			TA1CCR1 = MAX_DUTY_CYCLE-100;
			TA1CCR2 = TA1CCR1;
			P3SEL |= (CIOCAN_PWM1 + CIOCAN_PWM2);
			}
			_ciocanLipit_Mode = _ciocanLipit_Mode_temporar;
			_ciocanLipit_ButtonValidate++;
		}

		if (_ciocanLipit_EncoderValidate < NrForValidateStates) _ciocanLipit_EncoderValidate++;
		else if (_ciocanLipit_EncoderValidate == NrForValidateStates)
		{
			_ciocanLipit_Temperature = _ciocanLipit_TemperatureTemporar;
			_ciocanLipit_EncoderValidate++;
		}
	}
}
uint16 readAdc(uint8 channel)
{
	uint8 i;

	ADC10CTL0 &= ~ENC;						// Toggle enable in order to change the channel

	switch (channel)
	{
	case CIOCAN_ADC1:
		ADC10CTL1 = INCH_5;						// input A4 (CH4)
		break;
	case CIOCAN_ADC2:
		ADC10CTL1 = INCH_4;						// input A5 (CH5)
		break;
	}

	_ciocanLipit_ADC_Buffer = 0;

	for (i=AVERAGE;i>0;i--)
	{
		ADC10CTL0 |= (ENC + ADC10SC);  		// Sampling and conversion start
		while (ADC10CTL1 & ADC10BUSY);    	// ADC10BUSY?
		_ciocanLipit_ADC_Buffer += ADC10MEM;	  			// copy ADC result
	}
	_ciocanLipit_ADC_Buffer /= AVERAGE;
	return _ciocanLipit_ADC_Buffer;
}
