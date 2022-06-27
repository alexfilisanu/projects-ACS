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

/**
 * Problema 1 - 3p
 * Functia intoarce numarul de valori din arbore mai mari decat suma fiilor.
 */
int Numara(TArb a)
{
	int valst, valdr;
	if (!a)
		return 0;

	// daca subarborele este frunza
	if (!(a->st) && !(a->dr))
		return 1;

	// daca subarborele are doar fiul stang
	if (!(a->dr))
	{
		if (a->info > a->st->info)
			return 1 + Numara(a->st);
		
		return Numara(a->st);
	}

	// daca subarborele are doar fiul drept
	if (!(a->st))
	{
		if (a->info > a->dr->info)
			return 1 + Numara(a->dr);
		
		return Numara(a->dr);
	}

	// daca subarborele are ambii fii
	if (a->info > a->st->info + a->dr->info)
		return 1 + Numara(a->st) + Numara(a->dr);
	
	return Numara(a->st) + Numara(a->dr);

}

/**
 * Problema 2 - 3p
 * Functia intoarce un vector, in care elementul de pe pozitia i va contine suma valorilor din nodurile
 * unui arbore binar aflate pe nivelul i (se vor analiza numai nivelurile pare)
 * 
 */
// Pentru arborele de mai jos se va intoarce vectorul [23, 0, -92, 0, -1]
//                                |                                      
//                               23                                      
//              _________________/ \_________________                    
//             |                                     |                   
//            23                                    -35                  
//    ________/ \________                   ________/ \________          
//   |                   |                 |                   |         
//  -42                 38                -47                 -41        
//    \___           ___/                                  ___/ \___     
//        |         |                                     |         |    
//       -2        40                                    -44        3    
//                _/                                    _/ \_            
//               |                                     |     |           
//              28                                    -48   19           


// functie auxiliara care retuneaza prin efect lateral suma nodurilor de pe nivelul n
int aux2(TArb a, int* suma, int nivel_curent, int n)
{
	if (!a)
		return 0;

	// daca nivelul curent este cel cerut
	if (nivel_curent == n)
		*suma += a->info;
	else		// daca nivelul curent este cel cerut, nu se merge mai departe
	{
		aux2(a->st, suma, nivel_curent + 1, n);
		aux2(a->dr, suma, nivel_curent + 1, n);
	}
}

int f(TArb a, int n)
{
	int sum = 0, h = 0;
	aux2(a, &sum, h, n);
	return sum;
}


void aux(TArb a, int inaltime, int *v)
{
	if (!a)
		return;
	
	// daca nivelul e par adaug in vector valoarea
	if (inaltime % 2 == 0)
		v[inaltime] += a->info; 

	aux(a->st, inaltime + 1, v);
	aux(a->dr, inaltime + 1, v);
}

int *SumaNiveluriPare(TArb a)
{
	int nrniveluri = NrNiv(a), h = 0;
	int *v = calloc(nrniveluri , sizeof(int));
	aux(a, h, v);
	return v;
}

/**
 * Problema 3 - 4p
 * Functie care determina valoarea minima dintr-un arbore binar.
 */
int ValoareMinima(TArb a)
{
	if (!a)
		return 0;
	
	// daca valoarea din radacina e mai mica decat cea din subarb dr si st	
	if (a->info < ValoareMinima(a->st) && a->info < ValoareMinima(a->dr))
		return a->info;

	// val din subarborele st e mai mica decat cea din radacina si subarb dr
	if (ValoareMinima(a->st) < a->info && ValoareMinima(a->st) < ValoareMinima(a->dr))
		return ValoareMinima(a->st);
	
	// val din subarborele dr e mai mica decat cea din radacina si subarb st
	return ValoareMinima(a->dr);
}

/**
 * Bonus - 2p
 * Functia intoarce 1 daca doi arbori binari au informatii identice, 0 altfel.
 */
//              Arborii de mai jos sunt "egali"
// 						       |                                      
//                            25                                     
//           _________________/ \_________________                    
//          |                                     |                   
//          40                                    27                   
//  ________/ \________                   ________/                    
// |                   |                 |                             
// 43                  41                6         

// 						      |                                      
//                            25                                     
//           _________________/ \_________________                    
//          |                                     |                   
//          27                                    40                   
//  ________/ \________                   ________/                    
// |                   |                 |                             
// 41                  43                6         

// 0 daca sunt egali
int Egali(TArb a, TArb b)
{
	// daca aman2 subarborii nu exista
	if (!a && !b)
		return 0;
	// daca doar unul dintre subarbori nu mai exista -> diferenta
	if (!a || !b)
		return 1;
	
	// informatie diferita -> diferenta
	if (a->info != b->info)
		return 1;

	return Egali(a->st, b->st) + Egali(a->dr, b->dr);
}


int main ()
{
	TArb arb;

	randomize();

	arb = ConstrAA(5, random(16), -50, 50);
	AfiArb (arb);
	printf ("noduri: %i   niveluri: %i\n",
	NrNoduri(arb), NrNiv(arb));
	RSD(arb);

	/*-- se completeaza cu apelurile functiilor implementate --*/
	
	// Pb1
	printf("\nPb1. Numarul de noduri este: %d", Numara(arb));

	// Pb2
	printf("\nPb2. Vectorul sumelor este: ");
	int nr = NrNiv(arb);
	int *v = SumaNiveluriPare(arb);
	for (int i = 0; i < nr; i++)
		printf("%d ", v[i]);

	// Pb3
	printf("\nPb3. Valoarea minima din arbore este: %d", ValoareMinima(arb));
	
	// Pb bonus
	printf("\n\n");
	TArb arb2;
	arb2 = ConstrAA(5, random(16), -50, 50);
	AfiArb (arb2);
	printf ("noduri: %i   niveluri: %i\n",
	NrNoduri(arb2), NrNiv(arb2));
	RSD(arb2);

	if (Egali(arb, arb2) == 0)
		printf("\nPb bonus. Cei doi arbori sunt egali");
	else
		printf("\nPb bonus. Cei doi arbori NU sunt egali");



	free(v);
	DistrArb(&arb2);
	DistrArb (&arb);
	printf ("\n");


	return 0;
}
