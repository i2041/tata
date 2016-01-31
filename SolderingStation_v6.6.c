#include <msp430g2553.h>

#define Firmware 66

// first digit = software version 1,2,3...
// second digit: 
// 0: MSP430G2453, encoder tesit,
// 1: MSP430G2453, encoder tesit,
// 2: MSP430G2453, encoder zimtat,
// 3: MSP430G2453, encoder zimtat,

// 4: MSP430G2553, encoder tesit,
// 5: MSP430G2553, encoder tesit,
// 6: MSP430G2553, encoder zimtat,
// 7: MSP430G2553, encoder zimtat,

// 8: NA
// 9: NA

/****** SW changes**************************************************************
F.1.X :	first release
F.2.X : wdt_pwm in mode 1 is reset; firmware display 0.5s; updated PID parameters;
		Buzz on/off does not change mode
F.3.X : Error 5 (temp. sensor missing) disabled in mode 0
F.4.X : PID parameters depending on supply voltage
F.5.X : Removed trap for unwanted interrupt
F.6.X : Fixed bug for soldering stations witout I2C temperature sensor mounted
...
*******************************************************************************/


// ------------ TWEAK parameters -----------------------------------------------
// PID tuning parameters - merge cu ~toate vf-urile
#define Kp_initial 		5		// 5
#define Ki_initial 		0.05	// 0.008
#define Kd_initial 		0.5		// 0.5
#define Max_acumulated_error	1000

// depending on your resistor divider increase or decrease this value
#define SUPPLY_RESISTOR_DIVIDER	0.271	// 275/1024 = 0.268554

// 0.2762 pt Cristi ;0.27 pt Florin; 0.2774 pt Florin 2; 0.278 - Sacha 
// 0.271 Adi


// I2C temperature sensor address
// 0x90 for ADD0 connected to GND ; 0x94 for ADD0 connected to VCC
#define ADDRESS1		0x90

// Buzzer active time (when desired temperature is reached)
// how many 100ms intervals the buzzer will ring
#define BUZZER_100ms	5

// how many 100ms intervals a long button press needs
#define LONG_PRESS_100ms 8

#define LED_INITIAL_BRIGHTNESS 8	// 0-10 LED brightness settings

//#define SYS_TICK 		49759	// each 1000us	... 16000 ticks
//#define SYS_TICK 		62375	// each 200us	... 3200 ticks
//#define SYS_TICK 		65216	// each 20us	... 1600 ticks
#define SYS_TICK 		63936	// each 100us	... 1600 ticks
// -----------------------------------------------------------------------------

// -------------- PORT1 pin definitions ----------------------------------------
#define SUPPLY_DIAGNOSE	BIT0    // V_Bat measurement (4K7 - 47K voltage divider)
#define FET_PWM			BIT1    // output command for power MOSFET
#define SPEAKER			BIT2    // output for speaker
#define OUTPUT_DIAGNOSE	BIT3    // voltage feedback from MOSFET output (4K7 - 47K voltage divider)
#define CRT_SENSE		BIT4    // voltage measurement of letcon tip thermocouple
#define TIP_TEMP		BIT5    // current feedback from MOSFET output
#define SEGMENT_c		BIT6    //PORT1
#define SEGMENT_b		BIT7    //PORT1
// -----------------------------------------------------------------------------

// -------------- PORT2 pin definitions ----------------------------------------
#define SPARE_IN_OUT_1	BIT0    // for future use (idle switch...)
#define SPARE_IN_OUT_2	BIT1    // for future use (idle switch...)
#define COM1			BIT2    //PORT2
#define ENCODER2		BIT3    // Rotary encoder switch 2
#define BUTTON			BIT4    // Rotary encoder push button
#define SEGMENT_e		BIT5    //PORT2
#define COM3			BIT6    //PORT2
#define COM2			BIT7    //PORT2
// -----------------------------------------------------------------------------

// -------------- PORT3 pin definitions ----------------------------------------
#define SCL 			BIT0    // I2C clock for temperature sensor TMP100/101
#define SDA				BIT1    // I2C data for temperature sensor TMP100/101
#define SEGMENT_a		BIT2    //PORT3
#define SEGMENT_f		BIT3    //PORT3
#define ENCODER1		BIT4    // Rotary encoder switch 1
#define SEGMENT_d		BIT5    //PORT3
#define SEGMENT_h		BIT6    //PORT3 (dot)
#define SEGMENT_g		BIT7    //PORT2
// -----------------------------------------------------------------------------

// -------- Define masks for display -------------------------------------------
#define DOT_MASK		BIT6	//PIN3.6 for segment h (dot)
#define P1_MASK			0xC0
#define P1_MASK_NOT		0x3F
#define P2_MASK			0x20
#define P2_MASK_NOT		0xDF
#define P3_MASK			0xEC
#define P3_MASK_NOT		0x13
// -----------------------------------------------------------------------------

// ----------- variables for BCD display ---------------------------------------
const unsigned char LED_PWM[11] = {0,1,2,4,6,9,12,16,20,25,32};	// 10 steps for linear perception of LED brightness
// Displayed characters           {   0,   1,   2,   3,   4,   5,   6,   7,  8,   9,    -,   A,	  C,   E,   F,   H,   r,   n,   L,   P,   S,   U,nimic}
const unsigned char P1_digits[23]={0xC0,0xC0,0x80,0xC0,0xC0,0x40,0x40,0xC0,0xC0,0xC0,0x00,0xC0,0x00,0x00,0x00,0xC0,0x00,0x40,0x00,0x80,0x40,0xC0,0x00};
const unsigned char P2_digits[23]={0x20,0x00,0x20,0x00,0x00,0x00,0x20,0x00,0x20,0x00,0x00,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00,0x20,0x00};
const unsigned char P3_digits[23]={0x2C,0x00,0xA4,0xA4,0x88,0xAC,0xAC,0x04,0xAC,0xAC,0x80,0x8C,0x2C,0xAC,0x8C,0x88,0x80,0x80,0x28,0x8C,0xAC,0x28,0x00};
unsigned char 	PWM_current_step = 0;	// used for LED brightness control
unsigned char 	LED_PWM_index = LED_INITIAL_BRIGHTNESS;		// LED brightness index = 5 (10 brightness levels)
char 			display_time = 0;		// how much time a number is displayed until refreshed (x100ms)
unsigned char 	current_digit = 0;		// switches current digit that is displayed
unsigned char 	digit1 = 0;				// stores MSB character/digit
unsigned char 	digit2 = 0;				// stores middle character/digit
unsigned char 	digit3 = 0;				// stores LSB character/digit
unsigned char 	display_dot1 = 0;		// indicate if first dot is displayed 	0.00
unsigned char 	display_dot2 = 0;		// indicate if second dot is displayed 	00.0
unsigned char 	display_dot3 = 0;		// indicate if third dot is displayed 	000.
unsigned char   firmware_display_on = 1;// first thing to display is firmware version 
// -----------------------------------------------------------------------------

