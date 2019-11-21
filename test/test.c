#include <stdio.h>

int main ( void )
{
//	int a = 2, b = 1;
//	scanf ( "%d %d", &a, &b );
//	a = a + b;
//	b = a - b;
//	a = a - b;
unsigned long long ull = 0;
	if ( scanf ( "%llu", &ull ) == 1 )
	    printf ( "%llu\n", ull );
	else
        printf ( "Error\n" );
	return 0;
}
