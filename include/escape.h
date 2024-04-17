// escape.h - ESC escape code macros and functions

#ifndef ESCAPE_H
#define ESCAPE_H


#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif


#define CSI	"\x1B["


#define ESC_CURSOR_UP( n )  CSI #n "A"
void esc_cursor_up( uint32_t n );

#define ESC_CURSOR_DOWN( n ) CSI #n "B"
void esc_cursor_down( uint32_t n );

#define ESC_CURSOR_LEFT( n )  CSI #n "C"
void esc_cursor_left( uint32_t n );

#define ESC_CURSOR_RIGHT( n )  CSI #n "D"
void esc_cursor_right( uint32_t n );

#define ESC_CURSOR_LINE_NEXT( n ) CSI #n "E"
void esc_cursor_line_next( uint32_t n );

#define ESC_CURSOR_POSITION( x, y )  CSI #y ";" #x "H"
void esc_cursor_position( uint32_t x, uint32_t y );

#define ESC_CLEAR_SCREEN  CSI "2J"
void esc_clear_screen( void );

#define ESC_CLEAR_SCREEN_START CSI "1J"
void esc_clear_screen_start( void );

#define ESC_CLEAR_SCREEN_END CSI "0J"
void esc_clear_screen_end( void );

#define ESC_CLEAR_LINE CSI "2K"
void esc_clear_line( void );

#define ESC_CLEAR_LINE_START CSI "1L"
void esc_clear_line_start( void );

#define ESC_CLEAR_LINE_END  CSI "0L"
void esc_clear_line_end( void );

#define ESC_SCROLL_UP( n ) CSI #n "S"
void esc_scroll_up( uint32_t n );

#define ESC_SCROLL_DOWN( n ) CSI #n "T"
void esc_scroll_down( uint32_t n );

#define ESC_SGR  "m"

#define ESC_RESET          CSI "0" ESC_SGR
#define ESC_BOLD	       CSI "1" ESC_SGR
#define ESC_UNDERLINE      CSI "4" ESC_SGR
#define ESC_BLINK          CSI "5" ESC_SGR
#define ESC_REVERSE        CSI "7" ESC_SGR
#define ESC_CONCEAL        CSI "8" ESC_SGR
#define ESC_REVEAL         CSI "28" ESC_SGR
#define ESC_STRIKETHROUGH  CSI "9" ESC_SGR
#define ESC_OVERLINE       CSI "53" ESC_SGR

#define ESC_COLOR_FG_RGB( r, g, b )  CSI "38;2;" #r ";" #g ";" #b ESC_SGR
void esc_color_fg_rgb( uint32_t r, uint32_t g, uint32_t b );

#define ESC_COLOR_BG_RGB( r, g, b )  CSI "48;2;" #r ";" #g ";" #b ESC_SGR
void esc_color_bg_rgb( uint32_t r, uint32_t g, uint32_t b );

#define ESC_COLOR_FG_256( n )  CSI "38;5;" #n ESC_SGR
void esc_color_fg_256( uint32_t r, uint32_t g, uint32_t b );

#define ESC_COLOR_BG_256( n )  CSI "48;5;" #n ESC_SGR
void esc_color_bg_256( uint32_t r, uint32_t g, uint32_t b );

#define ESC_COLOR_FG( n )         CSI "3" #n ESC_SGR
#define ESC_COLOR_BG( n )         CSI "4" #n ESC_SGR
#define ESC_COLOR_FG_BRIGHT( n )  CSI "9" #n ESC_SGR
#define ESC_COLOR_BG_BRIGHT( n )  CSI "10" #n ESC_SGR


#ifdef __cplusplus
}
#endif

#endif // ESCAPE_H


#ifdef ESCAPE_IMPLEMENTATION

#include <stdio.h>

void esc_cursor_up( uint32_t n ) { fprintf( stdout, CSI "%dA", n ); }
void esc_cursor_down( uint32_t n ) { fprintf( stdout, CSI "%dB", n ); }
void esc_cursor_left( uint32_t n ) { fprintf( stdout, CSI "%dC", n ); }
void esc_cursor_right( uint32_t n ) { fprintf( stdout, CSI "%dD", n ); }
void esc_cursor_line_next( uint32_t n ) { fprintf( stdout, CSI "%dE", n ); }
void esc_cursor_position( uint32_t x, uint32_t y ) { fprintf( stdout, CSI "%d;%dH", y, x ); }
void esc_clear_screen( void ) { fputs( CSI "2J", stdout ); }
void esc_clear_screen_start( void ) { fputs( CSI "1J", stdout ); }
void esc_clear_screen_end( void ) { fputs( CSI "0J", stdout ); }
void esc_clear_line( void ) { fputs( CSI "2L", stdout ); }
void esc_clear_line_start( void ) { fputs( CSI "1L", stdout ); }
void esc_clear_line_end( void ) { fputs( CSI "0L", stdout ); }
void esc_scroll_up( uint32_t n ) { fprintf( stdout, "%dS", n ); }
void esc_scroll_down( uint32_t n ) { fprintf( stdout, "%dT", n ); }

void esc_color_fg_rgb( uint32_t r, uint32_t g, uint32_t b ) { fprintf( stdout, CSI "38;2;%d;%d;%d" ESC_SGR, r, g, b ); }
void esc_color_bg_rgb( uint32_t r, uint32_t g, uint32_t b ) { fprintf( stdout, CSI "48;2;%d;%d;%d" ESC_SGR, r, g, b ); }
void esc_color_fg_256( uint32_t r, uint32_t g, uint32_t b ) { fprintf( stdout, CSI "38;5;%d;%d;%d" ESC_SGR, r, g, b ); }
void esc_color_bg_256( uint32_t r, uint32_t g, uint32_t b ) { fprintf( stdout, CSI "48;5;%d;%d;%d" ESC_SGR, r, g, b ); }

#endif // ESCAPE_IMPLEMENTATION