// ----------- variables for PCB temperature measurement------------------------
unsigned char 	TMP101_address = ADDRESS1;		// ADD0 connected to GND
int				current_PCB_temp = 25;			// estimate initial temperature
int				PCB_temp = 0;					// filtered value
int				PCB_temp_avg = 0;				// used for filtering
int 			display_PCB_temperature = 0;
unsigned char 	counter_I2C_deffect = 0;	// if in 100 readings we get only 0xFF then I2c temperature sensor is deffect
// -----------------------------------------------------------------------------

// ----------- variables for Buttons and rotary encoder function ---------------
unsigned char 	BUTTON_pressed = 0;
unsigned char 	long_pres_delay = 0;
unsigned char 	rotary_right = 0;	// signals if rotary encoder was turned to the right
unsigned char 	rotary_left = 0;	// signals if rotary encoder was turned to the left
unsigned char 	Rotary_tesit = 0;	// rotary encoder direction is different for each encoder type
// -----------------------------------------------------------------------------

// ----------- variables for timing control ------------------------------------
unsigned char 	cycle_nr = 0;			// used for alternating PWM on solder tip and pause each 100ms for temperature reading
unsigned char 	new_100ms = 0;
unsigned char 	wdt_pwm = 100;
unsigned char 	wdt_pwm_position = 0;
// -----------------------------------------------------------------------------


// ----------- variables for soldering tip temperature measuring ---------------

// use these for 4x faster thermocouple voltage to temperature calculation
// precision is +- 2 degrees in interval [0, 430] degrees Celsius
//unsigned long int tip_temp_ADC = 0;		// stores ADC result for tip temperature measurement
//unsigned long int tip_temperature_1 = 0;	// for polynom (3rd order) calculation - I need at least 32 bits
//unsigned long int tip_temperature_2 = 0;	// for polynom (3rd order) calculation - I need at least 32 bits
//unsigned long int tip_temperature_3 = 0;	// for polynom (3rd order) calculation - I need at least 32 bits
//unsigned int tip_temp_Celsius = 0;

// use these for slow but precise tip temp calculation
unsigned int 	tip_temp_ADC = 0;				// stores ADC result for tip temperature measurement
double 			tip_temp_Celsius = 0;			// converts ADC to degrees Celsius
int 			current_tip_temperature = 0;	// instant value
int 			tip_temp_avg = 0; 				// used for filtering
int				TIP_temperature = 0;			// filtered value
int 			desired_temperature = 280;		// desired temperature of solder tip
// -----------------------------------------------------------------------------

// ----------- variables for PID process control -------------------------------
int 			P_err = 0;			// proportional error = set point - process value
int 			err_old = 0;		// previous error
int				I_err = 0;			// integral error = sum of previous errors
int 			D_err = 0;			// differential error = current error - previous error
float 			Kp = Kp_initial;	//
float 			Ki = Ki_initial;
float 			Kd = Kd_initial;
float 			scale_K = 1;

// -----------------------------------------------------------------------------

// ----------- variables for Buzzer function -----------------------------------
unsigned char	buzzer_on = 0;				// if active then BUZZZZZ
unsigned char	buzzer_needed = 1;			// if desired temperature changes then alert when ready
unsigned char 	buzzer_timer = 0;			// counts active time
unsigned char	buzzer_function_active = 1;	// enable/disable buzzer function
unsigned char 	short_beep = 0;				// 100ms on 1s off
// -----------------------------------------------------------------------------

// ----------- variables for averaging measurements ----------------------------
const char 		average_values = 4;			// used for averaging measurements
unsigned char 	counter_average = 0;		// used for averaging measurements
double 			voltage_to_display = 125;	// average supply voltage (125 = 12.5V)
double 			current_to_display = 0;		// average output current (125 = 1.25A)
float 			supply_voltage_avg = 0;		// averages supply voltage measurements
float 			output_current_avg = 0;		// average current reported by UPD166010 driver
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
unsigned char 	station_mode = 0;
	// 0 = display: tip temperature 	set: desired temperature;
	// 1 = display: PWM					set: PWM;
	// 2 = display: supply voltage 		set: PWM;
	// 3 = display: instant current		set: PWM;
	// 4 = display: PCB temperature		set: LED brightness;
	// in modes 0-3, long press toggles BUZZER function (BUZ...On...BUZ...OFF)
	// in mode 4 long press saves to flash these parameters: desired temperature, buzzer on/off. display brightness

unsigned char 	active_Err_no = 0;	// holds active system error
	// 1: solder tip disconnected
	// 2: overtemperature on tip
	// 3: supply voltage too low for soldering (<5V)
	// 4: supply voltage too high (>27V)
	// 5: I2C temperature sensor defect
// -----------------------------------------------------------------------------

// ----------- variables for flash writing -------------------------------------
unsigned char flash_KEY = 0xAA;				// if we don't find this in the first location of segment C then we use default values
unsigned char flash_temp_byte = 0;			// used for storing/restoring parameters

// -----------------------------------------------------------------------------


// ----------- general functions declaration -----------------------------------
void 			Init			(void);
unsigned char 	I2CTempRead		(void);
void 			delayMs			(unsigned int delay_ms);
void 			DisplayDigits	(unsigned char digit);
unsigned int 	ADCRead			(unsigned char channel,unsigned char average);
void 			display_number 	(unsigned int nr_to_display, char type);
int 			pid 			(void);
void 			Write_to_Flash 	(unsigned char data1, unsigned char data2, unsigned char data3);
void 			Read_from_Flash	(void);

// -----------------------------------------------------------------------------


