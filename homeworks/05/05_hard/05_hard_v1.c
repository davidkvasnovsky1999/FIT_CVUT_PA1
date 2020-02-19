#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>


typedef struct
{
    long double             coordinateX,
                            coordinateY;
    unsigned long long      indexOfNameBeginning; //to get flights name add this number (index) to a pointer stored in ( unsigned char * ) flightsContainer.names
    //If name of a flight is empty string then pointer called name in struct Flight points to '\n' in names of struct FlightsContainer.
    //This '\n' is only used for this one flight (it is possible that to have multiple succeeding '\n' chars in flightsContainer->names. Every '\n' is used for one flight.
    //Else it points to the first char of flights name in flightsContainer->names.
} Flight;

typedef struct
{
    long double             smallestDistance;
    unsigned long long      numberOfFlights,
                            namesLength,
                            numberOfIndexesOfClosestPairs,
                            numberOfAllocatedCharsForNames,
                            numberOfAllocatedFlights,
                            numberOfAllocatedIndexesOfClosestPairs,
                        *   indexesOfClosestPairs;
    // indexesOfClosestPairs - every pair is stored next to one another ( indexes of the first pair are stored at indexesOfClosestPairs [0] and indexesOfClosestPairs [1] )
    unsigned char       *   names;//Names are separated by '\n'
    Flight              *   flights;
} FlightsContainer;



int                     writeCharToFCNames                  ( unsigned char               readChar,
                                                              FlightsContainer          * pFlightsContainer )
{
    if ( pFlightsContainer == NULL )
        return 0;
    if ( pFlightsContainer->namesLength + 1 >= pFlightsContainer->numberOfAllocatedCharsForNames )
    {
        pFlightsContainer->numberOfAllocatedCharsForNames = ( pFlightsContainer->namesLength + 1 ) * 2;
        pFlightsContainer->names = ( unsigned char * ) realloc ( pFlightsContainer->names, pFlightsContainer->numberOfAllocatedCharsForNames * sizeof ( unsigned char ) );
    }
    *( pFlightsContainer->names + pFlightsContainer->namesLength ++ ) = readChar;
    return 1;
}

int                     readFlightsName                     ( FlightsContainer          * pFlightsContainer,
                                                              Flight                    * currentFlight )
{
    if ( pFlightsContainer == NULL || currentFlight == NULL )
        return 0;
    int readChar = 0, nonWhiteSpaceCharRead = 0;
    do
    {
        readChar = fgetc ( stdin );
        if ( nonWhiteSpaceCharRead == 0 && readChar != 9 && readChar != 11 && readChar != 13 && readChar != 32 )
        {
            currentFlight->indexOfNameBeginning = pFlightsContainer->namesLength;
            nonWhiteSpaceCharRead = 1;
        }
        if ( nonWhiteSpaceCharRead == 1 )
        {
            if ( readChar == '\n' )
                readChar = 0;
            writeCharToFCNames ( ( unsigned char ) readChar, pFlightsContainer );
        }
    }
    while ( readChar != 0 );
    if ( feof ( stdin ) != 0 && ferror ( stdin ) != 0 )
        return 0;
    return 1;
}

void                    resetFlights                        ( FlightsContainer          * pFlightsContainer )
{
    if ( pFlightsContainer != NULL )
    {
        free ( pFlightsContainer->flights );
        pFlightsContainer->flights                                  = NULL;
        pFlightsContainer->numberOfAllocatedFlights                 = 0;
        pFlightsContainer->numberOfFlights                          = 0;
        pFlightsContainer->smallestDistance                         = 0;
    }
}

void                    resetFlightsNames                   ( FlightsContainer          * pFlightsContainer )
{
    if ( pFlightsContainer != NULL )
    {
        free ( pFlightsContainer->names );
        pFlightsContainer->names                                    = NULL;
        pFlightsContainer->numberOfAllocatedCharsForNames           = 0;
        pFlightsContainer->namesLength                              = 0;
    }
}

