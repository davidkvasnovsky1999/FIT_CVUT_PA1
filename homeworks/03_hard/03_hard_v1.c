#ifndef __PROGTEST__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#endif /* __PROGTEST__ */


typedef struct PITP
{
    double x, y;
} PointITP; //Point in the plane


//typedef struct LS
//{
//    PointITP firstPoint;
//    PointITP secondPoint;
//} LineSegment;

typedef struct RECTANGLE
{
    PointITP A;
    PointITP B;
    PointITP C;
    PointITP D;
} Rectangle;


//LineSegment        createLineSegmentFrom                   ( double            x1,
//                                                             double            y1,
//                                                             double            x2,
//                                                             double            y2 )
//{
//    LineSegment lineSegment;
//    lineSegment.firstPoint.x = x1;
//    lineSegment.firstPoint.y = y1;
//    lineSegment.secondPoint.y = x2;
//    lineSegment.secondPoint.y = y2;
//    return lineSegment;
//}

PointITP           createPointFrom                         ( const double      x,
                                                             const double      y )
{
    PointITP point;
    point.x = x;
    point.y = y;
    return point;
}

int                swapValues                              ( double          * a,
                                                             double          * b )
{
    if ( a == NULL || b == NULL )
        return -1;
    double temp = *a;
    *a = *b;
    *b = temp;
    return 1;
}

Rectangle          createRectangle                         ( double            rx1,
                                                             double            ry1,
                                                             double            rx2,
                                                             double            ry2 )
{
    Rectangle   rectangle;
    if ( rx1 > rx2 )
        swapValues ( &rx1, &rx2 );
    if ( ry1 > ry2 )
        swapValues ( &ry1, &ry2 );
    rectangle.A = createPointFrom ( rx1, ry1 );
    rectangle.B = createPointFrom ( rx2, ry1 );
    rectangle.C = createPointFrom ( rx2, ry2 );
    rectangle.D = createPointFrom ( rx1, ry2 );
    return rectangle;
}

int                checkIfPointIsInsideOfRectangle         ( const Rectangle   rectangle,
                                                             const PointITP    point )
{
    return point.x > rectangle.A.x && point.x < rectangle.B.x
        && point.y > rectangle.B.y && point.y < rectangle.C.y;
}

int                checkIfTwoLineSegmentsIntersect         ( PointITP          firstLSsPoints  [ 2 ],
                                                             PointITP          secondLSsPoints [ 2 ],
                                                             PointITP        * pointOfIntersection )
