#ifndef _OCF_LIB_FUNCS_H_
#define _OCF_LIB_FUNCS_H_

void initTimer0(void);
void delayUS(unsigned int microseconds);
void startTimer0(void);
unsigned int stopTimer0(void);
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
