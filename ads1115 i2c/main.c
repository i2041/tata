#include <msp430.h>
int TXByteCtr = 1;
unsigned char PRxData;                     // Pointer to RX data
unsigned char PRxData2;                     // Pointer to RX data
int Rx = 0;
unsigned char WHO_AM_I = 0x00;
int TXByteCtr;
int configRegister=0;
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    DCOCTL = CALDCO_16MHZ;     				// Set DCO step and modulation
	BCSCTL1 = CALBC1_16MHZ;     			// Set range
	BCSCTL2 = DIVS_1;
	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	void init_I2C(void);
	void Transmit(void);
	void Receive(void);
	init_I2C();
	Rx = 0;
	PRxData=0;
	TXByteCtr=3;
	configRegister=1;
	Transmit();
	while(1)
	{
		if (!configRegister)
		{
		//Transmit process
		Rx = 0;
		TXByteCtr=1;
		Transmit();
		while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent

		//Transmit();
		//Receive process
		Rx = 1;
		Receive();
		while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
		}
		__delay_cycles(1000000);
	}
	return 0;
}
