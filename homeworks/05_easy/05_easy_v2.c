//Program works properly
#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned long long numberOfColumns, numberOfRows, numberOfMines;
    unsigned char * playingAreaValues;
} PlayingArea;

int                     readAndAllocateFirstRow   ( PlayingArea         * playingArea )
{
    int ret = 0;
    if ( playingArea != NULL )
    {
        unsigned long long numberOfAllocatedFields = 16;
        playingArea->numberOfColumns = 0;
        playingArea->playingAreaValues = ( unsigned char * ) malloc ( ( size_t ) numberOfAllocatedFields * sizeof ( unsigned char ) );
        char readChar = 0;
        while ( EOF != ( ret = scanf ( "%c", &readChar ) ) && readChar != '\n' )
        {
            if ( ret == 1 && ( readChar == '*' || readChar == '.' ) )
            {
                if ( ++ ( playingArea->numberOfColumns ) > numberOfAllocatedFields )
                {
                    numberOfAllocatedFields *= 2;
                    playingArea->playingAreaValues = ( unsigned char * ) realloc (  playingArea->playingAreaValues, ( size_t ) numberOfAllocatedFields * sizeof ( unsigned char ) );
//                    if ( ! playingArea->playingAreaValues )
//                    {
//                        free ( playingArea->playingAreaValues );
//                        return 0;
//                    }
                }
                *( playingArea->playingAreaValues + ( playingArea->numberOfColumns ) - 1 ) = readChar;
            }
            else
            {
                ret = 0;
                break;
            }
        }
        if ( playingArea->numberOfColumns == 0 || ret == 0 )
        {
            free ( playingArea->playingAreaValues );
            playingArea->playingAreaValues = NULL;
            playingArea->numberOfColumns = 0;
            playingArea->numberOfRows = 0;
        }
        else
        {
            playingArea->playingAreaValues = ( unsigned char * ) realloc ( playingArea->playingAreaValues, ( size_t ) playingArea->numberOfColumns * sizeof ( unsigned char ) );
            playingArea->numberOfRows = 1;
        }
    }
    return ret;
}

int                     readNextLines               ( PlayingArea           * playingArea )
{
    int ret = 0;
    if ( playingArea != NULL && playingArea->playingAreaValues != NULL )
    {
        unsigned long long numberOfReadValidChars = 0, numberOfAllocatedRows = 32, indexOfCurrentlyUsedRow = playingArea->numberOfRows - 1;
        playingArea->playingAreaValues = ( unsigned char * ) realloc ( playingArea->playingAreaValues, ( size_t ) playingArea->numberOfColumns * numberOfAllocatedRows * sizeof ( unsigned char ) );
        unsigned char readChar = 0;
        while ( EOF != ( ret = scanf ( "%c", &readChar ) ) )
        {
            if      ( ret == 1 && ( readChar == '*' || readChar == '.' ) && ( ++ numberOfReadValidChars <= playingArea->numberOfColumns ) )
            {
                if ( numberOfReadValidChars == 1 )
                    indexOfCurrentlyUsedRow ++;
                if ( indexOfCurrentlyUsedRow + 1 >= numberOfAllocatedRows )
                {
                    numberOfAllocatedRows *= 2;
                    playingArea->playingAreaValues = ( unsigned char * ) realloc ( playingArea->playingAreaValues, ( size_t ) playingArea->numberOfColumns * numberOfAllocatedRows * sizeof ( unsigned char ) );
                }
                *( playingArea->playingAreaValues + ( indexOfCurrentlyUsedRow * playingArea->numberOfColumns ) + numberOfReadValidChars - 1 ) = readChar;
            }
            else if ( ret == 1 && readChar == '\n' && numberOfReadValidChars == playingArea->numberOfColumns )
                numberOfReadValidChars = 0;
            else
            {
                ret = 0;
                break;
            }
        }
        if ( ( numberOfReadValidChars != 0 && numberOfReadValidChars != playingArea->numberOfColumns ) || ret == 0 )
        {
            free ( playingArea->playingAreaValues );
            playingArea->playingAreaValues = NULL;
        }
        else
            playingArea->numberOfRows = indexOfCurrentlyUsedRow + 1;
    }
    return ret;
}

int                     incrementPointersAroundMine ( PlayingArea           * playingArea,
                                                      unsigned long long      minesRowNumber,
                                                      unsigned long long      minesColumnNumber )
{
    if ( playingArea == NULL || playingArea->playingAreaValues == NULL || minesColumnNumber >= playingArea->numberOfColumns || minesRowNumber >= playingArea->numberOfRows )
        return 0;
    int prevRow = -1, nextRow = 1, prevCol = -1, nextCol = 1;
    if ( minesColumnNumber  == 0 )                                  prevCol = 0;
    if ( minesColumnNumber  == playingArea->numberOfColumns - 1 )   nextCol = 0;
    if ( minesRowNumber     == 0 )                                  prevRow = 0;
    if ( minesRowNumber     == playingArea->numberOfRows - 1 )      nextRow = 0;
    for ( int i = prevRow; i <= nextRow; i ++ )
    {
        for ( int j = prevCol; j <= nextCol; j ++ )
        {
            unsigned char * ptr = playingArea->playingAreaValues + ( ( minesRowNumber + i ) * playingArea->numberOfColumns ) + minesColumnNumber + j;
            if      ( *ptr == '.' )
                ( *ptr ) = '1';
            else if ( *ptr != '*' )
                ( *ptr ) ++;
        }
    }
    return 1;
}

int                     putNumbersIntoPlayingArea   ( PlayingArea           * playingArea )
{
    if ( playingArea == NULL )
        return 0;
    for ( unsigned long long i = 0; i < playingArea->numberOfRows; i ++ )
        for ( unsigned long long j = 0; j < playingArea->numberOfColumns; j ++ )
        {
            if ( *( playingArea->playingAreaValues + i * playingArea->numberOfColumns + j ) == '*' )
                incrementPointersAroundMine ( playingArea, i, j );
        }
    return 1;
}

void                    printPlayingArea            ( PlayingArea           * playingArea )
{
    if ( playingArea != NULL && playingArea->playingAreaValues != NULL )
    {
        for ( unsigned long long i = 0; i < playingArea->numberOfRows; i ++ )
        {
            for ( unsigned long long j = 0; j < playingArea->numberOfColumns; j ++ )
                printf ( "%c", *( playingArea->playingAreaValues + ( i * playingArea->numberOfColumns ) + j ) );
            printf ( "\n" );
        }
    }
}

int                     main                        ( void )
{
    PlayingArea * playingArea = ( PlayingArea * ) malloc ( sizeof ( PlayingArea ) );
    printf ( "Zadejte hraci plochu:\n" );
    if ( readAndAllocateFirstRow ( playingArea ) && readNextLines ( playingArea ) )
    {
//        printPlayingArea ( playingArea );
        putNumbersIntoPlayingArea ( playingArea );
        printf ( "Vyplnena hraci plocha:\n" );
        printPlayingArea ( playingArea );
        free ( playingArea->playingAreaValues );
    }
    else
        printf ( "Nespravny vstup.\n" );
    free ( playingArea );
    return 0;
}