// ----------------------------- MAIN PROGRAM ----------------------------------
int main(void)
{
	// All ports init
	Init();

	// configure Timer A: SMCLK = 16MHz (62.5ns/tick) , countmode, interrupt
	TACTL = TASSEL_2 + MC_2;
	// start timer
	TAR = SYS_TICK;			// load default value in Timer A Register
	TACTL |= TAIE;			// enable interrupt

	current_digit = 1;

	P2OUT &= ~COM1;
	P2OUT &= ~COM2;
	P2OUT &= ~COM3;

	// turn off the 8 segments:
	P1OUT &= P1_MASK_NOT;
	P2OUT &= P2_MASK_NOT;
	P3OUT &= P3_MASK_NOT;

	buzzer_needed = 1;

	Read_from_Flash();				// read previous settings

	switch (Firmware%10)			// remainder = second digit of firmware
	{
		case 0:
		case 1:
		case 4:
		case 5:
			Rotary_tesit = 1;		// direction of rotation depends on this parameter
			break;
			
		case 2:
		case 3:
		case 6:
		case 7:
			Rotary_tesit = 0;	// direction of rotation depends on this parameter
			break;	
	}

	display_number (Firmware,9); 	// display firmware version
	display_time = 5  ;			// x 100ms ~= 0.5s
	firmware_display_on = 1;

	/*
	// default working mode is mode 0: set desired tip temperature;  	display: actual tip temperature
	display_number(desired_temperature,0);		// display desired temperature for 2s
	display_time = 18  ;// x 100ms ~= 2s
	*/
	
	while(1)
	{
		// check for pressed buttons or rotary encoder

		if(BUTTON_pressed  != 0)	// interrupt function reports that the button was pressed
		{
			// we have an unprocessed button press

			// when pressing/releasing the button you can accidentally rotate it
			rotary_right = 0;
			rotary_left = 0;

			if(P2IN & BUTTON)		// if button released
			{
				BUTTON_pressed = 0;

				if(station_mode < 4)	// change 4 to number of modes
				{
					station_mode++;
					if(station_mode == 1)	// reset PWM value
					{
						wdt_pwm = 0;
					}
				}
				else
				{
					station_mode = 0;
				}

				display_number(station_mode,6);		// display current mode
				display_time = 20  ;// x 100ms

				long_pres_delay = 0;
			}

			else		// button still pressed
			{
				if(new_100ms)
				{
					long_pres_delay++;
				}

				if(long_pres_delay >= (LONG_PRESS_100ms >> 1))		// semi-long press detected
				{
					// in mode 4 a long press means flash storing of parameters
					if(station_mode == 4)
					{

						if(long_pres_delay == (LONG_PRESS_100ms >> 1))	// display rolling "FLASH---"
						{
							display_number(1,8);		// display "  F"
							display_time = 16;			// x 100ms
						}
						if(long_pres_delay == (LONG_PRESS_100ms >> 1)+1)	// display rolling "FLASH---"
						{
							display_number(2,8);		// display " FL"
							display_time = 16;			// x 100ms
						}
						if(long_pres_delay == (LONG_PRESS_100ms >> 1)+2)	// display rolling "FLASH---"
						{
							display_number(3,8);		// display "FLA"
							display_time = 16;			// x 100ms
						}
						if(long_pres_delay == (LONG_PRESS_100ms >> 1)+3)	// display rolling "FLASH---"
						{
							display_number(4,8);		// display "LAS"
							display_time = 16;			// x 100ms
						}
						if(long_pres_delay == (LONG_PRESS_100ms >> 1)+4)	// display rolling "FLASH---"
						{
							display_number(5,8);		// display "ASH"
							display_time = 16;			// x 100ms
						}
						if(long_pres_delay == (LONG_PRESS_100ms >> 1)+5)	// display rolling "FLASH---"
						{
							display_number(6,8);		// display "---"
							display_time = 16;			// x 100ms

							// store all parameters to flash memory
							flash_temp_byte = desired_temperature >> 1; 	// store 500 as 250

							Write_to_Flash (flash_temp_byte, LED_PWM_index, buzzer_function_active);

							long_pres_delay = 0;
							BUTTON_pressed = 0;
						}
					}
					else	// in modes 0,1,2,3 a long press toggles buzzer
					{
						if(long_pres_delay >= LONG_PRESS_100ms)
						{
							long_pres_delay = 0;
							if( buzzer_function_active == 0)
							{
								buzzer_function_active = 1;	// turn on/off
								display_number(1,5);		// display "On"
								display_time = 10;			// x 100ms
							}
							else
							{
								buzzer_function_active = 0;	// turn on/off
								display_number(0,5);		// display "OFF"
								display_time = 10;			// x 100ms
							}
							BUTTON_pressed = 0;
						}
						else
						{
							display_number(1,7);		// display "8U2" (BUZ)
							display_time = 10;			// x 100ms
						}
					}
				}

			}
		}

		if(rotary_right  != 0)	// rotary encoder was turned right
		{
			rotary_right = 0;

			switch (station_mode)
			{
			case 0:
					if(desired_temperature < 450)
					{
						desired_temperature+=10;
					}
					else
					{
						desired_temperature = 450;
					}
					buzzer_needed = 1;
					display_number(desired_temperature,0);		// display desired temperature
					display_time = 8;	// x 100ms
				break;
			case 4:
					if(LED_PWM_index < 10)
					{
						LED_PWM_index++;
					}
					else
					{
						LED_PWM_index = 10;
					}
				break;
			default:
					if(wdt_pwm < 99)
					{
						wdt_pwm+=2;
					}
					else
					{
						wdt_pwm = 100;
					}
					display_number(wdt_pwm,0);
					display_time = 8;	// x 100ms
				break;
			}

		}

		if(rotary_left  != 0)	// rotary encoder was turned left
		{
			rotary_left = 0;

			switch (station_mode)
			{
			case 0:
					if(desired_temperature > 40)
					{
						desired_temperature-=10;
					}
					else
					{
						desired_temperature = 40;
					}
					buzzer_needed = 1;

					display_number(desired_temperature,0);		// display desired temperature
					display_time = 8 ;// x 100ms
				break;
			case 4:
					if(LED_PWM_index > 0)
					{
						LED_PWM_index--;
					}
					else
					{
						LED_PWM_index = 0;
					}
				break;
			default:
					if(wdt_pwm > 1)
					{
						wdt_pwm-=2;
					}
					else
					{
						wdt_pwm = 0;
					}
					display_number(wdt_pwm,0);
					display_time = 8;	// x 100ms
				break;
			}
		}

		if (new_100ms == 1)
		{
			new_100ms = 0;

			//----------- update solder tip temperature reading ----------------
			// T_degrees_C = 9.712843528*10^(-8)*(D2^3) - 2.361429586*10^(-4)*D2^2 + 0.7141578774*D2 + 1.586170803
			// see excel file for details
			tip_temp_ADC = ADCRead(5,32);
			/*	do not delete these commented lines !!!
			// if you need the above equation four times faster this is it:
			tip_temperature_1 = (731*tip_temp_ADC)>>10;
			tip_temperature_2 = (tip_temp_ADC * tip_temp_ADC) >> 12 ;
			tip_temperature_3 = (tip_temperature_2 * tip_temp_ADC) >> 11 ;
			tip_temp_Celsius = (unsigned int)(tip_temperature_3 - tip_temperature_2 + tip_temperature_1);
			*/

			tip_temp_Celsius = (9.712843528*tip_temp_ADC*tip_temp_ADC*tip_temp_ADC)/100000000 - (2.361429586*tip_temp_ADC*tip_temp_ADC)/10000 + 0.7141578774*tip_temp_ADC + 1.586170803;

			// from measurements we determined that the following linearization is necessary:
			if (tip_temp_Celsius < 310)
			{
				tip_temp_Celsius = tip_temp_Celsius * 0.68 + 21;
			}
			else
			{
				tip_temp_Celsius = tip_temp_Celsius * 0.88 -42;
			}

			current_tip_temperature = (unsigned int)tip_temp_Celsius;	// after calibration this seems more acurate

			tip_temp_avg += current_tip_temperature;

			// use PID algorithm for PWM/temperature control only in mode 0
			if(station_mode == 0)	// only in mode 0 we use PID
			{
				wdt_pwm = pid();	// calculate current pwm
			}

			//----------- update PCB temperature reading -----------------------
			current_PCB_temp = I2CTempRead();

			if (current_PCB_temp > 127)						// negative temperature on PCB
			{
				current_PCB_temp = current_PCB_temp - 256;
			}
			PCB_temp_avg += current_PCB_temp;

			// ------- read supply voltage -------------------------------------
			// read CH1 - suply voltage monitoring (47K & 4.7K resistor divider)
			//supply_voltage_avg = (ADCRead(0,8) * 275)/1024;	// send 12.5V as 125
		 	supply_voltage_avg += ADCRead(0,32) * SUPPLY_RESISTOR_DIVIDER;

			// ------- read uPD166010 current sense line -----------------------

			// Mosfet sense output current amplification factor: K = Iload/Isense = 10000 arount 2.5A
			// current sense resistor R3 = 10K
			// Iload = K * Isense = K * Usense/R3 = 10000 * (ADC_CH5_val * 2.5V / 1024) / 10000 = ((ADC_CH5_val * 2.5) >> 10) [A]

			// need an RC low pass filter for average current reading R = 10k, C = 1uF

			// ADCRead(4,8) * 250)/1024;	// send 2.5A as 250 => 2 decimal points
			// ADCRead(4,8) * 0.244; 		// *250/1024
			output_current_avg += (ADCRead(4,8) >> 2); // 250/1024 almost equal to /4 <=> >> 2


			// calculate average values
			counter_average++;
			if(counter_average >= average_values)			// average for noise filtering
			{
				counter_average = 0;

				PCB_temp = PCB_temp_avg / average_values; 	// divide by average_values
				PCB_temp_avg = 0;

				TIP_temperature = tip_temp_avg / average_values;// divide by average_values
				tip_temp_avg = 0;

				voltage_to_display = supply_voltage_avg / average_values;// divide by average_values
				supply_voltage_avg = 0;
				
				
				//scale_K = 100.0 / voltage_to_display;	// at 10V = 100% K parameters, at 25V 40% of parameters
				scale_K = (250 - 0.9*voltage_to_display)/100.0;	// 200% @5V, 25% @25V
				
				Kp = Kp_initial * scale_K; 	// scale the proportional parameter according to supply voltage
																// at high supply we get a smoother PWM :)
				Ki = Ki_initial * scale_K;
				Kp = Kp_initial * scale_K;	
				
				current_to_display = output_current_avg / average_values;// divide by average_values
				output_current_avg = 0;
			}

			// -------- deactivate buzzer after BUZZER_100ms time has passed----
			if( buzzer_on == 1)
			{
				buzzer_timer--;
				if(buzzer_timer==0)
				{
					buzzer_on = 0;
					P1OUT &= ~SPEAKER;

					buzzer_needed = 0;
				}
			}

			// ----- run diagnostic measurements for error readings ------------
			active_Err_no = 0;

			// --- soldering tip diagnosis -------------------------------------
			if (TIP_temperature > 474)		// 476 degrees celsius =  open load 
			{
				// solder tip is disconnected - store open load Error
				active_Err_no = 1;			// Error: 1 = solder tip disconnected

				I_err = 0;					// prevent false acumulation of errors

				// turn off PWM just in case the tip is still there
				if(station_mode == 0)
				{
					wdt_pwm = 0;
				}
				else
				{
					// alert user in case he connected the soldering tip and it overheated
					short_beep++;
					if (short_beep >= 10)
					{
						short_beep = 0;
						buzzer_on = 1;
						buzzer_timer = BUZZER_100ms;
					}
						
				}
			}
			else
			{
				if (TIP_temperature > 470)	// 470 degrees celsius
				{
					// solder tip overheat
					active_Err_no = 2;	// Error: 2 = overtemperature on solder tip

					// turn off PWM for cooling
					if(station_mode == 0)
					{
						wdt_pwm = 0;
					}
					else
					{
						//wdt_pwm = 0;
						
						// alert user in case he connected the soldering tip and it overheated
						short_beep++;
						if (short_beep >= 10)
						{
							short_beep = 0;
							buzzer_on = 1;
							buzzer_timer = BUZZER_100ms;
						}
							
					}
				}
			}


			// ------------- supply voltage errors -----------------------------
			if (voltage_to_display < 50)	// supply voltage < 5V
			{
				active_Err_no = 3;		// Error: 3 = supply voltage too low for soldering (<5V)
			}
			else
			{
				if (voltage_to_display > 270)	// supply voltage > 25V
				{
					//LED_PWM_index = 2;		// dimming for LED display so we can prevent voltage regulator from overheating
					active_Err_no = 4;		// Error: 4 = supply voltage too high (>27V)

					// turn off PWM for protection
					wdt_pwm = 0;
				}
			}

			// --------------I2C temperature sensor diagnosis-------------------
			if (current_PCB_temp == -1)	// 0xFF reading from I2C sensor
			{
				counter_I2C_deffect++;
				if (counter_I2C_deffect > 100)	// we have 100 consecutive 0xFF readings
				{
					counter_I2C_deffect = 100;	// keep from overflowing 255
					
					if (station_mode == 4)
					{
						// sensor is deffect or missing
						active_Err_no = 5;	// Error: 5 = I2C temperature sensor deffect
					}
				}
			}
			else
			{
				counter_I2C_deffect = 0;
			}

			// ------------- MOSFET diagnosis ----------------------------------
			// measure OUTPUT_DIAGNOSE ADC pin during ON time = too complicated for now


			// ----------update displayed information---------------------------
			display_time--;
			if(display_time <= 0)
			{
				if (firmware_display_on == 1)					// after firmware display timeout
				{
					firmware_display_on = 0;
					
					display_number(desired_temperature,0);		// display desired temperature for 2s
					display_time = 18  ;// x 100ms ~= 2s
				}
				else
				{
				
					switch (station_mode)
					{
					case 1:		// display PWM duty cycle
						display_number(wdt_pwm,0);
						display_time = 4;	// x 100ms
						break;

					case 2:		// display supply voltage
						display_number((unsigned int)voltage_to_display,2);
						display_time = 8;	// x 100ms
						break;

					case 3:		// display instant current consumption
						display_number((unsigned int)current_to_display,3);
						display_time = 8;	// x 100ms
						break;

					case 4:		// display PCB (cold jonction) temperature
						if(active_Err_no == 5)	// in mode 4 display error for missing I2C temperature sensor
						{
							display_number(active_Err_no,4);			// 4 = error display mode
							display_time = 8;							// prevent other things to appear on display
						}
						else
						{
							if (PCB_temp < 0)				// negative temperature on PCB
							{
								display_number(-PCB_temp,1);	// negative temperature
							}
							else
							{
								display_number(PCB_temp,0);	// positive temperature
							}
							display_time = 4;	// x 100ms
						}
						break;

					default:		// display tip temperature for case 0 and 1
						if((active_Err_no > 0) && (active_Err_no < 5))	// in mode 0 display immediately all errors except I2C sensor
						{
							display_number(active_Err_no,4);			// 4 = error display mode
							display_time = 8;							// prevent other things to appear on display
						}
						else
						{
							display_number(TIP_temperature,0);
							display_time = 4;	// x 100ms
						}
						break;
					}
				}

			}
		}

  }
}

