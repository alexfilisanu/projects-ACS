/*-- testM-int.c --*/
#include "multimeV.h"

/**
 * functie de afisare multime intregi
 */
void AfisareMultime_int(TMultime *m)
{
    int *x = (int*)(m->p), n = Cardinal(m), i = 0;
    printf("[");
    for( ; i < n; i++)
        printf("%i%c ", x[i], ",]"[i == n-1]);
}

/**
 * functie de comparare pentru int
 */
int compI(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}


/**
 * 4 puncte
 * construieste multimea a ∩ b;
 * nr reprezinta numarul de elemente din r si va fi modificat prin efect lateral din interiorul functiei
 * multimile a si b contin elemente neordonate
 */
TMultime *Intersectie(TMultime *a, TMultime *b, int *nr)
{
    int nrelem;
    if (Cardinal(a) > Cardinal(b))
    {
        nrelem = Cardinal(a);   
    }
    else
    {
        nrelem = Cardinal(b);
    }

    TMultime *m = InitD(nrelem, a->d, a->cmp);

    char *pa;
    char *sf = (char *)a->s;

    for (pa = (char*)a->p; pa < sf; pa += a->d)
    {
        if (Apartine((void *)pa, b))
        {
            Adauga ((void *)pa, m);
        }
    }
    *nr = Cardinal(m);
    
    return m;
}

/**
 * 5 puncte
 * construieste multimea a - b;
 * nr reprezinta numarul de elemente din r si va fi modificat prin efect lateral din interiorul functiei
 * multimile a si b contin elemente ordonate
 */
TMultime *DiferentaO(TMultime *a, TMultime *b, int *nr)
{
    int nrelem = Cardinal(a);
    TMultime *m = InitD(nrelem, a->d, a->cmp);
    
    char *pa, *sfa = (char*)a->s;
    char *pb = (char*)b->p, *sfb = (char*)b->s;

    for (pa = (char*)a->p; pa < sfa; pa += a->d)
    {
        while (pb < sfb && a->cmp(pb, pa) < 0)
        {
            pb += b->d;
        }
        if (pb == sfb || (pb < sfb && a->cmp(pa, pb)))
        {
            Adauga ((void *)pa, m);
        }
    }
    *nr = Cardinal(m);    

    return m;
}

/**
 * 1 punct
 * Testare functia 1 sau 2 folosind multimi cu elemente numere reale
 * Se vor crea doua multimi de numere reale si va fi apelata functia Intersectie sau DiferentaO
 * Se va afisa rezultatul.
 */
void DemoVerificareNrReale()
{
   /* double  va[20] = {-9.6, -1, 4.1, 4.6, 6.3, 10.2, 23.8}, nva = 7,
            vb[10] = {-9.3, 1.3, 2, 4.5, 10.7, 11.6, 12.7}, nvb = 7;
    TMultime *a, *b;
    int i, rez;
    a = InitD(7, sizeof(double), compI);
    if (!a) {
        printf("eroare initializare\n");
        return 1;
    }
    for (i = 0; i < nva; i++) {
        rez = Adauga((void*)(va + i), a);
        //printf("%i %s\n", va[i], rez? "adaugat" : "exista deja");
    }

    b = InitD(10, sizeof(double), compI);
    for (i = 0; i < nvb; i++) {
        rez = Adauga((void*)(vb + i), b);
        //printf("%i %s\n", vb[i], rez? "adaugat" : "exista deja");
    }

    int nrElemInter = 0;
    TMultime *r = DiferentaO(a, b, &nrElemInter);
    printf("NrEl=%d\n", nrElemInter);
   
    AfisareMultime_int(r);*/

}


int main()
{
    #if 0 /* 1->0 pentru multimi ordonate */
    int va[20] = {-1, 23, 4, 6, -9, 4, 10}, nva = 7,
        vb[10] = {2, 4, -9, 12, 10, 1, 11}, nvb = 7;
    #else
    int va[20] = {-9, -1, 4, 4, 6, 10, 23}, nva = 7,
        vb[10] = {-9, 1, 2, 4, 10, 11, 12}, nvb = 7;
    #endif
    TMultime *a, *b;
    int i, rez;

    /* constructie multime a */
    a = InitD(7, sizeof(int), compI);
    if (!a) {
        printf("eroare initializare\n");
        return 1;
    }
    printf("\nConstructie multime a, cu elementele: ");
    for (i = 0; i < nva; i++)
        printf("%i ", va[i]);
    printf("\n");
    for (i = 0; i < nva; i++) {
        rez = Adauga((void*)(va + i), a);
        printf("%i %s\n", va[i], rez? "adaugat" : "exista deja");
    }
    printf("Cardinal(a) = %d,  ", Cardinal(a));
    printf("a: ");
    AfisareMultime_int(a);


    /* constructie multime b */
    b = InitD(10, sizeof(int), compI);
    printf("\nConstructie multime b, cu elementele: ");
    for (i = 0; i < nvb; i++)
        printf("%i ", vb[i]);
    printf("\n");
    for (i = 0; i < nvb; i++) {
        rez = Adauga((void*)(vb + i), b);
        printf("%i %s\n", vb[i], rez? "adaugat" : "exista deja");
    }
    printf("Cardinal(b) = %d, ", Cardinal(b));
    printf("b: ");
    AfisareMultime_int(b);

    /* apeluri functii implementate */
    printf("\n-----START REZOLVARE----\n");

    #if 0
    printf("\n-----Intersectie Neordonata----\n");
    int nrElemInter = 0;
    TMultime *r = Intersectie(a, b, &nrElemInter);
    printf("NrEl=%d\n", nrElemInter);
    AfisareMultime_int(r);
    #endif

    #if 1
    printf("\n-----Diferenta Ordonata----\n");
    int nrElemInter = 0;
    TMultime *r = DiferentaO(a, b, &nrElemInter);
    printf("NrEl=%d\n", nrElemInter);
    AfisareMultime_int(r);
    #endif

    #if 0
    printf("\n-----Numere Reale----\n");
    DemoVerificareNrReale();
    #endif

    DistrugeM(&a);
    DistrugeM(&b);

    return 0;
}