void                    resetIndexesOfClosestPairs          ( FlightsContainer          * pFlightsContainer )
{
    if ( pFlightsContainer != NULL )
    {
        free ( pFlightsContainer->indexesOfClosestPairs );
        pFlightsContainer->indexesOfClosestPairs                    = NULL;
        pFlightsContainer->numberOfAllocatedIndexesOfClosestPairs   = 0;
        pFlightsContainer->numberOfIndexesOfClosestPairs            = 0;
        pFlightsContainer->smallestDistance                         = LDBL_MAX;
    }
}

void                    freeFlightsContainer                ( FlightsContainer         ** ppFlightsContainer )
{
    if ( ppFlightsContainer != NULL && *ppFlightsContainer != NULL )
    {
        free ( ( *ppFlightsContainer )->names );
        free ( ( *ppFlightsContainer )->flights );
        free ( ( *ppFlightsContainer )->indexesOfClosestPairs );
        free ( *ppFlightsContainer );
        *ppFlightsContainer = NULL;
    }
}

int                     initFlightsContainer                ( FlightsContainer         ** ppFlightsContainer )
{
    if ( ppFlightsContainer == NULL )
        return 0;
    freeFlightsContainer ( ppFlightsContainer );
    *ppFlightsContainer                                               = ( FlightsContainer * ) malloc ( sizeof ( FlightsContainer ) );
    (**ppFlightsContainer).smallestDistance                           = LDBL_MAX;
    (**ppFlightsContainer).numberOfFlights                            = 0;
    (**ppFlightsContainer).namesLength                                = 0;
    (**ppFlightsContainer).numberOfIndexesOfClosestPairs              = 0;
    (**ppFlightsContainer).numberOfAllocatedFlights                   = 8;
    (**ppFlightsContainer).numberOfAllocatedCharsForNames             = 128;
    (**ppFlightsContainer).numberOfAllocatedIndexesOfClosestPairs     = 8;
    (**ppFlightsContainer).flights                  = ( Flight * )              malloc ( ( size_t ) ( **ppFlightsContainer ).numberOfAllocatedFlights               * sizeof ( Flight ) );
    (**ppFlightsContainer).names                    = ( unsigned char * )       malloc ( ( size_t ) ( **ppFlightsContainer ).numberOfAllocatedCharsForNames         * sizeof ( unsigned char ) );
    (**ppFlightsContainer).indexesOfClosestPairs    = ( unsigned long long * )  malloc ( ( size_t ) ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long ) );
    if ( (**ppFlightsContainer).flights               == NULL
      || (**ppFlightsContainer).names                 == NULL
      || (**ppFlightsContainer).indexesOfClosestPairs == NULL )
    {
        freeFlightsContainer ( ppFlightsContainer );
        return 0;
    }
    return 1;
}

int                     checkAndFixFContainersIntegrity     ( FlightsContainer         ** ppFlightsContainer )
{
    int ret = 1;
    if ( *ppFlightsContainer == NULL )
    {
        ret = 0;
        if ( ! initFlightsContainer ( ppFlightsContainer ) )
            return -1;
    }
    else
    {
        if ( 1 > ( **ppFlightsContainer ).numberOfAllocatedFlights )
        {
            ret = 0;
            resetFlights ( *ppFlightsContainer );
            ( **ppFlightsContainer ).numberOfAllocatedFlights               = 1;
            if ( !( ( **ppFlightsContainer ).flights
                    = ( Flight * )              malloc ( ( **ppFlightsContainer ).numberOfAllocatedFlights               * sizeof ( Flight ) ) ) )
            {
                resetFlights ( *ppFlightsContainer );
                return -1;
            }
        }
        if ( 1 > ( **ppFlightsContainer ).numberOfAllocatedCharsForNames )
        {
            ret = 0;
            resetFlightsNames ( *ppFlightsContainer );
            ( **ppFlightsContainer ).numberOfAllocatedCharsForNames         = 1;
            if ( !( ( **ppFlightsContainer ).names
                    = ( unsigned char * )       malloc ( ( **ppFlightsContainer ).numberOfAllocatedCharsForNames         * sizeof ( unsigned char ) ) ) )
            {
                resetFlightsNames ( *ppFlightsContainer );
                return -1;
            }
        }
        if ( 2 > ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs )
        {
            ret = 0;
            resetIndexesOfClosestPairs ( *ppFlightsContainer );
            ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = 2;
            if ( !( ( **ppFlightsContainer ).indexesOfClosestPairs
                    = ( unsigned long long * )  malloc ( ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long ) ) ) )
            {
                resetIndexesOfClosestPairs ( *ppFlightsContainer );
                return -1;
            }
        }
    }
    return ret;
}

