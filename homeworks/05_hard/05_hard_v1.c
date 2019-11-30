#include <stdio.h>
#include <stdlib.h>
#include <math.h>



typedef struct
{
    unsigned char * name;
    //If name of a flight is empty string then pointer called name in struct Flight points to '\n' in names of struct FlightsContainer.
    //This '\n' is only used for this one flight (it is possible that to have multiple succeeding '\n' chars in flightsContainer->names. Every '\n' is used for one flight.
    //Else it points to the first char of flights name in flightsContainer->names.
    long double x, y;
} Flight;

typedef struct
{
    unsigned long long numberOfFlights, namesLength, numberOfIndexesOfClosestPairs, numberOfAllocatedCharsForNames, numberOfAllocatedFlights, *arrayOfIndexesOfClosestPairs, numberOfAllocatedIndexesOfClosestPairs;
    long double smallestDistance;
    // arrayOfIndexesOfClosestPairs - every pair is stored next to one another ( indexes of the first pair are stored at arrayOfIndexesOfClosestPairs [0] and arrayOfIndexesOfClosestPairs [1] )
    unsigned char * names;//Names are separated by '\n'
    Flight * flights;
    
} FlightsContainer;



int                     writeCharToFCNames                  ( unsigned char       readChar,
                                                              FlightsContainer  * flightsContainer )
{
    if ( flightsContainer == NULL )
        return 0;
    if ( flightsContainer->namesLength + 1 == flightsContainer->numberOfAllocatedCharsForNames )
    {
        flightsContainer->numberOfAllocatedCharsForNames *= 2;
        flightsContainer->names = ( unsigned char * ) realloc ( flightsContainer->names, flightsContainer->numberOfAllocatedCharsForNames * sizeof ( unsigned char ) );
    }
    *( flightsContainer->names + flightsContainer->namesLength ++ ) = readChar;
    return 1;
}

int                     readFlightsName                     ( FlightsContainer  * flightsContainer,
                                                              Flight            * currentFlight )
{
    if ( flightsContainer == NULL || currentFlight == NULL )
        return 0;
    int readChar = 0, nonWhiteSpaceCharRead = 0;
    do
    {
        readChar = fgetc ( stdin );
        if ( nonWhiteSpaceCharRead == 0 && readChar != 9 && readChar != 11 && readChar != 13 && readChar != 32 )
        {
            currentFlight->name = flightsContainer->names + flightsContainer->namesLength;
            nonWhiteSpaceCharRead = 1;
        }
        if ( nonWhiteSpaceCharRead == 1 )
        {
            if ( readChar == '\n' )
                readChar = 0;
            writeCharToFCNames ( ( unsigned char ) readChar, flightsContainer );
        }
    }
    while ( readChar != 0 );
    if ( feof ( stdin ) != 0 && ferror ( stdin ) != 0 )
        return 0;
    return 1;
}

void                    freeFlightsContainer                ( FlightsContainer ** flightsContainer )
{
    if ( flightsContainer != NULL && *flightsContainer != NULL )
    {
        free ( ( *flightsContainer )->names );
        free ( ( *flightsContainer )->flights );
        free ( ( *flightsContainer )->arrayOfIndexesOfClosestPairs );
        free ( *flightsContainer );
        *flightsContainer = NULL;
    }
}

int                     initFlightsContainer                ( FlightsContainer ** ppFlightsContainer )
{
    if ( ppFlightsContainer == NULL )
        return 0;
    freeFlightsContainer ( ppFlightsContainer );
    *ppFlightsContainer                                               = ( FlightsContainer * ) malloc ( sizeof ( FlightsContainer ) );
    (**ppFlightsContainer).smallestDistance                           = 0;
    (**ppFlightsContainer).numberOfFlights                            = 0;
    (**ppFlightsContainer).namesLength                                = 0;
    (**ppFlightsContainer).numberOfIndexesOfClosestPairs              = 0;
    (**ppFlightsContainer).numberOfAllocatedFlights                   = 8;
    (**ppFlightsContainer).numberOfAllocatedCharsForNames             = 128;
    (**ppFlightsContainer).numberOfAllocatedIndexesOfClosestPairs     = 8;
    if ( ( (**ppFlightsContainer).flights                             = ( Flight * )              malloc ( ( **ppFlightsContainer ).numberOfAllocatedFlights               * sizeof ( Flight ) ) )
      && ( (**ppFlightsContainer).names                               = ( unsigned char * )       malloc ( ( **ppFlightsContainer ).numberOfAllocatedCharsForNames         * sizeof ( unsigned char ) ) )
      && ( (**ppFlightsContainer).arrayOfIndexesOfClosestPairs        = ( unsigned long long * )  malloc ( ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long ) ) ) )
        return 1;
    freeFlightsContainer ( ppFlightsContainer );
    return 0;
}