//Returns 1 is they do, returns 0 if they do not, returns -1 if an error occurred.
{
    if ( firstLSsPoints == NULL || secondLSsPoints == NULL )
        return -1;

    double s1_x, s1_y, s2_x, s2_y, s, t, denominator;
    s1_x = firstLSsPoints  [ 1 ].x - firstLSsPoints  [ 0 ].x;
    s1_y = firstLSsPoints  [ 1 ].y - firstLSsPoints  [ 0 ].y;
    s2_x = secondLSsPoints [ 1 ].x - secondLSsPoints [ 0 ].x;
    s2_y = secondLSsPoints [ 1 ].y - secondLSsPoints [ 0 ].y;
    
//    s = ( -s1_y * ( firstLSsPoints [ 0 ].x - secondLSsPoints [ 0 ].x ) + s1_x * ( firstLSsPoints [ 0 ].y - secondLSsPoints [ 0 ].y ) ) / ( -s2_x * s1_y + s1_x * s2_y );
//    t = (  s2_x * ( firstLSsPoints [ 0 ].y - secondLSsPoints [ 0 ].y ) - s2_y * ( firstLSsPoints [ 0 ].x - secondLSsPoints [ 0 ].x ) ) / ( -s2_x * s1_y + s1_x * s2_y );
    denominator = ( ( secondLSsPoints [ 1 ].x - secondLSsPoints [ 0 ].x ) * ( firstLSsPoints  [ 0 ].y - firstLSsPoints  [ 1 ].y )
                  - ( firstLSsPoints  [ 0 ].x - firstLSsPoints  [ 1 ].x ) * ( secondLSsPoints [ 1 ].y - secondLSsPoints [ 0 ].y ) );

    s = ( ( secondLSsPoints [ 0 ].y - secondLSsPoints [ 1 ].y ) * ( firstLSsPoints  [ 0 ].x - secondLSsPoints [ 0 ].x )
        + ( secondLSsPoints [ 1 ].x - secondLSsPoints [ 0 ].x ) * ( firstLSsPoints  [ 0 ].y - secondLSsPoints [ 0 ].y ) )
        / denominator;

    t = ( ( firstLSsPoints  [ 0 ].y - firstLSsPoints  [ 1 ].y ) * ( firstLSsPoints  [ 0 ].x - secondLSsPoints [ 0 ].x )
        + ( firstLSsPoints  [ 1 ].x - firstLSsPoints  [ 0 ].x ) * ( firstLSsPoints  [ 0 ].y - secondLSsPoints [ 0 ].y ) )
        / denominator;

    if ( s >= 0 && s <= 1 && t >= 0 && t <= 1 )
    {
        // Collision detected
        if ( pointOfIntersection != NULL )
        {
            pointOfIntersection->x = firstLSsPoints  [ 0 ].x + ( t * s1_x );
            pointOfIntersection->y = firstLSsPoints  [ 0 ].y + ( t * s1_y );
        }
        return 1;
    }
    return 0;
}


int                checkIfNumbersHaveOppositeSign          ( double            x1,
                                                             double            x2 )
{
    return ( x1 < 0 ) ? ( x2 >= 0 ) : ( x2 < 0 );
}

int                findIntersections                       ( Rectangle         rectangle,
                                                             PointITP          lineSegmentsPoints [ 2 ],
                                                             PointITP        * pointsOfIntersection )
// returns number of intersections
{
    if ( lineSegmentsPoints == NULL || pointsOfIntersection == NULL )
        return 0;
    PointITP rectanglesSides [ 4 ] [ 2 ] = { { rectangle.A, rectangle.B }, { rectangle.B, rectangle.C },
                                             { rectangle.D, rectangle.C }, { rectangle.A, rectangle.D } };
    short numOfIntersections = 0, i = 0;
    while ( numOfIntersections < 2 && i < 4 )
    {
        PointITP pointOfIntersection;
        if ( checkIfTwoLineSegmentsIntersect ( lineSegmentsPoints, rectanglesSides [ i ], &pointOfIntersection ) )
            pointsOfIntersection [ numOfIntersections ++ ] = pointOfIntersection;
        i ++;
    }
    return numOfIntersections;
}


