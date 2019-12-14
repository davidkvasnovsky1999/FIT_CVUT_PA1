#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define NUMBER_OF_PLATES_CHILDREN 2
#define MAX_PLATES_ASPECT_RATIO 2
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
            free ( (*plate)->children );
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

void                copyPlatesValuesFrom1stTo2nd            ( Plate                     * source,
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
    if ( ( pPlatesContainer = ( PlatesContainer * ) malloc ( sizeof ( PlatesContainer ) ) ) == NULL )
            exit ( 1 );
    pPlatesContainer->platesMaxArea = -1;
    pPlatesContainer->numberOfCuts  = 0;
    pPlatesContainer->rootPlate     = NULL;
    return pPlatesContainer;
}

int                getRootPlate                            ( Plate                     ** rootPlate )
{
    if ( rootPlate == NULL )
        return 0;
    if ( ( *rootPlate == NULL && ( *rootPlate = ( Plate * ) malloc ( sizeof ( Plate ) ) ) == NULL ) )
        exit ( 1 );
    (*rootPlate)->children = NULL;
    printf ( "Velikost:\n" );
    long long x, y;
    if ( scanf ( "%lld %lld", &x, &y ) != 2 || x < 1 || y < 0 )
        return 0;
    ( *rootPlate )->x = x;
    ( *rootPlate )->y = y;
    return 1;
}

char                getMaxArea                              ( unsigned long long        * pMaxArea )
{
    if ( pMaxArea == NULL )
        return 0;
    long long tempMaxArea = 0;
    printf ( "Maximalni plocha:\n" );
    scanf ( "%lld", &tempMaxArea );
    if ( tempMaxArea < 1 )
        return 0;
    *pMaxArea = ( unsigned long long ) tempMaxArea;
    return 1;
}

int                 getInputData                            ( PlatesContainer          ** pPlatesContainer )
{
    if ( pPlatesContainer == NULL )
        return 0;
    if ( ( *pPlatesContainer = getInitializedPlatesContainer ( ) ) == NULL )
        exit ( 1 );
    if ( getRootPlate ( &( (*pPlatesContainer)->rootPlate     ) ) != 1
      || getMaxArea   ( &( (*pPlatesContainer)->platesMaxArea ) ) != 1 )
        return 0;
    return 1;
}

char                compareLongDoubles                      ( const long double           a,
                                                              const long double           b )
{
    if ( fabsl ( a - b ) < 1024 * LDBL_EPSILON * a * b )
        return 0;
    return a > b ? 1 : -1;
}

long double         getPlatesAspectRatio                    ( const Plate                 plate )
{
    unsigned long long  x = plate.x,
                        y = plate.y;
    if ( plate.x < plate.y )
        swapULLs ( &x, &y );
    return ( long double ) x / ( long double ) y;
}

unsigned long long  splitPlate                              ( const unsigned long long    platesMaxArea,
                                                              const long double           platesMaxAspectRatio,
                                                              Plate                     * plateToCut )
{
    if ( plateToCut->x * plateToCut->y <= platesMaxArea
    && compareLongDoubles ( getPlatesAspectRatio ( *plateToCut ), platesMaxAspectRatio ) != 1 )
        return 0;
    else
    {
        unsigned long long  numberOfSplits      = -1,
                            tempNumOfSplits     = 0;
        Plate ** children       = ( Plate ** ) malloc ( sizeof ( Plate * ) * NUMBER_OF_PLATES_CHILDREN );
        Plate ** tempChildren   = ( Plate ** ) malloc ( sizeof ( Plate * ) * NUMBER_OF_PLATES_CHILDREN );
        for ( char i = 0; i < NUMBER_OF_PLATES_CHILDREN; i ++ )
            *( children + i ) = NULL;
        for ( char i = 0; i < 2; i ++ ) //In first and second iteration we cut plate by y and x dimension respectively.
        {
            for ( unsigned long long j = 0; j < ( plateToCut->x / 2 ); j ++  )
            {
                for ( char k = 0; k < NUMBER_OF_PLATES_CHILDREN; k ++ )
                {
                    *( tempChildren + k ) = ( Plate * ) malloc ( sizeof ( Plate ) );
                    copyPlatesValuesFrom1stTo2nd ( plateToCut, *( tempChildren + k ) );
                }
                (*( tempChildren + 0 ))->x = plateToCut->x / 2 + j;
                (*( tempChildren + 1 ))->x = plateToCut->x / 2 - j;
                if ( plateToCut->x % 2 != 0 )
                    (*( tempChildren + 0 ))->x += 1;
                tempNumOfSplits = 1
                                + splitPlate ( platesMaxArea, platesMaxAspectRatio, *( tempChildren + 0 ) )
                                + splitPlate ( platesMaxArea, platesMaxAspectRatio, *( tempChildren + 1 ) );
                if ( tempNumOfSplits < numberOfSplits )
                {
                    numberOfSplits = tempNumOfSplits;
                    for ( char k = 0; k < NUMBER_OF_PLATES_CHILDREN; k ++ )
                    {
                        freePlatesRecursively ( children + k );
                        *( children + k ) = *( tempChildren + k );
                    }
                }
                else
                {
                    freePlatesRecursively ( tempChildren + 0 );
                    freePlatesRecursively ( tempChildren + 1 );
                }
            }
            swapULLs ( &(plateToCut->x), &(plateToCut->y) );
        }
        free ( tempChildren );
        plateToCut->children = children;
        return numberOfSplits;
    }
}

void                printPlateCuttingTree                   ( Plate                     * plate,
                                                              unsigned long long          level,
                                                              char                      * formatPrefix,
                                                              boolean                     firstChild )
{
    if ( plate != NULL )
    {
        printf ("%s", formatPrefix );
        char c = '+';
        *( formatPrefix + level ) = '|';
        if ( !firstChild )
        {
            c = '\\';
            *( formatPrefix + level ) = ' ';
        }
        printf ( "%c-[%llu, %llu]\n", c, plate->x, plate->y );
        if ( plate->children != NULL )
        {
            level ++;
            printPlateCuttingTree ( *( plate->children + 0 ), level, formatPrefix, true );
            printPlateCuttingTree ( *( plate->children + 1 ), level, formatPrefix, false );
        }
    }
}

int                 main                                    (  )
{
    PlatesContainer * platesContainer = NULL;
    if ( getInputData ( &platesContainer ) > 0 )
    {
        platesContainer->numberOfCuts = splitPlate ( platesContainer->platesMaxArea, MAX_PLATES_ASPECT_RATIO ,platesContainer->rootPlate );
        printf ( "Rezu: %llu\n", platesContainer->numberOfCuts );
        char * formatPrefix = ( char * ) calloc ( platesContainer->numberOfCuts, sizeof ( char ) );
        printPlateCuttingTree ( platesContainer->rootPlate, 0, formatPrefix, true );
        free ( formatPrefix );
    }
    else
        printf ( "Nespravny vstup.\n" );
    freePlatesContainer ( &platesContainer );
    return 0;
}