int                     checkFlightsContainerIntegrity      ( FlightsContainer ** ppFlightsContainer )
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
        if ( ( **ppFlightsContainer ).numberOfAllocatedFlights < 1 )
        {
            ret = 0;
            free ( ( *ppFlightsContainer )->flights );
            ( **ppFlightsContainer ).numberOfAllocatedFlights = 1;
            ( **ppFlightsContainer ).numberOfFlights = 0;
            ( **ppFlightsContainer ).smallestDistance = 0;
            if ( !( ( **ppFlightsContainer ).flights = ( Flight * ) malloc ( ( **ppFlightsContainer ).numberOfAllocatedCharsForNames * sizeof ( Flight ) ) ) )
                return -1;
        }
        if ( ( **ppFlightsContainer ).numberOfAllocatedCharsForNames < 1 )
        {
            ret = 0;
            free ( ( *ppFlightsContainer )->names );
            ( **ppFlightsContainer ).numberOfAllocatedCharsForNames = 1;
            ( **ppFlightsContainer ).namesLength = 0;
            if ( !( ( **ppFlightsContainer ).names = ( unsigned char * ) malloc ( ( **ppFlightsContainer ).numberOfAllocatedCharsForNames * sizeof ( unsigned char ) ) ) )
                return -1;
        }
        if ( ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs < 1 )
        {
            ret = 0;
            free ( ( *ppFlightsContainer )->arrayOfIndexesOfClosestPairs );
            ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = 1;
            ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs = 0;
            ( **ppFlightsContainer ).smallestDistance = 0;
            if ( !( ( **ppFlightsContainer ).arrayOfIndexesOfClosestPairs = ( unsigned long long * ) malloc ( ( **ppFlightsContainer ).numberOfAllocatedCharsForNames * sizeof ( unsigned long long ) ) ) )
                return -1;
        }
    }
    return ret;
}

int                     getFlights                          ( FlightsContainer ** flightsContainer )
{
    if ( flightsContainer == NULL )
        return 0;
    initFlightsContainer ( flightsContainer );
    int ret = 0;
    long double x, y;
    unsigned char charForFormatCheck = 0;
    Flight * currentFlight;
    while ( ( ret = scanf ( "[ %Lf , %Lf %c", &x, &y, &charForFormatCheck ) ) == 3 && ret != EOF && charForFormatCheck == ']' )
    {
        if ( ( *flightsContainer )->numberOfAllocatedFlights == ( *flightsContainer )->numberOfFlights + 1 )
        {
            ( *flightsContainer )->numberOfAllocatedFlights *= 2;
            ( *flightsContainer )->flights = ( Flight * ) realloc ( ( *flightsContainer )->flights, ( *flightsContainer )->numberOfAllocatedFlights * sizeof ( Flight ) );
        }
        currentFlight = ( *flightsContainer )->flights + ( ( *flightsContainer )->numberOfFlights ++ );
        currentFlight->x = x;
        currentFlight->y = y;
        if ( readFlightsName ( *flightsContainer, currentFlight ) != 1 )
            return 0;
    }
    if ( ret != EOF || charForFormatCheck != ']' || ( *flightsContainer )->numberOfFlights < 2 )
        return 0;
    return 1;
}

int                     printFlights                        ( FlightsContainer  * flightsContainer )
{
    if ( flightsContainer == NULL || flightsContainer->names == NULL || flightsContainer->flights == NULL )
        return 0;
    printf ( "Zadene lety:\n" );
    for ( unsigned long long i = 0; i < flightsContainer->numberOfFlights; i ++ )
    {
        Flight * iterFlight = flightsContainer->flights + i;
        if ( iterFlight->name == NULL )
            return 0;
        printf ( "[%Lf, %Lf] %s\n", iterFlight->x, iterFlight->y, iterFlight->name );
    }
    return 1;
}

int                     printPairsOfFlightsWithSmallestDist ( FlightsContainer  * pFlightsContainer )
{
    if ( checkFlightsContainerIntegrity ( &pFlightsContainer ) < 0 )
            return 0;
    printf ( "Nejmensi vzdalenost: %Lf\n", pFlightsContainer->smallestDistance );
    for ( unsigned long long i = 0; i < pFlightsContainer->numberOfIndexesOfClosestPairs; i += 2 )
    {
        printf ( "%llu %llu\n", *( pFlightsContainer->arrayOfIndexesOfClosestPairs + i ), *( pFlightsContainer->arrayOfIndexesOfClosestPairs + i + 1 ) );
        printf ( "%s - %s\n",
                 ( ( *pFlightsContainer ).flights + *( pFlightsContainer->arrayOfIndexesOfClosestPairs + i ) )->name,
                 ( ( *pFlightsContainer ).flights + *( pFlightsContainer->arrayOfIndexesOfClosestPairs + i + 1 ) )->name );
    }
    return 1;
}


