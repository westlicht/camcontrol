/*********************************************
Project : ATmega128_MT
Version : 01
Date    : 14.1.2007
Author  : Slavcho TOmov                 
Company : Olimex                            
Comments: Demo program


Chip type           : ATmega128
Program type        : Application
Clock frequency     : 16,000000 MHz
Memory model        : Small
External SRAM size  : 0
Data Stack size     : 1024
*********************************************/
#include <avr/io.h>
// #include <avr/iom128.h>
#include "system.h"
#include "bits.h"
#include "lcd.h"
#include "delay.h"
                                                        

#define		B1				(PINA&BIT0)  
#define		B2				(PINA&BIT1)
#define		B3				(PINA&BIT2)
#define		B4				(PINA&BIT3)
#define		B5				(PINA&BIT4)


#define 	RELAY_HIGH		PORTA |= BIT6
#define 	RELAY_LOW		PORTA &= ~BIT6	                      

#define		TXD				(PIND&BIT3)	
#define		RXD				(PIND&BIT2)	
#define		DALLAS			(PINA&BIT5)
     

unsigned char ch;



int main(void) {     

	//Ports 
	InitPorts();


	//Init Uart1 
	InitUart1();
	               	 
	//LCD initialisation 
	LCD_Ini(); 					     
	// delay_ms(2);  		
	SEND_CMD(DISP_ON);
	// delay_ms(10);  						
	SEND_CMD(CLR_DISP); 
	// write
	SEND_STR(" www.olimex.com");


	while (1) {  

		// Uart Echo
		ch = ReceiveCharUart1_nonstop();
		if(ch!=0) {
	
			SendCharUart1(ch);
			SendCharUart1('*');
			ch = 0;
		}


		//Buttons scan                
   
		// Button 1
		if (B1==0) {
			RELAY_HIGH;        
		}
	    else { 
			RELAY_LOW;	      
		}
    
		// Button 2
		if (B2==0) {
	
			SEND_CMD(CLR_DISP);      	
			SEND_CMD(DD_RAM_ADDR);
			SEND_STR("Press button 2"); 
		}  
    

		// Button 3    
		if (B3==0) {
	       
			SEND_CMD(CLR_DISP);   
			SEND_CMD(DD_RAM_ADDR);  
			SEND_STR("Press button 3"); 
		  
		}       
		
		// Button 4
		while (B4==0) 
			Buzzer();


		// Button 3    
		if (B5==0) {
	       
			SEND_CMD(CLR_DISP);   
			SEND_CMD(DD_RAM_ADDR);  
			SEND_STR("Press button 5"); 
		  
		} 
   
  	}
}
