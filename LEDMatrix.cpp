#include "stdafx.h"

/*****************************************************************************
 * Name        : LEDMatrix.cpp
 * Author      : Steven Smethurst (https://github.com/funvill/) 
 * Version     : 0.01
 * Copyright   : Creative Commons Attribution-ShareAlike 3.0 Unported License
 *               http://creativecommons.org/licenses/by-sa/3.0/deed.en_GB
 * Description : Skeleton code for VHS LED Matrix Badge  
 *               https://github.com/vhs/vhs-pcbs/tree/master/led-matrix
*****************************************************************************/

// Key parameters for ATmega168P
// Flash (Kbytes):   16 Kbytes
// SRAM (Kbytes):     1 Kbytes
// EEPROM (Bytes):  512 Bytes
// LEDs are common annod 


#include <stdio.h>
#include <string.h>


// Configuration
#define LED_MATRIX_WIDTH 	16
#define LED_MATRIX_HIGHT	 8
#define LED_MATRIX_LEDS	 	 2

// 
#define LED_MATRIX_LENGTH 	LED_MATRIX_WIDTH * LED_MATRIX_HIGHT 
#define LED_MATRIX_SIZE		(LED_MATRIX_LENGTH * LED_MATRIX_LEDS) / 8

// ToDo: Change this to a char to add PWM... 
#define LED_COLOR_VAR		bool 
#define LED_COLOR_OFF		false
#define LED_COLOR_ON		true	

class CButtons
{
public:
	enum ButtonTypes { 
		BUTTON_UP = 0, 
		BUTTON_RIGHT, 
		BUTTON_LEFT, 
		BUTTON_DOWN, 
		BUTTON_A, 
		BUTTON_B, 
	} ; 

	unsigned char m_data ; 

	bool GetButtonState( ButtonTypes button ) {
		return ( (this->m_data >> button) & 1 ) ; 
	}

	void Debug() {
		printf("Buttons down: ");
		if( this->GetButtonState( BUTTON_UP ) )		{ printf( "UP "); } 
		if( this->GetButtonState( BUTTON_RIGHT ) )	{ printf( "RIGHT "); } 
		if( this->GetButtonState( BUTTON_LEFT ) )	{ printf( "LEFT "); } 
		if( this->GetButtonState( BUTTON_DOWN ) )	{ printf( "DOWN "); } 
		if( this->GetButtonState( BUTTON_A ) )		{ printf( "A "); } 
		if( this->GetButtonState( BUTTON_B ) )		{ printf( "B "); } 
		printf("\n");
	}
};

class CLEDMatrix
{
	public:
		unsigned char m_data[ LED_MATRIX_SIZE ];

		CLEDMatrix() { 
			// Clear the data. 
			memset( m_data, 0, LED_MATRIX_SIZE );
		}


		bool SetLEDRed	( unsigned char x, unsigned char y, LED_COLOR_VAR red ); 
		bool SetLEDGreen( unsigned char x, unsigned char y, LED_COLOR_VAR green ); 		
		bool SetLED		( unsigned char x, unsigned char y, LED_COLOR_VAR red, LED_COLOR_VAR green );
		void SetAllLEDS	( LED_COLOR_VAR red, LED_COLOR_VAR green );

		LED_COLOR_VAR GetLEDRed  ( unsigned char x, unsigned char y ); 
		LED_COLOR_VAR GetLEDGreen( unsigned char x, unsigned char y ); 		

		// Length = in bits 
		bool LoadLEDS( const char * data, const unsigned char length );
		unsigned char SaveLEDS( char * data, const unsigned char max_length ); 

		// Prints debug information about the LED matrix to the screen. 
		void Debug(); 
} ;

// Extract bit from byte 
// (byte >> pos) & 1

// Setting a bit in a BYTE 
// number |= 1 << x;

bool CLEDMatrix::SetLEDRed( unsigned char x, unsigned char y, LED_COLOR_VAR red ) {
	if( x > LED_MATRIX_WIDTH || y > LED_MATRIX_HIGHT ) {
		return false;
	}

	unsigned char offsetByte	= (x + y*LED_MATRIX_WIDTH) / 8;
	unsigned char offsetBit		= (x + y*LED_MATRIX_WIDTH) % 8 ;
	if( red == LED_COLOR_ON ) {
		this->m_data[ offsetByte ] |= 1 << offsetBit ; 
	} else { 
		this->m_data[ offsetByte ] &= 0 << offsetBit ; 
	}

	return true ;
}

