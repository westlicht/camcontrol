#ifndef __SYSTEM
#define __SYSTEM

//--------Init Ports --------------------
void InitPorts(void);

//--------Init Timers --------------------
void InitTimers(void);

//--------Init Uart interface --------------------
void InitUart1(void);
void SendCharUart1(unsigned char ch);
unsigned char ReceiveCharUart1(void);
unsigned char ReceiveCharUart1_nonstop(void);

//---------Buzzer --------------------------------- 
void Buzzer(void);


#endif


