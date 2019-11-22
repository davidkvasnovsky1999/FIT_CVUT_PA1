#include <stdio.h>
#include <stdlib.h>

#define MAX_NUM_OF_PARTS 500000;

typedef struct {
    unsigned size; // Current size of min heap
    unsigned capacity; // capacity of min heap
    long long * harr; // Array of min heap nodes
} MinHeap;

int                 getPartsLengthsFromInput    ( long long    ** partsLengths )
// Returns number of successfully read unsigned integers greater than 0 (numberOfParts) from stdin.
// Creates an array of read parts and stores it into *partsLengths. If numberOfParts is equal to 0 then *partsLengths is NULL.
// Dynamically allocated cell count of the array stored in *partsLengths is equal to number of its elements.
{
    int arraySize = 32, ret = 0, numberOfParts = 0;
    *partsLengths = ( long long * ) malloc ( arraySize * sizeof ( long long ) );
    long long readNumber = 0;
    while ( EOF != ( ret = scanf ( " %lld ", &readNumber ) ) )
    {
        if ( ret == 1 && readNumber > 0 && numberOfParts ++ <= MAX_NUM_OF_PARTS )
        {
            if ( numberOfParts >= arraySize )
            {
                arraySize *= 2;
                *partsLengths = ( long long * ) realloc ( *partsLengths, arraySize * sizeof ( long long ) );
            }
            *( ( *partsLengths ) + numberOfParts ) = readNumber;
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
    MinHeap* minHeap = new MinHeap;
    minHeap->size = 0; // current size is 0
    minHeap->capacity = capacity;
    minHeap->harr = new int[capacity];
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
                                                  long long         idx )
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if (left < minHeap->size && minHeap->harr[left] < minHeap->harr[smallest])
        smallest = left;
    
    if (right < minHeap->size && minHeap->harr[right] < minHeap->harr[smallest])
        smallest = right;
    
    if (smallest != idx) {
        swapMinHeapNode(&minHeap->harr[smallest], &minHeap->harr[idx]);
        minHeapify(minHeap, smallest);
    }
}

// A utility function to check if size of heap is 1 or not
int                 isSizeOne                   ( MinHeap         * minHeap )
{
    return (minHeap->size == 1);
}

// A standard function to extract minimum value node from heap
long long           extractMin                  ( MinHeap         * minHeap )
{
    long long temp = minHeap->harr[0];
    minHeap->harr[0] = minHeap->harr[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

// A utility function to insert a new node to Min Heap
void                insertMinHeap               ( MinHeap         * minHeap,
                                                  long long         val )
{
    ++ minHeap->size;
    long long i = minHeap->size - 1;
    while ( i && ( val < minHeap->harr [ ( i - 1 ) / 2 ] ) ) {
        minHeap->harr [ i ] = minHeap->harr [ ( i - 1 ) / 2 ];
        i = ( i - 1 ) / 2;
    }
    minHeap->harr [ i ] = val;
}

// A standard funvtion to build min heap
void                buildMinHeap                ( MinHeap         * minHeap )
{
    long long n = minHeap->size - 1;
    long long i;
    for ( i = ( n - 1 ) / 2; i >= 0; -- i )
        minHeapify ( minHeap, i );
}

// Creates a min heap of capacity equal to size and inserts all values
// from len[] in it. Initially size of min heap is equal to capacity
MinHeap*            createAndBuildMinHeap       ( const long long * length,
                                                  int               size )
{
    MinHeap * minHeap = createMinHeap ( size );
    for ( int i = 0; i < size; ++ i )
        minHeap->harr [ i ] = length [ i ];
    minHeap->size = size;
    buildMinHeap ( minHeap );
    return minHeap;
}

// The main function that returns the minimum cost to connect n ropes of
// lengths stored in len[0..n-1]
long long           minCost                     ( long long       * len,
                                                  int               n )
{
    long long cost = 0; // Initialize result
    // Create a min heap of capacity equal to n and put all ropes in it
    MinHeap * minHeap = createAndBuildMinHeap ( len, n );
    // Iterate while size of heap doesn't become 1
    while ( !isSizeOne ( minHeap ) ) {
        // Extract two minimum length ropes from min heap
        long long min       = extractMin ( minHeap );
        long long sec_min   = extractMin ( minHeap );
        
        cost += ( min + sec_min ); // Update total cost
        
        // Insert a new rope in min heap with length equal to sum
        // of two extracted minimum lengths
        insertMinHeap ( minHeap, min + sec_min );
    }
    // Finally return total minimum cost for connecting all ropes
    return cost;
}


int                 main                        ( void )
{
    printf ( "Zadejte delky:\n" );
    long long * partsLengths = NULL;
    int partsCount = 0;
    if ( ( partsCount = getPartsLengthsFromInput ( &partsLengths ) ) )
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