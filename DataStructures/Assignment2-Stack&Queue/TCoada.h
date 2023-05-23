/*-- TCoada.h --- tipul coada ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>


#ifndef _COADA__
#define _COADA_

typedef struct cel
{
    struct cel *urm;
    void *info;
} TCel, *ACel;

typedef struct coada
{ 
	  size_t 	dime; 		/* dimensiunea unui element din coada */
	  ACel 	ic; 		/* adresa primului element din coada */
	  ACel 	sc; 		/* adresa de sfarsit a cozii */
} TCoada, *AQ;

/*- macrodefinitii - acces campuri -*/
#define IC(a) 	(((AQ)(a))->ic)
#define SC(a) 	(((AQ)(a))->sc)
#define DIMEQ(a) (((AQ)(a))->dime)

/* verifica daca doua cozi au elemente de dimensiuni diferite */
#define DIMDIFQ(s,d) 	(DIMEQ(s) != DIMEQ(d))  
/* verifica daca o coada este vida */
#define VIDAQ(a) 		(IC(a) == NULL && SC(a) == NULL)

typedef void (*TF1)(FILE*, void*);
typedef int (*TFCmp)(void*, void*);
typedef void TFC(void*);         

/* functii coada */

void* InitQ(size_t d);  /* creeaza coada vida cu elemente de dimensiune d */
int IntrQ(void* a, void* ae);   /* adauga element la sfarsitul cozii */
int ExtrQ(void* a, void* ae);   /* extrage primul element din coada la adresa ae */
int VidaQ(void* a);   /* test coada vida */
int NrEQ(void* a);    /* numarul de elemente din coada */
size_t DimEQ(void* a);    /* dimensiunea elementelor din coada */
void ResetQ(void* a);     /* transforma coada in coada vida */
void DistrQ(void** aa);       /* elibereaza intregul spatiu ocupat de coada */
size_t PrelQ(FILE*, void* a, TF1 f);  /* prelucreaza elementele, folosind functia f;
								                  rezultat = numarul de elemente pentru care f != 0 */
int ConcatQ(void *ad, void *as);	  /* concatenare (muta toate elementele din sursa) */
void *ExtrageQ(void* a, void* ae, TFCmp fcmp);  /* functie de extragere al unui element din coada a */
int AddSezQ(FILE*, void* a, void* ae, TFCmp fcmp, TFC fc);   /* adauga un sezon in coada; reusita (0/1) */
void AfiQ(FILE*, TF1 AfiElem, void *a);    /* afiseaza coada */

#endif