int pid (void)
{
	int pid_result;

	err_old = P_err;
	P_err = desired_temperature - current_tip_temperature;

	// activate buzzer
	if ((P_err < 3) && (P_err > -3))
	{
		// small dead band
		//P_err = 0;

		if(buzzer_needed == 1)
		{
			buzzer_on = 1;
			buzzer_needed = 0;
			buzzer_timer = BUZZER_100ms;
		}
	}

	P_err = P_err;
	I_err += err_old;
	if (I_err > Max_acumulated_error)
	{
		I_err = Max_acumulated_error;
	}
	if (I_err < -Max_acumulated_error)
	{
		I_err = -Max_acumulated_error;
	}

	D_err = P_err - err_old;

	pid_result = (int) (Kp * P_err + Ki * I_err + Kd * D_err);

	if(pid_result > 100)
	{
		return 100;
	}
	else
	{
		if (pid_result < 0)
		{
			return 0;
		}
		else
		{
			return pid_result;
		}

	}

}


void display_number (unsigned int nr_to_display, char type)
{
	// 	|_digit1_||_digit2_||_digit3_|

	// type = 0 : positive number/temperature
	// type = 1 : negative temperature
	// type = 2 : voltage (one decimal)
	// type = 3 : current (two decimals)
	// type = 4 : error display (Er.x)
	// type = 5 : display buzzer function status "ON"/"OFF"
	// type = 6 : display station mode: "-2-"
	// type = 7 : display "8U2" standing for BUZ
	// type = 8 : display rolling "FLASH---" message
	// type = 9 : display firmware version: F.1.6.
	
	switch (type)
	{
	case 0:		//type = 0 : positive number/temperature
		display_dot1 = 0;
		display_dot2 = 0;	// no decimal points
		display_dot3 = 0;

		digit3 = nr_to_display % 10;		// remainder

		nr_to_display = nr_to_display / 10;
		digit2 = nr_to_display % 10;		// remainder

		nr_to_display = nr_to_display / 10;
		digit1 = nr_to_display%10;
		if(digit1 == 0)						// do not display leading zeros
		{
			digit1 = 22;					// blank
			if(digit2 == 0)					// do not display leading zeros
			{
				digit2 = 22;				// blank
			}
		}
		break;

	case 1:		// type = 1 : negative temperature
		display_dot1 = 0;
		display_dot2 = 0;			// no decimal points
		display_dot3 = 0;

		digit3 = nr_to_display % 10;		// remainder
		nr_to_display = nr_to_display / 10;

		digit2 = nr_to_display % 10;		// remainder
		if(digit2 == 0)						// do not display leading zeros
		{
			digit2 = 22;					// blank
		}

		digit1 = 10; 						// "-" sign
		break;

	case 2:		// type = 2 : voltage (one decimal)
		display_dot1 = 0;
		display_dot2 = 1;	// one decimal place (12.7V)
		display_dot3 = 0;

		digit3 = nr_to_display % 10;		// remainder
		nr_to_display = nr_to_display / 10;
		digit2 = nr_to_display % 10;		// remainder
		nr_to_display = nr_to_display / 10;
		digit1 = nr_to_display%10;

		if(digit1 == 0)						// do not display leading zeros
		{
			digit1 = 22;					// blank
		}
		break;

	case 3:		// type = 3 : current (two decimals)
		display_dot1 = 1;	// two decimal places (1.23A)
		display_dot2 = 0;
		display_dot3 = 0;

		digit3 = nr_to_display % 10;		// remainder
		nr_to_display = nr_to_display / 10;
		digit2 = nr_to_display % 10;		// remainder
		nr_to_display = nr_to_display / 10;
		digit1 = nr_to_display%10;
		break;

	case 4:		// type = 4 : error display (Er.x)
		display_dot1 = 0;
		display_dot2 = 1;	// dot after second digit (Er.5)
		display_dot3 = 0;

		digit3 = nr_to_display % 10;		// remainder

		digit2 = 16;		// character "r"

		digit1 = 13;		// character "E"
		break;

	case 5:		// type = 5 : display buzzer function status "ON"/"OFF"
		display_dot1 = 0;	// no dots
		display_dot2 = 0;
		display_dot3 = 0;

		if (nr_to_display == 0)
		{
			// display BUZZER "OFF"
			digit3 = 14;	// character "F"
			digit2 = 14;	// character "F"
			digit1 = 0;		// character "0"
		}
		else
		{
			// display BUZZER "On"
			digit3 = 17;	// character "n"
			digit2 = 0;		// character "0"
			digit1 = 22;	// blank
		}
		break;
	case 6:		// type = 6 : display station mode: "-2-"
		display_dot1 = 0;
		display_dot2 = 0;			// no decimal points
		display_dot3 = 0;
		digit3 = 10; 				// "-" sign
		digit2 = nr_to_display;		// station mode 0..4
		digit1 = 10; 				// "-" sign
		break;

	case 7:		// type = 7 : display "8U2" standing for BUZ
		display_dot1 = 0;
		display_dot2 = 0;			// no decimal points
		display_dot3 = 0;
		digit3 = 2; 				// "2"
		digit2 = 21;				// "U"
		digit1 = 8; 				// "8"
		break;

	case 8:			// type = 8 : display rolling "FLASH---" message
		display_dot1 = 0;
		display_dot2 = 0;			// no decimal points
		display_dot3 = 0;

		switch(nr_to_display)
		{
			case 1:
				digit1 = 22; 				// " " blank
				digit2 = 22;				// " " blank
				digit3 = 14; 				// "F"
				break;

			case 2:
				digit1 = 22; 				// " " blank
				digit2 = 14;				// "F"
				digit3 = 18; 				// "L"
				break;

			case 3:
				digit1 = 14; 				// "F"
				digit2 = 18;				// "L"
				digit3 = 11; 				// "A"
				break;

			case 4:
				digit1 = 18; 				// "L"
				digit2 = 11;				// "A"
				digit3 = 20; 				// "S"
				break;

			case 5:
				digit1 = 11; 				// "A"
				digit2 = 20;				// "S"
				digit3 = 15; 				// "H"
				break;

			case 6:
				digit1 = 10; 				// "-"
				digit2 = 10;				// "-"
				digit3 = 10; 				// "-"
				break;
		}

		break;

	case 9:		// type = 9 : display firmware version: F.1.6.

		display_dot1 = 1;
		display_dot2 = 1;	// all dots on
		display_dot3 = 1;

		digit3 = nr_to_display % 10;		// remainder
		nr_to_display = nr_to_display / 10;
		digit2 = nr_to_display % 10;		// remainder
		digit1 = 14;						// character "F" (Firmware)

		break;
			
	}
}

