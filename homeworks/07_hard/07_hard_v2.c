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
    struct Plate         ** helperChildren;
} Plate;

typedef struct PlatesContainer
{
    unsigned long long      numberOfCuts,
                            platesMaxArea;
    Plate                 * rootPlate;
} PlatesContainer;

typedef struct HashMap
{
    unsigned long long      key,
                            numberOfCuts;
    Plate                 * plate;
} HashMap;

typedef struct HashMap2D
{
    unsigned long long      key,
                            numberOfAllocatedHashMaps,
                            numberOfHashMaps;
    HashMap              ** hashMaps;
} HashMap2D;

typedef struct ContainerOf2DHashMaps
{
    unsigned long long      numberOfAllocated2DHashMaps,
                            numberOf2DHashMaps;
    HashMap2D            ** hashMaps2D;
} ContainerOf2DHashMaps;

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
        if ( (*plate)->helperChildren != NULL )
        {
            freePlatesRecursively ( (*plate)->helperChildren );
            freePlatesRecursively ( (*plate)->helperChildren + 1 );
            free ( (*plate)->helperChildren );
        }
        free ( *plate );
        *plate = NULL;
    }
}

void                freePlatesChildrenRecursively           ( Plate                   *** platesChildren )
{
    if ( platesChildren != NULL && *platesChildren != NULL )
    {
        for ( char i = 0; i < NUMBER_OF_PLATES_CHILDREN; i ++ )
            freePlatesRecursively ( ( *platesChildren ) + i );
        free ( *platesChildren );
        *platesChildren = NULL;
    }
}

