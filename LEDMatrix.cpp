/*****************************************************************************
 * Name        : LEDMatrix.cpp
 * Author      : Steven Smethurst (https://github.com/funvill/) 
 * Version     : 0.01
 * Copyright   : Creative Commons Attribution-ShareAlike 3.0 Unported License
 *               http://creativecommons.org/licenses/by-sa/3.0/deed.en_GB
 * Description : Skeleton code for VHS LED Matrix Badge  
 *               https://github.com/vhs/vhs-pcbs/tree/master/led-matrix
 *               The matrix hex codes can easily be generated from this page 
 *               http://www.abluestar.com/utilities/vhsbadge/ 
 * 
*****************************************************************************/

// Key parameters for ATmega168P
// Flash (Kbytes):   16 Kbytes
// SRAM (Kbytes):     1 Kbytes
// EEPROM (Bytes):  512 Bytes
// LEDs are common anode


#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

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


// defines 8x8 ascii characters 0x20-0x7F (32-127)
const unsigned char font_8x8[96][8] PROGMEM = {
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, //
        {0x00,0x60,0xfa,0xfa,0x60,0x00,0x00,0x00}, // !
        {0x00,0xe0,0xe0,0x00,0xe0,0xe0,0x00,0x00}, // "
        {0x28,0xfe,0xfe,0x28,0xfe,0xfe,0x28,0x00}, // #
        {0x24,0x74,0xd6,0xd6,0x5c,0x48,0x00,0x00}, // $
        {0x62,0x66,0x0c,0x18,0x30,0x66,0x46,0x00}, // %
        {0x0c,0x5e,0xf2,0xba,0xec,0x5e,0x12,0x00}, // &
        {0x20,0xe0,0xc0,0x00,0x00,0x00,0x00,0x00}, // '
        {0x00,0x38,0x7c,0xc6,0x82,0x00,0x00,0x00}, // (
        {0x00,0x82,0xc6,0x7c,0x38,0x00,0x00,0x00}, // )
        {0x10,0x54,0x7c,0x38,0x38,0x7c,0x54,0x10}, // *
        {0x10,0x10,0x7c,0x7c,0x10,0x10,0x00,0x00}, // +
        {0x00,0x05,0x07,0x06,0x00,0x00,0x00,0x00}, // ,
        {0x10,0x10,0x10,0x10,0x10,0x10,0x00,0x00}, // -
        {0x00,0x00,0x06,0x06,0x00,0x00,0x00,0x00}, // .
        {0x06,0x0c,0x18,0x30,0x60,0xc0,0x80,0x00}, // /
        {0x7c,0xfe,0x9a,0xb2,0xfe,0x7c,0x00,0x00}, // 0
        {0x42,0x42,0xfe,0xfe,0x02,0x02,0x00,0x00}, // 1
        {0x46,0xce,0x9a,0x92,0xf6,0x66,0x00,0x00}, // 2
        {0x44,0xc6,0x92,0x92,0xfe,0x6c,0x00,0x00}, // 3
        {0x18,0x38,0x68,0xc8,0xfe,0xfe,0x08,0x00}, // 4
        {0xe4,0xe6,0xa2,0xa2,0xbe,0x9c,0x00,0x00}, // 5
        {0x3c,0x7e,0xd2,0x92,0x9e,0x0c,0x00,0x00}, // 6
        {0xc0,0xc6,0x8e,0x98,0xf0,0xe0,0x00,0x00}, // 7
        {0x6c,0xfe,0x92,0x92,0xfe,0x6c,0x00,0x00}, // 8
        {0x60,0xf2,0x92,0x96,0xfc,0x78,0x00,0x00}, // 9
        {0x00,0x00,0x36,0x36,0x00,0x00,0x00,0x00}, // :
        {0x00,0x05,0x37,0x36,0x00,0x00,0x00,0x00}, // ;
        {0x10,0x38,0x6c,0xc6,0x82,0x00,0x00,0x00}, // <
        {0x28,0x28,0x28,0x28,0x28,0x28,0x00,0x00}, // =
        {0x00,0x82,0xc6,0x6c,0x38,0x10,0x00,0x00}, // >
        {0x40,0xc0,0x8a,0x9a,0xf0,0x60,0x00,0x00}, // ?
        {0x7c,0xfe,0x82,0xba,0xba,0xf8,0x78,0x00}, // @
        {0x3e,0x7e,0xc8,0xc8,0x7e,0x3e,0x00,0x00}, // A
        {0x82,0xfe,0xfe,0x92,0x92,0xfe,0x6c,0x00}, // B
        {0x38,0x7c,0xc6,0x82,0x82,0xc6,0x44,0x00}, // C
        {0x82,0xfe,0xfe,0x82,0xc6,0xfe,0x38,0x00}, // D
        {0x82,0xfe,0xfe,0x92,0xba,0x82,0xc6,0x00}, // E
        {0x82,0xfe,0xfe,0x92,0xb8,0x80,0xc0,0x00}, // F
        {0x38,0x7c,0xc6,0x82,0x8a,0xce,0x4e,0x00}, // G
        {0xfe,0xfe,0x10,0x10,0xfe,0xfe,0x00,0x00}, // H
        {0x00,0x82,0xfe,0xfe,0x82,0x00,0x00,0x00}, // I
        {0x0c,0x0e,0x02,0x82,0xfe,0xfc,0x80,0x00}, // J
        {0x82,0xfe,0xfe,0x10,0x38,0xee,0xc6,0x00}, // K
        {0x82,0xfe,0xfe,0x82,0x02,0x06,0x0e,0x00}, // L
        {0xfe,0xfe,0x60,0x30,0x60,0xfe,0xfe,0x00}, // M
        {0xfe,0xfe,0x60,0x30,0x18,0xfe,0xfe,0x00}, // N
        {0x38,0x7c,0xc6,0x82,0xc6,0x7c,0x38,0x00}, // O
        {0x82,0xfe,0xfe,0x92,0x90,0xf0,0x60,0x00}, // P
        {0x78,0xfc,0x84,0x8e,0xfe,0x7a,0x00,0x00}, // Q
        {0x82,0xfe,0xfe,0x98,0x9c,0xf6,0x62,0x00}, // R
        {0x64,0xe6,0xb2,0x9a,0xde,0x4c,0x00,0x00}, // S
        {0xc0,0x82,0xfe,0xfe,0x82,0xc0,0x00,0x00}, // T
        {0xfe,0xfe,0x02,0x02,0xfe,0xfe,0x00,0x00}, // U
        {0xf8,0xfc,0x06,0x06,0xfc,0xf8,0x00,0x00}, // V
        {0xfe,0xfe,0x0c,0x18,0x0c,0xfe,0xfe,0x00}, // W
        {0xc6,0xee,0x38,0x10,0x38,0xee,0xc6,0x00}, // X
        {0xe0,0xf2,0x1e,0x1e,0xf2,0xe0,0x00,0x00}, // Y
        {0xe6,0xce,0x9a,0xb2,0xe2,0xc6,0x8e,0x00}, // Z
        {0x00,0xfe,0xfe,0x82,0x82,0x00,0x00,0x00}, // [
        {0x80,0xc0,0x60,0x30,0x18,0x0c,0x06,0x00}, // "\"
        {0x00,0x82,0x82,0xfe,0xfe,0x00,0x00,0x00}, // ]
        {0x10,0x30,0x60,0xc0,0x60,0x30,0x10,0x00}, // ^
        {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01}, // _
        {0x00,0x00,0xc0,0xe0,0x20,0x00,0x00,0x00}, // `
        {0x04,0x2e,0x2a,0x2a,0x3c,0x1e,0x02,0x00}, // a
        {0x82,0xfc,0xfe,0x22,0x22,0x3e,0x1c,0x00}, // b
        {0x1c,0x3e,0x22,0x22,0x36,0x14,0x00,0x00}, // c
        {0x0c,0x1e,0x12,0x92,0xfc,0xfe,0x02,0x00}, // d
        {0x1c,0x3e,0x2a,0x2a,0x3a,0x18,0x00,0x00}, // e
        {0x12,0x7e,0xfe,0x92,0xc0,0x40,0x00,0x00}, // f
        {0x19,0x3d,0x25,0x25,0x1f,0x3e,0x20,0x00}, // g
        {0x82,0xfe,0xfe,0x10,0x20,0x3e,0x1e,0x00}, // h
        {0x00,0x22,0xbe,0xbe,0x02,0x00,0x00,0x00}, // i
        {0x02,0x23,0x21,0xbf,0xbe,0x00,0x00,0x00}, // j
        {0x82,0xfe,0xfe,0x08,0x1c,0x36,0x22,0x00}, // k
        {0x00,0x82,0xfe,0xfe,0x02,0x00,0x00,0x00}, // l
        {0x3e,0x3e,0x30,0x18,0x30,0x3e,0x1e,0x00}, // m
        {0x3e,0x3e,0x20,0x20,0x3e,0x1e,0x00,0x00}, // n
        {0x1c,0x3e,0x22,0x22,0x3e,0x1c,0x00,0x00}, // o
        {0x21,0x3f,0x1f,0x25,0x24,0x3c,0x18,0x00}, // p
        {0x18,0x3c,0x24,0x25,0x1f,0x3f,0x21,0x00}, // q
        {0x22,0x3e,0x1e,0x22,0x38,0x18,0x00,0x00}, // r
        {0x12,0x3a,0x2a,0x2a,0x2e,0x24,0x00,0x00}, // s
        {0x00,0x20,0x7c,0xfe,0x22,0x24,0x00,0x00}, // t
        {0x3c,0x3e,0x02,0x02,0x3c,0x3e,0x02,0x00}, // u
        {0x38,0x3c,0x06,0x06,0x3c,0x38,0x00,0x00}, // v
        {0x3c,0x3e,0x06,0x0c,0x06,0x3e,0x3c,0x00}, // w
        {0x22,0x36,0x1c,0x08,0x1c,0x36,0x22,0x00}, // x
        {0x39,0x3d,0x05,0x05,0x3f,0x3e,0x00,0x00}, // y
        {0x32,0x26,0x2e,0x3a,0x32,0x26,0x00,0x00}, // z
        {0x10,0x10,0x7c,0xee,0x82,0x82,0x00,0x00}, // {
        {0x00,0x00,0x00,0xee,0xee,0x00,0x00,0x00}, // |
        {0x82,0x82,0xee,0x7c,0x10,0x10,0x00,0x00}, // }
        {0x40,0xc0,0x80,0xc0,0x40,0xc0,0x80,0x00}, // ~
        {0x1e,0x3e,0x62,0xc2,0x62,0x3e,0x1e,0x00}, //
};



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

		void SetLetter( unsigned char x, char letter ); 
		void ScrollLeft(); 

		// Prints debug information about the LED matrix to the screen. 
		void Debug(); 
} ;

