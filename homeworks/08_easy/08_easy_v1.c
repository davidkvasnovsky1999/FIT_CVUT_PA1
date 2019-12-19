#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define LIST_BY_YEAR       0
#define LIST_BY_TYPE       1

#define TYPE_MAX           100
#define SETUP_MAX          100

#define true 1
#define false 0

typedef char boolean;

typedef struct TEngine
{
  struct TEngine * m_Next;
  int              m_Year;
  char             m_Type  [ TYPE_MAX ];
  int              m_Setup [ SETUP_MAX ];
} TENGINE;

typedef struct TArchive
{
  struct TArchive * m_Next;
  TENGINE         * m_Engines;
} TARCHIVE;

TENGINE           * createEngine                            ( const char      * type,
                                                              int               year )
{
  TENGINE * res = (TENGINE *) malloc ( sizeof  (*res ) );
  res -> m_Next = NULL;
  res -> m_Year = year;
  strncpy ( res -> m_Type, type, sizeof ( res -> m_Type ) );
  for ( int i = 0; i < SETUP_MAX; i ++ )
    res -> m_Setup[i] = 0;
  return res;
}
#endif /* __PROGTEST__ */

char                findPtrByYearWhereArchiveShouldBeSaved  ( const int         year,
                                                              TARCHIVE      *** ptrToWhereTheArchiveSBS )
{
    if ( ptrToWhereTheArchiveSBS != NULL && *ptrToWhereTheArchiveSBS != NULL )
    {
        while ( **ptrToWhereTheArchiveSBS != NULL )
        {
            if ( (**ptrToWhereTheArchiveSBS)->m_Engines != NULL && year <= (**ptrToWhereTheArchiveSBS)->m_Engines->m_Year )
            {
                if ( year == (**ptrToWhereTheArchiveSBS)->m_Engines->m_Year )
                    return 1;
                return 0;
            }
            *ptrToWhereTheArchiveSBS = &( ( **ptrToWhereTheArchiveSBS )->m_Next );
        }
    }
    return 0;
}

char                findPtrByTypeWhereArchiveShouldBeSaved  ( const char      * type,
                                                              TARCHIVE      *** ptrToWhereTheArchiveSBS )
{
    if ( ptrToWhereTheArchiveSBS != NULL && *ptrToWhereTheArchiveSBS != NULL )
    {
        while ( **ptrToWhereTheArchiveSBS != NULL )
        {
            int strcmpRet;
            if ( (**ptrToWhereTheArchiveSBS)->m_Engines != NULL
            && ( strcmpRet = strcmp ( type, (**ptrToWhereTheArchiveSBS)->m_Engines->m_Type ) ) <= 0 )
            {
                if ( strcmpRet == 0 )
                    return 1;
                return 0;
            }
            *ptrToWhereTheArchiveSBS = &( ( **ptrToWhereTheArchiveSBS )->m_Next );
        }
    }
    return 0;
}


TARCHIVE          * AddEngine                               ( TARCHIVE        * list,
                                                              int               listBy,
                                                              TENGINE         * engine )
{
    TARCHIVE ** firstArchive = &list;
    if ( engine != NULL )
    {
        TARCHIVE   ** ptrToWhereTheArchiveSBS = &list;
        TENGINE    ** walkEnginePtr           = NULL;
        if      ( listBy == LIST_BY_YEAR )
        {
            if ( findPtrByYearWhereArchiveShouldBeSaved ( engine->m_Year, &ptrToWhereTheArchiveSBS ) == 0 )
            {
                TARCHIVE * newArchive = ( TARCHIVE * ) malloc ( sizeof ( TARCHIVE ) );
                newArchive->m_Engines = NULL;
                newArchive->m_Next = *ptrToWhereTheArchiveSBS;
                *ptrToWhereTheArchiveSBS = newArchive;
            }
            walkEnginePtr = &( (*ptrToWhereTheArchiveSBS)->m_Engines );
            while ( *walkEnginePtr != NULL && strcmp ( (*walkEnginePtr)->m_Type, engine->m_Type ) < 0 )
                walkEnginePtr = &( ( *walkEnginePtr )->m_Next );
            engine->m_Next = *walkEnginePtr;
            *walkEnginePtr = engine;
        }
        else if ( listBy == LIST_BY_TYPE )
        {
            if ( findPtrByTypeWhereArchiveShouldBeSaved ( engine->m_Type, &ptrToWhereTheArchiveSBS ) == 0 )
            {
                TARCHIVE * newArchive = ( TARCHIVE * ) malloc ( sizeof ( TARCHIVE ) );
                newArchive->m_Engines = NULL;
                newArchive->m_Next = *ptrToWhereTheArchiveSBS;
                *ptrToWhereTheArchiveSBS = newArchive;
            }
            walkEnginePtr = &( (*ptrToWhereTheArchiveSBS)->m_Engines );
            while ( *walkEnginePtr != NULL && engine->m_Year >= (*walkEnginePtr)->m_Year )
                walkEnginePtr = &( ( *walkEnginePtr )->m_Next );
            engine->m_Next = *walkEnginePtr;
            *walkEnginePtr = engine;
        }
    }
    return *firstArchive;
}

