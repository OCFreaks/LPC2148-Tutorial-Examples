/*(C) Umang Gajera- www.ocfreaks.com
ARM7 LPC2148 Input Capture Tutorial - Example 2 for frequency counter using ARM KEIL
http://www.ocfreaks.com/lpc2148-timer-capture-mode-tutorial-frequency-counter-example/
More Embedded tutorials @ www.ocfreaks.com/cat/embedded/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h> //visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ 
#include "lib_funcs.h" //OCFreaks LPC214x Tutorials Library Header

__irq void timer1ISR(void);
long regVal; //Used by timer1ISR()
void initPWM(void);
unsigned int period = 0;
unsigned int previous = 0;
unsigned int current = 0 ;

int main(void)
{
	initClocks(); //Set PCLK = CCLK = 60Mhz - used by: UART, Timer & PWM
	initUART0();  //Initialize UART0 for retargeted printf()
	initTimer0(); //Init Timer for delay functions
	
	//Assuming PCLK = 60Mhz
	PINSEL0 |= (1<<21); //Select CAP1.0 for P0.10
	T1CTCR = 0x0; //Run in Timer Mode
	T1PR = 3-1; //3 Clock cycles @60Mhz = 0.05us resolution
	T1TCR = 0x02; //Reset Timer
	T1CCR = (1<<0) | (1<<2); //Capture on Rising Edge(0->1) and generate an interrupt
	T1TCR = 0x01; //Enable timer1
	
	VICIntEnable |= (1<<5) ; //Enable TIMER1 IRQ
	VICVectCntl0 = (1<<5) | 5; //5th bit must 1 to enable the slot. Refer: http://www.ocfreaks.com/lpc2148-interrupt-tutorial/
	VICVectAddr0 = (unsigned) timer1ISR;

	initPWM(); //Generate Test square wave
	
	printf("OCFreaks.com - Measuring Frequency using Timer Capture\n");
	double frequencyMhz = 0;
	while(1)
	{
		frequencyMhz = (1.0 / (period*0.05) ) * 1000; //0.05 is Timer resolution in us
		printf("Frequency = %0.2f Khz\n",frequencyMhz);
		delayMS(500); //2 Udpates per second
	}
	
	//return 0; //This won't execute normally
}

__irq void timer1ISR(void)
{
	current = T1CR0;

	if(current < previous) //TC has overflowed
	{
		period = 0xFFFFFFFF + current - previous;
	}
	else
	{
		period = current - previous;
	}
	
	previous = T1CR0;
	
	T1IR = (1<<4); //write back to clear the interrupt flag
	VICVectAddr = 0x0; //Acknowledge that ISR has finished execution
}

void initPWM(void)
{
	//Refer : http://www.ocfreaks.com/lpc2148-pwm-programming-tutorial/
	/*Assuming that PLL0 has been setup with CCLK = 60Mhz and PCLK also = 60Mhz.*/
	/*This is as per the Setup & Init Sequence given in the tutorial*/

	PINSEL0 |= (1<<15); // Select PWM2 output for Pin0.7
	PWMPCR = 0x0; //Select Single Edge PWM - by default its single Edged so this line can be removed
	PWMPR = 3-1; //3 Clock cycles @60Mhz = 0.05us
	PWMMR0 = 20; //20x0.05 = 1us - period duration i.e. 1MHz frequency
	PWMMR2 = 10; //10x0.05 = 0.5us - pulse duration i.e. width
	PWMMCR = (1<<1); // Reset PWMTC on PWMMR0 match
	PWMLER = (1<<2) | (1<<0); // update MR0 and MR2
	PWMPCR = (1<<10); // enable PWM2 output
	PWMTCR = (1<<1) ; //Reset PWM TC & PR

	//Now, the final moment - enable everything
	PWMTCR = (1<<0) | (1<<3); // enable counters and PWM Mode
	//PWM Generation goes active now!!
}

