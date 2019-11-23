#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    char * name;
    long double x, y;
} Flight;




int                     readOneFlight           ( Flight            * flight )
{
    
    return 1;
}



int                     getFlights              ( Flight                ** flights,
                                                  unsigned long long     * numberOfFlights )
{
    *numberOfFlights = 0;
    unsigned long long numberOfAllocatedFlights = 32;
    *flights = ( Flight * ) malloc ( numberOfAllocatedFlights * sizeof ( Flight ) );
    return 1;
}


int                     main                    ( void )
{
    Flight * flights;
    return 0;
}