/* FILISANU Mihai-Alexandru - 314CB */
/*--- functiiLG.c -- operatii de baza pentru lista dublu inlantuita generica---*/
#include "tlg.h"

// insereaza elementul dat in lista
int InsDupa(TLG a, void *x)
{
	TLG aux = malloc(sizeof(TCelulaG));
	if (!aux)
	    return 0;

	aux->info = x;
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;

    return 1;
}


// returneaza o lista cu un singur element
TLG InitLDI(void *ae)
{
	TLG aux = (TLG)malloc(sizeof(TCelulaG));
    if (!aux)
		return NULL;

	aux->pre = aux->urm = aux;
    aux->info = ae;

	return aux;
}