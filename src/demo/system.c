
#include <avr/io.h>
//#include <avr/iom128.h>
#include "system.h"
#include "bits.h"
#include "delay.h"

#define		BUZZ1_HIGH		PORTE |= BIT4 
#define		BUZZ1_LOW		PORTE &= ~BIT4 

#define		BUZZ2_HIGH		PORTE |= BIT5 
#define		BUZZ2_LOW		PORTE &= ~BIT5 


//--------Init Ports --------------------
void InitPorts(void) {

	// Input/Output Ports initialization
	// Port A initialization
	PORTA=0x00;
	DDRA=0x40;

	// Port B initialization
	PORTB=0x00;
	DDRB=0x00;

	// Port C initialization
	PORTC=0x00;
	DDRC=0xF7;

	// Port D initialization
	PORTD=0x00;
	DDRD=0x08;

	// Port E initialization
	PORTE=0x00;
	DDRE=0x30;

	// Port F initialization
	PORTF=0x00;
	DDRF=0x00;

	// Port G initialization
	PORTG=0x00;
	DDRG=0x00;

}

//--------Init Timers --------------------
void InitTimers(void) {

	// Timer/Counter 0 initialization
	// Clock source: System Clock
	// Clock value: Timer 0 Stopped
	// Mode: Normal top=FFh
	// OC0 output: Disconnected
	///ASSR=0x00;
	///TCCR0=0x00;
	///TCNT0=0x00;
	///OCR0=0x00;

	// Timer/Counter 1 initialization
	// Clock source: T1 pin Rising Edge
	// Mode: Normal top=FFFFh
	// OC1A output: Discon.
	// OC1B output: Discon.
	// OC1C output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	TCCR1A=0x00;
	TCCR1B=0x07;
	TCNT1H=0x00;
	TCNT1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;
	OCR1CH=0x00;
	OCR1CL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: T2 pin Rising Edge
	// Mode: Normal top=FFh
	// OC2 output: Disconnected
	TCCR2=0x07;
	TCNT2=0x00;
	OCR2=0x00;

	// Timer/Counter 3 initialization
	// Clock source: System Clock
	// Clock value: 2000,000 kHz
	// Mode: Normal top=FFFFh
	// OC3A output: Discon.
	// OC3B output: Toggle
	// OC3C output: Toggle
	//TCCR3A=0x14;
	//TCCR3B=0x02;
	//TCNT3H=0x00;
	//TCNT3L=0x00;
	//OCR3AH=0x00;
	//OCR3AL=0x00;
	//OCR3BH=0x00;
	//OCR3BL=0xFA;
	//OCR3CH=0x00;
	//OCR3CL=0xFA;

	// External Interrupt(s) initialization
	// INT0: Off
	// INT1: Off
	// INT2: Off
	// INT3: Off
	// INT4: Off
	// INT5: Off
	// INT6: Off
	// INT7: Off
	///EICRA=0x00;
	///EICRB=0x00;
	///EIMSK=0x00;

	// Timer(s)/Counter(s) Interrupt(s) initialization
	///TIMSK=0x00;
	///ETIMSK=0x00;

} 

//--------Init Uart interface --------------------
void InitUart1(void) {

	// USART1 initialization
	// Communication Parameters: 8 Data, 1 Stop, No Parity
	// USART1 Receiver: On
	// USART1 Transmitter: On
	// USART1 Mode: Asynchronous
	// USART1 Baud rate: 9600
	UCSR1A=0x00;
	UCSR1B=0x18;     		//(00011000)
	UCSR1C=0x06;
	UBRR1H=0x00;
	UBRR1L=0x67;			//103

}


void SendCharUart1(unsigned char ch) {

	// wait for data to be received
	while(!(UCSR1A & (1<<UDRE1)));
	// send data
	UDR1 = ch; 
	
}

unsigned char ReceiveCharUart1(void) {
	
	// wait for data to be received
	while(!(UCSR1A & (1<<RXC1)));
	// get and return received data from buffer
	return UDR1; 

}

unsigned char ReceiveCharUart1_nonstop(void) {
	
	// wait for data to be received
	if((UCSR1A & (1<<RXC1)))
		// get and return received data from buffer
		return UDR1; 
	else
		// return 0
		return 0;

}

//---------Buzzer --------------------------------- 
void Buzzer(void) {
  
	
	BUZZ1_LOW;			
	BUZZ2_HIGH;
	delay_us(125);
    BUZZ2_LOW;
	BUZZ1_HIGH;
	delay_us(125);

}   


