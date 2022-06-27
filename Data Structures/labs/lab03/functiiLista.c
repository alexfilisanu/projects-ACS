/*-- functiiLista.c --*/
#include <ctype.h>
#include "tlista.h"

/*--- operatii de baza pentru lista simplu inlantuita ---*/

/* adresa celulei create sau NULL */
TLista AlocCelula(int e)
{
    TLista aux = (TLista)malloc(sizeof(TCelula));  /* (1) incearca alocare */
    if (aux) {                          /* aux contine adresa noii celule */
        aux->info = e;                   /* (2,3) completeaza noua celula */
        aux->urm = NULL;
    }
    
    return aux;             /* rezultatul este adresa noii celule sau NULL */
}

/* afiseaza valorile elementelor din lista */
void AfisareL(TLista L)
{
    printf("Lista: [");                      /* marcheaza inceput lista */
    for (; L != NULL; L = L->urm)       /* pentru fiecare celula */
        printf("%d ", L->info);              /* afiseaza informatie */
    printf("]");                               /* marcheaza sfarsit lista */
}

void DistrugeL(TLista* aL)
{
    TLista aux;
    while(*aL) {
        aux = *aL;
        *aL = aux->urm;
        free(aux);
    }
}
