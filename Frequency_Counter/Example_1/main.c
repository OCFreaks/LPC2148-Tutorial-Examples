/*(C) Umang Gajera- www.ocfreaks.com
ARM7 LPC2148 Input Capture Tutorial - Example 1 for frequency counter using ARM KEIL
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h> //visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

void initPWM(void); //To generate Test Square wave
unsigned int pulses = 0;
#define GATE_TIME_MS 1000 //Probing/Gating Time in ms

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer & PWM
	initUART0();  //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions
	
	//Assuming PCLK = 60Mhz
	PINSEL0 |= (1<<21); //Select CAP1.0 for P0.10
	T1CTCR = 0x1; //Increment TC on rising edges of External Signal for CAP1.0
	T1PR = 0; //0.01667us res, 1 clock cycles @60Mhz = 0.01667 us
	T1TCR = 0x02; //Reset Timer
	T1CCR = (1<<0); //Capture on Rising Edge(0->1)
	T1TCR = 0x01; //Enable timer1

	initPWM(); //Generate 5Mhz square wave
	float FreqKhz = 0;
	printf("OCFreaks.com - Measuring Frequency using LCP2148 Timer Capture Ex 1:\n");
	
	while(1)
	{
		
	  T1TCR = 0x1; //Start Timer2
		delayMS(GATE_TIME_MS); //'Gate' signal for defined Time (1 second)
		T1TCR = 0x0; //Stop Timer2
		
		pulses = T1TC; //Read current value in TC, which contains  no.of. pulses counted in 1s
		T1TCR = 0x2; //Reset Timer2 TC
		
		FreqKhz = (double)pulses/GATE_TIME_MS;
		
		if(FreqKhz >= 1000.0) //Display Freq. In MHz
		{
			printf("Frequency = %0.4f MHz\n", FreqKhz/1000.0);
		}
		else //Display Freq. in KHz
		{
			printf("Frequency = %0.2f KHz\n", FreqKhz);
		}
	}

	//return 0; //This won't execute normally
}

void initPWM(void)
{
	//Refer : http://www.ocfreaks.com/lpc2148-pwm-programming-tutorial/
	/*Assuming that PLL0 has been setup with CCLK = 60Mhz and PCLK also = 60Mhz.*/
	/*This is a per the Setup & Init Sequence given in the tutorial*/

	PINSEL0 |= (1<<15); // Select PWM2 output for Pin0.7
	PWMPCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line can be removed
	PWMPR = 3-1; //3 Clock cycles @60Mhz = 0.05us resoultion
	PWMMR0 = 4; //4x0.05 = 0.2us - period duration i.e. 5MHz test frequency
	PWMMR2 = 2; //2x0.05 = 0.1us - pulse duration i.e. width
	PWMMCR = (1<<1); // Reset PWMTC on PWMMR0 match
	PWMLER = (1<<2) | (1<<0); // update MR0 and MR2
	PWMPCR = (1<<10); // enable PWM2 output
	PWMTCR = (1<<1) ; //Reset PWM TC & PR

	//Now, the final moment - enable everything
	PWMTCR = (1<<0) | (1<<3); // enable counters and PWM Mode
	//PWM Generation goes active now!!
}

