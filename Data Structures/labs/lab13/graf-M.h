/*--- graf-M.h --- Graf reprezentat cu Matrice de adiacente --*/
#include "graf.h"

#ifndef _GRAF_M_
#define _GRAF_M_

typedef struct 
{
    int n;  /* numar noduri */
    int* x;   /* vector costuri = matrice liniarizata */
} TGM, *AGM;

#endif
