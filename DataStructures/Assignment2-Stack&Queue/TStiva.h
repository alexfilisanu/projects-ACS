/*-- TStiva.h --- tipul stiva ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


#ifndef _STIVA_
#define _STIVA_

typedef struct celst
{
	struct celst *urm;
	void *info;
} TCelSt, *ACelSt;

typedef struct stiva
{ 
	size_t 	dime; 		/* dimensiunea unui element al stivei */
	ACelSt vf;			/* varful stivei */
} TStiva, *ASt;

/*- macrodefinitii - acces campuri */
#define DIMES(a) (((ASt)(a))->dime)
#define VF(a) 	(((ASt)(a))->vf)

/* verifica daca doua stive au elemente de dimensiuni diferite */
#define DIMDIFS(s,d)	(DIMES(s) != DIMES(d))
/* verifica daca o stiva este vida */
#define VIDAS(a)  	(VF(a) == NULL)

typedef void (*TF1)(FILE*, void*);
typedef int (*TFCmp)(void*, void*);
typedef void TFC(void*);         

void* InitS(size_t d);    /* initializeaza stiva cu elem.de dimensiune d */
int Push(void* a, void* ae);    /* pune element in varful stivei */
int Pop (void* a, void* ae);    /* extrage elementul din varful stivei la adresa ae */
int Top (void* a, void* ae);    /* copiaza elementul din varful stivei la adresa ae */
int VidaS(void* a);   /* test stiva vida */
int NrES (void* a);   /* numarul de elemente din stiva */
size_t DimES(void* a);    /* dimensiunea elementelor din stiva */
void ResetS (void* a);    /* transforma stiva in stiva vida */
void DistrS (void** aa);    /* elibereaza intregul spatiu ocupat de stiva */
size_t PrelS(FILE*, void* a, TF1 f);    /*prelucreaza elementele, folosind functia f;
                                  rezultat = numarul de elemente pentru care f != 0 */
int Suprapune(void* ad, void* as);    /* muta stiva sursa peste destinatie */
int Rastoarna(void* ad, void* as);    /* suprapune inversul sursei peste destinatie */
void* ExtrageS(void* a, void* ae, TFCmp fcmp);  /* functie de extragere al unui element din stiva */
int AddSezS(FILE*, void* a, void* ae, TFCmp fcmp, TFC fc);  /* adauga un sezon in stiva; reusita (0/1) */
void AfiS(FILE *outputFile, TF1 AfiElem, void *a);    /* afiseaza stiva */


#endif
