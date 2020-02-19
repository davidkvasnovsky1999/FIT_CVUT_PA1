#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MESSAGE_INVALID_INPUT "Nespravny vstup.\n"
#define MESSAGE_ERROR "Vyskytla se chyba\n"

int readInput 							( char 		*   		operation,
										  short 	*			base,
										  long long * 			low,
										  long long * 			high )
{
	int ret = scanf ( "%c %hi %lli %lli", operation, base, low, high );
	if ( 4 == ret )
	{
		while ( 1 ) //check for possible not numeric input that that follows
		{
			char c = ( char ) fgetc ( stdin );
			if ( c == '\n' || feof ( stdin ) )
				break;
			else if ( c != 32 && c != 9 && c != '\r' ) //If read char is not Horizontal tab or Space
				return 0;
		}
		if ( ( *operation != 'c' && *operation != 'l' ) || *base < 2 || *base > 36 || *low < 0 || *low > *high )
			return 0;
		return 1;
	}
	else if ( ret == EOF )
		return -1;
	else
		return 0;
}

long long int createPalindrome			( long long 			input,
										  short 				base,
										  int					isOdd )
{
	long long n 	 	 = input;
	long long palindrome = input;
	
	if ( isOdd != 0 )
		n /= base;
	
	while ( n > 0 )
	{
		palindrome = palindrome * base + ( n % base );
		n /= base;
	}
	return palindrome;
}

unsigned long long * generatePalindromes( long long				low,
										  long long 			high,
										  short 				base,
										  unsigned long long * 	numberOfPalindromes )
{
	*numberOfPalindromes 	= 0;
	long long 				palindrome;
	unsigned long long 		sizeOfPalindromeArray 	= 4;
	unsigned long long * 	palindromes 			= ( unsigned long long * ) malloc ( sizeOfPalindromeArray * sizeof ( unsigned long long ) );
	// Run two times for odd and even length palindromes
	for ( short j = 0; j < 2; j++ )
	{
		// Creates palindrome numbers with first half as i.
		// Value of j decided whether we need an odd length
		// of even length palindrome.
		long long i = 0;
		while ( ( palindrome = createPalindrome ( i, base, j % 2 ) ) <= high )
		{
			if ( ( palindrome != 0 || j == 0 ) && palindrome >= low )
			{
				( *numberOfPalindromes ) ++;
				if ( *numberOfPalindromes > sizeOfPalindromeArray )
				{
					sizeOfPalindromeArray *= 2;
					palindromes = ( unsigned long long * ) realloc ( palindromes, sizeOfPalindromeArray * sizeof ( unsigned long long ) );
				}
				palindromes [ ( *numberOfPalindromes ) - 1 ] = palindrome;
			}
			i ++;
		}
	}
	return palindromes;
}

int cmpfunc 							( const void * 			a,
										  const void *			b )
{
	return ( *(int*)a - *(int*)b );
}

char ulonglongToChar					( unsigned long long	number )
{
	if ( number >= 0 && number <= 9 )
		return ( char ) ( number + '0' );
	else
		return ( char ) ( number - 10 + 'a' );
}

void reverseString						( char * 				string )
{
	unsigned long length = strlen ( string ), i = 0;
	for ( ; i < length / 2; i ++ )
	{
		char temp = string [ i ];
		string [ i ] = string [ length - i - 1 ];
		string [ length - i - 1 ] = temp;
	}
}

char * convertLongToOtherNumeralSystem 	( unsigned long long 	value,
										  short 				resultsBase )
{
	if ( value < 0 )
		return NULL;
	int resultsLength = 16;
	char * result = ( char * ) malloc ( resultsLength );
	int index = 0;
	
	if ( value == 0 )
		result [ index ++ ] = '0';
	
	while ( value > 0 )
	{
		if ( resultsLength <= index )
		{
			resultsLength *= 2;
			result = ( char * ) realloc ( result, resultsLength );
		}
		result [ index ++ ] = ulonglongToChar ( value % resultsBase );
		value /= resultsBase;
	}
	result [ index ] = '\0';
//	reverseString ( result );
	return result;
}


int main 								( void )
{
	printf ( "Vstupni intervaly:\n" );
	while ( feof ( stdin ) == 0 )
	{
		char 		operation;
		long long 	low, high;
		short 		base;
		int ret = readInput ( &operation, &base, &low, &high );
		if ( ret == 1 )
		{
			unsigned long long numberOfPalindromes;
			unsigned long long * palindromes = generatePalindromes ( low, high, base, &numberOfPalindromes );
			if ( operation == 'c' )
				printf ( "Celkem: %lld\n", numberOfPalindromes );
			else
			{
				qsort ( palindromes, numberOfPalindromes, sizeof ( long long ), cmpfunc );
				for ( unsigned long long i = 0; i < numberOfPalindromes; i ++ )
					printf ( "%lld = %s (%hd)\n", palindromes [ i ], convertLongToOtherNumeralSystem ( palindromes [ i ], base ), base );
			}
			free ( palindromes );
		}
		else if ( ret == 0 )
		{
			printf ( MESSAGE_INVALID_INPUT );
			return 1;
		}
		else
			return 0;
	}
	return 0;
}