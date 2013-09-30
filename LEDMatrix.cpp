//============================================================================
// Name        : LEDMatrix.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

// Key parameters for ATmega168P
// Flash (Kbytes):   16 Kbytes
// SRAM (Kbytes):     1 Kbytes
// EEPROM (Bytes):  512 Bytes
// LEDs are common annod 


#include <stdio.h>



// Configuration
#define LED_MATRIX_WIDTH 	16
#define LED_MATRIX_HIGHT	 8
#define LED_MATRIX_LEDS	 	 2

// 
#define LED_MATRIX_SIZE 	LED_MATRIX_WIDTH * LED_MATRIX_HIGHT * LED_MATRIX_LEDS 

// ToDo: Change this to a char to add PWM... 
#define LED_COLOR_VAR		bool 
#define LED_COLOR_OFF		false
#define LED_COLOR_ON		true	

class CLEDMatrix
{
	public:
		char m_data[ LED_MATRIX_SIZE / 8 ];

		CLEDMatrix() { }

		bool SetLED( unsigned char x, unsigned char y, LED_COLOR_VAR red, LED_COLOR_VAR green );
		void SetAllLEDS( LED_COLOR_VAR red, LED_COLOR_VAR green );

		// Length = in bits 
		bool LoadString( char * data, unsigned short length );
} ;

// Extract bit from byte 
// (byte >> pos) & 1

// Setting a bit in a BYTE 
// number |= 1 << x;

bool CLEDMatrix::SetLED( unsigned char x, unsigned char y, LED_COLOR_VAR red, LED_COLOR_VAR green ) {
	if( x > LED_MATRIX_WIDTH || y > LED_MATRIX_HIGHT ) {
		return false;
	}

	// ToDo: this is messed. here is where i am 
	unsigned char offset = ( x * LED_MATRIX_LEDS * LED_MATRIX_WIDTH / 8 ) ; 
	if( red == LED_COLOR_OFF ) {
		this->m_data[ offset   ] |= 0 << (x * y * 2) % 8 ;		
	} else {
		this->m_data[ offset   ] |= 1 << (x * y * 2) % 8;	
	}

	if( green == LED_COLOR_OFF ) {
		this->m_data[ offset+1 ] |= 0 << (x * y * 2) % 8;		
	} else {
		this->m_data[ offset+1 ] |= 1 << (x * y * 2) % 8;	
	}

	return true ;
}

void CLEDMatrix::SetAllLEDS( LED_COLOR_VAR red, LED_COLOR_VAR green ) {
	// Turn off all the LEDS
	for( unsigned short offsetWidth = 0 ; offsetWidth < LED_MATRIX_WIDTH; offsetWidth++ ) {
		for( unsigned short offsetHeight = 0 ; offsetHeight < LED_MATRIX_HIGHT; offsetHeight++ ) {
			this->SetLED(offsetWidth, offsetHeight, red, green );
		}	
	}
}


int char2int(char input)
{
	if(input >= '0' && input <= '9') {
		return input - '0';
	} else if(input >= 'A' && input <= 'F') {
		return input - 'A' + 10;
	} else if(input >= 'a' && input <= 'f') {
		return input - 'a' + 10;
	} else {
		return 0;
	}
}




// This function will set up LED glyph from a string made with the online editor.
// 8x16x2 = 256 LEDs, or 32 BYTES. 64 letters in hex for the entire display. 
bool CLEDMatrix::LoadString( char * data, unsigned short length ) {
	// Check the size of the string to make sure we don't get any buffer overflows.
	// ToDo: 	

	// Reset the matrix. 
	this->SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ) ; 

	// Decode the string, and set the LEDs 	
	for( unsigned short offset = 0 ; offset < length ; offset++) {
		if( offset % 2 == 0 ) {
			output += char2int( data[offset] ) * 16  ; 
		} else {
			output += char2int( data[offset] ) ; 

			this->m_data[ offset / 2 ] = output ; 

			// Reset the var 
			output = 0 ; 
		}
		// printf( "\noffset=[%d], char=[%d] output=[%d]", offset, data[offset], output);
		
	}

	return true ; 
}



int main() {

	CLEDMatrix matrix ;

	if( ! matrix.LoadString( "A0B1C2", 6 ) ) {
		cout << "Matrix loaded" << endl;
	}


	return 0;
}
