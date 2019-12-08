#include <stdio.h>
#include <stdlib.h>

int main ( void )
{
    long long integer = 0;
    for ( int i = 1; i <= 4799; i += i )
        for ( int j = 1; j <= 4799; j += j )
            integer ++;
    printf ( "integer = %lld\n", integer );
	return 0;
}
