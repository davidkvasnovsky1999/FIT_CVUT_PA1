#include <stdio.h>
#include <errno.h>
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
                            numberOfAllocatedPhrasePointers;
    Phrase               ** phrases;
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
        phrasesContainer->numberOfAllocatedPhrasePointers = 8;
        phrasesContainer->phrases = ( Phrase ** ) malloc ( phrasesContainer->numberOfAllocatedPhrasePointers * sizeof ( Phrase * ) );
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
    do
    {
        readChar = fgetc ( stream );
        if ( readChar == delimiter )
            readChar = 0;
        else if ( !( STARTING_ASCII_CHAR <= readChar && readChar <= ENDING_ASCII_CHAR ) )
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
    while ( readChar != 0 );
    *numberOfAllocatedChars = lineLength;
    *linePointer = ( unsigned char * ) realloc ( *linePointer, *numberOfAllocatedChars );
    if ( *linePointer == NULL )
        exit ( 1 );
    return ( int ) numberOfAllocatedChars;
}

int                 readWordFrequency                       ( long double               * wordFrequency )
{
    unsigned char * readWordFreq = NULL;
    size_t          size         = 0;
    int             ret          = 1;
    if ( ( ret = getLineOfAllowedChars ( &readWordFreq, &size, ':', stdin ) ) )
    {
        ret = sscanf ( ( const char * ) readWordFreq, "%Lf", wordFrequency );
        free ( readWordFreq );
    }
    return ret;
}

int                 loadPhrases                             ( PhrasesContainer         ** phrasesContainer )
{
    if ( phrasesContainer == NULL )
        return 0;
    if ( *phrasesContainer == NULL && ( *phrasesContainer = getNewInitPhraseContainer ( ) ) == NULL )
        exit ( 1 );
    int             ret0                                = 1,
                    ret1                                = 1;
    size_t          numberOfAllocatedCharsForWordFreq   = 0,
                    numberOfAllocatedCharsForWord       = 0;
    long double     tempWordFrequency                   = 0;
    unsigned char * readWord                            = NULL;
    Phrase        * newPhrase                           = NULL;
    while ( ( ret0 = readWordFrequency ( &tempWordFrequency ) ) )
    {
        ret1 = getLineOfAllowedChars ( &readWord, &numberOfAllocatedCharsForWord, '\n', stdin );
        if ( ret1 == 1 && numberOfAllocatedCharsForWord > 1 )
        {
            if ( (*phrasesContainer)->numberOfPhrases + 1 >= (*phrasesContainer)->numberOfAllocatedPhrasePointers )
            {
                (*phrasesContainer)->numberOfAllocatedPhrasePointers = ( (*phrasesContainer)->numberOfPhrases + 1 ) * 2;
                if ( ( (*phrasesContainer)->phrases = ( Phrase ** ) realloc ( (*phrasesContainer)->phrases,
                        (*phrasesContainer)->numberOfAllocatedPhrasePointers * sizeof ( Phrase * ) ) ) == NULL )
                    exit ( 0 );
            }
            if ( ( newPhrase = ( Phrase * ) malloc ( sizeof ( Phrase ) ) ) == NULL )
                exit ( 1 );
            newPhrase->word = readWord;
            newPhrase->wordFrequency = tempWordFrequency;
            *( (*phrasesContainer)->phrases + (*phrasesContainer)->numberOfPhrases ++ ) = newPhrase;
        }
        else break;
    }
    if ( ret0 != EOF && ret0 != 1 )
        return 0;
    return 1;
}

int                 comparePhrasesByWordFrequency           ( const void                * p,
                                                              const void                * q )
{
    if ( p == NULL || q == NULL )
        return 0;
    long double pWordFreq       = ( ( Phrase * ) p )->wordFrequency,
                qWordFreq       = ( ( Phrase * ) q )->wordFrequency,
                areAlmostEqual  = fabsl ( pWordFreq - qWordFreq ) <= qWordFreq / 1024;
    if ( !areAlmostEqual )
        return pWordFreq > qWordFreq ? -1 : 1;
    else
        return 0;
}

int                 comparePhrasesByWord                    ( const void                * p,
                                                              const void                * q )
{
    if ( p == NULL || q == NULL )
        return 0;
    unsigned char * pWord = ( ( Phrase * ) p )->word,
                  * qWord = ( ( Phrase * ) q )->word;
    return strcmp ( ( const char * ) pWord, ( const char * ) qWord );
}

void                printAllPhrases                         ( PhrasesContainer          * phrasesContainer )
{
    if ( phrasesContainer != NULL )
    {
        printf ( "Number of phrases: %llu\n", phrasesContainer->numberOfPhrases );
        for ( unsigned long long i = 0; i < phrasesContainer->numberOfPhrases; i ++ )
            printf ( "wordFreq = %Lf; word = %s\n", (*( phrasesContainer->phrases + i ))->wordFrequency, (*( phrasesContainer->phrases + i ))->word );
    }
}

void                freePhrasesContainer                    ( PhrasesContainer         ** phrasesContainer )
{
    if ( phrasesContainer != NULL && *phrasesContainer != NULL )
    {
        Phrase * ppPhrase = NULL;
        for ( unsigned long long i = 0; i < (*phrasesContainer)->numberOfPhrases; i ++ )
        {
            ppPhrase = *((*phrasesContainer)->phrases + i);
            free ( ppPhrase->word );
            free ( ppPhrase );
        }
        free ( (*phrasesContainer)->phrases );
        free ( *phrasesContainer );
        *phrasesContainer = NULL;
    }
}

int                 main                                    ( void )
{
    PhrasesContainer * pPhrasesContainer = NULL;
    if ( loadPhrases ( &pPhrasesContainer ) )
    {
//        qsort ( pPhrasesContainer->phrases, pPhrasesContainer->numberOfPhrases, sizeof ( Phrase * ), comparePhrasesByWord );
//        printAllPhrases ( pPhrasesContainer );
    }
    else
        printf ( "Nespravny vstup\n" );
    freePhrasesContainer ( &pPhrasesContainer );
    return 0;
}