unsigned int ADCRead(unsigned char channel,unsigned char average)
{
	unsigned int ADC_VAL10 = 0;
	unsigned char i=0;

	ADC10CTL0 &= ~ENC;						// Toggle enable in order to change the channel
	switch (channel)
	{
	case 0:
		ADC10CTL1 = INCH_0;						// input A0 (CH0)
		break;
	case 1:
		ADC10CTL1 = INCH_1;						// input A1 (CH1)
		break;
	case 2:
		ADC10CTL1 = INCH_2;						// input A2 (CH2)
		break;
	case 3:
		ADC10CTL1 = INCH_3;						// input A3 (CH3)
		break;
	case 4:
		ADC10CTL1 = INCH_4;						// input A4 (CH4)
		break;
	case 5:
		ADC10CTL1 = INCH_5;						// input A5 (CH5)
		break;
	case 6:
		ADC10CTL1 = INCH_6;						// input A6 (CH6)
		break;
	case 7:
		ADC10CTL1 = INCH_7;						// input A7 (CH7)
		break;
	}

	ADC_VAL10 = 0;

	for (i=average;i>0;i--)
	{
		ADC10CTL0 |= ENC + ADC10SC;  		// Sampling and conversion start
		while (ADC10CTL1 & ADC10BUSY);    	// ADC10BUSY?
		ADC_VAL10 += ADC10MEM;	  			// copy ADC result
	}


	ADC_VAL10 = ADC_VAL10/average;

	_NOP();			// SET BREAKPOINT HERE

	return ADC_VAL10;
}




