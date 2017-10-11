/* (C) Umang Gajera. www.ocfreaks.com
Example: Retarget printf() & scanf() to UART on LPC2148
UART code from OCFreaks LPC2148 UART Tutorial
@ http://www.ocfreaks.com/lpc2148-uart-programming-tutorial/
License: GPL.*/

#include <lpc214x.h>
#include <stdio.h> //Obviously!
#include "uart.h"  //for initUART0() and other macros

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

int main(void)
{
	/*CCLK is set to 60Mhz by Startup code*/
	int num=0;
	VPBDIV = 0x01; // set PCLK same as CCLK i.e 60Mhz for UART
	initUART0();
	
	printf("Hey! This is your beloved printf().\nI have been redirected here with scanf()!\n");
	
	while(1)
	{
		printf("Enter an Integer between 0 and 1000:");
		scanf("%d",&num);
		if(num>=0 && num<= 1000) printf("\nYou entered: %d\n",num);
		else printf("\nInvalid input!\n");
	}	
	
	//return 0; //This won't execute.
}

