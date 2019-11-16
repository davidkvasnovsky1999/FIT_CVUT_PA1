#include <stdio.h>
#include <stdlib.h>

int main ( void )
{
    printf ( "ml' nob:\n" );
    char *p, s[4];
    long inputNumber;
    fgets ( s, sizeof ( s ), stdin );
    inputNumber = strtol ( s, &p, 10);
    if ( p == s || *p != '\n' || inputNumber > 5 || inputNumber < 1 )
        printf ( "luj\n" );
    else
        switch ( inputNumber )
        {
            case 1:
                printf ( "Qapla'\n"
                         "noH QapmeH wo' Qaw'lu'chugh yay chavbe'lu' 'ej wo' choqmeH may' DoHlu'chugh lujbe'lu'.\n" );
                break;
            case 2:
                printf ( "Qapla'\n"
                         "Qu' buSHa'chugh SuvwI', batlhHa' vangchugh, qoj matlhHa'chugh, pagh ghaH SuvwI''e'.\n" );
                break;
            case 3:
                printf ( "Qapla'\n"
                         "qaStaHvIS wa' ram loS SaD Hugh SIjlaH qetbogh loD.\n" );
                break;
            case 4:
                printf ( "Qapla'\n"
                         "Ha'DIbaH DaSop 'e' DaHechbe'chugh yIHoHQo'.\n" );
                break;
            case 5:
                printf ( "Qapla'\n"
                         "leghlaHchu'be'chugh mIn lo'laHbe' taj jej.\n");
                break;
            default:
                printf ( "luj\n" );
        }
    return 0;
}