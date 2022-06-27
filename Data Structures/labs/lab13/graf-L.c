/*--- graf-L.c --- Graf implementat cu liste --*/
#include "graf-L.h"

void* AlocG(int nr) /* aloca spatiu pentru descriptor graf si
	                      vector de (n+1) adrese liste arce */
{
    AGL g = (AGL)malloc(sizeof(TGL));    /* spatiu descriptor */
    if (!g)
        return NULL;

    g->x = (AArc*)calloc((nr+1), sizeof(AArc)); /* spatiu vector adr.liste arce */
    if (!g->x)
    {
        free(g);
        return NULL;
    }

    g->n = nr;
    
    return (void*)g;
}

void DistrG(void** ag)
{
    int i;
    AArc p, aux;

    for (i = 1; i < NrNoduri(*ag); i++)
    {
        p = ((AGL)(*ag))->x[i];
        while (p)
        {
            aux = p;
            p = p->urm;
            free(aux);
        }
    }

    free(((AGL)(*ag))->x);
    free(*ag);
    *ag = NULL;
}


int CostArc(void* g, int s, int d)
{   /* intoarce costul arcului (s, d); daca arcul nu exista intoarce ArcInex */
    AGL graf = (AGL)g;

    for (AArc p = graf->x[s]; p != NULL ; p = p->urm)
    {
        if (p->d == d)
            return p->c;
    }

    return ArcInex;
}

int NrNoduri(void* g)
{
    return ((AGL)(g))->n;
}



int CitGraf(void** ag)
{
    FILE * f;
    char numefis[21];
    AGL g;
    int s, d;
    int x;
    AArc *p, aux;
    int n;

    printf("Numele fisierului de descriere a grafului = ");
    if (!scanf("%s", numefis))
        return -1;

    f = fopen (numefis, "rt");
    if (!f)
        return -2;

    if (!fscanf (f, "%i", &n) || n <= 0)
        return -3;

    *ag = AlocG(n);
    g = *(AGL*)ag;
    if (!g)
        return 0;

    while (fscanf(f, "%i%i%i", &s, &d, &x) == 3)
    {
        if (s < 1 || s > n || d < 1 || d > n)
        {
            fclose(f);
            return -4;
        }
        p = g->x + s;
        while (*p && (*p)->d < d)
            p = &(*p)->urm;
        aux = (TCelArc*)calloc(sizeof(TCelArc),1);
        if (!aux)
            return -2;                     /* criza spatiu */
        aux->urm = *p; *p = aux;
        aux->d = d; aux->c = x;
  }

  fclose(f);

  return 1;
} 

/**
 * Problema 1 - 2p
 * Functia returneaza gradul de intrare al nodului n.
 */
int grad_intrare(void *g, int nod) {

    int count = 0;
    AGL graf = (AGL)g;
    for (int i = 1; i <= graf->n; i++)
    {
        if (CostArc(g, i, nod) != ArcInex)
            count++;
    }
    
    return count;
}

/**
 * Problema 2 - 3p
 * Functia parcurge un graf g in latime pornind de la nodul nod. Se vor lista nodurile traversate.
 */
void BFS(void *g, int nod)
{
    AGL graf = (AGL)g;

    int vizitat[1000] = {0};
    int coada[1000];

    int poz = 0;
    int dim = 1;

    coada[0] = nod;

    for (int i = 1; i <= graf->n; i++)
        vizitat[i] = 1;
    vizitat[nod] = 0;

    while (poz < dim)
    {
        nod = coada[poz++];
        printf("%d, ", nod);
        
        AArc p = graf->x[nod];

        while(p)
        {
            if (vizitat[p->d] == 1)
            {
                coada[dim++] = p->d;
                vizitat[p->d] = 0;
            }
            p = p->urm;
        }
    }

}

/**
 * Bonus - 1p
 * Functia intoarce costul minim al unui arc din graful g.
 * Prin efect lateral intoarce capetele arcului de cost minim.
 */
int ArcCostMin(void *g, int *src, int *dst)
{
    AGL graf = (AGL)g;
    int minim = -1, cost_curent;

    for (int i = 1; i <= graf->n; i++)
    {
        AArc p = graf->x[i];
        while (p)
        {
            if (minim == -1 || p->c < minim)
            {
                minim = p->c;
                *src = i;
                *dst = p->d;
            }
            p = p->urm;
        }
    }

    return minim;
}