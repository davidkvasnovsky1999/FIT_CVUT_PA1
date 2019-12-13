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
    struct Plate         ** children;
} Plate;

typedef struct PlatesContainer
{
    unsigned long long      numberOfCuts,
                            platesMaxArea;
    Plate                 * rootPlate;
} PlatesContainer;

void                freePlatesRecursively                   ( Plate                    ** plate )
{
    if ( plate != NULL && *plate != NULL )
    {
        if ( (*plate)->children != NULL )
        {
            freePlatesRecursively ( (*plate)->children );
            freePlatesRecursively ( (*plate)->children + 1 );
        }
        free ( *plate );
        *plate = NULL;
    }
}

void                freePlatesContainer                     ( PlatesContainer          ** pPlatesContainer )
{
    if ( pPlatesContainer != NULL && *pPlatesContainer != NULL )
    {
        freePlatesRecursively ( &((*pPlatesContainer)->rootPlate) );
//        free ( (*pPlatesContainer)->plates );
//        (*pPlatesContainer)->plates = NULL;
        free ( *pPlatesContainer );
        *pPlatesContainer = NULL;
    }
}

void                copyPlatesValuesToAnother               ( Plate                     * source,
                                                              Plate                     * target )
{
    if ( source != NULL && target != NULL )
    {
        target->x = source->x;
        target->y = source->y;
        target->children = source->children;
    }
}

void                swapULLs                                ( unsigned long long        * a,
                                                              unsigned long long        * b )
{
    if ( a != NULL && b != NULL )
    {
        unsigned long long temp = *a;
        *a = *b;
        *b = temp;
    }
}

PlatesContainer *   getInitializedPlatesContainer           (  )
{
    PlatesContainer * pPlatesContainer = NULL;
    if
    (
            ( pPlatesContainer              = ( PlatesContainer * ) malloc ( sizeof ( pPlatesContainer ) ) ) == NULL
            ||
//            ( pPlatesContainer->plates      = ( Plate * )           malloc ( sizeof ( Plate ) ) )            == NULL
//            ||
            ( pPlatesContainer->rootPlate   = ( Plate * )           malloc ( sizeof ( Plate ) ) )            == NULL
    )
            exit ( 1 );
    pPlatesContainer->platesMaxArea             = -1;
//    pPlatesContainer->numberOfAllocatedPlates   = 1;
    pPlatesContainer->numberOfCuts            = 0;
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

char                compareLongDoubles                      ( const long double           a,
                                                              const long double           b )
{
    if ( fabsl ( a - b ) < 1024 * LDBL_EPSILON * a * b )
        return 0;
    return a > b ? -1 : 1;
}

long double         getPlatesAspectRatio                    ( const Plate                 plate )
{
    if ( plate.x > plate.y )
        return ( long double ) plate.x / ( long double ) plate.y;
    else
        return ( long double ) plate.y / ( long double ) plate.x;
}

unsigned long long  splitPlate                              ( const unsigned long long    platesMaxArea,
                                                              const long double           platesMaxAspectRatio,
                                                              Plate                     * plateToCut )
{
    if ( plateToCut->x * plateToCut->y <= platesMaxArea
    && compareLongDoubles ( getPlatesAspectRatio ( *plateToCut ), platesMaxAspectRatio ) != -1 )
        return 0;
    else
    {
        const char          numberOfChildren    = 2;
        unsigned long long  numberOfSplits      = -1,
                            tempNumOfSplits     = 0;
        Plate ** children = ( Plate ** ) malloc ( sizeof ( Plate * ) * numberOfChildren );
        *( children + 0 )  = ( Plate * ) malloc ( sizeof ( Plate ) );
        *( children + 1 )  = ( Plate * ) malloc ( sizeof ( Plate ) );
        for ( char i = 0; i < numberOfChildren; i ++ )
        {
            swapULLs ( &(plateToCut->x), &(plateToCut->y) );
            for ( unsigned long long j = 1; j <= ( plateToCut->x / 2 ); j ++  )
            {
                Plate * tempPlateA = ( Plate * ) malloc ( sizeof ( Plate ) );
                Plate * tempPlateB = ( Plate * ) malloc ( sizeof ( Plate ) );
                copyPlatesValuesToAnother ( plateToCut, tempPlateA );
                copyPlatesValuesToAnother ( plateToCut, tempPlateB );
                (*( plateToCut->children + 0 ))->x -= 1;
                (*( plateToCut->children + 1 ))->x = j;
                tempNumOfSplits = splitPlate ( platesMaxArea, platesMaxAspectRatio, tempPlateA )
                                + splitPlate ( platesMaxArea, platesMaxAspectRatio, tempPlateB ) + 1;
                if ( tempNumOfSplits < numberOfSplits )
                {
                    freePlatesRecursively ( children );
                    freePlatesRecursively ( children + 1 );
                    numberOfSplits = tempNumOfSplits;
                    *( children + 0 ) = tempPlateA;
                    *( children + 1 ) = tempPlateB;
                }
                else
                {
                    freePlatesRecursively ( &tempPlateA );
                    freePlatesRecursively ( &tempPlateB );
                }
            }
        }
        plateToCut->children = children;
        return numberOfSplits;
    }
}

int                 main                                    (  )
{
    PlatesContainer * platesContainer = NULL;
    if ( getInputData ( &platesContainer ) > 0 )
    {
        platesContainer->numberOfCuts = splitPlate ( platesContainer->platesMaxArea, 2 ,platesContainer->rootPlate );
    }
    else
        printf ( "Nespravny vstup.\n" );
    freePlatesContainer ( &platesContainer );
    return 0;
}