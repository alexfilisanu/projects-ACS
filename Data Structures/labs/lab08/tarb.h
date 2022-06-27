/*--- tarb.h ---*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifndef ARBORE_BINAR
#define ARBORE_BINAR

typedef int TInfo;
typedef struct nod {
	TInfo info;
	struct nod *st, *dr;
} TNod, *TArb, **AArb;

/*-- Operatii elementare --*/
TArb InitA();           /* initializare arbore */
TArb ConstrNod(TInfo x, TArb s, TArb d); 
     /* -> adresa nod (cu info x, fiu stanga s, fiu dreapta d)
		   sau NULL daca nu exista spatiu */
TArb ConstrFr(TInfo x);
     /* -> adresa frunza cu informatia x, sau NULL daca nu exista spatiu */
void DistrArb(AArb);    /* distruge arbore */
int NrNoduri(TArb);     /* numar noduri */
int NrNiv(TArb);        /* numar niveluri (0 pentru arbore vid) */

/*-- Operatii suplimentare --*/
void AfiArb(TArb r);
	  /*- afiseaza arborele r -*/
TArb ConstrAA(int lim, int nn, int vi, int vs);
	  /*- construieste un arbore de inaltime < lim, cu cel mult
	      nn noduri cu chei in intervalul [vi,vs] -*/
#endif

#ifndef _random_
#define _random_
#define random(n) (rand() % (n))
#define randomize() (srand((unsigned int)time(NULL)))
#endif
