#include <stdio.h>
#include <stdlib.h>

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
    unsigned long long numberOfFlights, namesLength, numberOfAllocatedCharsForNames, numberOfAllocatedFlights;
    unsigned char * names;//Names are separated by '\n'
    Flight * flights;
} FlightsContainer;


int                     writeCharToFCNames      ( unsigned char       readChar,
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

int                     readFlightsName         ( FlightsContainer  * flightsContainer,
                                                  Flight            * currentFlight )
{
    if ( flightsContainer == NULL || currentFlight == NULL )
        return 0;
    int readChar = 0, nonWhiteSpaceCharRead = 0;
    while ( ( readChar = fgetc ( stdin ) ) )
    {
        if ( nonWhiteSpaceCharRead == 0 && readChar != 9 && readChar != 11 && readChar != 13 && readChar != 32 )
        {
            currentFlight->name = flightsContainer->names + flightsContainer->namesLength;
            nonWhiteSpaceCharRead = 1;
        }
        if ( nonWhiteSpaceCharRead == 1 )
        {
            writeCharToFCNames ( ( unsigned char ) readChar, flightsContainer );
            if ( readChar == '\n' )
            {
                writeCharToFCNames ( 0, flightsContainer );
                break;
            }
        }
    }
    if ( feof ( stdin ) != 0 && ferror ( stdin ) != 0 )
        return 0;
    return 1;
}

void                    freeFlightsContainer    ( FlightsContainer ** flightsContainer )
{
    if ( flightsContainer != NULL && *flightsContainer != NULL )
    {
        free ( ( *flightsContainer )->names );
        free ( ( *flightsContainer )->flights );
        free ( *flightsContainer );
        *flightsContainer = NULL;
    }
}

int                     initFlightsContainer    ( FlightsContainer ** flightsContainer )
{
    if ( flightsContainer == NULL )
        return 0;
    freeFlightsContainer ( flightsContainer );
    *flightsContainer = ( FlightsContainer * ) malloc ( sizeof ( FlightsContainer ) );
    (*flightsContainer)->namesLength = 0;
    (*flightsContainer)->numberOfFlights = 0;
    (*flightsContainer)->numberOfAllocatedCharsForNames = 128;
    (*flightsContainer)->numberOfAllocatedFlights = 8;
    (*flightsContainer)->names = ( unsigned char * ) malloc ( ( *flightsContainer )->numberOfAllocatedCharsForNames * sizeof ( unsigned char ) );
    (*flightsContainer)->flights = ( Flight * ) malloc ( ( *flightsContainer )->numberOfAllocatedFlights * sizeof ( Flight ) );
    return 1;
}

int                     getFlights              ( FlightsContainer ** flightsContainer )
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
    if ( ret != EOF || charForFormatCheck != ']' )
        return 0;
    return 1;
}

int                     printFlights            ( FlightsContainer  * flightsContainer )
{
    if ( flightsContainer == NULL || flightsContainer->names == NULL || flightsContainer->flights == NULL )
        return 0;
    printf ( "Zadene lety:\n" );
    for ( unsigned long long i = 0; i < flightsContainer->numberOfFlights; i ++ )
    {
        Flight * iterFlight = flightsContainer->flights + i;
        if ( iterFlight->name == NULL )
            return 0;
        printf ( "[%Lf, %Lf] %s", iterFlight->x, iterFlight->y, iterFlight->name );
    }
    return 1;
}


int                     main                    ( void )
{
    FlightsContainer * flightsContainer = NULL;
    printf ( "Zadejte lety:\n" );
    if ( getFlights ( &flightsContainer ) )
    {
        printFlights ( flightsContainer );
    }
    else
        printf ( "Nespravny vstup.\n" );
    freeFlightsContainer ( &flightsContainer );
    return 0;
}