int                clipLine                                ( double            rx1,
                                                             double            ry1,
                                                             double            rx2,
                                                             double            ry2,
                                                             double          * ax,
                                                             double          * ay,
                                                             double          * bx,
                                                             double          * by )
{
//    const Rectangle rectangle = createRectangle ( rx1, ry1, rx2, ry2 );
    PointITP lineSegmentsPoints [ 2 ] = { createPointFrom ( *ax, *ay ), createPointFrom ( *bx, *by ) };
    PointITP pointsOfIntersection [ 2 ];
//    PointITP rectanglePoints [ 4 ];
//    createPointsForRectangle ( rx1, ry1, rx2, ry2, rectanglePoints );
    Rectangle rectangle = createRectangle ( rx1, ry1, rx2, ry2 );
    int pointsAreInside [ 2 ]  = { checkIfPointIsInsideOfRectangle ( rectangle, lineSegmentsPoints [ 0 ] ),
                                   checkIfPointIsInsideOfRectangle ( rectangle, lineSegmentsPoints [ 1 ] ) };

    if ( pointsAreInside [ 0 ] && pointsAreInside [ 1 ] )
        return 1;

    short numOfIntersections = ( short ) findIntersections ( rectangle, lineSegmentsPoints, pointsOfIntersection );
    if ( numOfIntersections == 1 )
    {
        if ( pointsAreInside [ 0 ] == 0 )
        {
            *ax = pointsOfIntersection->x;
            *ay = pointsOfIntersection->y;
        }
        else
        {
            *bx = pointsOfIntersection->x;
            *by = pointsOfIntersection->y;
        }
        return 1;
    }
    else if ( numOfIntersections == 2 )
    {
        double newVector [ 2 ] = { pointsOfIntersection [ 1 ].x - pointsOfIntersection [ 0 ].x, pointsOfIntersection [ 1 ].y - pointsOfIntersection [ 0 ].y };
        double oldVector [ 2 ] = { *bx - *ax, *by - *ay };
        if ( checkIfNumbersHaveOppositeSign ( newVector [ 0 ], oldVector [ 0 ] )
          || checkIfNumbersHaveOppositeSign ( newVector [ 1 ], oldVector [ 1 ] ) )
        {
            *ax = pointsOfIntersection [ 1 ].x;
            *ay = pointsOfIntersection [ 1 ].y;
            *bx = pointsOfIntersection [ 0 ].x;
            *by = pointsOfIntersection [ 0 ].y;
        }
        else
        {
            *ax = pointsOfIntersection [ 0 ].x;
            *ay = pointsOfIntersection [ 0 ].y;
            *bx = pointsOfIntersection [ 1 ].x;
            *by = pointsOfIntersection [ 1 ].y;
        }
        return 1;
    }
    else
        return 0;
}

#ifndef __PROGTEST__
int                almostEqual                             ( double            x,
                                                             double            y )
{
    return fabs( x - y ) < DBL_EPSILON * x * y;
}

int                main                                    ( void )
{
    double x1, y1, x2, y2;

//    x1 = 60;
//    y1 = 40;
//    x2 = 70;
//    y2 = 50;
//    assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
//             && almostEqual ( x1, 60 )
//             && almostEqual ( y1, 40 )
//             && almostEqual ( x2, 70 )
//             && almostEqual ( y2, 50 ) );
//
//    x1 = 0;
//    y1 = 50;
//    x2 = 20;
//    y2 = 30;
//    assert ( clipLine ( 90, 100, 10, 20, &x1, &y1, &x2, &y2 )
//             && almostEqual ( x1, 10 )
//             && almostEqual ( y1, 40 )
//             && almostEqual ( x2, 20 )
//             && almostEqual ( y2, 30 ) );
//
//    x1 = 0;
//    y1 = 30;
//    x2 = 120;
//    y2 = 150;
//    assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
//             && almostEqual ( x1, 10 )
//             && almostEqual ( y1, 40 )
//             && almostEqual ( x2, 70 )
//             && almostEqual ( y2, 100 ) );
//
//    x1 = -10;
//    y1 = -10;
//    x2 = -20;
//    y2 = -20;
//    assert ( ! clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 ) );
//
//    x1 = 0;
//    y1 = 30;
//    x2 = 20;
//    y2 = 10;
//    assert ( clipLine ( 10, 20, 90, 100, &x1, &y1, &x2, &y2 )
//             && almostEqual ( x1, 10 )
//             && almostEqual ( y1, 20 )
//             && almostEqual ( x2, 10 )
//             && almostEqual ( y2, 20 ) );

    x1 = 0;
    y1 = 0.3553;
    x2 = 10.45;
    y2 = 0;
    assert ( clipLine ( 0.95, 0.323, 1, 1, &x1, &y1, &x2, &y2 )
             && almostEqual ( x1, 0.95 )
             && almostEqual ( y1, 0.323 )
             && almostEqual ( x2, 0.95 )
             && almostEqual ( y2, 0.323 ) );
    return 0;
}
#endif /* __PROGTEST__ */
