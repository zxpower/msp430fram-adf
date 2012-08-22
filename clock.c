#include "main.h"
#include "clock.h"

void clock_setup(void) {
	
	#if XTAL_32KHZ
		// Set 32kHz crystal ON
		PJSEL0 |= BIT4+BIT5;
	#endif
	
	CSCTL0_H = 0xA5;			    // Send clock password
	CSCTL1 |= DCOFSEL0 + DCOFSEL1;            // Set max. DCO
	CSCTL2 = SELA_0 + SELS_3 + SELM_3;        // set ACLK = XT1; SMCLK = MCLK = DCO
	CSCTL3 = DIVA_0 + DIVS_3 + DIVM_3;        // set all dividers 
	CSCTL4 |= XT1DRIVE_0; 
	CSCTL4 &= ~XT1OFF;

	do
	{
		CSCTL5 &= ~XT1OFFG;                     // Clear XT1 fault flag
		SFRIFG1 &= ~OFIFG; 
	} while (SFRIFG1&OFIFG);                  // Test oscillator fault flag
}
