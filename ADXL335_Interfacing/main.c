/*(C) Umang Gajera- www.ocfreaks.com
Interfacing ADXL335 Accelerometer sensor with LPC2148 - Example Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-adxl335-accelerometer-sensor-lpc2148/
Also see: http://www.ocfreaks.com/lpc2148-adc-programming-tutorial/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h>		 //visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

#define SEL_AD01 (1<<1) //Select AD0.1 Channel
#define SEL_AD02 (1<<2) //Select AD0.2 Channel
#define SEL_AD03 (1<<3) //Select AD0.3 Channel
#define CLKDIV   (15-1) //4Mhz ADC clock (ADC_CLOCK=PCLK/CLKDIV) where "CLKDIV-1" is actually used, in our case PCLK=60mhz  
#define BURST_MODE_OFF (0<<16) //1 for on and 0 for off
#define PowerUP   (1<<21)
#define START_NOW ((0<<26)|(0<<25)|(1<<24)) //001 for starting the conversion immediately
#define ADC_DONE  (1UL<<31)

#define VREF 3.3 //Reference Voltage at VREF pin
#define VS 	 3.3 //Supply Voltage for ADXL335
#define ZERO_G (VS/2) //Zero g bias

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer and ADC
	initUART0();  //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions
	
	PINSEL1 |= (1<<24) | (1<<26) | (1<<28); //select AD0.1/2/3 for P0.28/29/30.
	int adcX,adcY,adcZ;
	float resultVolts,Xg,Yg,Zg;
	unsigned long ADC0CR_Setup = (CLKDIV<<8) | BURST_MODE_OFF | PowerUP;
	printf("OCFreaks.com LPC214x ADXL335 Sensor Interfacing Tutorial\n");
	
	int skipFirst = 1;
	
	while(1)
	{
		AD0CR = ADC0CR_Setup | SEL_AD01 | START_NOW;//Start new Conversion on AD0.1
		while( (AD0DR1 & ADC_DONE) == 0 );
		adcX = (AD0DR1>>6) & 0x3FF; //get the 10bit ADC result
		
		AD0CR = ADC0CR_Setup | SEL_AD02 | START_NOW; //Start new Conversion on AD0.2
		while( (AD0DR2 & ADC_DONE) == 0 );
		adcY = (AD0DR2>>6) & 0x3FF; //get the 10bit ADC result
		
		AD0CR = ADC0CR_Setup | SEL_AD03 | START_NOW; //Start new Conversion on AD0.3
		while( (AD0DR3 & ADC_DONE) == 0 );
		adcZ = (AD0DR3>>6) & 0x3FF; //get the 10bit ADC result
		
		if(skipFirst) //Ignore first ADC readings.
		{
			skipFirst = 0; 
			continue;
		}
		
		//Computing in 2 steps to keep things simple.
		resultVolts = ((float)adcX * VREF) / 1024; //ADC Result converted to millivolts
		Xg = (resultVolts - ZERO_G) / (VS*0.1);
		
		resultVolts = ((float)adcY * VREF) / 1024; //ADC Result converted to millivolts
		Yg = (resultVolts - ZERO_G) / (VS*0.1);
		
		resultVolts = ((float)adcZ * VREF) / 1024; //ADC Result converted to millivolts
		Zg = (resultVolts - ZERO_G) / (VS*0.1);
		
		printf("X=%0.2fg Y=%0.2fg Z=%0.2fg\n", Xg,Yg,Zg);
		
		delayMS(250); //4 Updates per second
	}
	
	//return 0; //This won't execute normally
}