bool CLEDMatrix::SetLEDGreen( unsigned char x, unsigned char y, LED_COLOR_VAR green ) {
	if( x > LED_MATRIX_WIDTH || y > LED_MATRIX_HIGHT ) {
		return false;
	}

	unsigned char offsetByte	= ((x + y*LED_MATRIX_WIDTH) / 8 ) + (LED_MATRIX_LENGTH/8) ;
	unsigned char offsetBit		= (x + y*LED_MATRIX_WIDTH) % 8 ;
	if( green == LED_COLOR_ON ) {
		this->m_data[ offsetByte ] |= 1 << offsetBit ; 
	} else { 
		this->m_data[ offsetByte  ] &= 0 << offsetBit ; 
	}

	return true ;
}


bool CLEDMatrix::SetLED( unsigned char x, unsigned char y, LED_COLOR_VAR red, LED_COLOR_VAR green ) {
	return (this->SetLEDRed( x,y,red ) && this->SetLEDGreen( x,y, green ) ) ;
}
LED_COLOR_VAR CLEDMatrix::GetLEDRed( unsigned char x, unsigned char y ) {
	unsigned char offsetByte	= (x + y*LED_MATRIX_WIDTH) / 8 ;
	unsigned char offsetBit		= (x + y*LED_MATRIX_WIDTH) % 8 ;
	return ( ( this->m_data[ offsetByte ] >> offsetBit) & 1 ) ; 
}
LED_COLOR_VAR CLEDMatrix::GetLEDGreen( unsigned char x, unsigned char y ) {
	unsigned char offsetByte	= ((x + y*LED_MATRIX_WIDTH) / 8) + (LED_MATRIX_LENGTH/8) ;
	unsigned char offsetBit		= (x + y*LED_MATRIX_WIDTH) % 8 ;
	return ( ( this->m_data[ offsetByte ] >> offsetBit) & 1 ) ; 
}

void CLEDMatrix::SetAllLEDS( LED_COLOR_VAR red, LED_COLOR_VAR green ) {
	// Much faster with memset then a double for loop 
	if( red ) { 
		// On 
		memset( this->m_data, 0xFF, sizeof( char ) * (LED_MATRIX_LENGTH / 8) );
	} else {
		// Off 
		memset( this->m_data, 0x00, sizeof( char ) * (LED_MATRIX_LENGTH / 8) );
	}

	// Green 
	if( green ) { 
		// On 
		memset( this->m_data + sizeof( char ) * (LED_MATRIX_LENGTH / 8), 0xFF, sizeof( char ) * (LED_MATRIX_LENGTH / 8) );
	} else {
		// Off 
		memset( this->m_data + sizeof( char ) * (LED_MATRIX_LENGTH / 8), 0x00, sizeof( char ) * (LED_MATRIX_LENGTH / 8) );
	}
}


int char2int( const char input)
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
bool CLEDMatrix::LoadLEDS( const char * data, const unsigned char length ) {
	// Check the size of the string to make sure we don't get any buffer overflows.
	// ToDo: 	

	// Reset the matrix. 
	this->SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ) ; 

	// Decode the string, and set the LEDs 	
	unsigned char output = 0 ; 
	for( unsigned short offset = 0 ; offset < length ; offset++) {
		if( offset % 2 == 0 ) {
			output += char2int( data[offset] ) * 16  ; 
		} else {
			output += char2int( data[offset] ) ; 

			// Copy the data in to the LED stream 
			this->m_data[ offset / 2 ] = output ; 

			// Reset the var 
			output = 0 ; 
		}
	}
	return true ; 
}

unsigned char CLEDMatrix::SaveLEDS( char * output, const unsigned char max_length ) {
	if( max_length < LED_MATRIX_SIZE * 2 + 1 ) {
		return 0; // Error not enugh space. 
	}

	for( unsigned short offset = 0 ; offset < LED_MATRIX_SIZE ; offset++ ) {
		sprintf( output+(offset*2), "%02X", this->m_data[ offset ] ); 
	}
	return LED_MATRIX_SIZE * 2 ; 
}

