#include <stdio.h>
#include <stdlib.h>

int main ( void )
{
    int * ptr = ( int * ) malloc ( sizeof ( int ) );
    if ( ptr )
    {
        *ptr = 5;
        printf ( "%d\n", *ptr );
    }
	return 0;
}
