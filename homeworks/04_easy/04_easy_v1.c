#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_OF_PARTS ( ( int ) 500000 )

typedef struct {
    unsigned size; // Current size of min heap
    unsigned capacity; // capacity of min heap
    long long * arrayOfNodes; // Array of min heap nodes
} MinHeap;

int                 getPartsLengthsFromInput    ( long long    ** partsLengths )
// Returns number of successfully read unsigned integers greater than 0 (numberOfParts) from stdin.
// Creates an array of read parts and stores it into *partsLengths. If numberOfParts is equal to 0 then *partsLengths is NULL.
// Dynamically allocated cell count of the array stored in *partsLengths is equal to number of its elements.
{
    int arraySize = 32, ret = 0, numberOfParts = 0;
    *partsLengths = ( long long * ) malloc ( arraySize * sizeof ( long long ) );
    long long readNumber = 0;
    while ( EOF != ( ret = scanf ( "%lld", &readNumber ) ) )
    {
        if ( ret == 1 && readNumber > 0 && ( numberOfParts ++ ) <= MAX_NUM_OF_PARTS )
        {
            if ( numberOfParts >= arraySize )
            {
                arraySize *= 2;
                *partsLengths = ( long long * ) realloc ( *partsLengths, arraySize * sizeof ( long long ) );
            }
            *( ( *partsLengths ) + ( numberOfParts - 1 ) ) = readNumber;
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


// A utility function to create a min heap of given capacity
MinHeap*            createMinHeap               ( unsigned          capacity )
{
    MinHeap* minHeap = ( MinHeap * ) malloc ( sizeof ( MinHeap ) );
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->arrayOfNodes = ( long long * ) malloc ( capacity * sizeof ( long long ) );
    return minHeap;
}

// A utility function to swap two min heap nodes
void                swapMinHeapNode             ( long long       * a,
                                                  long long       * b )
{
    long long temp = *a;
    *a = *b;
    *b = temp;
}

// The standard minHeapify function.
void                minHeapify                  ( MinHeap         * minHeap,
                                                  const long long   index )
{
    long long smallest = index;
    long long left     = 2 * index + 1;
    long long right    = 2 * index + 2;
    
    if ( left  < minHeap->size && minHeap->arrayOfNodes [ left ]  < minHeap->arrayOfNodes [ smallest ] )
        smallest = left;
    
    if ( right < minHeap->size && minHeap->arrayOfNodes [ right ] < minHeap->arrayOfNodes [ smallest ] )
        smallest = right;
        
    if ( smallest != index ) {
        swapMinHeapNode ( &minHeap->arrayOfNodes [ smallest ], &minHeap->arrayOfNodes [ index ] );
        minHeapify ( minHeap, smallest );
    }
}

// A standard function to extract minimum value node from heap
long long           extractMin                  ( MinHeap         * minHeap )
{
    long long temp = minHeap->arrayOfNodes [ 0 ];
    minHeap->arrayOfNodes [ 0 ] = minHeap->arrayOfNodes [ minHeap->size - 1 ];
    -- minHeap->size;
    minHeapify ( minHeap, 0 );
    return temp;
}

// A utility function to insert a new node to Min Heap
void                insertMinHeap               ( MinHeap         * minHeap,
                                                  const long long   value )
{
    ++ minHeap->size;
    long long i = minHeap->size - 1;
    while ( i && ( value < minHeap->arrayOfNodes [ ( i - 1 ) / 2 ] ) ) {
        minHeap->arrayOfNodes [ i ] = minHeap->arrayOfNodes [ ( i - 1 ) / 2 ];
        i = ( i - 1 ) / 2;
    }
    minHeap->arrayOfNodes [ i ] = value;
}

// A standard function to build min heap
void                buildMinHeap                ( MinHeap         * minHeap )
{
    long long n = minHeap->size - 1;
    long long i;
    for ( i = ( n - 1 ) / 2; i >= 0; -- i )
        minHeapify ( minHeap, i );
}

// Creates a min heap of capacity equal to size and inserts all values
// from *length in it. Initially size of min heap is equal to capacity
MinHeap*            createAndBuildMinHeap       ( const long long * length,
                                                  const int         size )
{
    MinHeap * minHeap = createMinHeap ( size );
    for ( int i = 0; i < size; ++ i )
        minHeap->arrayOfNodes [ i ] = length [ i ];
    minHeap->size = size;
    buildMinHeap ( minHeap );
    return minHeap;
}

// The main function that returns the minimum cost to connect n parts of lengths stored in partsLengths
long long           minCost                     ( const long long * partsLengths,
                                                  const int         partsCount )
{
    long long minCost = 0;
    MinHeap * minHeap = createAndBuildMinHeap ( partsLengths, partsCount );
    while ( minHeap->size != 1 )
    {
        // Extract two minimum length ropes from min heap
        long long min       = extractMin ( minHeap );
        long long sec_min   = extractMin ( minHeap );
        minCost += ( min + sec_min );
        insertMinHeap ( minHeap, min + sec_min );
    }
    return minCost;
}


int                 main                        ( void )
{
    printf ( "Zadejte delky:\n" );
    long long * partsLengths = NULL;
    int partsCount = getPartsLengthsFromInput ( &partsLengths );
    if ( partsCount > 0 )
    {
//        for ( int i = 0; i < partsCount; i ++ )
//            printf ( "%lld ", partsLengths [ i ] );
//        printf ( "\n" );
        printf ( "Cena za deleni: %lld\n", minCost ( partsLengths, partsCount ) );
        free ( partsLengths );
    }
    else
        printf ( "Nespravny vstup.\n" );
    return 0;
}