void                freePlatesContainer                     ( PlatesContainer          ** pPlatesContainer )
{
    if ( pPlatesContainer != NULL && *pPlatesContainer != NULL )
    {
        freePlatesRecursively ( &((*pPlatesContainer)->rootPlate) );
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
//        target->children = source->children;
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
    (*rootPlate)->helperChildren = NULL;
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

void                allocateAndInitializePlate              ( Plate                    ** plate )
{
    if ( plate != NULL )
    {
        (*plate) = ( Plate * ) malloc ( sizeof ( Plate ) );
        (*plate)->children = NULL;
        (*plate)->helperChildren = NULL;
        (*plate)->x = 0;
        (*plate)->y = 0;
    }
}

void                allocatePlatesChildrenIfNULL            ( Plate                   *** pPlatesChildren )
{
    if ( pPlatesChildren != NULL && *pPlatesChildren == NULL )
    {
        *pPlatesChildren = ( Plate ** ) malloc ( sizeof ( Plate * ) * NUMBER_OF_PLATES_CHILDREN );
        for ( char i = 0; i < NUMBER_OF_PLATES_CHILDREN; i ++ )
            allocateAndInitializePlate ( ( ( *pPlatesChildren ) + i ) );
    }
}

int                 compareKeyToHashMap                     ( const void                * key,
                                                              const void                * hashMap )
{
    const unsigned long long    yDimension  = *( ( const unsigned long long * ) key );
    const HashMap            ** pHashMap    = ( const HashMap ** ) hashMap;
    return (*pHashMap)->key >= yDimension ? ( (*pHashMap)->key > yDimension ? 1 : 0 ) : -1;
}

int                 compareKeyTo2DHashMap                   ( const void                * key,
                                                              const void                * hashMap2D )
{
    const unsigned long long    xDimension  = *( ( const unsigned long long * ) key );
    const HashMap2D          ** pHashMap2D  = ( const HashMap2D ** ) hashMap2D;
    return (*pHashMap2D)->key >= xDimension ? ( (*pHashMap2D)->key > xDimension ? 1 : 0 ) : -1;
}

boolean             findPlateTreeIn2DHashMap                ( ContainerOf2DHashMaps     * containerOf2DHashMaps,
                                                              unsigned long long        * numberOfCuts,
                                                              const Plate               * plateToFind,
                                                              Plate                   *** foundChildren )
{
    if ( containerOf2DHashMaps != NULL && numberOfCuts != NULL && plateToFind != NULL && foundChildren != NULL )
    {
        for ( unsigned long long i = 0; i < containerOf2DHashMaps->numberOf2DHashMaps; i ++ )
        {
            HashMap2D * iter2DHashMap = (*( containerOf2DHashMaps->hashMaps2D + i ));
            if ( plateToFind->x == iter2DHashMap->key )
            {
                for ( unsigned long long j = 0; j < iter2DHashMap->numberOfHashMaps; j ++ )
                {
                    HashMap * iterHashMap = (*( iter2DHashMap->hashMaps + j ) );
                    if ( plateToFind->x == iterHashMap->key )
                    {
                        *numberOfCuts   = iterHashMap->numberOfCuts;
                        *foundChildren  = iterHashMap->plate->children;
                        return true;
                    }
                }
            }
        }
    }
    return false;
//    HashMap2D * found2DHashMap = ( HashMap2D * ) bsearch ( x, containerOf2DHashMaps->hashMaps2D, containerOf2DHashMaps->numberOf2DHashMaps, sizeof ( HashMap2D * ), compareKeyTo2DHashMap );
//    if ( found2DHashMap == NULL )
//        return 0;
//    HashMap * foundHashMap = ( HashMap * ) bsearch ( y, found2DHashMap->hashMaps, found2DHashMap->numberOfHashMaps, sizeof ( HashMap * ), compareKeyToHashMap );
//    if ( foundHashMap == NULL )
//        return 0;
}

unsigned long long  splitPlate                              ( ContainerOf2DHashMaps     * containerOf2DHashMaps,
                                                              const unsigned long long    platesMaxArea,
                                                              const long double           platesMaxAspectRatio,
                                                              const boolean               cutByY,
                                                              Plate                     * plateToCut )
{
    if ( cutByY )
        swapULLs ( &( plateToCut->x ), &( plateToCut->y ) );
    if ( plateToCut->x * plateToCut->y <= platesMaxArea
    && compareLongDoubles ( getPlatesAspectRatio ( *plateToCut ), platesMaxAspectRatio ) != 1 )
    {
        freePlatesChildrenRecursively ( &( plateToCut->children ) );
        return 0;
    }
    unsigned long long  numberOfCuts     = -1,
                        tempNumberOfCuts = 0;
    if ( findPlateTreeIn2DHashMap ( containerOf2DHashMaps, &numberOfCuts, plateToCut, &( plateToCut->children ) ) )
        return numberOfCuts;
    allocatePlatesChildrenIfNULL ( &( plateToCut->children ) );
    allocatePlatesChildrenIfNULL ( &( plateToCut->helperChildren ) );
    for ( char i = 0; i < 2; i ++ ) //In first and second iteration we cut plate by x and y dimension respectively.
    {
        for ( unsigned long long j = 0; j < ( plateToCut->x / 2 ); j ++  )
        {
            for ( char k = 0; k < NUMBER_OF_PLATES_CHILDREN; k ++ )
            {
                copyPlatesValuesFrom1stTo2nd ( plateToCut, *( plateToCut->helperChildren + k ) );
                (*( plateToCut->helperChildren + k ))->x = plateToCut->x / 2
                                                      + ( j * ( unsigned long long ) pow ( -1, k ) );
            }
            if ( plateToCut->x % 2 != 0 )
                (*( plateToCut->helperChildren + 0 ))->x += 1;
            tempNumberOfCuts = 1
                                 + splitPlate ( containerOf2DHashMaps, platesMaxArea, platesMaxAspectRatio, i,
                                    *( plateToCut->helperChildren + 0 ) )
                                 + splitPlate ( containerOf2DHashMaps, platesMaxArea, platesMaxAspectRatio, i,
                                    *( plateToCut->helperChildren + 1 ) );
            if ( tempNumberOfCuts < numberOfCuts )
            {
                numberOfCuts = tempNumberOfCuts;
                Plate ** temp = plateToCut->children;
                plateToCut->children = plateToCut->helperChildren;
                plateToCut->helperChildren = temp;
            }
        }
        swapULLs ( &(plateToCut->x), &(plateToCut->y) );
    }
    
    return numberOfCuts;
}

void                printPlatesRecursively                  ( Plate                     * plate,
                                                              char                      * formatPrefix,
                                                              int                         level,
                                                              boolean                     firstChild )
{
    if ( plate != NULL )
    {
        printf ("%.*s", level, formatPrefix );
        const char  * prefixCharsA = "\\+",
                    * prefixCharsB = " |";
        printf ( "%c-[%llu, %llu]\n", prefixCharsA [ firstChild ], plate->x, plate->y );
        if ( plate->children != NULL )
        {
            *( formatPrefix + level + 0 ) = prefixCharsB [ firstChild ];
            *( formatPrefix + level + 1 ) = prefixCharsB [ 0 ];
            level += 2;
            printPlatesRecursively ( *( plate->children + 0 ), formatPrefix, level, true );
            printPlatesRecursively ( *( plate->children + 1 ), formatPrefix, level, false );
        }
    }
}

void                printPlateCuttingTree                   ( PlatesContainer           * pPlatesContainer )
{
    if ( pPlatesContainer != NULL )
    {
        printf ("Rezu: %llu\n", pPlatesContainer->numberOfCuts );
        printf ( "[%llu, %llu]\n", pPlatesContainer->rootPlate->x, pPlatesContainer->rootPlate->y );
        if ( pPlatesContainer->rootPlate->children != NULL )
        {
            char * formatPrefix = ( char * ) calloc ( pPlatesContainer->numberOfCuts * 2, sizeof ( char ) );
            printPlatesRecursively ( *( pPlatesContainer->rootPlate->children + 0 ), formatPrefix, 0, true );
            printPlatesRecursively ( *( pPlatesContainer->rootPlate->children + 1 ), formatPrefix, 0, false );
            free ( formatPrefix );
        }
    }
}

char                getNewHashMap                           ( HashMap                  ** pHashMap )
{
    if ( pHashMap == NULL )
        return 0;
    *pHashMap = ( HashMap * ) malloc ( sizeof ( HashMap ) );
    (*pHashMap)->key = 0;
    (*pHashMap)->numberOfCuts = 0;
    (*pHashMap)->plate = NULL;
    return 1;
}

char                getNew2DHashMap                         ( HashMap2D                ** pHashMap2D )
{
    if ( pHashMap2D == NULL )
        return 0;
    *pHashMap2D = ( HashMap2D * ) malloc ( sizeof ( HashMap2D ) );
    (*pHashMap2D)->key = 0;
    (*pHashMap2D)->numberOfHashMaps = 0;
    (*pHashMap2D)->numberOfAllocatedHashMaps = 16;
    (*pHashMap2D)->hashMaps = ( HashMap ** ) malloc ( sizeof ( HashMap * ) * (*pHashMap2D)->numberOfAllocatedHashMaps );
    return 1;
}

char                getNew2DHMContainer                     ( ContainerOf2DHashMaps    ** pContainerOf2DHashMaps )
{
    if ( pContainerOf2DHashMaps == NULL )
        return 0;
    *pContainerOf2DHashMaps = ( ContainerOf2DHashMaps * ) malloc ( sizeof ( ContainerOf2DHashMaps ) );
    (*pContainerOf2DHashMaps)->numberOf2DHashMaps = 0;
    (*pContainerOf2DHashMaps)->numberOfAllocated2DHashMaps = 32;
    (*pContainerOf2DHashMaps)->hashMaps2D = ( HashMap2D ** ) malloc ( sizeof ( HashMap2D * ) * (*pContainerOf2DHashMaps)->numberOfAllocated2DHashMaps );
    for ( unsigned long long i = 0; i < (*pContainerOf2DHashMaps)->numberOfAllocated2DHashMaps; i ++ )
        *( (*pContainerOf2DHashMaps)->hashMaps2D + i ) = NULL;
    return 1;
}

void                free2DHashMap                           ( HashMap2D                ** pHashMap2D )
{
    if ( pHashMap2D != NULL && *pHashMap2D != NULL )
    {
        for ( unsigned long long i = 0; i < (*pHashMap2D)->numberOfHashMaps; i ++ )
            free ( (*pHashMap2D)->hashMaps + i );
        free ( *pHashMap2D );
        *pHashMap2D = NULL;
    }
}

void                free2DHashMapContainer                  ( ContainerOf2DHashMaps    ** pContainerOf2DHashMaps )
{
    if ( pContainerOf2DHashMaps != NULL && *pContainerOf2DHashMaps != NULL )
    {
        for ( unsigned long long i = 0; i < (*pContainerOf2DHashMaps)->numberOf2DHashMaps; i ++ )
            free2DHashMap ( (*pContainerOf2DHashMaps)->hashMaps2D + i );
        free ( *pContainerOf2DHashMaps );
        *pContainerOf2DHashMaps = NULL;
    }
}

int                 main                                    (  )
{
    PlatesContainer * platesContainer = NULL;
    if ( getInputData ( &platesContainer ) > 0 )
    {
        ContainerOf2DHashMaps * containerOf2DHashMaps = NULL;
        getNew2DHMContainer ( &containerOf2DHashMaps );
        platesContainer->numberOfCuts = splitPlate ( containerOf2DHashMaps, platesContainer->platesMaxArea,
                                                     MAX_PLATES_ASPECT_RATIO , true, platesContainer->rootPlate );
        printPlateCuttingTree ( platesContainer );
        free2DHashMapContainer ( &containerOf2DHashMaps );
    }
    else
        printf ( "Nespravny vstup.\n" );
    freePlatesContainer ( &platesContainer );
    return 0;
}