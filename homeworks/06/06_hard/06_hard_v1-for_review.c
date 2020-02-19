#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

#define true 1
#define false 0
#define STARTING_ASCII_CHAR 32
#define ENDING_ASCII_CHAR 126

typedef unsigned char boolean;

typedef struct Phrase
{
    long double             wordFrequency;
    char                  * word;
} Phrase;

typedef struct PhrasesContainer
{
    unsigned long long      numberOfPhrases,
                            numberOfAllocatedPhrasePointers;
    Phrase               ** phrases;
} PhrasesContainer;

PhrasesContainer *  getNewPhrasesContainer                  ( )
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

int                 getLineOfAllowedChars                   ( char                     ** linePointer,
                                                              size_t                    * numberOfAllocatedChars,
                                                              char                        delimiter,
                                                              FILE                      * stream )
{
    if ( linePointer == NULL || numberOfAllocatedChars == NULL )
        return 0;
    if ( *linePointer == NULL || *numberOfAllocatedChars == 0 )
    {
        *numberOfAllocatedChars = 64;
        *linePointer = ( char * ) malloc ( *numberOfAllocatedChars );
        if ( *linePointer == NULL )
            exit ( EXIT_FAILURE );
    }
    int     readChar    = 0;
    size_t  lineLength  = 0;
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
            return ( lineLength == 0 && ( readChar == '\n' || readChar == EOF ) ) ? EOF : 0;
        }
        if ( lineLength + 1 > *numberOfAllocatedChars )
        {
            *numberOfAllocatedChars = ( lineLength + 1 ) * 2;
            *linePointer = ( char * ) realloc ( *linePointer, *numberOfAllocatedChars );
            if ( *linePointer == NULL )
                exit ( EXIT_FAILURE );
        }
        *( ( *linePointer ) + lineLength ++ ) = ( char ) readChar;
    }
    while ( readChar != 0 );
    *numberOfAllocatedChars = lineLength;
    *linePointer = ( char * ) realloc ( *linePointer, *numberOfAllocatedChars );
    if ( *linePointer == NULL )
        exit ( EXIT_FAILURE );
    return 1;
}

int                 getWordFrequency                        ( long double               * wordFrequency )
{
    if ( wordFrequency == NULL )
        return 0;
    char          * readWordFreq = NULL;
    size_t          size         = 0;
    int             ret          = 1;
    if ( ( ret = getLineOfAllowedChars ( &readWordFreq, &size, ':', stdin ) ) ==  1 )
    {
        ret = sscanf ( ( const char * ) readWordFreq, "%Lf", wordFrequency );
        if ( ret == EOF )
            ret = 0;
    }
    free ( readWordFreq );
    return ret;
}

void                ensureMemSpaceForPhrases                ( PhrasesContainer         ** phrasesContainer )
{
    if ( (*phrasesContainer)->numberOfPhrases >= (*phrasesContainer)->numberOfAllocatedPhrasePointers )
    {
        (*phrasesContainer)->numberOfAllocatedPhrasePointers = (*phrasesContainer)->numberOfPhrases * 2;
        if ( ( (*phrasesContainer)->phrases = ( Phrase ** ) realloc ( (*phrasesContainer)->phrases,
                (*phrasesContainer)->numberOfAllocatedPhrasePointers * sizeof ( Phrase * ) ) ) == NULL )
            exit ( EXIT_FAILURE );
    }
}

