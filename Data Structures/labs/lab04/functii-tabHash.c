/*  functii-tabHash.c - functii tabela hash */


#include "thash.h"

TH* IniTH(size_t M, TFHash fh)
{
    TH *h = (TH*)calloc(sizeof(TH), 1);
    if (!h) {
        printf("eroare alocare hash\n");
        return NULL;
    }

    h->v = (TLG*)calloc(M, sizeof(TLG));
    if (!h->v) {
        printf("eroare alocare vector de pointeri TLG in hash\n");
        free(h);
        return NULL;
    }

    h->M = M;
    h->fh = fh;

    return h;
}

void DistrTH(TH **ah, TF freeInfo)
{
    TLG *p, el, aux;

    //parcurgere cu pointeri
    for (p = (*ah)->v; p < (*ah)->v + (*ah)->M; p++) {
        //daca exista elemente corespunzatoare acestui hash
        //eliberam info din celula si apoi eliberam celula
        for (el = *p; el != NULL;) {
            aux = el;
            el = el->urm;
            freeInfo(aux->info);
            free(aux);
        }
    }
    free((*ah)->v);
    free(*ah);
    *ah = NULL;
}

void AfiTH(TH* ah, TF afiEl)
{
    TLG p, el;
    int i;

    for (i = 0; i < ah->M; i++) {
        p = ah->v[i];
        if (p) {
            printf("LISTA %d:\n", i);
            for (el = p; el != NULL; el = el->urm)
                afiEl(el->info);
            printf("\n");
        }
    }
}

// daca elementul exista functia intoarce 0
// altfel se incearca inserarea elementului si se intoarce rezultatul inserarii
int InsTH(TH *a, void *ae, TFCmp cmp)
{
    int cod = a->fh(ae), rez;
    TLG el;

    for(el = a->v[cod]; el != NULL; el = el->urm) {
        if (cmp(el->info, ae) == 0)
            return 0;
    }

    rez = InsLG(a->v+cod, ae); //reminder: a->v+cod <=> &a->v[cod]

    return rez;
}
