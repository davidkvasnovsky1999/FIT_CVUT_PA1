#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define true 1
#define false 0
#define STARTING_ASCII_CHAR 32
#define ENDING_ASCII_CHAR 126

typedef unsigned char boolean;

typedef struct TrieNode
{
    boolean                 isWordsEnd;
    long double             wordFrequency;
    unsigned char           character;
    unsigned long long      indexOfNodesFirstChildInRA,
                            numberOfChildren;
    struct TrieNode      ** rootsAncestors;
} TrieNode;

typedef struct TrieRoot
{
    unsigned long long      numberOfAncestors,
                            numberOfChildren;
    TrieNode              * ancestors;
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

//int                 initNewNode                             ( TrieNode                  * newNode )
//{
//    if ( newNode == NULL )
//        return 0;
//    newNode->wordFrequency              = 0;
//    newNode->character                  = 0;
//    newNode->numberOfChildren           = 0;
//    newNode->indexOfNodesFirstChildInRA = 0;
//    newNode->isWordsEnd                 = false;
//    return 1;
//}

TrieNode *          initNode                                ( TrieNode                  * node,
                                                              long double                 wordFrequency,
                                                              unsigned char               characterItHolds,
                                                              boolean                     isWordsEnd )
{
    if ( node != NULL )
    {
        node->wordFrequency              = wordFrequency;
        node->character                  = characterItHolds;
        node->numberOfChildren           = 0;
        node->indexOfNodesFirstChildInRA = 0;
        node->isWordsEnd                 = isWordsEnd;
    }
    return node;
}


TrieRoot *          getEmptyTrieRoot                        ( )
{
    TrieRoot * trieRoot;
    if ( ( trieRoot = ( TrieRoot * ) malloc ( sizeof ( TrieRoot ) ) ) != NULL )
    {
        trieRoot->numberOfAncestors = 0;
        trieRoot->numberOfChildren = 0;
        trieRoot->ancestors = ( TrieNode * ) malloc ( sizeof ( TrieNode ) );
    }
    return trieRoot;
}

int                 findAndReturnChildNodeByCharacter       ( TrieNode                  * parentNode,
                                                              TrieNode                *** foundChildren,//may return upto 2 children if input parameter "caseSensitive" is set to "false".
                                                              unsigned char               characterToFindTheChildBy,
                                                              boolean                     caseSensitive )
{
    if ( parentNode == NULL || parentNode->numberOfChildren == 0 )
        return 0;
    unsigned char maxNumberOfFoundChildren = 2 - caseSensitive;
    if ( *foundChildren == NULL && ( *foundChildren = ( TrieNode ** ) malloc ( maxNumberOfFoundChildren * sizeof ( TrieNode * ) ) ) == NULL )
        return -1;
    for ( unsigned char i = 0; i < maxNumberOfFoundChildren; i ++ )
        *( *( foundChildren ) + i ) = NULL;
    const unsigned long long indexOfLastChild = ( parentNode->indexOfNodesFirstChildInRA + parentNode->numberOfChildren ) - 1;
    for ( unsigned long long index = parentNode->indexOfNodesFirstChildInRA;
          index <= indexOfLastChild;
          index ++ )
    {
//        if ( && ( caseSensitive || ( STARTING_ASCII_CHAR <= characterToFindTheChildBy && characterToFindTheChildBy <= ENDING_ASCII_CHAR ) || ( *( *( foundChildren ) + 1 ) = ( *( parentNode->rootsAncestors ) + index )->character == characterToFindTheChildBy ?  ) )
//        if ( ( *( parentNode->rootsAncestors ) + index )->character == characterToFindTheChildBy )
//        {
//
//        }
        if ( !caseSensitive )
        {
            if      ( *( *( foundChildren ) + 0 ) == NULL )
            {
                if ( 'a' <= characterToFindTheChildBy && characterToFindTheChildBy <= 'z'
                     && characterToFindTheChildBy - 32 ==  ( *( parentNode->rootsAncestors ) + index )->character )
                    *( *( foundChildren ) + 0 ) = ( *( parentNode->rootsAncestors ) + index );
            }
            else if ( *( *( foundChildren ) + 1 ) == NULL )
            {
                if ( 'A' <= characterToFindTheChildBy && characterToFindTheChildBy <= 'Z'
                     && characterToFindTheChildBy + 32 ==  ( *( parentNode->rootsAncestors ) + index )->character )
                    *( *( foundChildren ) + 1 ) = ( *( parentNode->rootsAncestors ) + index );
            }
            else
                break;
        }
        else if ( *( *( foundChildren ) + 0 ) == NULL &&  characterToFindTheChildBy ==  ( *( parentNode->rootsAncestors ) + index )->character )
            *( *( foundChildren ) + 0 ) = ( *( parentNode->rootsAncestors ) + index );
        
    }
    return 1;
}

int                 insertKey                               ( TrieRoot                  * pTrieRoot,
                                                              const char                * key,
                                                              const long double           wordFrequency )
{
    if ( pTrieRoot == NULL || key == NULL || key [ 0 ] == 0 )
        return 0;
    TrieNode tRootConvertedToTNode;
    tRootConvertedToTNode.numberOfChildren           = pTrieRoot->numberOfChildren;
    tRootConvertedToTNode.indexOfNodesFirstChildInRA = &tRootConvertedToTNode;
    TrieNode ** pCharsNode                           = 0;
    TrieNode * pCrawlNode                            = NULL;
    for ( unsigned long long level = 0; level < strlen ( key ); level ++ )
    {
        findAndReturnChildNodeByCharacter ( pCrawlNode, &pCharsNode, key [ level ], true );
        if ( pCharsNode == NULL || ( *pCharsNode == NULL  && !( *pCharsNode = getNewTrieNode () ) )
            return -1;
        pCrawlNode = pCharsNode;
    }
    free ( pCharsNode );
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
    TrieRoot * pTrieRoot = getEmptyTrieRoot ( );
    if ( pTrieRoot == NULL )
        return -1;
    
    return 0;
}