#ifndef OCF_UART_HEADER
#define OCF_UART_HEADER

/* (C) Umang Gajera. www.ocfreaks.com
Taken from OCFreaks LPC2148 UART Tutorial
@ http://www.ocfreaks.com/lpc2148-uart-programming-tutorial/
License: GPL.*/

#define THRE (1<<5) // Transmit Holding Register Empty
#define RDR (1<<0) // Receiver Data Ready
#define MULVAL 15
#define DIVADDVAL 1
#define LINE_FEED   0x0A //LF, For Linux, MAC and Windows Terminals  
#define CARRIAGE_RETURN 0x0D //CR, For Windows Terminals (CR+LF).

void initUART0(void);
void uartWrite(char c);
char uartRead(void);

#endif
