#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//#define boolean char;
#define true 1
#define false 0
#define STARTING_ASCII_CHAR 32
#define ENDING_ASCII_CHAR 126

typedef unsigned char boolean;

typedef struct Phrase
{
    long double             wordFrequency;
    unsigned char         * word;
} Phrase;

typedef struct PhrasesContainer
{
    unsigned long long      numberOfPhrases,
                            numberOfAllocatedPhrases;
    Phrase                * phrases;
} PhrasesContainer;

int                 getNumberOfAllowedCharsForPhrase        ( )
{
    int n = STARTING_ASCII_CHAR;
    int m = ENDING_ASCII_CHAR;
    return n - m;
}

PhrasesContainer *  getNewInitPhraseContainer               ( )
{
    PhrasesContainer * phrasesContainer = ( PhrasesContainer * ) malloc ( sizeof ( PhrasesContainer ) );
    if ( phrasesContainer != NULL )
    {
        phrasesContainer->numberOfPhrases = 0;
        phrasesContainer->numberOfAllocatedPhrases = 8;
        phrasesContainer->phrases = ( Phrase * ) malloc ( phrasesContainer->numberOfAllocatedPhrases * sizeof ( Phrase ) );
        if ( phrasesContainer->phrases == NULL )
        {
            free ( phrasesContainer );
            phrasesContainer = NULL;
        }
    }
    return phrasesContainer;
}

int                 getLineOfAllowedChars                   ( unsigned char            ** linePointer,
                                                              size_t                    * numberOfAllocatedChars,
                                                              unsigned char               delimiter,
                                                              FILE                      * stream )
{
    if ( linePointer == NULL || numberOfAllocatedChars == NULL )
        return 0;
    if ( *linePointer == NULL || *numberOfAllocatedChars == 0 )
    {
        *numberOfAllocatedChars = 64;
        *linePointer = ( unsigned char * ) malloc ( *numberOfAllocatedChars );
        if ( *linePointer == NULL )
            exit ( 1 );
    }
    int readChar = 0;
    size_t lineLength = 0;
    while ( ( readChar = fgetc ( stream ) ) != 0 )
    {
        if ( readChar == delimiter )
            readChar = 0;
        else if ( !( STARTING_ASCII_CHAR <= readChar && readChar >= ENDING_ASCII_CHAR ) )
        {
            free ( *linePointer );
            *numberOfAllocatedChars = 0;
            *linePointer = NULL;
            return readChar == EOF && lineLength == 0 ? EOF : 0;
        }
        if ( lineLength + 1 > *numberOfAllocatedChars )
        {
            *numberOfAllocatedChars = ( lineLength + 1 ) * 2;
            *linePointer = ( unsigned char * ) realloc ( *linePointer, *numberOfAllocatedChars );
            if ( *linePointer == NULL )
                exit ( 1 );
        }
        *( ( *linePointer ) + lineLength ++ ) = ( unsigned char ) readChar;
    }
    *numberOfAllocatedChars = lineLength;
    *linePointer = ( unsigned char * ) realloc ( *linePointer, *numberOfAllocatedChars );
    if ( *linePointer == NULL )
        exit ( 1 );
    return 1;
}

int                 loadPhrases                             ( PhrasesContainer         ** phrasesContainer )
{
    if ( *phrasesContainer == NULL && ( *phrasesContainer = getNewInitPhraseContainer ( ) ) == NULL )
        return 0;
    int             ret                 = 0;
    long double     tempWordFrequency   = 0;
    unsigned char   charAfterWF         = 0,
                  * readFreqNumber      = ( unsigned char * ) malloc ( 64 ),
                  * readPhrase          = NULL;
    size_t numberOfAllocatedChars = 0;
    while ( 1 )
    {
        ret = getLineOfAllowedChars ( &readFreqNumber, &numberOfAllocatedChars, '\n', stdin );
        if ( EOF == ret )
            break;
    }
    return 1;
}


int                 main                                    ( void )
{
    PhrasesContainer * pPhrasesContainer = NULL;
    if ( !loadPhrases ( &pPhrasesContainer ) )
        return 1;
    
    
    return 0;
}