int                 loadPhrases                             ( PhrasesContainer         ** phrasesContainer )
{
    if ( phrasesContainer == NULL )
        return 0;
    if ( *phrasesContainer == NULL && ( *phrasesContainer = getNewPhrasesContainer ( ) ) == NULL )
        exit ( EXIT_FAILURE );
    int                ret0                                = 1,
                       ret1                                = 1;
    size_t             numberOfAllocatedCharsForWord       = 0;
    long double        tempWordFrequency                   = 0;
    char            ** readWord                            = ( char ** )   malloc ( sizeof ( char *) );
    Phrase          ** newPhrase                           = ( Phrase ** ) malloc ( sizeof ( Phrase *) );
    printf ( "Casto hledane fraze:\n" );
    while ( ( ret0 = getWordFrequency ( &tempWordFrequency ) ) > 0 )
    {
        *readWord = NULL;
        ret1 = getLineOfAllowedChars ( readWord, &numberOfAllocatedCharsForWord, '\n', stdin );
        if ( ret1 == 1 && numberOfAllocatedCharsForWord > 1 )
        {
            (*phrasesContainer)->numberOfPhrases ++;
            ensureMemSpaceForPhrases ( phrasesContainer );
            if ( ( *newPhrase = ( Phrase * ) malloc ( sizeof ( Phrase ) ) ) == NULL )
                exit ( EXIT_FAILURE );
            (*newPhrase)->word = *readWord;
            (*newPhrase)->wordFrequency = tempWordFrequency;
            *( (*phrasesContainer)->phrases + (*phrasesContainer)->numberOfPhrases - 1 ) = *newPhrase;
        }
        else
            break;
    }
    *( (*phrasesContainer)->phrases + (*phrasesContainer)->numberOfPhrases ) = NULL;
    (*phrasesContainer)->numberOfAllocatedPhrasePointers = (*phrasesContainer)->numberOfPhrases + 1;
    if ( ( (*phrasesContainer)->phrases = ( Phrase ** ) realloc ( (*phrasesContainer)->phrases,
            (*phrasesContainer)->numberOfAllocatedPhrasePointers * sizeof ( Phrase * ) ) ) == NULL )
        exit ( EXIT_FAILURE );
    free ( readWord );
    free ( newPhrase );
    if ( ret0 != EOF || numberOfAllocatedCharsForWord < 2 )
        return 0;
    return 1;
}


int                 comparePhrasesByWordFrequency           ( const void                * p,
                                                              const void                * q )
{
    if ( p == NULL || q == NULL )
        return 0;
    long double pWordFreq       = (*( ( Phrase ** ) p ))->wordFrequency,
                qWordFreq       = (*( ( Phrase ** ) q ))->wordFrequency,
                areAlmostEqual  = fabsl ( pWordFreq - qWordFreq ) < 1024 * LDBL_EPSILON * pWordFreq * qWordFreq;
    if ( !areAlmostEqual )
        return pWordFreq > qWordFreq ? -1 : 1;
    return 0;
}

int                 compareChars                            ( const char                  charA,
                                                              const char                  charB,
                                                              boolean                     caseInsensitive )
{
    if ( charA != charB )
    {
        if
        (   caseInsensitive == 1
            &&
            (
                ( 'a' <= charA && charA <= 'z' && charA - 32 == charB )
                ||
                ( 'A' <= charA && charA <= 'Z' && charA + 32 == charB )
            )
        )
            return 0;
        return charA < charB ? -1 : 1;
    }
    return 0;
}

int                 caseInsensitiveStringCmp                ( const char                * stringA,
                                                              const char                * stringB,
                                                              unsigned long long          numberOfCharsToCompare,
                                                              size_t                    * indexOfFirstNotMatchingChar )
{
    if ( stringA == NULL || stringB == NULL )
        return 0;
    unsigned long long      lengthOfStringA = strlen ( stringA ),
                            lengthOfStringB = strlen ( stringB ),
                          * biggerLength    = lengthOfStringA > lengthOfStringB ? &lengthOfStringA : &lengthOfStringB;
    int ret = 0;
    if ( numberOfCharsToCompare > *biggerLength )
        numberOfCharsToCompare = *biggerLength;
    for ( size_t i = 0; i < numberOfCharsToCompare; i ++ )
    {
        if ( ( ret = compareChars ( stringA [ i ], stringB [ i ], true ) ) != 0 )
        {
            *indexOfFirstNotMatchingChar = i;
            return ret;
        }
        if ( stringA [ i ] == 0 || stringB [ i ] == 0 )
            break;
    }
    *indexOfFirstNotMatchingChar = 0;
    return 0;
}

int                 isStrInsideOf                           ( const char                * string,
                                                              const char                * stringToFind )
{
    if ( string == NULL || stringToFind == NULL || strlen ( string ) < strlen ( stringToFind ) )
        return 0;
    char      * stringToCompare         = ( char * ) string;
    size_t      indexOfNotMatchingChar  = 0;
    int         ret                     = 1;
    do
    {
        stringToCompare += indexOfNotMatchingChar;
        while ( *stringToCompare != 0 && compareChars ( *stringToCompare, *stringToFind, true ) != 0 )
            stringToCompare ++;
    }
    while ( *stringToCompare != 0 && 0 != ( ret = caseInsensitiveStringCmp ( stringToCompare, stringToFind, strlen ( stringToFind ), &indexOfNotMatchingChar ) ) );
    return ret == 0 ? 1 : 0;
}

