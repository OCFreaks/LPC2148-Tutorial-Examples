/*(C) Umang Gajera - www.ocfreaks.com
LPC2148 Servo Interfacing Tutorial - Example 2
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc214x.h>
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz
	initTimer0(); //For delay functions

	PINSEL0 |=  (1<<1); //set Bits [1:0] = 10 to select PWM1 for P0.0

	PWMPR = 59; //PWMTC increments every 59+1 Clock cycles @ 60mhz to yield a resolution of 1us
	PWMMCR = 0x1; //Reset PWMTC on PWMMR0 Match which defines the PWM period
	PWMMR0 = 20000; //PWM period of 20ms
	PWMMR1 = 1000; //Default(starting) Pulse time for sweep.
	PWMLER = (1<<0) | (1<<1); //Enable Match 1 & Match 2 Latch to update new values
	PWMPCR = (1<<9); //Enable PWM1 Output
	PWMTCR = (1<<1); //Reset PWM Counter
	PWMTCR = (1<<0) | (1<<3); //IMP! - Enable Counter and PWM

	delayMS(1000); //Initial delay
	
	int step = 20; //In us
	int stepDelay = 20; //In ms 
	int pulse=1000;
	while(1)
	{
		while(pulse<2000)
		{
			PWMMR1 = pulse;
			PWMLER |= (1<<1);
			delayMS(stepDelay);
			pulse = pulse + step;
		}
		while(pulse>1000)
		{
			PWMMR1 = pulse;
			PWMLER |= (1<<1);
			delayMS(stepDelay);
			pulse = pulse - step;
		}
	}
	
	//return 0; //This won't execute normally
}
