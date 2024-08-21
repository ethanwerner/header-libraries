#define UFO_IMPLEMENTATION
#include "ufo.h"


#include <stdio.h>


int main() {
	ufo_t *ufo = ufo_init( 1, 100 );
	printf( "%f\n", ufo_evaluate( ufo, (fp_t[]){ 1.0 } ) );
	free( ufo );
	
	return 0;
}