void CLEDMatrix::Debug() {
	for( unsigned char y = 0 ; y < LED_MATRIX_HIGHT; y++ ) {
		// Print headers 
		if( y == 0 ) {
			printf( " *|"); 
			for( unsigned char x = 0 ; x < LED_MATRIX_WIDTH; x++ ) {
				printf( "%2d|", x); 	
			}
		}
		printf( "\n %d|", y ); 

		for( unsigned char x = 0 ; x < LED_MATRIX_WIDTH; x++ ) {
			
			
			LED_COLOR_VAR red	= this->GetLEDRed  ( x, y ) ; 
			LED_COLOR_VAR green = this->GetLEDGreen( x, y ) ; 
			if( red == LED_COLOR_ON && green == LED_COLOR_ON ) {
				// Both Red and Green LEDs are on == Yellow 
				printf( "GR"); 
			} else if ( red == LED_COLOR_ON ) {
				printf( " R"); 
			} else if ( green == LED_COLOR_ON ) {
				printf( "G "); 
			} else {
				printf( "  "); 
			}

			printf( "|"); 
		}	
		
	}
	printf( "\n"); 

	// LED_MATRIX_SIZE converted in to 2 byte hex code with 1 byte for null terminal. 
	char buffer[LED_MATRIX_SIZE*2+1] ; 
	this->SaveLEDS( buffer,  LED_MATRIX_SIZE*2+1 );
	
	printf( "Code: %s\n\n", buffer ); 

}

int _tmain(int argc, _TCHAR* argv[])
{
	CLEDMatrix matrix ;
	

	// Test patterns 
	printf("Red\n");
	matrix.SetAllLEDS( LED_COLOR_ON, LED_COLOR_OFF ); 
	matrix.Debug(); 
	
	printf("Green\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_ON ); 
	matrix.Debug(); 

	printf("Yellow\n");
	matrix.SetAllLEDS( LED_COLOR_ON, LED_COLOR_ON ); 
	matrix.Debug(); 

	printf("Black\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	matrix.Debug(); 

	printf("Red Checkboard\n");
	for( unsigned char x = 0 ; x < LED_MATRIX_WIDTH; x++ ) {
		for( unsigned char y = 0 ; y < LED_MATRIX_HIGHT; y++ ) {
			if( x % 2 == 0 ) { 
				if( y % 2 == 0 ) {
					matrix.SetLEDRed( x, y, LED_COLOR_ON );
				} else if(x < LED_MATRIX_WIDTH -1 ) {
					matrix.SetLEDRed( x+1, y, LED_COLOR_ON );
				}
			}
		}	
	}
	matrix.Debug(); 

	printf("Green Checkboard\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	for( unsigned char x = 0 ; x < LED_MATRIX_WIDTH; x++ ) {
		for( unsigned char y = 0 ; y < LED_MATRIX_HIGHT; y++ ) {
			if( x % 2 == 0 ) { 
				if( y % 2 == 0 ) {
					matrix.SetLEDGreen( x+1, y, LED_COLOR_ON );
				} else if(x < LED_MATRIX_WIDTH -1 ) {
					matrix.SetLEDGreen( x, y, LED_COLOR_ON );
				}
			}
		}	
	}
	matrix.Debug(); 

	printf("Color Checkboard\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	for( unsigned char x = 0 ; x < LED_MATRIX_WIDTH; x++ ) {
		for( unsigned char y = 0 ; y < LED_MATRIX_HIGHT; y++ ) {
			if( x % 2 == 0 ) { 
				if( y % 2 == 0 ) {
					matrix.SetLEDRed( x, y, LED_COLOR_ON );
				} else if(x < LED_MATRIX_WIDTH -1 ) {
					matrix.SetLEDRed( x+1, y, LED_COLOR_ON );
				}
			} else {
				if( y % 2 == 0 ) {
					matrix.SetLEDGreen( x, y, LED_COLOR_ON );
				} else if(x < LED_MATRIX_WIDTH -1 ) {
					matrix.SetLEDGreen( x+1, y, LED_COLOR_ON );
				}
			}
		}	
	}
	matrix.Debug(); 

	// From string 
	if( ! matrix.LoadLEDS( "A0B1C2", 6 ) ) {
		printf( "Matrix loaded\n" );
	}
	matrix.Debug(); 

	return 0;
}