int                     shrinkAllocatedMemoryToMinimum      ( FlightsContainer         ** ppFlightsContainer )
{
    int ret = 1;
    if ( checkAndFixFContainersIntegrity ( ppFlightsContainer ) < 0 )
        return -1;
    if ( ! ( ( **ppFlightsContainer ).flights
             = ( Flight * )             realloc ( ( **ppFlightsContainer ).flights,
                    ( ( **ppFlightsContainer ).numberOfFlights               + 1 ) * sizeof ( Flight ) ) ) )
    {
        ret = 0;
        resetFlights               ( *ppFlightsContainer );
    }
    else
        ( **ppFlightsContainer ).numberOfAllocatedFlights               = ( **ppFlightsContainer ).numberOfFlights + 1;
    if ( ! ( ( **ppFlightsContainer ).names
             = ( unsigned char * )      realloc ( ( **ppFlightsContainer ).names,
                    ( ( **ppFlightsContainer ).namesLength                   + 1 ) * sizeof ( unsigned char ) ) ) )
    {
        ret = 0;
        resetFlightsNames          ( *ppFlightsContainer );
    }
    else
        ( **ppFlightsContainer ).numberOfAllocatedCharsForNames         = ( **ppFlightsContainer ).namesLength + 1;
    if ( ! ( ( **ppFlightsContainer ).indexesOfClosestPairs
             = ( unsigned long long * ) realloc ( ( **ppFlightsContainer ).indexesOfClosestPairs,
                    ( ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs + 2 ) * sizeof ( unsigned long long ) ) ) )
    {
        ret = 0;
        resetIndexesOfClosestPairs ( *ppFlightsContainer );
    }
    else
        ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs + 2;
    return ret;
}

int                     getFlights                          ( FlightsContainer         ** ppFlightsContainer )
{
    if ( !initFlightsContainer ( ppFlightsContainer ) )
        return 0;
    int ret = 0;
    long double x, y;
    unsigned char charForFormatCheck = 0;
    Flight * currentFlight;
    while ( ( ret = scanf ( " [ %Lf , %Lf %c", &x, &y, &charForFormatCheck ) ) == 3 && ret != EOF && charForFormatCheck == ']' )
    {
        if ( ( *ppFlightsContainer )->numberOfAllocatedFlights == ( *ppFlightsContainer )->numberOfFlights + 1 )
        {
            ( *ppFlightsContainer )->numberOfAllocatedFlights *= 2;
            ( *ppFlightsContainer )->flights = ( Flight * ) realloc ( ( *ppFlightsContainer )->flights, ( *ppFlightsContainer )->numberOfAllocatedFlights * sizeof ( Flight ) );
        }
        currentFlight = ( *ppFlightsContainer )->flights + ( ( *ppFlightsContainer )->numberOfFlights ++ );
        currentFlight->coordinateX = x;
        currentFlight->coordinateY = y;
        if ( readFlightsName ( *ppFlightsContainer, currentFlight ) != 1 )
            return 0;
    }
    if ( ret != EOF || charForFormatCheck != ']' || ( *ppFlightsContainer )->numberOfFlights < 2 )
        return 0;
    shrinkAllocatedMemoryToMinimum ( ppFlightsContainer );
    return 1;
}

//int                     printFlights                        ( FlightsContainer          * pFlightsContainer )
//{
//    if ( pFlightsContainer == NULL || pFlightsContainer->names == NULL || pFlightsContainer->flights == NULL )
//        return 0;
//    printf ( "Zadene lety:\n" );
//    for ( unsigned long long i = 0; i < pFlightsContainer->numberOfFlights; i ++ )
//    {
//        Flight * iterFlight = pFlightsContainer->flights + i;
//        if ( iterFlight->indexOfNameBeginning > pFlightsContainer->namesLength )
//            return 0;
////        printf ( "[%Lf, %Lf] %s\n", iterFlight->coordinateX, iterFlight->coordinateY, iterFlight->name );
//        printf ( "%s\n", pFlightsContainer->names + iterFlight->indexOfNameBeginning );
//    }
//    return 1;
//}