// ----- Timer_A Interrupt Vector (TAIV) handler ------------------------------------ //
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)		//interrupt each 100us
{
	TAR = SYS_TICK;
	TACTL &= ~TAIFG;		// reset interrupt flag

	if((buzzer_on == 1) && (buzzer_function_active == 1))
	{
		P1OUT ^= SPEAKER;
	}

	PWM_current_step++;
	if (PWM_current_step == 32)
	{
		PWM_current_step = 0;
	}

    // turn off the 8 segments:
    P1OUT &= P1_MASK_NOT;
    P2OUT &= P2_MASK_NOT;
    P3OUT &= P3_MASK_NOT;

    current_digit++;
    DisplayDigits(current_digit);

}

void DisplayDigits(unsigned char digit)
{
	switch (digit)
	{
	case 3:
		current_digit = 0;
    	if (PWM_current_step < LED_PWM[LED_PWM_index])
    	{
            // display digit 3
    	    P2OUT &= ~COM1;
    	    P2OUT &= ~COM2;
    	    P2OUT |= COM3;

            P1OUT |= P1_digits[digit3];
    	    P2OUT |= P2_digits[digit3];
    	    P3OUT |= P3_digits[digit3];


    	    // display the dot for LSB digit
			if (display_dot3 == 1)
			{
				 P3OUT |= DOT_MASK;
			}
    	}
    	break;

	case 2:
    	if (PWM_current_step < LED_PWM[LED_PWM_index])
    	{
            // display digit 2

    	    P2OUT &= ~COM1;
    	    P2OUT |= COM2;
    	    P2OUT &= ~COM3;

            P1OUT |= P1_digits[digit2];
    	    P2OUT |= P2_digits[digit2];
    	    P3OUT |= P3_digits[digit2];

    	    // display the dot for middle digit
			if (display_dot2 == 1)
			{
				 P3OUT |= DOT_MASK;
			}

    	}
    	break;
	case 1:
    	if (PWM_current_step < LED_PWM[LED_PWM_index])
    	{
    		// display digit 1
            P2OUT |= COM1;
            P2OUT &= ~COM2;
            P2OUT &= ~COM3;

            P1OUT |= P1_digits[digit1];
            P2OUT |= P2_digits[digit1];
            P3OUT |= P3_digits[digit1];

    	    // display the dot for middle digit
			if (display_dot1 == 1)
			{
				 P3OUT |= DOT_MASK;
			}
    	}
    	break;
	}
}

void delayMs(unsigned int delay_ms)
{
	unsigned int i,j;

	for (i=0;i<1666;i++)
		for (j=0;j<delay_ms;j++);         // wait  milliseconds
}


