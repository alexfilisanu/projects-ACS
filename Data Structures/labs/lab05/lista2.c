/*--- lista2.c ---
      operatii lista generica dublu inlantuita, circulara, cu santinela ---*/

#include "lista2.h"

/**
 * inserare dupa celula cu adresa a -> 1/0
 */
int InsDupa(TLDI a, void *x)
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
	if (!aux)
		return 0;

	aux->info = x;
	aux->pre = a;
	aux->urm = a->urm;
	a->urm->pre = aux;
	a->urm = aux;

    return 1;
}


/**
 * transforma lista in lista vida
 */
void ResetLDI(TLDI s)
{
	TLDI p = s->urm, aux;
	while (p != s)
	{
		aux = p;
		p = p->urm;
		free(aux->info);
		free(aux);
	}
	s->urm = s->pre = s;
}

/**
 * elibereaza tot spatiul ocupat de lista, inclusiv santinela
 */
void DistrLDI(TLDI *as)
{
	ResetLDI(*as);
	free(*as);
	*as = NULL;
}

/**
 * -> lista vida, cu info din santinela specifica aplicatiei
 */
TLDI InitLDI()
{
	TLDI aux = (TLDI)malloc(sizeof(TCelula2));
    if (!aux)
		return NULL;

	aux->pre = aux->urm = aux;
    aux->info = NULL;

	return aux;     /* rezultatul este adresa santinelei sau NULL */
}

/**
 * afisare lista
 */
void AfisareLDI(TLDI s, void (*afiEl)(void *))
{
	TLDI p;
	printf("Lista: ");
	if (s->urm == s)
	{
		printf("vida\n");
		return;
	}
	for (p = s->urm; p != s; p = p->urm)
		afiEl(p->info);
	printf("\n");
}
