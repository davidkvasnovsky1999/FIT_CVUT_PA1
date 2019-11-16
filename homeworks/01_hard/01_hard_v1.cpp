#include <stdio.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define MESSAGE_TRIANGLES_ARE_EQUAL "Trojuhelniky jsou shodne.\n"
#define MESSAGE_TRIANGLES_ARE_SIMILAR "Trojuhelniky nejsou shodne, ale jsou podobne.\n"
#define MESSAGE_TRIANGLES_ARE_NOT_EQUAL "Trojuhelniky nejsou shodne ani podobne.\n"
#define MESSAGE_NOT_A_TRIANGLE "Vstup netvori trojuhelnik.\n"
#define MESSAGE_INVALID_INPUT "Nespravny vstup.\n"
#define MESSAGE_ERROR "Vyskytla se chyba\n"

typedef struct TRIANGLE
{
	char inputDefType [ 4 ];
	long double inputValues [ 3 ];
	long double a, b, c;
//	long double alpha, beta, gamma;
} Triangle;


int readInput 							( Triangle * triangle )
{
	if ( 4 == scanf ( "%3s %Lf %Lf %Lf", triangle->inputDefType, &( triangle->inputValues [ 0 ] ) ,  &( triangle->inputValues [ 1 ] ),  &( triangle->inputValues [ 2 ] ) ) )
	{
		while ( 1 ) //check for possible not numeric input that that follows the last input number stored in triangle [ 2 ]
		{
			char c = ( char ) fgetc ( stdin );
			if ( c == '\n' || feof ( stdin ) )
				break;
			else if ( c != 32 && c != 9 ) //If read char is not Horizontal tab or Space
				return 0;
		}
		if 		( !( triangle->inputValues [ 0 ] > 0 && triangle->inputValues [ 1 ] > 0 && triangle->inputValues [ 2 ] > 0 ) )//Check if values are grater than 0
			return 0;
		if 		( strcmp ( triangle->inputDefType, "SUS" ) == 0 )
		{
			if ( triangle->inputValues [ 1 ] >= 180 )//Check if angle input value is less than 180 degrees
				return 0;
		}
		else if ( strcmp ( triangle->inputDefType, "USU" ) == 0 )
		{
			if ( triangle->inputValues [ 0 ] >= 180 || triangle->inputValues [ 2 ] >= 180 )//Check if angle input values are less than 180 degrees
				return 0;
		}
		else if ( strcmp ( triangle->inputDefType, "SSS" ) != 0 )//If inputDefType does not equal to any possible valid triangle definition type (SSS, SUS, USU)
			return 0;
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

int convertDegreesToRadians				( long double * ptrToDegreeValue )
{
	if ( ptrToDegreeValue == NULL )
		return 0;
	long double radians = *ptrToDegreeValue * acos ( -1 ) / 180;
	*ptrToDegreeValue = radians;
	return 1;
}


int calculateRemainingSides   			( Triangle * triangle )
//Return 1 if success. Return 0 if values do not compose triangle.
{
	long double a;
	long double b;
	long double c;
	if 		( strcmp ( triangle->inputDefType, "SSS" ) == 0 )
	{
		a = triangle->inputValues [ 0 ];
		b = triangle->inputValues [ 1 ];
		c = triangle->inputValues [ 2 ];
	}
	else if ( strcmp ( triangle->inputDefType, "SUS" ) == 0 )
	{
		//Calculation may be bad in this block.
		b = triangle->inputValues [ 0 ];
		c = triangle->inputValues [ 2 ];
		long double alpha 	= triangle->inputValues [ 1 ];
		convertDegreesToRadians ( &alpha );
		a = sqrtl ( b*b + c*c - 2*b*c * cosl ( alpha ) );
	}
	else if ( strcmp ( triangle->inputDefType, "USU" ) == 0 )
	{
		//Bad calculation in this block!
		long double alpha 	= triangle->inputValues [ 0 ];
		long double beta 	= triangle->inputValues [ 2 ];
		long double gamma 	= 180 - alpha - beta;
		convertDegreesToRadians ( &alpha );
		convertDegreesToRadians ( &beta );
		convertDegreesToRadians ( &gamma );
		c = triangle->inputValues [ 1 ];
		b = sinl ( beta )  * c / sinl ( gamma );
		a = sinl ( alpha ) * c / sinl ( gamma );
	}
	else
		return 0;
	
	triangle->a = a;
	triangle->b = b;
	triangle->c = c;
	orderTrianglesSides ( triangle );
	return triangle->a + triangle->b - triangle->c > LDBL_EPSILON * ( triangle->a + triangle->b + triangle->c );
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
		if ( calculateRemainingSides ( &( triangles [ i ] ) ) != 1 )
		{
			printf ( MESSAGE_NOT_A_TRIANGLE );
			return 1;
		}
//		printf ( "Delky stran trojuhelniku #%d:\n" "%.*Lf %.*Lf %.*Lf\n", i + 1,
//				DECIMAL_DIG, triangles [ i ].a,
//				DECIMAL_DIG, triangles [ i ].b,
//				DECIMAL_DIG, triangles [ i ].c );//for debug
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