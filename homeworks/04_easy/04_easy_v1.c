#include <stdio.h>
#include <stdlib.h>



int                 getPartsLengthsFromInput    ( long long    ** partsLengths )
// Returns number of successfully read unsigned integers greater than 0 (numberOfParts) from stdin.
// Creates an array of read parts and stores it into *partsLengths. If numberOfParts is equal to 0 then *partsLengths is NULL.
// Dynamically allocated cell count of the array stored in *partsLengths is equal to number of its elements.
{
    int arraySize = 32, ret = 0, numberOfParts = 0;
    *partsLengths = ( long long * ) malloc ( arraySize * sizeof ( long long ) );
    long long readNumber = 0;
    while ( EOF != ( ret = scanf ( " %lld ", &readNumber ) ) )
    {
        if ( ret == 1 && readNumber > 0 )
        {
            if ( ( numberOfParts ) ++ >= arraySize )
            {
                arraySize *= 2;
                *partsLengths = ( long long * ) realloc ( *partsLengths, arraySize * sizeof ( long long ) );
            }
            *( ( *partsLengths ) + numberOfParts ) = readNumber;
        }
        else
        {
            numberOfParts = 0;
            break;
        }
    }
    if ( numberOfParts == 0 )
    {
        free ( *partsLengths );
        *partsLengths = NULL;
    }
    else
        *partsLengths = ( long long * ) realloc ( *partsLengths, numberOfParts * sizeof ( long long ) );
    return numberOfParts;
}




int                 main                        ( void )
{
    printf ( "Zadejte delky:\n" );
    long long * partsLengths = NULL;
    int partsCount = 0;
    if ( ( partsCount = getPartsLengthsFromInput ( &partsLengths ) ) && partsCount <= 500000 )
    {
        for ( int i = 0; i < partsCount; i ++ )
            printf ( "%lld ", partsLengths [ i ] );
        printf ( "\n" );
        free ( partsLengths );
    }
    else
        printf ( "Nespravny vstup.\n" );
    return 0;
}