// ------------ I2C communication with TMP101 sensor -------//
unsigned char  I2CTempRead(void)
{
	unsigned char I2C_temperature = 0;
	unsigned char i = 0;
	unsigned char mask = 0x80; // (1000.0000)
	unsigned char ACK = 1;

	// clock is always output
	P3DIR |= SCL;

	// MSP430 is the master SDA - output
	P3DIR |= SDA;

// ---------------- START condition  --------------------
	// SDA high to low ------------------START comunication SDA = 0
	P3OUT &= ~SDA;	//**
	// SCL high to low ------------------PULL DOWN SCL
	P3OUT &= ~SCL;	//**

// -----Slave Address (TMP101_address = 0b1001.0000) -----
	for ( i=0 ; i<7 ; i++)
	{
		if ( mask & TMP101_address )
		{
			// SDA low to high ------------------SDA = 1
			P3OUT |= SDA;   //**
			// SCL low to high ------------------CLK up
			P3OUT |= SCL;   //**
			// SCL high to low ------------------CLK down
			P3OUT &= ~SCL;  //**
		}
		else
		{
			// SDA high to low ------------------SDA = 0
			P3OUT &= ~SDA; //**
			// SCL low to high ------------------CLK up
			P3OUT |= SCL;  //**
			// SCL high to low ------------------CLK down
			P3OUT &= ~SCL; //**
		}
		mask = mask >> 1;
	}

// ----------------- R/#W bit ----------------------------
	// SDA low to high -------------------R/#W = 1 Read
	P3OUT |= SDA;  //**
	// SCL low to high ------------------CLK up
	P3OUT |= SCL;   //**
	// SCL high to low ------------------CLK down
	P3OUT &= ~SCL;

// --------- wait for ACK from TMP101 --------------------
	// SDA input
	P3DIR &= ~SDA;  //**
	// SCL low to high ------------------CLK up
	P3OUT |= SCL;   //**
	while(ACK == 1) //when ACK = 0 go forward
	{
		ACK = P3IN & SDA;
	}
	ACK = 1;
	// SCL high to low ------------------CLK down
	P3OUT &= ~SCL;  //**

// ----- start reading the temperature (first 8 bits)----
	for (i=0;i<8;i++)
	{
		// SCL low to high ------------------CLK up
		P3OUT |= SCL;   //**
		I2C_temperature = I2C_temperature << 1;
		if( P3IN & SDA)
		{
			I2C_temperature += 1;
		}
		// SCL high to low ------------------CLK down
		P3OUT &= ~SCL;  //**
	}

// ----- send ACK from master --------------------------
	// SCL low to high ------------------CLK up
	P3OUT |= SCL;   //**
	P3DIR |= SDA;                //SDA is output
	P3OUT &= ~SDA;  //**   //SDA = low
	// SCL high to low ------------------CLK down
	P3OUT &= ~SCL;  //**

// at this point I2C_temperature contains 8 MSB out of 9 bits

// ------- read the last 8 bits (only MSB is of interest) --------------------
	//I2C_temperature = 0;	// ignore last bit

	for (i=0;i<8;i++)
	{
		// SCL low to high ------------------CLK up
		P3OUT |= SCL;   //**
		//I2C_temperature = I2C_temperature << 1;	// ignore last bit
		if( P3IN & SDA)
		{
			//I2C_temperature += 1;				// ignore last bit
		}
		// SCL high to low ------------------CLK down
		P3OUT &= ~SCL;  //**
	}

// ----- send ACK from master --------------------------
	// SCL low to high ------------------CLK up
	P3OUT |= SCL;   //**
	P3DIR |= SDA;					//SDA is output
	P3OUT &= ~SDA;  //**   		//SDA = low
	// SCL high to low ------------------CLK down
	P3OUT &= ~SCL;  //**

	// add last bit
	//if(I2C_temperature & 0x80)        // MSB of second byte is LSB of final result
	//{
		//final_return_value += 0.5;
	//}

//----- STOP by master  ------------------------------
	// SCL low to high ------------------CLK up
	P3OUT |= SCL;   	//**
	P3OUT |= SDA;   	//**    //SDA = high
	P3DIR &= ~SDA;   	//**   //SDA = Input

	return I2C_temperature;

}


void Init(void)
{
  // Stop watchdog timer from timing out during initial start-up.
  //WDTCTL = WDTPW + WDTHOLD;

//	WDTCTL = WDT_MDLY_32;           // Set Watchdog Timer interval to ~2ms
	WDTCTL = WDT_MDLY_8;			// Set Watchdog Timer interval to ~0.5ms
	IE1 |= WDTIE;                   // Enable WDT interrupt
  //IE1 &= ~WDTIE;    				// Disable WDT interrupt


  //----------------- GPIO Init --------------------//

  // Port 1 Outputs
  P1DIR |= FET_PWM + SPEAKER + SEGMENT_c + SEGMENT_b;
  P1OUT &= ~(FET_PWM + SPEAKER + SEGMENT_c + SEGMENT_b);	// output = GND on these pins

  // Port 1 Inputs: ADC channels
  P1DIR &= ~(SUPPLY_DIAGNOSE + OUTPUT_DIAGNOSE + TIP_TEMP + CRT_SENSE);

  // Port 2 Functions
  P2SEL = 0x00;	// XIN and XOUT pins used as GPIO

  // Port 2 Outputs
  P2DIR |= COM1 + COM2 + COM3 + SEGMENT_e;
  P2OUT &= ~( COM3 + COM2 + SEGMENT_e );


  // Port 2 Inputs
  P2DIR  &= ~( ENCODER2 +  BUTTON + SPARE_IN_OUT_1 + SPARE_IN_OUT_2);

  // Port 2 Internal pull-up resistor
  // (P2OUT bit corresponding to PINx = 1 for pull-up and 0 for pull-down)
  P2REN = ENCODER2 +  BUTTON + SPARE_IN_OUT_1 + SPARE_IN_OUT_2;
  P2OUT |= ENCODER2 + BUTTON + SPARE_IN_OUT_1 + SPARE_IN_OUT_2;

  // Port 2 Interrupt Enable Register
  P2IE = BUTTON + ENCODER2;

  // Port 2 Interrupt Edge Select Register High to low transition
  P2IES = BUTTON + ENCODER2;

  // Port 2 Interrupt Flag Register = 0 (no interrupt pending)
  P2IFG = 0;


  // Port 3 Outputs
  P3DIR |= SCL + SDA + SEGMENT_f + SEGMENT_a + SEGMENT_d + SEGMENT_h + SEGMENT_g ;
  P3OUT &= ~( SEGMENT_f + SEGMENT_a + SEGMENT_d + SEGMENT_h + SEGMENT_g );  // output zero on these pins

  P3OUT |= SCL + SDA;

  // Port 3 Inputs
  P3DIR &= ~ENCODER1;

  // Port 3 Internal pull-up resistor
  // (P3OUT bit corresponding to PINx = 1 for pull-up and 0 for pull-down)
  P3REN = ENCODER1 ;
  P3OUT |= ENCODER1 ;

  // Port 3 does not have external interrupts !!!

  // Configure ADC10
  ADC10CTL0 = SREF_1 + ADC10SHT_3 + REF2_5V + REFON + ADC10ON;				// no interrupt
  ADC10AE0 |= SUPPLY_DIAGNOSE + OUTPUT_DIAGNOSE + TIP_TEMP + CRT_SENSE;		// Port ADC option select


  // initialize clock - 16MHz
  //16Mhz
  if (CALBC1_16MHZ==0xFF)	           // If calibration constants erased
  {
    while(1);                               // do not load, trap CPU!!
  }
  DCOCTL = 0;                               // Select lowest DCOx and MODx settings
  BCSCTL1 = CALBC1_16MHZ;                   // Set range
  DCOCTL = CALDCO_16MHZ;                    // Set DCO step + modulation

  FCTL2 = FWKEY + FSSEL0 + (FN5 + FN4 + FN3 + FN2 + FN1 + FN0);     // MCLK/64 for Flash Timing Generator

  // Enable interrupts
  __bis_SR_register(OSCOFF + GIE);      // low frequency oscillator off, Global Interrupt Enable

}

