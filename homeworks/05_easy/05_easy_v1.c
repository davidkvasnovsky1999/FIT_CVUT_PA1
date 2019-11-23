#include <stdio.h>
#include <stdlib.h>

int                     readAndAllocateFirstRow   ( int           * numberOfColumns,
                                                    char         ** playingArea )
{
    *numberOfColumns = 0;
    int ret = 0, numberOfAllocatedFields = 16;
    if ( playingArea != NULL )
    {
        *playingArea = ( char * ) malloc ( numberOfAllocatedFields * sizeof ( char ) );
        char readChar = 0;
        while ( EOF != ( ret = scanf ( "%c", &readChar ) ) && readChar != '\n' )
        {
            if ( ret == 1 && ( readChar == '*' || readChar == '.' ) )
            {
                if ( ++ ( *numberOfColumns ) > numberOfAllocatedFields )
                {
                    numberOfAllocatedFields *= 2;
                    *playingArea = ( char * ) realloc ( *playingArea, numberOfAllocatedFields * sizeof ( char ) );
                }
                *( ( *playingArea ) + ( *numberOfColumns ) - 1 ) = readChar;
            }
            else
            {
                ret = 0;
                break;
            }
        }
        if ( *numberOfColumns == 0 || ret == 0 )
        {
            free ( *playingArea );
            *playingArea = NULL;
            *numberOfColumns = 0;
        }
        else
            *playingArea = ( char * ) realloc ( *playingArea, *numberOfColumns * sizeof ( char ) );
    }
    return ret;
}

int                     readLine                    ( int             rowNumber,
                                                      int             numberOfColumns,
                                                      char         ** playingArea )
{
    int ret = 0, numberOfReadValidChars = 0;
    if ( playingArea != NULL && *playingArea != NULL )
    {
        char readChar = 0;
        while ( EOF != ( ret = scanf ( "%c", &readChar ) ) && readChar != '\n' )
        {
            if ( ret == 1 && ( readChar == '*' || readChar == '.' ) && ( ++ numberOfReadValidChars <= numberOfColumns ) )
                *( ( *playingArea ) + ( rowNumber * numberOfColumns ) + numberOfReadValidChars - 1 ) = readChar;
            else
            {
                ret = 0;
                break;
            }
        }
        if ( numberOfReadValidChars != numberOfColumns || ret == 0 )
        {
            free ( *playingArea );
            *playingArea = NULL;
        }
    }
    return ret;
}



int                     getPlayingAreaFromInput     ( char         ** playingArea,
                                                      int           * numberOfRows,
                                                      int           * numberOfColumns )
// Returns list of read unsigned integers. Returns NULL if an error occurred or no number was read.
{
    *playingArea = NULL;
    if ( readAndAllocateFirstRow ( numberOfColumns, playingArea ) )
    {
        *numberOfRows = 0;
        int numberOfAllocatedRows = 32, ret = 0;
        *playingArea = ( char * ) realloc ( *playingArea, *numberOfColumns * numberOfAllocatedRows * sizeof ( char ) );
        do
        {
            if ( ++ ( *numberOfRows ) >= numberOfAllocatedRows )
            {
                numberOfAllocatedRows *= 2;
                *playingArea = ( char * ) realloc ( *playingArea, *numberOfColumns * numberOfAllocatedRows * sizeof ( char ) );
            }
            ret = readLine ( ( *numberOfRows ) - 1, *numberOfColumns, playingArea );
            if ( ret == 0 )
                return 0;
        }
        while ( ret != EOF );
        return 1;
    }
    return 0;
}




int                     main                        ( void )
{
    char * playingArea = NULL;
    int numberOfRows, numberOfColumns;
    printf ( "Zadejte hraci plochu:\n" );
    if ( getPlayingAreaFromInput ( &playingArea, &numberOfRows, &numberOfColumns ) )
    {
        for ( int i = 0; i < numberOfRows; i ++ )
        {
            for ( int j = 0; j < numberOfColumns; j ++ )
                printf ( "%c", playingArea [ i * numberOfColumns + j ] );
            printf ( "\n" );
        }
    }
    else
        printf ( "Nespravny vstup.\n" );
    return 0;
}