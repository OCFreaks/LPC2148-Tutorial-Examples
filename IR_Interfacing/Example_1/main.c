/*(C) Umang Gajera- www.ocfreaks.com
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
LPC2148 IR Sensor Interfacing Example 1 Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-ir-sensor-arm7-lpc2148/
Also see: http://www.ocfreaks.com/lpc2148-adc-programming-tutorial/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h>		 //visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

#define AD06 ((1<<9)|(1<<8)) //Select AD0.6 function for P0.4
#define SEL_AD06 (1<<6)
#define CLKDIV (15-1) // 4Mhz ADC clock (ADC_CLOCK=PCLK/CLKDIV) where "CLKDIV-1" is actually used , in our case PCLK=60mhz  
#define BURST_MODE_OFF (0<<16) // 1 for on and 0 for off
#define PowerUP (1<<21)
#define START_NOW ((0<<26)|(0<<25)|(1<<24)) //001 for starting the conversion immediately
#define ADC_DONE (1UL<<31)

#define VREF 3.3 //Reference Voltage at VREF pin

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer and ADC
	initUART0(); //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions
	
	PINSEL0 |= AD06 ; //select AD0.6 for P0.4
	int result = 0;
	unsigned long AD0CR_setup = (CLKDIV<<8) | BURST_MODE_OFF | PowerUP; //Setup ADC block
	
	printf("OCFreaks.com LPC214x IR Sensor Interfacing Tutorial - Example 1.\n");
	
	while(1)
	{
		AD0CR =  AD0CR_setup | SEL_AD06; 
		AD0CR |= START_NOW; //Start new Conversion

		while( (AD0DR6 & ADC_DONE) == 0 );
		
		result = (AD0DR6>>6) & 0x3FF; //get the 10bit ADC result

		printf("AD0.6 = %d\n" , result); //Print raw ADC values corresponding to IR photo-diode reverse current 
		delayMS(500); //Slowing down Updates to 2 Updates per second
	}
	
	//return 0; //This won't execute normally
}