int                     compareFlightsByXYCoordinates       ( const void                * a,
                                                              const void                * b )
{
    if ( a == NULL || b == NULL )
        return 0;
    if ( ( *( Flight * ) a ).coordinateX == ( *( Flight * ) b ).coordinateX )
        return ( ( *( Flight * ) a ).coordinateY > ( *( Flight * ) b ).coordinateY ) ? 1 : -1;
    return ( ( *( Flight * ) a ).coordinateX > ( *( Flight * ) b ).coordinateX ) ? 1 : -1;
}

long double             getDistanceBetweenTwoFlights        ( Flight                      flightA,
                                                              Flight                      flightB )
{
    return sqrtl ( powl ( ( flightA.coordinateX - flightB.coordinateX ), 2 ) + powl ( ( flightA.coordinateY - flightB.coordinateY ), 2 ) );
}

long double             getMax                              ( long double                  numA,
                                                              long double                  numB )
{
    if ( numA >= numB )
        return numA;
    return numB;
}

int                     calculateSmallestDistances          ( FlightsContainer ** ppFlightsContainer )
{
    if ( checkAndFixFContainersIntegrity ( ppFlightsContainer ) < 0 || ( *ppFlightsContainer )->numberOfFlights < 2 )
        return 0;
    qsort ( ( *ppFlightsContainer )->flights, ( *ppFlightsContainer )->numberOfFlights, sizeof ( Flight ), compareFlightsByXYCoordinates );
    long double tempSmallestDistance = 0;
    for ( unsigned long long i = 0; i < ( *ppFlightsContainer )->numberOfFlights - 1; i ++ )
        for ( unsigned long long j = i + 1; j < ( *ppFlightsContainer )->numberOfFlights; j ++ )
        {
            Flight iterFlightA = *( ( *ppFlightsContainer )->flights + i );
            Flight iterFlightB = *( ( *ppFlightsContainer )->flights + j );
            tempSmallestDistance = getDistanceBetweenTwoFlights ( iterFlightA, iterFlightB );
            long double areAlmostEqual = fabsl ( ( *ppFlightsContainer )->smallestDistance - tempSmallestDistance ) <= tempSmallestDistance / 1024;
            
//            areAlmostEqual = fabsl ( ( *ppFlightsContainer )->smallestDistance - tempSmallestDistance ) < tempSmallestDistance / powl ( 10, 8 );
//            printf ( "tempSmallestDistance = %.*Lf\n", DECIMAL_DIG, tempSmallestDistance );
//            printf ( "areAlmostEqual = %.*Lf\n", DECIMAL_DIG, areAlmostEqual );
            if ( !areAlmostEqual && ( *ppFlightsContainer )->smallestDistance > tempSmallestDistance )
            {
                free ( ( *ppFlightsContainer )->indexesOfClosestPairs );
                ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs = 0;
                ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = 2;
                if ( !( ( **ppFlightsContainer ).indexesOfClosestPairs =
                                ( unsigned long long * ) malloc ( ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long ) ) ) )
                    return 0;
            }
            if ( areAlmostEqual || ( *ppFlightsContainer )->smallestDistance > tempSmallestDistance )
            {
                ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs += 2;
                if ( ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs > ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs )
                {
                    ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs * 2;
                    if ( !( ( **ppFlightsContainer ).indexesOfClosestPairs =
                                    ( unsigned long long * ) realloc (
                                            ( **ppFlightsContainer ).indexesOfClosestPairs,
                                            ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long )
                                    ) ) )
                        return 0;
                }
                ( *ppFlightsContainer )->smallestDistance = tempSmallestDistance;
                *( ( **ppFlightsContainer ).indexesOfClosestPairs + ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs - 2 ) = i;
                *( ( **ppFlightsContainer ).indexesOfClosestPairs + ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs - 1 ) = j;
            }
        }
    return 1;
}

