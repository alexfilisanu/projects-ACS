/**-- main.c --  prelucreaza arbori binari cu chei intregi --*/
#include "tarb.h"

/*-- se completeaza cu definitiile functiilor implementate --*/

void RSD(TArb a)
{
	if(!a) {printf("-"); return;}
	if(!a->st && !a->dr) {printf(" %d ", a->info); return;}
	printf(" %d ", a->info);
	printf("(");
	RSD(a->st);
	printf(",");
	RSD(a->dr);
	printf(")");
}

int InserAC(TArb *aa, int cheie)
{
    /* cautare */
    while (*aa)
    {
        if (cheie == (*aa)->info)
            return -1;
        if (cheie < (*aa)->info)
            aa = &((*aa)->st);
        else
            aa = &((*aa)->dr);
    }
    /* inserare */
    *aa = ConstrFr(cheie);
    return *aa != NULL; /* 1 sau 0 */
}

TArb CitABC(int n)
{
    TArb arb = NULL;
    int cheie;
    while(n--)
    {
        /* scanf("%d", &cheie); */
        cheie = rand() % 30;
        InserAC(&arb, cheie);
    }
    return arb;
}

/**
 * Problema 1 - 3p
 * Functia parcurge arborele in ordinea DSR si afisează informațiile aflate pe nivelurile < niv.
 * Functia intoarce numarul de valori afisate.
 */

int aux(TArb a, int niv, int nivcrt, int *nr)
{
	// arbore vid
	if (!a)
		return 0;
	
	// parcurgere DSR
	if (nivcrt < niv)
	{
		aux(a->dr, niv, nivcrt + 1, nr);
		aux(a->st, niv, nivcrt + 1, nr);
	}
	(*nr)++;
}


int Numara(TArb a, int niv)
{
	int nr = 0;
	aux(a, niv, 1, &nr);
	return nr;
}

/**
 * Problema 2 - 3p
 * Functia parcurge nerecursiv un arbore binar de cautare si intoarce cel mai apropiat stramos comun
 * pentru doua noduri din arbore care contin v1 si v2, v2 < v2.
*/
TArb Stramos(TArb a, int v1, int v2)
{
	if (!a)
		return NULL;

	// cat timp val din radacina este < v1, v2 -> subarb drept
	while (a->info < v1 && a->info < v2 && a->dr)
	{
		a = a->dr;
	}

	while (a->info > v1 && a->info > v2 && a->st)
	{
		a = a->st;
	}

	// in caz contrar v1 si v2 sunt in subarb diferiti -> stramos diferit

	return a;

}

/**
 * Problema 3 - 4p
 * Functia verifica daca toate valorile din intervalul [x, y] sunt pe acelasi nivel. x < y
 * Functia intoarce nivelul gasit sau -1.
 */


// daca exista mai mult de 2 elemente din interval in arbore -> -1
int auxPb3(TArb a, int x, int y, int *nivel)
{
	if (!a)
		return 0;


	if (a->info >= x && a->info <= y)
	{
		(*nivel)++;
		return 1 + auxPb3(a->st, x, y, nivel) + auxPb3(a->dr, x, y, nivel);
	}

	(*nivel)++;
	return auxPb3(a->st, x, y, nivel) + auxPb3(a->dr, x, y, nivel);
}

int Verifica(TArb a, int x, int y)
{
	int nivel = 1;
	auxPb3(a, x, y, &nivel);
	if (nivel > 1)
		return -1;
	else
		return nivel;
}

/**
 * Bonus - 2p
 * Functia duplică nodurile de ordin 2. Nodul duplicat va fi inserat ca fiu stang al nodului original
 * (vechiul arbore stȃng devine arborele stȃng al noului nod). Arborele astfel obtinut va fi afișat in inordine.
 */
TArb Duplica(TArb a)
{
	return NULL;
}

int main ()
{
	TArb arb;

	randomize();

	arb = CitABC(12);
	AfiArb (arb);
	printf ("noduri: %i   niveluri: %i\n",
	NrNoduri(arb), NrNiv(arb));
	RSD(arb);

	/*-- se completeaza cu apelurile functiilor implementate --*/

	// Pb1
	printf("\nPb1. Nr de valori pe maxim nivelul 2: %d", Numara(arb, 2));

	// Pb2
	TArb arb2 = Stramos(arb, 2, 20);
	printf("\nPb2. Stramos comun pt v1=2, v2=20:\n");
	AfiArb(arb2);

	// Pb3
	printf("\nPb3. Nivelul pt x=10, y=12 este:");
	printf("%d",Verifica(arb, 10, 12));

	DistrArb (&arb);
	printf ("\n");


	return 0;
}
