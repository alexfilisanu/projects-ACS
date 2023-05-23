/* FILISANU Mihai-Alexandru - 314CB */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "tlg.h"

#ifndef _TAB_HASH_
#define _TAB_HASH_

typedef int (*TFCmp)(void*, void*); /* functie de comparare doua elemente */
typedef void (*TF)(void*);     /* functie afisare/eliberare un element */
typedef void (*TFAfis)(FILE*, void*);   /* functie de AFISARE   */ 
typedef int (*TFHash)(void*);

typedef struct
{
    size_t M;
    TFHash fh;
    TLG *v;
} TH;

/* functii tabela hash */
TH* IniTH(size_t M, TFHash fh);
void DistrTH(TH**aa, TF fe);
void AfiTH(FILE*f, TH*a, TFAfis afiEl);
void *ExistaTH(TH*a, void*ae, TFCmp f);
int InsTH(TH*a, void*ae, TFCmp f);
int RemoveTH(TH*a, void*ae, TFCmp f);
void AfiTH_v2(FILE*f, TH* ah, int n, TFAfis afiel);
#endif