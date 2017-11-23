/*(C) Umang Gajera- www.ocfreaks.com
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
LPC2148 LDR Interfacing Example 2 Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-ldr-with-lpc2148/
Also see: http://www.ocfreaks.com/lpc2148-adc-programming-tutorial/
License: GPL.*/

#include <lpc214x.h>
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

#define AD06 ((1<<9)|(1<<8)) //Select AD0.6 function for P0.4
#define SEL_AD06 (1<<6)
#define CLKDIV (15-1) // 4Mhz ADC clock (ADC_CLOCK=PCLK/CLKDIV) where "CLKDIV-1" is actually used , in our case PCLK=60mhz  
#define BURST_MODE_OFF (0<<16) // 1 for on and 0 for off
#define PowerUP (1<<21)
#define START_NOW ((0<<26)|(0<<25)|(1<<24)) //001 for starting the conversion immediately
#define ADC_DONE (1UL<<31)
#define VREF 3.3 //Reference Voltage at VREF pin

#define CUT_OFF 200 //Define your cut-off value here

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz
	initTimer0(); //Init Timer for delay functions
	
	PINSEL0 |= AD06 ; //select AD0.6 for P0.4
	IO0DIR |= (1<<5); //Select P0.5 as output 
	int result = 0;
	unsigned long AD0CR_setup = (CLKDIV<<8) | BURST_MODE_OFF | PowerUP; //Setup ADC block
	
	while(1)
	{
		AD0CR =  AD0CR_setup | SEL_AD06; 
		AD0CR |= START_NOW; //Start new Conversion

		while( (AD0DR6 & ADC_DONE) == 0 );
		
		result = (AD0DR6>>6) & 0x3FF; //get the 10bit ADC result
		
		if(result < CUT_OFF)
		{
			IO0SET = (1<<5); //LED ON
		}
		else
		{
			IO0CLR = (1<<5); //LED OFF
		}
		
		delayMS(100); //wait some time since LDRs don't react immediately.
	}
	//return 0; //This won't execute normally
}
