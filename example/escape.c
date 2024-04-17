// escape.c - Simple test print code


#include <stdint.h>
#include <tgmath.h>

#define ESCAPE_IMPLEMENTATION
#include "../include/escape.h"

#define TEST_STRING "HELLO"

void hsv_rgb( float h, float s, float v, uint8_t *r, uint8_t *g, uint8_t *b )
{
	float h0 = h * 6.0;

	float k = fmod( 5.0 + h0, 6.0 );
	*r = 255 * ( v - v * s * fmax( 0.0, fmin( k, fmin( 4.0 - k, 1.0 ) ) ) );

	k = fmod( 3.0 + h0, 6.0 );
	*g = 255 * ( v - v * s * fmax( 0.0, fmin( k, fmin( 4.0 - k, 1.0 ) ) ) );

	k = fmod( 1.0 + h0, 6.0 );
	*b = 255 * ( v - v * s * fmax( 0.0, fmin( k, fmin( 4.0 - k, 1.0 ) ) ) );
}


int main( void )
{
	// Clear screen
	fputs( ESC_CLEAR_SCREEN, stdout );

	// Print a rectangle of Xs with positional coordinates
	fputs( ESC_CURSOR_POSITION( 1, 1 ), stdout ); fflush( stdout );
	fputs( ESC_CURSOR_POSITION( 3, 2 ) "X", stdout );	fflush( stdout );
	fputs( ESC_CURSOR_POSITION( 10, 2 ) "X", stdout );  fflush( stdout );
	fputs( ESC_CURSOR_POSITION( 10, 4 ) "X", stdout ); fflush( stdout );
	fputs( ESC_CURSOR_POSITION( 3, 4 ) "X", stdout ); fflush( stdout );
	fputs( "\n", stdout );

	fputs( "\n" ESC_RESET TEST_STRING "\n\n", stdout );
	fputs( ESC_BOLD TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_OVERLINE TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_UNDERLINE TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_STRIKETHROUGH TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_COLOR_FG_RGB( 0, 128, 128 ) TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_COLOR_BG_RGB( 128, 0, 128 ) TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_BLINK TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_REVERSE TEST_STRING ESC_RESET "\n\n", stdout );
	fputs( ESC_CONCEAL TEST_STRING ESC_REVEAL TEST_STRING ESC_RESET "\n\n", stdout );

	uint8_t r,g,b;

	for( float i = 0; i < 1; i += 0.01 )
	{
		hsv_rgb( i, 1.0, 1.0, &r, &g, &b );

		esc_color_fg_rgb( r, g, b );
		fputs( "█", stdout );
	}

	fputs( "\n", stdout );
}
