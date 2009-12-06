#include <avr/io.h>
//#include <avr/iom128.h>
#include "lcd.h"
#include "bits.h"
#include "delay.h"

#define		RS_HIGH			PORTC |= BIT0 
#define		RS_LOW			PORTC &= ~BIT0

#define		RW_HIGH			PORTC |= BIT1
#define		RW_LOW			PORTC &= ~BIT1

#define		E_HIGH			PORTC |= BIT2			
#define		E_LOW			PORTC &= ~BIT2	


#define		LCD_CHAR_SPEED		2

  

unsigned char data;

void _E (void) {
	E_HIGH;
	delay_us(5);
	E_LOW;
}



//--------Initialize LCD-----------------------------
void LCD_Ini(void) {

	RS_LOW;				
	delay_ms(110);	
	PORTC=0b00110000;		
	_E();	
	delay_ms(10);	
	PORTC=0b00110000;
	_E();
	delay_ms(10);
	PORTC=0b00110000;
	_E();
	delay_ms(10);
	PORTC=0b00100000;
	_E();

}      
    
//--------Send char to LCD-----------------------------
void SEND_CHAR (unsigned char c){
    
	delay_ms(LCD_CHAR_SPEED);              	//Delay for LCD char
	data = 0b00001111 | c;			    	//get upper nibble
	PORTC = (PORTC | 0b11110000) & data;    //set D4-D7
	RS_HIGH;		  			    		//set LCD to data mode
	_E();
	data = c << 4;				    		//get down nibble
	PORTC = (PORTC & 0b00001111) | data;	//set D4-D7 (only PORTC4-PORTC7)
	RS_HIGH;					    	   	//set LCD to data mode
	_E();
}               
    

//--------Send command to LCD-----------------------------
void SEND_CMD (unsigned char c) {
    
	delay_ms(LCD_CHAR_SPEED);              	//Delay for LCD char
	data = 0b00001111 | c;			    	//get upper nibble
	PORTC = (PORTC | 0b11110000) & data;    //set D4-D7
	RS_LOW;					    			//set LCD to data mode
	_E();    
	data = c << 4;				    		//get down nibble
	PORTC = (PORTC & 0b00001111) | data;	//set D4-D7 (only PORTC4-PORTC7)
	RS_LOW;					    			//set LCD to data mode
	_E();
}                    

//--------Send string to LCD-----------------------------
void SEND_STR(unsigned char* str) {

	unsigned int i=0;
	
	// set first row 
	SEND_CMD(DD_RAM_ADDR);

	// loop to the end of the string
	while(str[i]!='\0') {	
		
		SEND_CHAR(str[i]);  
		if (i == 15) 
			SEND_CMD(DD_RAM_ADDR2);
		i++;
	}

}