void                deleteEngines                           ( TENGINE         * engine )
{
    if ( engine != NULL )
    {
        deleteEngines ( engine->m_Next );
        engine->m_Next = NULL;
        free ( engine );
    }
}

void                deleteArchivesWithoutEngines            ( TARCHIVE        * list )
{
    if ( list != NULL )
    {
        deleteArchivesWithoutEngines ( list->m_Next );
        list->m_Next = NULL;
        free ( list );
    }
}

void                DelArchive                              ( TARCHIVE        * list )
{
    if ( list != NULL )
    {
        DelArchive ( list->m_Next );
        list->m_Next = NULL;
        deleteEngines ( list->m_Engines );
        free ( list );
    }
}

char                getArrayOfTEnginePtrsFromEveryTArchive  ( TARCHIVE        * list,
                                                              TENGINE       *** listOfEnginesPointers,
                                                              unsigned long   * numberOfEngines )
{
    if ( listOfEnginesPointers == NULL )
        return 0;
    size_t numberOfAllocatedTEPointers = 128;
    *numberOfEngines = 0;
    if ( *listOfEnginesPointers != NULL )
        free ( *listOfEnginesPointers );
    *listOfEnginesPointers = ( TENGINE ** ) malloc ( sizeof ( TENGINE * ) * numberOfAllocatedTEPointers );
    while ( list != NULL )
    {
        TENGINE * walkEngine = list->m_Engines;
        while ( walkEngine != NULL )
        {
            ( *numberOfEngines ) ++;
            if ( *numberOfEngines >= numberOfAllocatedTEPointers )
            {
                numberOfAllocatedTEPointers *= 2;
                *listOfEnginesPointers = ( TENGINE ** ) realloc ( *listOfEnginesPointers, sizeof ( TENGINE * ) * numberOfAllocatedTEPointers );
            }
            *( (*listOfEnginesPointers) + (*numberOfEngines) - 1 ) = walkEngine;
            walkEngine = walkEngine->m_Next;
        }
        list = list->m_Next;
    }
    if ( *numberOfEngines > 0 )
        *listOfEnginesPointers = ( TENGINE ** ) realloc ( *listOfEnginesPointers, sizeof ( TENGINE * ) * ( *numberOfEngines ) );
    else
    {
        free ( *listOfEnginesPointers );
        *listOfEnginesPointers = NULL;
    }
    return 1;
}

int                 compareEnginesByYear                    ( const void      * a,
                                                              const void      * b )
{
    TENGINE ** engineA = ( TENGINE ** ) a,
            ** engineB = ( TENGINE ** ) b;
    if ( (*engineA)->m_Year == (*engineB)->m_Year )
        return strcmp ( (*engineA)->m_Type, (*engineB)->m_Type );
    return (*engineA)->m_Year > (*engineB)->m_Year ?  1 : -1;
}

int                 compareEnginesByType                    ( const void      * a,
                                                              const void      * b )
{
    TENGINE ** engineA = ( TENGINE ** ) a,
            ** engineB = ( TENGINE ** ) b;
    int strcmpRet = strcmp ( (*engineA)->m_Type, (*engineB)->m_Type );
    if ( strcmpRet == 0 )
        return (*engineA)->m_Year > (*engineB)->m_Year ?  1 : -1;
    return strcmpRet;
}