// Port 1 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
	unsigned long i;

	for (i=0;i<100;i++);			// debounce

	if((P2IN & BUTTON) == 0)	// BUTTON pressed
	{
		BUTTON_pressed = 1;
	}

	if((P2IN & ENCODER2) == 0)	// rotary encoder was turned
	{
		//for (i=0;i<100;i++);  // debounce

		if((P3IN & ENCODER1) == 0)	// rotary encoder was turned to the right
		{
			if (Rotary_tesit == 1)
			{
				rotary_right = 1;
			}
			else
			{
				rotary_left = 1;
			}
		}
		else
		{
			if (Rotary_tesit == 1)
			{
				rotary_left = 1;
				
			}
			else
			{
				rotary_right = 1;
			}			
			
		}
	}

	// prevent another interrupt for a while
	//P2IE &= ~(BUTTON + ENCODER2);

	P2IFG &= ~(BUTTON + ENCODER2);			// IFG cleared
}

// Watchdog Timer interrupt service routine
#pragma vector=WDT_VECTOR
__interrupt void watchdog_timer(void)
{
	//each 0.5 ms we get an interrupt

	// run for 100ms (200 interrupts)
	// wdt_pwm always a multiple of 2 => divide by 2 (shift right 1 unit) for doubling PWM frequency
	// run 4 cycles then read solder tip temperature (3ms off)

	wdt_pwm_position++;

	if(cycle_nr < 4)
	{
		if(wdt_pwm_position > 49)	// PWM off time
		{
			wdt_pwm_position = 0;

			cycle_nr++;
			if(cycle_nr == 4)
			{
				if (station_mode == 0)		// we need to read tip temperature
				{
					P1OUT &= ~FET_PWM;   //turn OFF soldering tip
				}
				else
				{
					// start over with the PWM
					cycle_nr = 0;

					// enable main application
					new_100ms = 1;
				}
			}
		}
		else
		{
			if(wdt_pwm_position > wdt_pwm>>1)	// PWM off time
			{
				P1OUT &= ~FET_PWM;   //turn OFF soldering tip
			}
			else
			{
				if(wdt_pwm > 0)	// PWM on time
				{
					P1OUT |= FET_PWM;   //turn ON soldering tip
				}
			}
		}
	}
	else
	{
		if (wdt_pwm_position == 3)		// stay off (2ms) until tip temperature feedback stabilizes
		{
			//enable tip temperature read
			new_100ms = 1;
		}
		else
		{
			if (wdt_pwm_position >= 5)		// stay off another 1ms until tip temperature is sampled
			{
				//disable tip temperature read just to make sure tip temperature is not sampled after this moment on
				new_100ms = 0;

				wdt_pwm_position = 0;
				cycle_nr = 0;

				if(wdt_pwm > 0)	// PWM on time
				{
					P1OUT |= FET_PWM;   //turn ON soldering tip
				}

			}
		}
	}
}

// ----------- Store settings to flash info memory (to segment C) --------//
void Write_to_Flash ( unsigned char data1, unsigned char data2, unsigned char data3 )
{
	char *Flash_ptr;                          // Flash pointer

	Flash_ptr = (char *) 0x1040;              // Initialize Flash pointer
	FCTL1 = FWKEY + ERASE;                    // Set Erase bit
	FCTL3 = FWKEY;                            // Clear Lock bit
	*Flash_ptr = 0;                           // Dummy write to erase Flash segment

	FCTL1 = FWKEY + WRT;                      // Set WRT bit for write operation

	if(data3 == 1)							// first nibble (MSB) corresponds to buzzer, second nibble corresponds to PWM index
	{
		// Buzzer on if data2 > 15
		data2 += 0x10;
	}

	*Flash_ptr++ = data1;
	*Flash_ptr = data2;

	FCTL1 = FWKEY;                            // Clear WRT bit
	FCTL3 = FWKEY + LOCK;                     // Set LOCK bit

}

// ----------- read settings from flash info memory (segment C and D) -------//
void Read_from_Flash (void)
{
	char *Flash_ptrC;				// Segment C pointer

	Flash_ptrC = (char *) 0x1040;	// Initialize Flash segment C pointer

	flash_temp_byte = *Flash_ptrC++;	// value stored in Flash_ptrC

	if (flash_temp_byte != 0xFF)	// if memory not blank
	{
		// 500 degrees Celsius are stored as 250 => multiply by 2 the flash stored value
		desired_temperature = flash_temp_byte << 1;
	}

	flash_temp_byte = *Flash_ptrC;	// value stored in Flash_ptrC
	if (flash_temp_byte != 0xFF)	// if memory not blank
	{
		if (flash_temp_byte > 15)
		{
			// Buzzer ON
			buzzer_function_active = 1;
			LED_PWM_index = flash_temp_byte - 0x10;
		}
		else
		{
			// Buzzer OFF
			buzzer_function_active = 0;
			LED_PWM_index = flash_temp_byte;
		}

		if (LED_PWM_index > 10)
		{
			LED_PWM_index = LED_INITIAL_BRIGHTNESS;
		}
	}
}


#pragma vector=PORT1_VECTOR,ADC10_VECTOR,USCIAB0TX_VECTOR,USCIAB0RX_VECTOR,TIMER0_A0_VECTOR,COMPARATORA_VECTOR,TIMER1_A1_VECTOR,TIMER1_A0_VECTOR,NMI_VECTOR
__interrupt void TrapIsr(void)
{
	// this line should never be reached
	//_NOP();
}