int                     compareFlightsByXYCoordinate        ( const void        * a,
                                                              const void        * b )
{
    if ( a == NULL || b == NULL )
        return 0;
    if ( ( *( Flight * ) a ).x == ( *( Flight * ) b ).x )
        return ( ( *( Flight * ) a ).y > ( *( Flight * ) b ).y ) ? 1 : -1;
    return ( ( *( Flight * ) a ).x > ( *( Flight * ) b ).x ) ? 1 : -1;
}

long double             getDistanceBetweenTwoFlights        ( Flight              flightA,
                                                              Flight              flightB )
{
    return sqrtl ( powl ( ( flightA.x - flightB.x ), 2 ) + powl ( ( flightA.y - flightB.y ), 2 ) );
}

int                     calculateSmallestDistances          ( FlightsContainer ** ppFlightsContainer )
{
    if ( checkFlightsContainerIntegrity ( ppFlightsContainer ) < 0 || ( *ppFlightsContainer )->numberOfFlights < 2 )
        return 0;
    qsort ( ( *ppFlightsContainer )->flights, ( *ppFlightsContainer )->numberOfFlights, sizeof ( Flight ), compareFlightsByXYCoordinate );
    long double tempSmallestDistance = 0;
    for ( unsigned long long i = 0; i < ( *ppFlightsContainer )->numberOfFlights; i ++ )
        for ( unsigned long long j = i + 1; j < ( *ppFlightsContainer )->numberOfFlights; j ++ )
        {
            if ( i != j )
            {
                Flight iterFlightA = *( ( *ppFlightsContainer )->flights + i );
                Flight iterFlightB = *( ( *ppFlightsContainer )->flights + j );
                tempSmallestDistance = getDistanceBetweenTwoFlights ( iterFlightA, iterFlightB );
                printf ( "%Lf\n", tempSmallestDistance );
                if ( ( *ppFlightsContainer )->smallestDistance >= tempSmallestDistance || ( i == 0 && j == 1 ) )
                {
                    ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs += 2;
                    if ( ( *ppFlightsContainer )->smallestDistance > tempSmallestDistance )
                    {
                        free ( ( *ppFlightsContainer )->arrayOfIndexesOfClosestPairs );
                        ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = 2;
                        if ( !( ( **ppFlightsContainer ).arrayOfIndexesOfClosestPairs =
                                ( unsigned long long * ) malloc ( ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long ) ) ) )
                            return 0;
                    }
                    if ( ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs > ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs )
                    {
                        ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs = ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs * 2;
                        if ( !( ( **ppFlightsContainer ).arrayOfIndexesOfClosestPairs =
                                ( unsigned long long * ) realloc (
                                        ( **ppFlightsContainer ).arrayOfIndexesOfClosestPairs,
                                        ( **ppFlightsContainer ).numberOfAllocatedIndexesOfClosestPairs * sizeof ( unsigned long long )
                                        ) ) )
                            return 0;
                    }
                    ( *ppFlightsContainer )->smallestDistance = tempSmallestDistance;
                    *( ( **ppFlightsContainer ).arrayOfIndexesOfClosestPairs + ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs - 2 ) = i;
                    *( ( **ppFlightsContainer ).arrayOfIndexesOfClosestPairs + ( **ppFlightsContainer ).numberOfIndexesOfClosestPairs - 1 ) = j;
                }
            }
        }
    return 1;
}

void                    printFlightNames                    ( FlightsContainer  * pFlightsContainer )
{
    checkFlightsContainerIntegrity ( &pFlightsContainer );
    unsigned char c = 0;
    for ( unsigned long long i = 0; i < ( *pFlightsContainer ).namesLength; i ++ )
    {
        c = * ( ( *pFlightsContainer ).names + i );
        if ( c == 0 )
            c = '\n';
        printf ("%c", c );
    }
}

int                     main                                ( void )
{
    FlightsContainer * flightsContainer = NULL;
    printf ( "Zadejte lety:\n" );
    if ( getFlights ( &flightsContainer ) )
    {
        calculateSmallestDistances ( &flightsContainer );
        printFlights ( flightsContainer );
        printFlightNames ( flightsContainer );
        printPairsOfFlightsWithSmallestDist ( flightsContainer );
    }
    else
        printf ( "Nespravny vstup.\n" );
    freeFlightsContainer ( &flightsContainer );
    return 0;
}