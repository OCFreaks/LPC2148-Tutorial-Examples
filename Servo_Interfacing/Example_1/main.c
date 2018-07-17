/*(C) Umang Gajera - www.ocfreaks.com
LPC2148 Servo Interfacing Tutorial - Example 1
http://www.ocfreaks.com/lpc2148-servo-motor-interfacing-tutorial/
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
	PWMMR1 = 1500; //Default Pulse time. Brings servo in neutral position
	PWMLER = (1<<0) | (1<<1); //Enable Match 1 & Match 2 Latch to update new values
	PWMPCR = (1<<9); //Enable PWM1 Output
	PWMTCR = (1<<1); //Reset PWM Counter
	PWMTCR = (1<<0) | (1<<3); //IMP! - Enable Counter and PWM

	delayMS(2000); //Inital delay
	
	while(1)
	{
		PWMMR1 = 1000; //1ms Pulse
		PWMLER |= (1<<1); //Set MR1 Latch
		delayMS(2000); //2 Secs Delay
		
		PWMMR1 = 1500; //1.5ms Pulse
		PWMLER |= (1<<1); //Set MR1 Latch
		delayMS(2000); //2 Secs Delay
		
		PWMMR1 = 2000; //2ms Pulse
		PWMLER |= (1<<1); //Set MR1 Latch
		delayMS(2000); //2 Secs Delay
		
		PWMMR1 = 1500; //1.5ms Pulse
		PWMLER |= (1<<1); //Set MR1 Latch
		delayMS(2000); //2 Secs Delay	
	}
	
	//return 0; //This won't execute normally
}
