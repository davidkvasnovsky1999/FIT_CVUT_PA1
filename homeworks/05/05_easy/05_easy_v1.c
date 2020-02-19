//Program does not work

#include <stdio.h>
#include <stdlib.h>

#define PLAYING_AREA_CHARS ".12345678*"

const char * playingAreaChars = PLAYING_AREA_CHARS;

typedef struct
{
    unsigned long long numberOfColumns, numberOfRows, numberOfMines;
    char ** playingAreaValues;
} PlayingArea;



int                     getIndexOf                ( const char            playingAreaChar )
{
    if ( playingAreaChar == '*' )
        return 9;
    else
        return 0;
}

int                     readAndAllocateFirstRow   ( PlayingArea         * playingArea )
{
    int ret = 0;
    if ( playingArea != NULL )
    {
        unsigned long long numberOfAllocatedFields = 16;
        playingArea->numberOfColumns = 0;
        playingArea->playingAreaValues = ( char ** ) malloc ( numberOfAllocatedFields * sizeof ( char * ) );
        char readChar = 0;
        while ( ( ret = scanf ( "%c", &readChar ) ) && readChar != '\n' )
        {
            if ( ret == 1 && ( readChar == '*' || readChar == '.' ) )
            {
                if ( ++ ( playingArea->numberOfColumns ) > numberOfAllocatedFields )
                {
                    numberOfAllocatedFields *= 2;
                    playingArea->playingAreaValues = ( char ** ) realloc ( playingArea->playingAreaValues, numberOfAllocatedFields * sizeof ( char * ) );
                }
                *( playingArea->playingAreaValues + ( playingArea->numberOfColumns ) - 1 ) = ( char * ) ( playingAreaChars + getIndexOf ( readChar ) );
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
            playingArea->playingAreaValues = ( char ** ) realloc ( playingArea->playingAreaValues, playingArea->numberOfColumns * sizeof ( char * ) );
            playingArea->numberOfRows = 1;
        }
    }
    return ret;
}

int                     readLine                    ( unsigned long long      rowNumber,
                                                      PlayingArea           * playingArea )
{
    int ret = 0;
    unsigned long long numberOfReadValidChars = 0;
    if ( playingArea != NULL && playingArea->playingAreaValues != NULL )
    {
        char readChar;
        while ( EOF != ( ret = scanf ( "%c", &readChar ) ) && readChar != '\n' && readChar != 'k' )
        {
            if ( ret == 1 && ( readChar == '*' || readChar == '.' ) && ( ++ numberOfReadValidChars <= playingArea->numberOfColumns ) )
                *( playingArea->playingAreaValues + ( rowNumber * playingArea->numberOfColumns ) + numberOfReadValidChars - 1 ) = ( char * ) ( playingAreaChars + getIndexOf ( readChar ) );
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
        if ( readChar == 'k' ) ret = EOF;
    }
    return ret;
}

int                     getPlayingAreaFromInput     ( PlayingArea           * playingArea )
{
    if ( playingArea != NULL && readAndAllocateFirstRow ( playingArea ) )
    {
        unsigned long long numberOfAllocatedRows = 32;
        int ret = 0;
        playingArea->playingAreaValues = ( char ** ) realloc ( playingArea->playingAreaValues, playingArea->numberOfColumns * numberOfAllocatedRows * sizeof ( char * ) );
        while ( 1 )
        {
            if ( ( playingArea->numberOfRows ) ++ >= numberOfAllocatedRows )
            {
                numberOfAllocatedRows *= 2;
                playingArea->playingAreaValues = ( char ** ) realloc ( playingArea->playingAreaValues, playingArea->numberOfColumns * numberOfAllocatedRows * sizeof ( char * ) );
            }
            ret = readLine ( ( playingArea->numberOfRows ) - 1, playingArea );
            if      ( ret == 0 )
                return 0;
            else if ( ret == EOF )
                return 1;
        }
    }
    return 0;
}


int                     incrementPointersAroundMine ( PlayingArea           * playingArea,
                                                      unsigned long long      minesRowNumber,
                                                      unsigned long long      minesColumnNumber )
{
    if ( playingArea == NULL || minesColumnNumber >= playingArea->numberOfColumns || minesRowNumber >= playingArea->numberOfRows )
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
            char ** ptr = playingArea->playingAreaValues + ( ( minesRowNumber + i ) * playingArea->numberOfColumns ) + minesColumnNumber + j;
            if ( **ptr != '*' )
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
            if ( **( playingArea->playingAreaValues + i * playingArea->numberOfColumns + j ) == '*' )
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
                printf ( "%c", **( playingArea->playingAreaValues + ( i * playingArea->numberOfColumns ) + j ) );
            printf ( "\n" );
        }
    }
}


int                     main                        ( void )
{
    PlayingArea * playingArea = ( PlayingArea * ) malloc ( sizeof ( PlayingArea ) );
    printf ( "Zadejte hraci plochu:\n" );
    if ( getPlayingAreaFromInput ( playingArea ) )
    {
        printPlayingArea ( playingArea );
//        putNumbersIntoPlayingArea ( &playingArea );
//        printf ( "Vyplnena hraci plocha:\n" );
//        printPlayingArea ( *playingArea );
        free ( playingArea->playingAreaValues );
        free ( playingArea );
    }
    else
        printf ( "Nespravny vstup.\n" );
    return 0;
}