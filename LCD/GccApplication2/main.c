#ifndef F_CPU
#define F_CPU 1000000
#endif

#define MAX_VOLTAGE 5

#define D4 eS_PORTB4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <stdlib.h>

#include <avr/io.h>

int main(void)
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	
	ADMUX = 0b00000100;		// AREF				: 00
	// right-Justified	: 0
	// ADC4 for ADC		: 00100
	
	ADCSRA = 0b10000011;	// Enable ADC : 1
	// Do not start ADC : 0
	// No Trigger : 0
	// No Interrupt Flag : 0
	// No Interrupt : 0
	// ADC Clock : 011 -> 1MHz/8 = 125kHz
	
	Lcd4_Init();			// Initialize LCD in 4 bit mode
	
	unsigned int value = 0;	// Digital Value
	
	while(1)
	{
		ADCSRA |= (1 << ADSC);			// Start ADC Conversion
		while(ADCSRA & (1 << ADSC));	// Wait to finish ADC Conversion
		
		int adcl = ADCL;
		int adch = ADCH;
		
		value = (adcl)|(adch<<8);	// XXXXXXDD | DDDDDDDD
		
		Lcd4_Clear();
		
		Lcd4_Set_Cursor(1,1);	// Cursor in (row, column) = (1, 1)
		
		char vol[10] = "VOLTAGE: ";
		Lcd4_Write_String(vol);	// Write Text "Voltage: " in first row
		
		
		float voltage = value * MAX_VOLTAGE / 1024.0;
		char result[8];
		dtostrf(voltage, 1, 2, result);	// double-to-string-format 1 precision, 2 width
		
		Lcd4_Set_Cursor(2,1);			// Cursor in (row, column) = (2, 1)
		Lcd4_Write_String(result);
		
		_delay_ms(1000);
	}
}