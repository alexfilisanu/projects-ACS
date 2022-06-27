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
 * Funcția primește ca parametru o listă simplu înlănțuită (cu elemente întregi) ordonată descrescător 
 * (lista se citește ordonată de la tastatură) și doi întregi a, b (a < b).
 * Funcția modifică lista prin eliminarea elementelor aflate în intervalul [a, b]
 * (eliminarea va ține cont de faptul că lista este ordonată descrescător).
 * Se va întoarce ca rezultat numărul de elemente eliminate.
 */
int Elimina(TLista *L, int a, int b)
{
    int nr = 0;
    TLista p, ant;

    //while(L != NULL && (*L)->info >=)

    for(ant = NULL, p = *L; p != NULL && p->info >= a;)
    {
        if(p->info <= b)
        {
            if(ant == NULL)
            {
                *L = p->urm;
                p = *L;
            }
            else
            {
                ant->urm = p->urm;
                p = ant->urm;
            }
            nr++;
        }
        else
        {
            ant = p;
            p = p->urm;
        }
    }
    return nr;
}

/** 
 * Problema 2 - 3p
 * Funcția primește ca parametru o listă simplu înlănțuită (cu elemente întregi) și construiește o nouă listă
 * prin copierea elementelor pare aflate pe poziții impare. Se va întoarce ca rezultat principal noua listă construită,
 * iar prin efect lateral dimensiunea acesteia. Primul element din lista se afla pe pozitia 1.
 */

TLista CopiazaPare(TLista L, int *len)
{
    int pos = 1;
    *len = 0;
    TLista p, copie = NULL, aux, u;
    
    for(p = L; p != NULL; p = p->urm, pos++)
    {
        if(pos % 2 == 1 && p->info % 2 == 0)
        {
            aux = AlocCelula(p->info);
            if(!aux)
                return 0;
            if(copie == NULL)
            {
                copie = aux;
            }
            else
            {
                u->urm = aux;
            }
            u = aux;
            (*len)++;
        }
    }
    return copie;
}

TLista Duplica(TLista L)
{
    TLista p = L, aux;

    // parcurg pana la ultimul element
    for(; p->urm != NULL; p = p->urm)
    {
        if (p->info % 2 == 0)
        {
            aux = AlocCelula(p->info);
            if (!aux)
                return NULL;
            
            aux->urm = p->urm;
            p->urm = aux;

            // trec peste elemetul par inserat 
            p = p->urm;
        }
    }

    // verific daca ultimul element e par
    // daca e par -> inserare la sfarsit
     
    if (p->info  % 2 == 0)
    {
        aux = AlocCelula(p->info);
        if (!aux)
            return NULL;
        
        p->urm = aux;
    }

    return L;
}


/** 
 * Problema 3 - 4p
 * Funcția primește ca parametrii două liste simplu înlănțuite (cu elemente întregi) și construiește o nouă listă,
 * parcurgând cele două liste simultan și copiind elementul care are mai mulți divizori
 * (dacă numărul de divizori e egal, se va copia elementul minim).
 * În momentul în care elementele unei liste sunt epuizate (are dimensiune mai mică),
 * restul valorilor din cealalta listă vor fi copiate in lista rezultat.
 * Functia intoarce lista rezultat.
 */

int nrDiv(int n)
{
    int nr = 0;
    for(int i = 1; i*i <= n; i++)
    {
        if(n % i == 0)
        {
            if(n == i*i)
                nr++;
            else
                nr += 2;
        }
    }
    return nr;
}

int minim(int a, int b)
{
    if(b < a)
        return b;
    else
        return a;

}

int compara(TLista A, TLista B)
{
    int a = A->info;
    int b = B->info;
    if(nrDiv(a) > nrDiv(b))
    {
        return a;
    }
    else
    {
        if(nrDiv(a) == nrDiv(b))
            return minim(a, b);
    }
    return b;

}


TLista CopiazaMaxDivizori(TLista A, TLista B)
{
    // in rez_start salvez referinta la lista pe care o creez
    TLista rez = NULL, rez_start = NULL;
    
    while(A && B)
    {
        TLista p = AlocCelula(compara(A, B));
        if(!rez_start)
        {
            rez_start = p; 
        }
        else
        {
            rez->urm = p;
        }
        rez = p;
        A = A->urm;
        B = B->urm;
    }

    while(A)
    {
        TLista p = AlocCelula(A->info);
        if(!rez_start)
        {
            rez_start = p; 
        }
        else
        {
            rez->urm = p;
        }
        rez = p;
        A = A->urm;
    }

    while(B)
    {
        TLista p = AlocCelula(B->info);
        if(!rez_start)
        {
            rez_start = p; 
        }
        else
        {
            rez->urm = p;
        }
        rez = p;
        B = B->urm;
    }

    return rez_start;
}

/** 
 * Bonus - 2p
 * BONUS: Funcția primește ca parametru două liste simplu înlănțuite (cu elemente întregi sortate) și
 * transformă prima listă în intersecția celor două (diferență de mulțimi), fără a folosi memorie suplimentară,
 * doar modificând prima listă.
 * A = A - B
 */
void Intersectie(TLista *A, TLista B)
{
    
}


int main ()
{
    TLista x = NULL, y = NULL;     /* Lista prelucrata */
    int lx, ly;             /* lungime lista */

    for (;;) {
        /* citeste si afiseaza lista */
        printf("\n1. Introduceti valori elemente terminate cu valoare nenumerica:\n");
        x = CitireL(&lx);
        AfisareL(x);
        if (!x)
            continue;

        printf("\n2. Introduceti valori elemente terminate cu valoare nenumerica:\n");
        y = CitireL(&ly);
        AfisareL(y);
        if (!y)
            continue;



        /* apeluri functii implementate */
        int a = 2, b = 6, len;
        //printf("\nPb1: %d \n", Elimina(&x, a, b));

        //AfisareL(CopiazaPare(x, &len));
        //printf("\nPb2: %d", len);
        
        //Pb3:
        TLista C = CopiazaMaxDivizori(x, y);
        AfisareL(C);
        //AfisareL(x);

        AfisareL(x);

        DistrugeL(&x);
        DistrugeL(&y);
        printf ("\n  Inca un test ? [d/n]");
        if (getchar() == 'n')
            break;
    }

    return 0;
}