unsigned long long  getAllPhrasesStartingWith               ( const char                * string,
                                                              const PhrasesContainer    * pPhrasesContainer,
                                                              Phrase                  *** foundPhrases )
{
    unsigned long long numberOfMatchingPhrases = 0;
    if ( foundPhrases != NULL )
    {
        if ( *foundPhrases != NULL )
        {
            free ( *foundPhrases );
            *foundPhrases = NULL;
        }
        if ( !( string == NULL || pPhrasesContainer == NULL || pPhrasesContainer->numberOfPhrases == 0 || pPhrasesContainer->phrases == NULL ) )
        {
            size_t numberOfAllocatedPhrasePointers = 16;
            if ( *foundPhrases == NULL )
            {
                *foundPhrases = ( Phrase ** ) malloc ( sizeof ( Phrase * ) * numberOfAllocatedPhrasePointers );
                if ( *foundPhrases == NULL )
                    exit ( EXIT_FAILURE );
            }
            for ( unsigned long long i = 0; i < pPhrasesContainer->numberOfPhrases; i ++ )
            {
                Phrase * iterPhrase = *( pPhrasesContainer->phrases + i );
                if ( isStrInsideOf ( iterPhrase->word, string ) == 1 )
                {
                    if ( numberOfMatchingPhrases + 1 >= numberOfAllocatedPhrasePointers )
                    {
                        numberOfAllocatedPhrasePointers = ( numberOfMatchingPhrases + 1 ) * 2;
                        *foundPhrases = ( Phrase ** ) realloc ( *foundPhrases, sizeof ( Phrase * ) * numberOfAllocatedPhrasePointers );
                        if ( *foundPhrases == NULL )
                            exit ( EXIT_FAILURE );
                    }
                    *( *foundPhrases + numberOfMatchingPhrases ++ ) = iterPhrase;
                }
            }
            *( *foundPhrases + numberOfMatchingPhrases ) = NULL; //NULL terminated array of ( Phrase * )
            numberOfAllocatedPhrasePointers = numberOfMatchingPhrases + 1;
            *foundPhrases = ( Phrase ** ) realloc ( *foundPhrases, sizeof ( Phrase * ) * numberOfAllocatedPhrasePointers );
            if ( *foundPhrases == NULL )
                exit ( EXIT_FAILURE );
            qsort ( *foundPhrases, numberOfMatchingPhrases, sizeof ( Phrase * ), comparePhrasesByWordFrequency );
        }
    }
    return numberOfMatchingPhrases;
}

void                printPhrases                            ( Phrase                   ** phrases,
                                                              unsigned long long          count )
{
    if ( phrases != NULL )
    {
        printf ("Nalezeno: %llu\n", count );
        Phrase * iterPhrase = NULL;
        if ( count > 50 )
            count = 50;
        for ( unsigned long long i = 0; i < count; i ++ )
        {
            iterPhrase = *( phrases + i );
            if ( iterPhrase != NULL )
                printf ( "> %s\n", iterPhrase->word );
        }
    }
}

int                 whisper                                 ( PhrasesContainer          * phrasesContainer )
{
    if ( phrasesContainer == NULL )
        return 0;
    Phrase           ** matchingPhrases             = NULL;
    char              * inputString                 = NULL;
    size_t              inputStringsAllocatedLength = 0;
    int                 ret                         = 1;
    unsigned long long  numberOfMatchingPhrases     = 0;
    printf ( "Hledani:\n" );
    while ( ( ret = getLineOfAllowedChars ( &inputString, &inputStringsAllocatedLength, '\n', stdin ) ) > 0 )
    {
        numberOfMatchingPhrases = getAllPhrasesStartingWith ( inputString, phrasesContainer, &matchingPhrases );
        printPhrases ( matchingPhrases, numberOfMatchingPhrases );
    }
    free ( matchingPhrases );
    return ret == EOF ? 1 : 0;
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
    if ( !( loadPhrases ( &pPhrasesContainer ) && whisper ( pPhrasesContainer ) ) )
        printf ( "Nespravny vstup.\n" );
    freePhrasesContainer ( &pPhrasesContainer );
    return 0;
}