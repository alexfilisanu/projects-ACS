/* FILISANU Mihai-Alexandru - 314CB */
/*  functii-tabHash.c - functii tabela hash */


#include "thash.h"

// functia de initializare a tabelei hash
TH* IniTH(size_t M, TFHash fh)
{
    TH *h = (TH*)calloc(sizeof(TH), 1);
    if (!h) 
    {
        return NULL;
    }

    h->v = (TLG*)calloc(M, sizeof(TLG));
    if (!h->v) 
    {
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
            //printf("segaici\n");
            aux = el;
            el = el->urm;
            freeInfo(aux->info);
            free(aux);
        }
      //  freeInfo(el->info);
    }
    free((*ah)->v);
    free(*ah);
    *ah = NULL;
}


// functia de afisare a tabelei hash
void AfiTH(FILE* outputFile, TH* ah, TFAfis afiEl)
{
    TLG p, el;
    int i;
    for (i = 0; i < ah->M; i++)
    {
        p = ah->v[i];
        if (p) 
        {
            fprintf(outputFile, "%d: ", i);
            afiEl(outputFile, p->info);
            for (el = p->urm; el != p; el = el->urm)
                afiEl(outputFile, el->info);

            fprintf(outputFile, "\n");
        }
    }
}


// functia de afisare a unui bucket a tabelei hash
void AfiTH_v2(FILE *outputFile, TH* ah, int n, TFAfis afiEl)
{
    // verific ca bucket ul cerut sa existe in tabela hash
    if (n >= ah->M)
        return;

    TLG p, el;
    p = ah->v[n];
    if (p) 
    {
        afiEl(outputFile, p->info);
        for (el = p->urm; el != p; el = el->urm)
            afiEl(outputFile, el->info);

        fprintf(outputFile, "\n");
        return;
    }

    // in cazul in care bucket ul exista, dar nu are niciun element
    // afisez "VIDA"
    fprintf(outputFile, "VIDA\n");
}

// daca elementul exista functia intoarce 0
// altfel se incearca inserarea elementului si se intoarce rezultatul inserarii
int InsTH(TH *a, void *ae, TFCmp cmp)
{
    int cod = (a->fh(ae)) % (a->M);
    TLG el;

    // verific daca elementul exista
    if (a->v[cod] != NULL)
    {
        if (cmp(a->v[cod]->info, ae) == 0)
            return 0;
        
        for (el = a->v[cod]->urm; el != a->v[cod]; el = el->urm) 
        {
            if (cmp(el->info, ae) == 0)
                return 0;
        }
    }

    // in cazul in care nu mai exista niciun element in lista
    // initializez o lista ce contine elementul dat
    if (a->v[cod] == NULL)
    {
        a->v[cod] = InitLDI(ae);
        return 1;
    }
    else
    {
        // in cazul in care  elementul inserat o sa fie primul din lista
        if (cmp(a->v[cod]->info, ae) > 0)
        {
            InsDupa(a->v[cod]->pre, ae);
            a->v[cod] = a->v[cod]->pre;
            return 1; 
        }
        else
        {
            // inserez in ordinea ceruta 
            for (el = a->v[cod]->urm; el != a->v[cod]; el = el->urm)
            {
                if(cmp(el->info, ae) > 0)
                {
                    InsDupa(el->pre, ae);
                    return 1;
                }
            }
            InsDupa(a->v[cod]->pre, ae);
            return 1;
        }
    }
}

// daca elementul este sters functia intoarce 1, altfel 0
int RemoveTH(TH *a, void *ae, TFCmp cmp)
{
    int cod = (a->fh(ae)) % (a->M);
    TLG el;

    if(a->v[cod] != NULL)
    {
        // daca elementul ce va fi sters va fi primul din lista
        if(cmp(a->v[cod]->info, ae) == 0)
        {
            // daca alemtul ce va fi sters este singurul din lista
            if (a->v[cod]->urm == a->v[cod])
            {
                free((a->v[cod])->info);
                free(a->v[cod]);
                a->v[cod] = NULL;
            }
            // daca elelemtul ce va fi sters este primul, dar nu singurul
            else
            {
                el = a->v[cod];
                a->v[cod] = a->v[cod]->urm;
                el->urm->pre = el->pre;
                el->pre->urm = el->urm;
            
                free(el->info);
                free(el);
            }
            // eliminare cu succes
            return 1;
        }

        for (el = a->v[cod]->urm; el != a->v[cod]; el = el->urm)
        {
            // orice alta pozitie in afara de cele analizate anterior
            if (cmp(el->info, ae) == 0)
            {
                el->urm->pre = el->pre;
                el->pre->urm = el->urm;
                free(el->info);
                free(el);
        
                return 1;
            }
        }
    }
    // elementul nu exista in tabela hash
    return 0;
}


// functie de cautare in tabela hash
void *ExistaTH(TH *a, void *ae, TFCmp f)
{
	int cod = (a->fh(ae)) % (a->M);
	TLG el;

	if (a->v[cod] != NULL)
	{
		if (f(ae, a->v[cod]->info) == 0)
			return a->v[cod]->info;

		for (el = a->v[cod]->urm; el != a->v[cod]; el = el->urm)
		{
			if (f(ae, el->info) == 0)
				return el->info;
		}
	}
	return NULL;
}