TARCHIVE          * ReorderArchive                          ( TARCHIVE        * list,
                                                              int               listBy )
{
    TARCHIVE         * headPtr                  = NULL;
    TENGINE         ** listOfEnginesPointers    = NULL;
    unsigned long      numberOfEngines          = 0;
    getArrayOfTEnginePtrsFromEveryTArchive ( list, &listOfEnginesPointers, &numberOfEngines );
    if      ( numberOfEngines > 0 )
    {
        headPtr = ( TARCHIVE * ) malloc ( sizeof ( TARCHIVE ) );
        headPtr->m_Engines = NULL;
        headPtr->m_Next = NULL;
        if      ( listBy == LIST_BY_YEAR )
            qsort ( listOfEnginesPointers, numberOfEngines, sizeof ( TENGINE ), compareEnginesByYear );
        else if ( listBy == LIST_BY_TYPE )
            qsort ( listOfEnginesPointers, numberOfEngines, sizeof ( TENGINE ), compareEnginesByType );
        else
            return list;
        deleteArchivesWithoutEngines ( list );
        int         yearOfPrevEngine = ( *listOfEnginesPointers )->m_Year;
        char      * typeOfPrevEngine = ( *listOfEnginesPointers )->m_Type;
        TARCHIVE  * walkedArchive    = headPtr;
        TENGINE  ** ptrToSaveEngine  = &( headPtr->m_Engines );
        for ( unsigned long long i = 0; i < numberOfEngines; i ++ )
        {
            TENGINE * iterEngine = ( *( listOfEnginesPointers + i ) );
            if ( ( listBy == LIST_BY_YEAR && yearOfPrevEngine != iterEngine->m_Year )
              || ( listBy == LIST_BY_TYPE && typeOfPrevEngine != iterEngine->m_Type ))
            {
                *ptrToSaveEngine = NULL;
                walkedArchive->m_Next = ( TARCHIVE * ) malloc ( sizeof ( TARCHIVE ) );
                walkedArchive = walkedArchive->m_Next;
                walkedArchive->m_Engines = NULL;
                walkedArchive->m_Next = NULL;
                ptrToSaveEngine = &( walkedArchive->m_Engines );
            }
            yearOfPrevEngine = iterEngine->m_Year;
            typeOfPrevEngine = iterEngine->m_Type;
            *ptrToSaveEngine = iterEngine;
            ptrToSaveEngine = &( iterEngine->m_Next );
        }
        *ptrToSaveEngine = NULL;
        free ( listOfEnginesPointers );
    }
    return headPtr;
}

#ifndef __PROGTEST__
int                 main                                    ( int               argc,
                                                              char            * argv [] )
{
  TARCHIVE * a;

  a = NULL;
  a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TDI 1.9", 2010 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TDI 1.9", 2005 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TSI 1.2", 2010 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "TDI 2.0", 2005 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 2.0" )
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_YEAR, createEngine ( "MPI 1.4", 2005 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Next -> m_Type, "TDI 2.0" )
            && a -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
  a = ReorderArchive ( a, LIST_BY_TYPE );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  DelArchive ( a );

  a = NULL;
  a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 1.9", 2010 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 1.9", 2005 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TSI 1.2", 2010 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 2.0", 2005 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "MPI 1.4", 2005 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  a = AddEngine ( a, LIST_BY_TYPE, createEngine ( "TDI 1.9", 2010 ) );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Next -> m_Next -> m_Engines -> m_Type, "TDI 2.0" )
            && a -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next
            && a -> m_Next -> m_Next -> m_Next -> m_Engines
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Next -> m_Next -> m_Engines -> m_Next == NULL
            && a -> m_Next -> m_Next -> m_Next -> m_Next == NULL );
  a = ReorderArchive ( a, LIST_BY_YEAR );
  assert ( a
            && a -> m_Engines
            && a -> m_Engines -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Type, "MPI 1.4" )
            && a -> m_Engines -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Engines -> m_Next -> m_Next -> m_Year == 2005
            && ! strcmp ( a -> m_Engines -> m_Next -> m_Next -> m_Type, "TDI 2.0" )
            && a -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next
            && a -> m_Next -> m_Engines
            && a -> m_Next -> m_Engines -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Type, "TDI 1.9" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Year == 2010
            && ! strcmp ( a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Type, "TSI 1.2" )
            && a -> m_Next -> m_Engines -> m_Next -> m_Next -> m_Next == NULL
            && a -> m_Next -> m_Next == NULL );
  DelArchive ( a );

  return 0;
}
#endif /* __PROGTEST__ */
