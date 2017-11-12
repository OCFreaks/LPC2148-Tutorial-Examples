#include <lpc214x.h>
#include <stdio.h>
#include "lib_funcs.h"

namespace std {
//Retarget printf to divert output to UART0
//visit http://www.ocfreaks.com/retarget-redirect-printf-scanf-uart-keil/ for complete printf retargetting tutorial
	struct __FILE
	{
		int dummyVar; //Just for the sake of redefining __FILE, we won't we using it anyways ;)
	};

	FILE __stdout; //STDOUT
	FILE __stdin;  //STDIN

	int fputc(int c, FILE * stream)
	{
		uartWrite(c); //Transmit Character
		return c; //return the character written to denote a successful write
	}

	int fgetc(FILE * stream)
	{
		char c = uartRead(); //Receive Character
		uartWrite(c); //To echo Received characters back to serial Terminal
		return c;
	}
}

void uartWrite(char c)
{
	while (!(U0LSR & THRE)); //wait till the THR is empty
	if( c == '\n' ) //Send <CR+LF>
	{
		U0THR = CARRIAGE_RETURN;
		while(!(U0LSR & THRE)); 
		U0THR = LINE_FEED;
	}
	else
	{
		U0THR = c; 
	}
}

char uartRead(void)
{
	while(!(U0LSR & RDR)); // wait till any data arrives
	return U0RBR;
}

void initTimer0(void)
{
	/*Assuming that PLL0 has been setup with CCLK = 60Mhz*/
	VPBDIV = 0x01; //Set PCLK = CCLK = 60Mhz
	T0CTCR = 0x0;
	T0PR = 60-1; //60 clock cycles @60Mhz = 1 us
	T0TCR = 0x02; //Reset Timer
}

void delayUS(unsigned int microseconds) //Using Timer0
{
	T0TCR = 0x02; //Reset Timer
	T0TCR = 0x01; //Enable timer
	while(T0TC < microseconds); //wait until timer counter reaches the desired delay
	T0TCR = 0x00; //Disable timer
}

void startTimer0(void)
{
	T0TCR = 0x02; //Reset Timer
	T0TCR = 0x01; //Enable timer
}

unsigned int stopTimer0(void)
{
	T0TCR = 0x00; //Disable timer
	return T0TC;
}

void initUART0(void)
{
	PINSEL0 = 0x5;  /* Select TxD for P0.0 and RxD for P0.1 */
	U0LCR = 3 | (1<<7) ; /* 8 bits, no Parity, 1 Stop bit | DLAB set to 1  */
	U0DLL = 110;
	U0DLM = 1;   
	U0FDR = (MULVAL<<4) | DIVADDVAL; /* MULVAL=15(bits - 7:4) , DIVADDVAL=0(bits - 3:0)  */
	U0LCR &= 0x0F; // Set DLAB=0 to lock MULVAL and DIVADDVAL
	//BaudRate is now ~9600 and we are ready for UART communication!
}