int                     printPairsOfFlightsWithSmallestDist ( FlightsContainer          * pFlightsContainer )
{
    if ( checkAndFixFContainersIntegrity ( &pFlightsContainer ) < 0 )
        return 0;
    printf ( "Nejmensi vzdalenost: %Lf\n", pFlightsContainer->smallestDistance );
    for ( unsigned long long i = 0; i < pFlightsContainer->numberOfIndexesOfClosestPairs; i += 2 )
    {
//        printf ( "[%llu] [%llu] ", *( ( pFlightsContainer->indexesOfClosestPairs ) + i ), *( ( pFlightsContainer->indexesOfClosestPairs ) + i + 1 ) );
        printf ( "%s - %s\n",
                 pFlightsContainer->names + ( ( *pFlightsContainer ).flights + *( pFlightsContainer->indexesOfClosestPairs + i     ) )->indexOfNameBeginning,
                 pFlightsContainer->names + ( ( *pFlightsContainer ).flights + *( pFlightsContainer->indexesOfClosestPairs + i + 1 ) )->indexOfNameBeginning );
    }
    return 1;
}

//void                    printFlightNames                    ( FlightsContainer          * pFlightsContainer )
//{
//    if ( checkAndFixFContainersIntegrity ( &pFlightsContainer ) >= 0 )
//    {
//        unsigned char c = 0;
//        for ( unsigned long long i = 0; i < ( *pFlightsContainer ).namesLength; i ++ )
//        {
//            c = * ( ( *pFlightsContainer ).names + i );
//            if ( c == 0 )
//                c = '\n';
//            printf ("%c", c );
//        }
//    }
//}
//
//void                    printNamesAddresses                 ( FlightsContainer          * pFlightsContainer )
//{
//    if ( pFlightsContainer != NULL )
//    {
//        for ( unsigned long long i = 0; i < pFlightsContainer->namesLength; i ++ )
//        {
//            printf ( "%p %c\n", pFlightsContainer->names + i, *( pFlightsContainer->names + i ) );
//        }
//    }
//}

void                    printFlightsCoordinates             ( FlightsContainer          * pFlightsContainer )
{
    if ( pFlightsContainer != NULL )
    {
        for ( unsigned long long i = 0; i < pFlightsContainer->numberOfFlights; i ++ )
        {
            printf ( "Coordinates x,y: [%.24Lf] [%.24Lf] %s\n", ( pFlightsContainer->flights + i )->coordinateX, ( pFlightsContainer->flights + i )->coordinateY,
                     pFlightsContainer->names + ( ( *pFlightsContainer ).flights + i )->indexOfNameBeginning );
        }
    }
}

//void                    printFlightsCoordinates             ( FlightsContainer          * pFlightsContainer )
//{
//    if ( pFlightsContainer != NULL )
//    {
//        for ( unsigned long long i = 0; i < pFlightsContainer->numberOfFlights; i ++ )
//        {
////            printf ( "Coordinates x,y: [%.24lf] [%.24lf] %s\n", ( pFlightsContainer->flights + i )->coordinateX, ( pFlightsContainer->flights + i )->coordinateY,
////                     pFlightsContainer->names + ( ( *pFlightsContainer ).flights + i )->indexOfNameBeginning );
//            printf ( "Coordinates x,y > 0: " );
//            ( pFlightsContainer->flights + i )->coordinateX > 0 ? printf ( "true " ) : printf ( "false " );
//            ( pFlightsContainer->flights + i )->coordinateY > 0 ? printf ( "true\n" ) : printf ( "false\n" );
//            printf ( "%s\n", pFlightsContainer->names + ( ( *pFlightsContainer ).flights + i )->indexOfNameBeginning );
//        }
//    }
//}

int                     main                                ( void )
{
    FlightsContainer * pFlightsContainer = NULL;
    printf ( "Zadejte lety:\n" );
    if ( getFlights ( &pFlightsContainer ) )
    {
        calculateSmallestDistances ( &pFlightsContainer );
//        printFlightsCoordinates ( pFlightsContainer );
        printPairsOfFlightsWithSmallestDist ( pFlightsContainer );
    }
    else
        printf ( "Nespravny vstup.\n" );
    freeFlightsContainer ( &pFlightsContainer );
    return 0;
}