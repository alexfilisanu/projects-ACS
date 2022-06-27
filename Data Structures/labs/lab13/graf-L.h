/*--- graf-L.h --- Graf reprezentat cu liste --*/
#include "graf.h"

#ifndef _GRAF_L_
#define _GRAF_L_

typedef struct celarc 
{
    int d;           /* destinatie arc */
    int c;            /* cost arc */
    struct celarc *urm; /* adresa urmatorul adiacent */
} TCelArc, *AArc;

typedef struct 
{
    int n;     /* numar noduri */
    AArc* x;    /* vector adrese liste arce */
} TGL, *AGL;

#endif