bool CLEDMatrix::SetLEDRed( unsigned char x, unsigned char y, LED_COLOR_VAR red ) {
	if( x > LED_MATRIX_WIDTH || y > LED_MATRIX_HIGHT ) {
		return false;
	}

	unsigned char offsetByte	= (x + y*LED_MATRIX_WIDTH) / 8;
	unsigned char offsetBit		= (x + y*LED_MATRIX_WIDTH) % 8 ;
	if( red == LED_COLOR_ON ) {
		this->m_data[ offsetByte ] |= 1 << offsetBit ; 
	} else { 
		this->m_data[ offsetByte ] &= ~(1 << offsetBit) ; 
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
		this->m_data[ offsetByte  ] &= ~(1 << offsetBit) ; 
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



// Extract bit from byte 
// (byte >> pos) & 1

// Setting a bit in a BYTE 
// number |= 1 << x;

bool GetBit( unsigned char byte, unsigned char bit_offset ) {
	return ((byte >> bit_offset) & 1 ) == 1; 
}
unsigned char SetBit( unsigned char byte, unsigned char bit_offset, bool value ) {
	byte |= value << bit_offset ; 
	return byte ; 
}

void CLEDMatrix::SetLetter( unsigned char x, char letter ) {
	if( letter - 0x20 > 96 ) {
		return ; 
	}

	for( unsigned char byte_offset = 0 ; byte_offset < 8 ; byte_offset++ ) {
		for( unsigned char bit_offset = 0 ; bit_offset < 8 ; bit_offset++ ) {

			bool value = GetBit( pgm_read_byte(font_8x8[letter - 0x20][byte_offset]), bit_offset ) ; 
			unsigned char data_offset = (8*2)-(bit_offset*2) ;

			// ToDo: Proper x offset. 
			if( x >= 8 ) {
				data_offset++; 
			}

			this->m_data[data_offset] = SetBit( this->m_data[data_offset], byte_offset, value ); 
		}
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
	if( length/2 > LED_MATRIX_SIZE ) {
		// Too much data 
		return false ; 
	}

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

int _tmain(int argc, char* argv[])
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

	// Set an indiviaul pixel 
	printf("Set an indiviaul pixel\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	matrix.SetLEDRed(0,					 0,					 LED_COLOR_ON ); 
	matrix.SetLEDRed(LED_MATRIX_WIDTH-1, 0,					 LED_COLOR_ON ); 
	matrix.SetLEDRed(0,					 LED_MATRIX_HIGHT-1, LED_COLOR_ON ); 
	matrix.SetLEDRed(LED_MATRIX_WIDTH-1, LED_MATRIX_HIGHT-1, LED_COLOR_ON ); 
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

	// Load some predefined matrix.
	// Generated with http://www.abluestar.com/utilities/vhsbadge/
	printf("Smile\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	matrix.LoadLEDS( "22007700220008000800410063003e00004400ee004400100010008200c6007c", 65 ); 
	matrix.Debug(); 

	// Load a letter 
	printf("Letter V\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	matrix.SetLetter( 0, 'V' ); 
	matrix.Debug(); 

	printf("Letter H\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	matrix.SetLetter( 0, 'H' ); 
	matrix.Debug(); 

	printf("Letter S\n");
	matrix.SetAllLEDS( LED_COLOR_OFF, LED_COLOR_OFF ); 
	matrix.SetLetter( 0, 'S' ); 
	matrix.Debug(); 

	return 0;
}

