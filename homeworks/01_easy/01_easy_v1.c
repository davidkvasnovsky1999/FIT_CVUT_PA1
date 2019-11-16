#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define MESSAGE_TRIANGLES_ARE_EQUAL "Trojuhelniky jsou shodne.\n"
#define MESSAGE_TRIANGLES_ARE_SIMILAR "Trojuhelniky nejsou shodne, ale jsou podobne.\n"
#define MESSAGE_TRIANGLES_ARE_NOT_EQUAL "Trojuhelniky nejsou shodne ani podobne.\n"
#define MESSAGE_NOT_A_TRIANGLE "Strany netvori trojuhelnik.\n"
#define MESSAGE_INVALID_INPUT "Nespravny vstup.\n"
#define MESSAGE_ERROR "Vyskytla se chyba\n"

typedef struct TRIANGLE
{
	long double a, b, c;
} Triangle;


int readInput 							( Triangle * triangle )
{
	if ( 3 == scanf ( "%Lf %Lf %Lf", &( triangle->a ) ,  &( triangle->b ),  &( triangle->c ) ) )
	{
		while ( 1 ) //check for possible not numeric input that that follows the last input number stored in triangle [ 2 ]
		{
			char c = ( char ) fgetc ( stdin );
			if ( c == '\n' || feof ( stdin ) )
				break;
			else if ( c != 32 && c != 9 ) //If read char is not Horizontal tab or Space
				return 0;
		}
		if ( triangle->a > 0 && triangle->b > 0 && triangle->c > 0 )
			return 1;
	}
	return 0;
}

int switchValues 						( long double * a,
										  long double * b )
{
	if ( a == NULL || b == NULL )
		return 0;
//	*a = (*a) + (*b);
//	*b = (*a) - (*b);
//	*a = (*a) - (*b);
	long double temp = *a;
	*a = *b;
	*b = temp;
	return 1;
}

int orderTrianglesSides 				( Triangle * triangle ) //order of sides: a < b < c
{
	if ( triangle == NULL )
		return 0;
	if ( triangle->a > triangle->c )
		switchValues ( &( triangle->a ), &( triangle->c ) );
	if ( triangle->b > triangle->c )
		switchValues ( &( triangle->b ), &( triangle->c ) );
	if ( triangle->a > triangle->b )
		switchValues ( &( triangle->a ), &( triangle->b ) );
	return 1;
}

int checkIfSidesComposeTriangle 		( const Triangle triangle )
{
	long double res1 = triangle.a + triangle.b - triangle.c;
	long double res2 = LDBL_EPSILON * ( triangle.a + triangle.b + triangle.c );
	return res1 > res2;
}

int checkIfSidesRatioIsTheSame			( const Triangle triangles [ 2 ] )
//Returns 1 if same. Returns 0 if different. Returns -1 if an error occurred.
{
	if ( triangles == NULL )
		return -1;
	long double aSideRatio = triangles [ 0 ].a / triangles [ 1 ].a;
	long double bSideRatio = triangles [ 0 ].b / triangles [ 1 ].b;
	long double cSideRatio = triangles [ 0 ].c / triangles [ 1 ].c;
	if ( fabsl ( aSideRatio - bSideRatio ) <= 1024 * LDBL_EPSILON * triangles [ 0 ].b * triangles [ 0 ].b
	  && fabsl ( bSideRatio - cSideRatio ) <= 1024 * LDBL_EPSILON * triangles [ 0 ].c * triangles [ 0 ].c )
		return 1;
	return 0;
}

int compareTriangles					( const Triangle triangles [ 2 ] )
//Returns 2 if equal. Returns 1 if similar. Returns 0 if different. Returns -1 if an error occurred.
{
	if ( triangles == NULL )
		return -1;
	if ( checkIfSidesRatioIsTheSame ( triangles ) )
	{
		if ( fabsl ( triangles [ 0 ].c - triangles [ 1 ].c ) <= 1024 * LDBL_EPSILON * triangles [ 0 ].c * triangles [ 0 ].c )
			return 2;
		return 1;
	}
	return 0;
}

int main 								( void )
{
	Triangle triangles [ 2 ] ;
	for ( int i = 0; i < 2; i ++ )
	{
		printf ( "Trojuhelnik #%d:\n", i + 1 );
		if ( readInput ( &( triangles [ i ] ) ) != 1 )
		{
			printf ( MESSAGE_INVALID_INPUT );
			return 1;
		}
		orderTrianglesSides ( &( triangles [ i ] ) );
//		printf ( "Delky stran trojuhelniku #%d:\n" "%.*Lf %.*Lf %.*Lf\n", i + 1,
//				DECIMAL_DIG, triangles [ i ].a,
//				DECIMAL_DIG, triangles [ i ].b,
//				DECIMAL_DIG, triangles [ i ].c );//for debug
		if ( checkIfSidesComposeTriangle ( triangles [ i ] ) != 1 )
		{
			printf ( MESSAGE_NOT_A_TRIANGLE );
			return 1;
		}
	}
	switch ( compareTriangles ( triangles ) )
	{
		case 0:
			printf ( MESSAGE_TRIANGLES_ARE_NOT_EQUAL );
			break;
		case 1:
			printf ( MESSAGE_TRIANGLES_ARE_SIMILAR );
			break;
		case 2:
			printf ( MESSAGE_TRIANGLES_ARE_EQUAL );
			break;
		default:
			printf ( MESSAGE_ERROR );
	}
    return 0;
}