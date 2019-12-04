#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//#define boolean char;
#define true 1;
#define false 0;
#define STARTING_ASCII_CHAR 32;
#define ENDING_ASCII_CHAR 126;

typedef unsigned char boolean;

typedef struct TrieNode
{
    long double             wordFrequency;
    boolean                 isWordsEnd;
//    struct TrieNode       * parent;
    struct TrieNode      ** children;
} TrieNode;

typedef struct TrieRoot
{
    struct TrieNode       * rootNode;
    unsigned long long      numberOfAncestors;
} TrieRoot;

int                 asciiCharToIndex                        ( const unsigned char         charToGetIndexFrom )
{
    return charToGetIndexFrom - STARTING_ASCII_CHAR;
}

int                 getNumberOfAllowedCharsForPhrase        ( )
{
    int n = STARTING_ASCII_CHAR;
    int m = ENDING_ASCII_CHAR;
    return n - m;
}

TrieNode **         getNewChildren                          ( unsigned long long          numberOfChildren )
{
    TrieNode ** trieChildren;
    if ( ( trieChildren = ( TrieNode ** ) malloc ( sizeof ( TrieNode * ) * numberOfChildren ) ) != NULL )
        for ( unsigned long long i = 0; i < numberOfChildren; i ++  )
            *( trieChildren + i ) = NULL;
    return trieChildren;
}

TrieNode *          getNewTrieNode                          ( )
{
    TrieNode * newTrieNodePointer;
    if ( ( newTrieNodePointer = ( TrieNode * ) malloc ( sizeof ( TrieNode ) ) ) != NULL )
    {
        newTrieNodePointer->isWordsEnd = false;
        newTrieNodePointer->wordFrequency = 0;
        if ( ( newTrieNodePointer->children = getNewChildren ( getNumberOfAllowedCharsForPhrase ( ) ) ) == NULL )
        {
            free ( newTrieNodePointer );
            newTrieNodePointer = NULL;
        }
    }
    return newTrieNodePointer;
}

TrieRoot *          getNewTrieRootWithInitRootNode          ( )
{
    TrieRoot * trieRoot;
    if ( ( trieRoot = ( TrieRoot * ) malloc ( sizeof ( TrieRoot ) ) ) != NULL )
    {
        trieRoot->numberOfAncestors = getNumberOfAllowedCharsForPhrase ( );
        if ( ( trieRoot->rootNode          = getNewTrieNode ( ) ) == NULL )
        {
            free ( trieRoot );
            trieRoot = NULL;
        }
    }
    return trieRoot;
}

int                 insertKeyIntoTrie                       ( TrieRoot                  * pTrieRoot,
                                                              const char                * key,
                                                              const long double           wordFrequency )
{
    if ( pTrieRoot == NULL || pTrieRoot->rootNode == NULL || pTrieRoot->rootNode->children == NULL || key == NULL || key [ 0 ] == 0 )
        return 0;
    TrieNode * pCrawlNode = pTrieRoot->rootNode;
    for ( unsigned long long level = 0; level < strlen ( key ); level ++ )
    {
        TrieNode * pCharsNode = *( pCrawlNode->children + asciiCharToIndex ( key [ level ] ) );
        if ( pCharsNode == NULL && !( pCharsNode = getNewTrieNode () ) )
            return -1;
        pCrawlNode = pCharsNode;
    }
    pCrawlNode->isWordsEnd = true;
    pCrawlNode->wordFrequency = wordFrequency;
    return 1;
}

void                freeTrieFromRoot                        ( TrieNode                  * pRootNode )
{
    if ( pRootNode != NULL )
    {
    
    }
}


int                 loadOftenWantedPhrasesFromStdin         ( TrieNode                  * pRootNode )
{
    if ( pRootNode == NULL )
        return 0;
    
    return 1;
}




int                 main                                    ( void )
{
    TrieRoot * pTrieRoot = getNewTrieRootWithInitRootNode ( );
    if ( pTrieRoot == NULL )
        return -1;
    
    return 0;
}