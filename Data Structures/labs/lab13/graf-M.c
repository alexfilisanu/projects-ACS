/*--- graf-M.c --- Graf implementat cu Matrice de adiacente --*/
#include "graf-M.h"
#include <string.h>

void* AlocG(int nr)
{
    int i;
    AGM g = (AGM)malloc (sizeof(TGM));              /* spatiu descriptor */
    if (!g)
        return NULL;

    g->x = (int*)malloc(nr * nr * sizeof(int)); /* matrice liniarizata */
    if (!g->x) {
        free(g);
        return NULL;
    }
    
    for (i = 0; i < nr * nr; i++)
        g->x[i] = ArcInex;
    g->n = nr;
    
    return (void*)g;
}


void DistrG(void** ag)
{
    free (((AGM)(*ag))->x);
    free(*ag);
    *ag = NULL;
}


int CostArc(void* g, int s, int d)
{
  /* functia intoarce costul arcului (s, d); daca arcul nu exista intoarce ArcInex */
    AGM graf = (AGM)g;

    return graf->x[(graf->n) * (s - 1) + d - 1];
}

int NrNoduri(void* g)
{
    return ((AGM)g)->n;
}

int CitGraf(void** ag)
{
    FILE * f;
    char numefis[21];
    int i, j, n;
    int x;
    AGM g;

    printf("Numele fisierului de descriere a grafului = ");
    if (!scanf("%s", numefis))
        return -1;

    f = fopen(numefis, "rt");
    if (!f)
        return -2;

    if (!fscanf(f, "%i", &n) || n <= 0)
        return -3;

    *ag = g = (AGM)AlocG(n);
    if (!g)
        return 0;

    while (fscanf(f, "%i%i%i", &i, &j, &x) == 3)
    {
        if (i < 1 || i > n || j < 1 || j > n)
        {
            fclose(f);
            return -4;
        }
        ((AGM)g)->x[(i-1)*((AGM)g)->n + j-1] = x;
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
    AGM graf = (AGM)g;
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
    AGM graf = (AGM)g;

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
        
        for (int i = 1; i <= graf->n; i++)
        {
            if (CostArc(g, nod, i) != ArcInex && vizitat[i] == 1)
            {
                coada[dim++] = i;
                vizitat[i] = 0;
            }
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
    AGM graf = (AGM)g;
    int minim = -1, cost_curent;
    
    for (int i = 1; i <= graf->n; i++)
    {
        for (int j = 1; j <= graf->n; j++)
        {
            cost_curent = CostArc(g, i, j);
            if (cost_curent != ArcInex)
            {
                if (minim == -1 || cost_curent < minim)
                {
                    minim = cost_curent;
                    *src = i;
                    *dst = j;
                }
            }
        }
    }

    return minim;
}
