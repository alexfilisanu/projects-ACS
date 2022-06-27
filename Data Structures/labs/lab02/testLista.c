/*--- testLista.c ---*/
#include <ctype.h>
#include <stdio.h>
#include "tlista.h"

/**
 * construieste lista cu elemente citite
 */
TLista CitireL(int *lg)
{
    TLista L = NULL, u, aux;
    int x;
    char ch;
    for (*lg = 0; scanf("%i", &x) == 1; ) {
        aux = AlocCelula(x);           /* incearca inserarea valorii citite */
        if (!aux)
            return L;              /* alocare esuata => sfarsit citire */
        if (L == NULL)
            L = aux;
        else
            u->urm = aux;
        u = aux;
        (*lg)++;
    }

    while ((ch = getchar()) != EOF && ch != '\n');

    return L;                     /* intoarce lista rezultat */
}

/**
 * Problema 1 - 3p
 * Functia primeste ca parametru o lista L si un element x si afiseaza perechile de elemente (i, i+1)
 * cu i impar, din 2 in 2, care sunt multiplu al numarului el dat ca parametru.
 * Daca nu sunt ambele multiplu al elementului, atunci perechea nu va fi afisata.
 * Afisarea fiecarei perechi se va face pe cate o linie sub forma:
 * (indice_val_poz_i, indice_val_poz_i+1): val_poz_i val_poz_i+1
 * Indicii in lista sunt numerotati de la 1, iar functia va intoarce numarul de perechi afisate
 * (si va fi afisat pe o linie, dupa afisarea perechilor).
 * 
 * Exemplu:
 * L: 3 6 8 1 12 9
 * el: 3
 * Se va afisa:
 * (1,2): 3 6
 * (5,6): 12 9
 * 2
 */
int perechi(TLista L, int x)
{
    int pos = 1, nrperechi = 0;
    for(; L->urm != NULL; L = L->urm, pos++)
    {
        if((L->info % x == 0) && (L->urm->info % x == 0))
        {
            printf("\n(%d,%d): %d %d", pos, pos + 1, L->info, L->urm->info);
            nrperechi++;
        }
    }
    
    return nrperechi;
}

/**
 * Problema 2 - 3p
 * Functia primeste ca parametri doua liste A si B si verifica daca cele doua liste nu contin aceleasi elemente.
 * Functia intoarce 1, in cazul in care cele doua liste nu sunt identice si 0 altfel.
 * 
 * Exemplu:
 * A: 2 5 6 9 10 22
 * B: 6 2 5 9 10 12 4 => se returneaza 1 si se afiseaza 1
 * 
 * A: 4 2 8 10
 * B: 10 2 4 8 => se returneaza 0 si se afiseaza 0
 */

void sort(TLista *l)
{
    TLista A, B;
    for(A = *l; A != NULL; A = A->urm)
    {
        for(B = A->urm; B != NULL; B = B->urm)
        {
            if(B->info < A->info)
            {
                int aux = A->info;
                A->info = B->info;
                B->info = aux;
            }
        }
    }
}

int identice(TLista A, TLista B)
{
    sort(&A);
    sort(&B);

    for(; A != NULL || B != NULL; A = A->urm, B = B->urm)
    {
        if(A->info != B->info)
            return 1;
    }
    return 0;
}

/**
 * Problema 3 - 4p
 * Functia primeste ca parametru o lista L si returneaza prin efect lateral indicele de inceput si de sfarsit 
 * pentru ultima secventa ordonata crescator, de lungime minim 2. Functia intoarce lungimea secventei identificate.
 * Daca nu exista o astfel de secventa, functia intoarce ca rezultat principal 0, iar cei doi indici vor fi -1.
 * Indicii in lista sunt numerotati de la 1.
 * 
 * Exemplu:
 * L: 1 2 7 4 5 3 4 5 => se returneaza 3, iar indicii start / stop vor fi 6 respectiv 8
 */
int secventa(TLista L, int *start, int *stop)
{
    *start = -1, *stop = -1;
    int lungime = 1, lungimemax = 1, pos = 1;
    for(; L->urm != NULL; L = L->urm, pos++)
    {
        if(L->info <= L->urm->info)
        {
            lungime++;
        }
        else
        {   
            if(lungime >= lungimemax)
            {
                lungimemax = lungime;
                *stop = pos+1;
            }
            lungime = 1;
        }
    }
    if(lungime >= lungimemax)
    {
        lungimemax = lungime;
        *stop = pos+1;
    }
    if(lungimemax - 1)
    {
        *start = *stop - lungimemax;
        return lungimemax;
    }
    return 0;
}

/**
 * Bonus - 2p
 * Functia primeste ca parametru o lista L si o valoare intreaga x.
 * Functia insereaza in lista L elementul x dupa fiecare aparitie a unui numar prim.
 * Functia intoarce numarul de inserari realizate.
 * 
 * Exemplu:
 * L: 1 3 6 9 2
 * el: 3
 * Lista va deveni:
 * L: 1 3 3 6 9 2 3
 * si se intoarce numarul 2
 */

int verificare_prim(int x)
{
    if(x <= 1)
        return 0;
    for(int i = 2; i * i <= x; i++)
    {
        if(x % i == 0)
        {
            return 0;
        }
    }
    return 1;
}

int prime(TLista L, int x)
{
    TLista p, aux;
    int nr = 0;
    for(p = L; p != NULL; p = p->urm)
    {
        if(verificare_prim(p->info))
        {
            aux = AlocCelula(x);
            // verificare alocare
            if(!aux)
                return -1;
            aux->urm = p->urm;
            p->urm = aux;
            p = aux;
            nr++;
        }
    }
    //AfisareL(L);
    return nr;
}


int main ()
{
    TLista x = NULL, y = NULL;     /* Lista prelucrata */
    int lx, ly;             /* lungime lista */

    for (;;) {
        /* citeste si afiseaza lista */
        printf("\nIntroduceti valori elemente terminate cu valoare nenumerica:\n");
        x = CitireL(&lx);
        AfisareL(x);
        if (!x)
            continue;
        
    /*    printf("\n2: Introduceti valori elemente terminate cu valoare nenumerica:\n");
        y = CitireL(&ly);
        AfisareL(y);
        if (!y)
            continue;
    */
        int start, stop;
        /* apeluri functii implementate */
        //printf("\nPb1: %d", perechi(x,3));
        //printf("\nPb2: %d", identice(x, y));
    /*    printf("\nPb3: %d, ", secventa(x, &start, &stop));
        printf("%d, %d", start, stop); 
    */
    	printf("\nBonus: %d ", prime(x,3));

        DistrugeL(&x);
    //    DistrugeL(&y);
        printf ("\n  Inca un test ? [d/n]");
        if (getchar() == 'n')
            break;
    }

    return 0;
}
