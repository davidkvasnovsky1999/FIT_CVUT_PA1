#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define true 1
#define false 0

typedef unsigned char boolean;

typedef struct Plate
{
    unsigned long long      x,
                            y;
} Plate;

typedef struct PlatesContainer
{
    unsigned long long      numberOfPlates,
                            numberOfAllocatedPlates,
                            platesMaxArea;
    Plate                 * plates,
                          * rootPlate;
} PlatesContainer;

void                freePlatesContainer                     ( PlatesContainer          ** pPlatesContainer )
{
    if ( pPlatesContainer != NULL && *pPlatesContainer != NULL )
    {
        free ( (*pPlatesContainer)->rootPlate );
        (*pPlatesContainer)->rootPlate = NULL;
        free ( (*pPlatesContainer)->plates );
        (*pPlatesContainer)->plates = NULL;
        free ( *pPlatesContainer );
        *pPlatesContainer = NULL;
    }
}

PlatesContainer *   getInitializedPlatesContainer           (  )
{
    PlatesContainer * pPlatesContainer = NULL;
    if
    (
            ( pPlatesContainer              = ( PlatesContainer * ) malloc ( sizeof ( pPlatesContainer ) ) ) == NULL
            ||
            ( pPlatesContainer->plates      = ( Plate * )           malloc ( sizeof ( Plate ) ) )            == NULL
            ||
            ( pPlatesContainer->rootPlate   = ( Plate * )           malloc ( sizeof ( Plate ) ) )            == NULL
    )
            exit ( 1 );
    pPlatesContainer->platesMaxArea             = -1;
    pPlatesContainer->numberOfAllocatedPlates   = 1;
    pPlatesContainer->numberOfPlates            = 0;
    return pPlatesContainer;
}

int                getRootPlate                            ( Plate                     ** rootPlate )
{
    if ( rootPlate == NULL )
        return 0;
    if ( ( *rootPlate == NULL && ( *rootPlate = ( Plate * ) malloc ( sizeof ( Plate ) ) ) == NULL ) )
        exit ( 1 );
    printf ( "Velikost:\n" );
    return 2 == scanf ( "%lld %lld", &( ( *rootPlate )->x ), &( ( *rootPlate )->y ) );
}

long long           getMaxArea                              (  )
{
    long long maxArea = 0;
    printf ( "Maximalni plocha:\n" );
    scanf ( "%llu", &maxArea );
    return maxArea;
}

int                 getInputData                            ( PlatesContainer          ** pPlatesContainer )
{
    if ( pPlatesContainer == NULL )
        return 0;
    if ( ( *pPlatesContainer = getInitializedPlatesContainer ( ) ) == NULL )
        exit ( 1 );
    if ( getRootPlate ( &((*pPlatesContainer)->rootPlate) ) == 0 )
        return 0;
    if ( ( (*pPlatesContainer)->platesMaxArea = getMaxArea ( ) ) < 1 )
        return 0;
    return 1;
}

int                 splitPlate                              ( PlatesContainer           * pPlatesContainer,
                                                              long long                   x,
                                                              long long                   y )
{
    int numberOfSplits = 0;
    if ( x * y <= pPlatesContainer->platesMaxArea )
    {
        
        return numberOfSplits;
    }
    else
    {
//        long long x, y;
////        Plate * cutOffPlate = ( Plate * ) malloc ( sizeof ( cutOffPlate ) );
////        if ( cutOffPlate == NULL )
////            exit ( 0 );
//        do
//        {
//            cutOffPlate =
//        }
//        while ( plate->x / plate->y <= 2 );
    }
}

int                 main                                    (  )
{
    PlatesContainer * platesContainer = NULL;
    if ( getInputData ( &platesContainer ) > 0 )
    {
    
    }
    else
        printf ( "Nespravny vstup.\n" );
    freePlatesContainer ( &platesContainer );
    return 0;
}