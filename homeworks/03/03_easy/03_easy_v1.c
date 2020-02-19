#ifndef __PROGTEST__
#include <stdio.h>
#include <assert.h>
#define RECT_NO_OVERLAP 0
#define RECT_OVERLAP    1
#define RECT_A_IN_B     2
#define RECT_B_IN_A     3
#define RECT_ERROR    (-1)
#endif /* __PROGTEST__ */



int         swapValues                              ( int * a, int * b )
{
    if ( a == NULL || b == NULL )
        return -1;
    int temp = *a;
    *a = *b;
    *b = temp;
    return 1;
}


int         adjustRectanglesPoints                  ( int * ax, int * ay,
                                                      int * cx, int * cy )
{
    if ( ax == NULL || ay == NULL || cx == NULL || cy == NULL )
        return -1;
    if ( *ax > *cx )
        swapValues ( ax, cx );
    if ( *ay > *cy )
        swapValues ( ay, cy );
    return 1;
}

int         checkIfRectanglesOverlap                ( int ax1, int ay1,
                                                      int ax2, int ay2,
                                                      int bx1, int by1,
                                                      int bx2, int by2 )
{
    return ! ( ax1 >= bx2 || bx1 >= ax2 || ay1 >= by2 || by1 >= ay2 );
}

int         rectanglePosition                       ( int ax1, int ay1,
                                                      int ax2, int ay2,
                                                      int bx1, int by1,
                                                      int bx2, int by2 )
{
    adjustRectanglesPoints ( &ax1, &ay1, &ax2, &ay2 );
    adjustRectanglesPoints ( &bx1, &by1, &bx2, &by2 );
    int vector1 [ 2 ] = { ax2 - ax1, ay2 - ay1 };
    int vector2 [ 2 ] = { bx2 - bx1, by2 - by1 };

    if ( vector1 [ 0 ] == 0 || vector1 [ 1 ] == 0 || vector2 [ 0 ] == 0 || vector2 [ 1 ] == 0 )
        return RECT_ERROR;

    if ( ! checkIfRectanglesOverlap ( ax1, ay1, ax2, ay2, bx1, by1, bx2, by2 ) )
        return RECT_NO_OVERLAP;

    if ( ( ax1 > bx1 && ay1 > by1 && ax2 < bx2 && ay2 < by2 ) )
        return RECT_A_IN_B;
    if ( bx1 > ax1 && by1 > ay1 && bx2 < ax2 && by2 < ay2 )
        return RECT_B_IN_A;

    return RECT_OVERLAP;

//    int vector1 [ 2 ] = { ax2 - ax1, ay2 - ay1 };
//    int vector2 [ 2 ] = { bx2 - bx1, by2 - by1 };

//    int biggerRectangle = 0;
//
//    if      ( vector1 [ 0 ] > vector2 [ 0 ] && vector1 [ 1 ] > vector2 [ 1 ] )
//        biggerRectangle = 1;
//    else if ( vector1 [ 0 ] < vector2 [ 0 ] && vector1 [ 1 ] < vector2 [ 1 ] )
//        biggerRectangle = 2;

}

#ifndef __PROGTEST__
int         main                                    ( int argc, char * argv [] )
{
  assert ( rectanglePosition ( 0, 0, 50, 20,
                               10, 5, 75, 40 ) == RECT_OVERLAP );
  assert ( rectanglePosition ( 0, 20, 50, 0,
                               75, 40, 10, 5 ) == RECT_OVERLAP );
  assert ( rectanglePosition ( 0, 0, 50, 20,
                               -100, 100, 100, 90 ) == RECT_NO_OVERLAP );
  assert ( rectanglePosition ( 0, 0, 50, 20,
                               50, -100, 100, 100 ) == RECT_NO_OVERLAP );
  assert ( rectanglePosition ( 0, 0, 10, 10,
                               2, 8, 4, 6 ) == RECT_B_IN_A );
  assert ( rectanglePosition ( 2, 6, 3, 7,
                               1, 5, 4, 8 ) == RECT_A_IN_B );
  assert ( rectanglePosition ( 1, 6, 3, 7,
                               1, 5, 4, 8 ) == RECT_OVERLAP );
  assert ( rectanglePosition ( 0, 0, 1, 1,
                               1, 0, 0, 1 ) == RECT_OVERLAP );
  assert ( rectanglePosition ( 0, 0, 50, 20,
                               50, -100, 100, -100 ) == RECT_ERROR );
  return 0;
}
#endif /* __PROGTEST__ */
