/* FILISANU Mihai-Alexandru - 314CB */
/*-- tlg.h --- LISTA DUBLU INLANTUITA GENERICA ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celulag
{
    void* info;           /* adresa informatie */
    struct celulag *urm, *pre;   /* adresa urmatoarei celule si precedentei celule */
} TCelulaG, *TLG; /* tipurile Celula, Lista  */


typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*);     /* functie afisare/eliberare un element */
typedef void (*TFAfis)(FILE*, void*);    /* functie de AFISARE   */

/* functii lista dublu inlantuita generica */
TLG InitLDI(void *ae);
int InsDupa(TLG p, void *x);

#endif
