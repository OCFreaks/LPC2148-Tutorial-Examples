/*(C) Umang Gajera- www.ocfreaks.com
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
LPC2148 IR (Module) Interfacing Example 2 Source Code for KEIL ARM.
http://www.ocfreaks.com/interfacing-ir-sensor-arm7-lpc2148/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h>		 //visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header
#define PIN0_2 (1<<2)

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer
	initUART0(); //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions

	printf("OCFreaks.com LPC214x IR Sensor Interfacing Tutorial - Example 2.\n");
	
	while(1)
	{
		//Check the O/P Logic of your IR module. Mine gives LOW when obstacle is detected else HIGH
		if((IO0PIN & PIN0_2) == 0 ) 
		{
			printf("Obstacle Detected!\n");
		}
		else
		{
			printf("No Obstacle ahead.\n");
		}

		delayMS(500); //Slowing down Updates to 2 Updates per second
	}
	
	//return 0; //This won't execute normally
}
