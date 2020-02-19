#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <float.h>

//#define boolean char;
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
            exit ( 1 );
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
                exit ( 1 );
        }
        *( ( *linePointer ) + lineLength ++ ) = ( char ) readChar;
    }
    while ( readChar != 0 );
    *numberOfAllocatedChars = lineLength;
    *linePointer = ( char * ) realloc ( *linePointer, *numberOfAllocatedChars );
    if ( *linePointer == NULL )
        exit ( 1 );
    return 1;
}

int                 readWordFrequency                       ( long double               * wordFrequency )
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

int                 loadPhrases                             ( PhrasesContainer         ** phrasesContainer )
{
    if ( phrasesContainer == NULL )
        return 0;
    if ( *phrasesContainer == NULL && ( *phrasesContainer = getNewInitPhraseContainer ( ) ) == NULL )
        exit ( 1 );
    int                ret0                                = 1,
                       ret1                                = 1;
    size_t             numberOfAllocatedCharsForWord       = 0;
    long double        tempWordFrequency                   = 0;
    char            ** readWord                            = ( char ** ) malloc ( sizeof ( char *) );
    Phrase          ** newPhrase                           = ( Phrase ** ) malloc ( sizeof ( Phrase *) );
    printf ( "Casto hledane fraze:\n" );
    while ( ( ret0 = readWordFrequency ( &tempWordFrequency ) ) > 0 )
    {
        *readWord = NULL;
        ret1 = getLineOfAllowedChars ( readWord, &numberOfAllocatedCharsForWord, '\n', stdin );
        if ( ret1 == 1 && numberOfAllocatedCharsForWord > 1 )
        {
            (*phrasesContainer)->numberOfPhrases ++;
            if ( (*phrasesContainer)->numberOfPhrases >= (*phrasesContainer)->numberOfAllocatedPhrasePointers )
            {
                (*phrasesContainer)->numberOfAllocatedPhrasePointers = (*phrasesContainer)->numberOfPhrases * 2;
                if ( ( (*phrasesContainer)->phrases = ( Phrase ** ) realloc ( (*phrasesContainer)->phrases,
                        (*phrasesContainer)->numberOfAllocatedPhrasePointers * sizeof ( Phrase * ) ) ) == NULL )
                    exit ( 0 );
            }
            if ( ( *newPhrase = ( Phrase * ) malloc ( sizeof ( Phrase ) ) ) == NULL )
                exit ( 1 );
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
        exit ( 0 );
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
    else
        return 0;
}

int                 caseInsensitiveStringCmp                ( const char                * stringA,
                                                              const char                * stringB,
                                                              unsigned long long          numberOfCharsToCompare )
{
    if ( stringA == NULL || stringB == NULL )
        return 0;
    unsigned long long      lengthOfStringA = strlen ( stringA ),
                            lengthOfStringB = strlen ( stringB ),
                          * biggerLength = lengthOfStringA > lengthOfStringB ? &lengthOfStringA : &lengthOfStringB;
    if ( numberOfCharsToCompare > *biggerLength )
        numberOfCharsToCompare = *biggerLength;
    for ( unsigned long long i = 0; i < numberOfCharsToCompare; i ++ )
    {
        if ( stringA [ i ] != stringB [ i ]
             && !( 'a' <= stringA [ i ] && stringA [ i ] <= 'z' && stringA [ i ] - 32 == stringB [ i ] )
             && !( 'A' <= stringA [ i ] && stringA [ i ] <= 'Z' && stringA [ i ] + 32 == stringB [ i ] ) )
            return stringA [ i ] < stringB [ i ] ? -1 : 1;
        if ( stringA [ i ] == 0 || stringB [ i ] == 0 )
            break;
    }
    return 0;
}

//int                 comparePhrasesByWord                    ( const void                * p,
//                                                              const void                * q )
//{
//    if ( p == NULL || q == NULL )
//        return 0;
//    char * pWord = (*( ( Phrase ** ) p ))->word,
//         * qWord = (*( ( Phrase ** ) q ))->word;
//    return strcmp ( pWord, qWord );
//}

//int                 compareKeyToPhrase                      ( const void                * key,
//                                                              const void                * phrase )
//{
//    if ( key == NULL || phrase == NULL )
//        return 0;
//    const char      * keyString = ( const char * ) key;
//    const Phrase   ** pPhrase   = ( const Phrase ** ) phrase;
//    return strncmp ( keyString, (*pPhrase)->word, strlen ( keyString ) );
//}

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
                    exit ( 1 );
            }
            for ( unsigned long long i = 0; i < pPhrasesContainer->numberOfPhrases; i ++ )
            {
                Phrase * iterPhrase = *( pPhrasesContainer->phrases + i );
                if ( caseInsensitiveStringCmp ( iterPhrase->word, string, strlen ( string ) ) == 0 )
                {
                    if ( numberOfMatchingPhrases + 1 >= numberOfAllocatedPhrasePointers )
                    {
                        numberOfAllocatedPhrasePointers = ( numberOfMatchingPhrases + 1 ) * 2;
                        *foundPhrases = ( Phrase ** ) realloc ( *foundPhrases, sizeof ( Phrase * ) * numberOfAllocatedPhrasePointers );
                        if ( *foundPhrases == NULL )
                            exit ( 1 );
                    }
                    *( *foundPhrases + numberOfMatchingPhrases ++ ) = iterPhrase;
                }
            }
            *( *foundPhrases + numberOfMatchingPhrases ) = NULL; //NULL terminated array of ( Phrase * )
            numberOfAllocatedPhrasePointers = numberOfMatchingPhrases + 1;
            *foundPhrases = ( Phrase ** ) realloc ( *foundPhrases, sizeof ( Phrase * ) * numberOfAllocatedPhrasePointers );
            if ( *foundPhrases == NULL )
                exit ( 1 );
            qsort ( *foundPhrases, numberOfMatchingPhrases, sizeof ( Phrase * ), comparePhrasesByWordFrequency );
//            qsort ( pPhrasesContainer->phrases, pPhrasesContainer->numberOfPhrases, sizeof ( Phrase * ), comparePhrasesByWord );
//            Phrase ** foundPhrase = ( Phrase ** ) bsearch ( string, pPhrasesContainer->phrases, pPhrasesContainer->numberOfPhrases, sizeof ( Phrase * ), compareKeyToPhrase );
//            if ( foundPhrase != NULL )
//            {
//                while ( foundPhrase != pPhrasesContainer->phrases
//                        && strncmp ( ( const char * ) string, ( const char * ) (*( foundPhrase - 1 ))->word, strlen ( ( const char * ) string ) ) == 0 )
//                    foundPhrase --;
//                while ( foundPhrase <= ( pPhrasesContainer->phrases + pPhrasesContainer->numberOfPhrases - 1 )
//                        && strncmp ( ( const char * ) string, ( const char * ) (*foundPhrase)->word, strlen ( ( const char * ) string ) ) == 0 )
//                {
//                    if ( numberOfMatchingPhrases >= numberOfAllocatedPhrasePointers )
//                    {
//                        numberOfAllocatedPhrasePointers = numberOfMatchingPhrases * 2;
//                        *foundPhrases = ( Phrase ** ) realloc ( *foundPhrases, sizeof ( Phrase * ) * numberOfAllocatedPhrasePointers );
//                        if ( *foundPhrases == NULL )
//                            exit ( 1 );
//                    }
//                    *( *foundPhrases + numberOfMatchingPhrases ++ ) = *foundPhrase;
//                    foundPhrase ++;
//                }
//                *foundPhrases = ( Phrase ** ) realloc ( *foundPhrases, sizeof ( Phrase * ) * numberOfMatchingPhrases );
//                qsort ( *foundPhrases, numberOfMatchingPhrases, sizeof ( Phrase * ), comparePhrasesByWordFrequency );
//            }
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

//void                printAllPhrases                         ( PhrasesContainer          * phrasesContainer )
//{
//    if ( phrasesContainer != NULL )
//    {
//        printf ( "Number of phrases: %llu\n", phrasesContainer->numberOfPhrases );
//        for ( unsigned long long i = 0; i < phrasesContainer->numberOfPhrases; i ++ )
//            printf ( "wordFreq = %Lf; word = %s\n", (*( phrasesContainer->phrases + i ))->wordFrequency, (*( phrasesContainer->phrases + i ))->word );
//    }
//}

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