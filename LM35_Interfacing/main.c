/*(C) Umang Gajera- www.ocfreaks.com
Interfacing LM35 Temperature Sensor with LPC2148 - Example Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-lm35-sensor-with-lpc2148/
Also see: http://www.ocfreaks.com/lpc2148-adc-programming-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h>		 //visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

#define AD06 ((1<<9)|(1<<8)) //Select AD0.6 function for P0.4
#define SEL_AD06 (1<<6) //Select AD0.6 Channel
#define CLKDIV (15-1) //4Mhz ADC clock (ADC_CLOCK=PCLK/CLKDIV) where "CLKDIV-1" is actually used, in our case PCLK=60mhz  
#define BURST_MODE_OFF (0<<16) //1 for on and 0 for off
#define PowerUP (1<<21)
#define START_NOW ((0<<26)|(0<<25)|(1<<24)) //001 for starting the conversion immediately
#define ADC_DONE (1UL<<31)

#define VREF 3.3 //Reference Voltage at VREF pin

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer and ADC
	initUART0();  //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions
	
	PINSEL0 |= AD06; //select AD0.6 for P0.4
	unsigned long AD0CR_setup = (CLKDIV<<8) | BURST_MODE_OFF | PowerUP; 
	int result = 0;
	float temp = 0;
	
	printf("OCFreaks.com LPC214x LM35 Interfacing Tutorial\n");
	
	AD0CR =  AD0CR_setup | SEL_AD06; //Setup ADC block
	
	AD0CR |= START_NOW; //Start new Conversion
	while( (AD0DR6 & ADC_DONE) == 0 );
	//Ignore the first ADC reading.
	
	while(1)
	{
		AD0CR |= START_NOW; //Start new Conversion
		while( (AD0DR6 & ADC_DONE) == 0 );
		
		result = (AD0DR6>>6) & 0x3FF; //get the 10bit ADC result
		
		temp = ((float)result * VREF * 100)/1023; //As per the Equation given in the tutorial

		printf("Temp = %0.1f Deg. Celsius\n" , temp);
		
		delayMS(1000); //1 Update per second
	}
	
	//return 0; //This won't execute normally
}
