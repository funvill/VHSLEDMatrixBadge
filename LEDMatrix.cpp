//============================================================================
// Name        : LEDMatrix.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

// Configuration
#define LEX_MATRIX_WIDTH 	16
#define LEX_MATRIX_HIGHT	 8

// Settings
#define LEX_MATRIX_SIZE 	LEX_MATRIX_WIDTH * LEX_MATRIX_HIGHT

union led_t {
  struct {
		unsigned char red;
		unsigned char green;
		unsigned char blue;
    } colors;
  char data[3];
} led_u;


class CLED
{
	public:
		led_t m_data ;

		void Set( unsigned char red, unsigned char green, unsigned char blue );
		unsigned char GetRed() { return this->m_data.colors.red ; }
		unsigned char GetGreen() { return this->m_data.colors.green ; }
		unsigned char GetBlue() { return this->m_data.colors.blue ; }
};

void CLED::Set( unsigned char red, unsigned char green, unsigned char blue ) {
	this->m_data.colors.red   = red ;
	this->m_data.colors.green = green ;
	this->m_data.colors.blue  = blue ;
}


class CLEDMatrix
{
	public:
		CLED m_data[ LEX_MATRIX_SIZE ];

		CLEDMatrix() { }

		bool SetLED( unsigned char x, unsigned char y, unsigned char red, unsigned char green, unsigned char blue  );
		void SetAllLEDS( unsigned char red, unsigned char green, unsigned char blue );

		bool LoadString( char * data, unsigned short length );
} ;

bool CLEDMatrix::SetLED( unsigned char x, unsigned char y, unsigned char red, unsigned char green, unsigned char blue ) {
	if( x > LEX_MATRIX_WIDTH || y > LEX_MATRIX_HIGHT ) {
		return false;
	}

	this->m_data[ x * y ].Set( red, green, blue ); ;
	return true ;
}

void CLEDMatrix::SetAllLEDS( unsigned char red, unsigned char green, unsigned char blue ) {
	// Turn off all the LEDS
	for( unsigned short offset = 0 ; offset < LEX_MATRIX_SIZE; offset++ ) {
		this->m_data[ offset ].Set(red, green, blue );
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

// Assuming src to be zero terminated and target to be sufficiently large
void hex2bin(const char* src, char* target)
{
	while(*src && src[1])
	{
		*(target++) = char2int(*src))*16 + char2int(src[1]);
		src += 2;
	}
}

// This function will set up LED glyph from a string made with the online editor.
// "010203"
bool CLEDMatrix::LoadString( char * data, unsigned short length ) {
	if( length > LEX_MATRIX_SIZE * 3 ) {
		return false ; // Too big
	}

	unsigned short offset = 0 ;
	while( offset < length ) {
		unsigned short led_offset = offset / 3 ;
		unsigned char led_color = 0 ;
		this->m_data[ led_offset ].Set( x, y,  );
	}
	return true ;
}



int main() {

	CLEDMatrix matrix ;

	if( ! matrix.LoadString( "A0B1C2", 2 ) ) {
		cout << "Matrix loaded" << endl;
	}


	return 0;
}
