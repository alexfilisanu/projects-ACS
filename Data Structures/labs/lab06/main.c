#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"

/* numar maxim elemente coada / stiva */
#define MAX 256

typedef struct
{
	int x, y;
} TPct;

int fConditie(void *a) {
	// TODO: returneaza 1 daca pct.x == pct.y
	if ((*(TPct*)a).x == (*(TPct*)a).y)
		return 1;
	return 0;
}

int GenereazaCoadaPct(void *c, size_t N, int v1, int v2)
{
  	int n = 0;
  	TPct p;

  	srand(time(NULL));
  	for (; N > 0; N--)
  	{
    	p.x = rand() % (v2-v1+1) + v1;
    	p.y = rand() % (v2-v1+1) + v1;

		if (!IntrQ(c, &p))
			return n;
		n++;
  	}

  	return n;
}

int GenereazaStivaPct(void *s, size_t N, int v1, int v2)
{
	int n = 0;
  	TPct p;

  	srand(time(NULL));
  	for (; N > 0; N--)
  	{
    	p.x = rand() % (v2-v1+1) + v1;
    	p.y = rand() % (v2-v1+1) + v1;

		if (!Push(s, &p))
			return n;
		n++;
  	}

  	return n;
}

/* afiseaza coada */
void AfiQ(char antet[], TF1 AfiElem, void *a)
{
	/* afiseaza antet si apoi aplica functia de afisare AfiElem tuturor elementelor */
	printf ("%s", antet);          	/* afiseaza antetul */
  	if (VidaQ(a))
    	printf ("Coada vida!");
  	else
    	PrelQ(a, AfiElem);       	/* afiseaza elementele in ordinea introducerii */
  	printf("\n");
}

/* afiseaza stiva */
void AfiS(char antet[], TF1 AfiElem, void *a)
{
	/* afiseaza antet si apoi aplica functia de afisare AfiElem tuturor elementelor */
	printf ("%s", antet);          /* afiseaza antetul */
	if (VidaS(a))
    	printf("Stiva vida!");
  	else
      	Prel_B_V(a, AfiElem);       /* afiseaza elementele in ordinea introducerii */
  	printf("\n");
}

int AfiPct(void *a)
{
	printf("[%d %d] ", (*(TPct*)a).x, (*(TPct*)a).y);
	return 1;
}

/**
 * Problema FC1 - 2p
 * Functia primeste o structura COADA de tip si determina procentul de elemente care îndeplinesc o anumita condiție
 * (condiție specificata pe baza unei funcții condiție primita ca parametru)
 * pentru prelucrarea unui element, acesta va fi scos din COADA
 * funcția va reface COADA inițială
 */
float FC1(void *c, TF1 fConditie)
{
	// TODO
	void *c_aux = InitQ(DimEQ(c), MAX); // initializare coada aux
	if (!c_aux)
	{
		DistrQ(&c_aux);
		return 0;
	}
	void *el = malloc(DimEQ(c)); // in el se vor extrage pe rand elemente
	if (!el)
	{
		return 0;
	}
	int nr_fav = 0, nr_pos = 0; // conotoare; la final return nr_fav/nr_pos

	while (ExtrQ(c, el)) // cat timp se extrag elemente
	{
		nr_pos++;
		if (fConditie(el))
			nr_fav++;

		IntrQ(c_aux, el); // adaug elementul in coada aux		
	}

	ConcatQ(c, c_aux); // recreez coada initiala
	
	// eliberare memorie
	DistrQ(&c_aux);
	free(el);

	return ((float)nr_fav) / nr_pos;
	
}

/**
 * Problema FS1 - 2p
 * Functia primeste o structura STIVA de tip si determina procentul de elemente care îndeplinesc o anumita condiție
 * (condiție specificata pe baza unei funcții condiție primita ca parametru)
 * pentru prelucrarea unui element, acesta va fi scos din STIVA
 * funcția va reface STIVA inițială
 */
float FS1(void *s, TF1 fConditie)
{
	// TODO
	void *s_aux = InitS(DimES(s), MAX); // initializare stiva aux
	if (!s_aux)
	{
		DistrS(&s_aux);
		return 0;
	}
	void *el = malloc(DimES(s)); // in el se vor extrage pe rand elemente
	if (!el)
	{
		return 0;
	}
	int nr_fav = 0, nr_pos = 0; // conotoare; la final return nr_fav/nr_pos

	while (Pop(s, el)) // cat timp se extrag elemente
	{
		nr_pos++;
		if (fConditie(el))
			nr_fav++;

		Push(s_aux, el); // adaug elementul in coada aux		
	}

	Rastoarna(s, s_aux); // recreez stiva initiala
	
	// eliberare memorie
	DistrQ(&s_aux);
	free(el);

	return ((float)nr_fav) / nr_pos;

}

/**
 * Problema FC2 - 3p
 * Funcția primeste ca parametru o COADA si elimina elementele care îndeplinesc o anumita condiție
 * (condiție specificata pe baza unei funcții condiție primita ca parametru)
 * pentru prelucrarea unui element, acesta va fi scos din COADA
 * structurile inițiale vor fi refăcute, eliminând elementele care îndeplinesc condiția,
 * dar păstrând ordinea relativă a celorlalte elemente
 */
void FC2(void *c, TF1 fConditie)
{
	// TODO
	void *c_aux = InitQ(DimEQ(c), MAX); // initializare coada aux
	if (!c_aux)
	{
		DistrQ(&c_aux);
		return;
	}
	void *el = malloc(DimEQ(c)); // in el se vor extrage pe rand elemente
	if (!el)
	{
		return;
	}

	while (ExtrQ(c, el)) // cat timp se extrag elemente
	{
		if (!fConditie(el)) // daca conditia nu e respectata
			IntrQ(c_aux, el); // adaug elementul in coada aux
	}

	ConcatQ(c, c_aux); // recreez coada initiala
	
	// eliberare memorie
	DistrQ(&c_aux);
	free(el);

}

/**
 * Problema FS2 - 3p
 * Funcția primeste ca parametru o STIVA si elimina elementele care îndeplinesc o anumita condiție
 * (condiție specificata pe baza unei funcții condiție primita ca parametru)
 * pentru prelucrarea unui element, acesta va fi scos din STIVA
 * structurile inițiale vor fi refăcute, eliminând elementele care îndeplinesc condiția,
 * dar păstrând ordinea relativă a celorlalte elemente
 */
void FS2(void *s, TF1 fConditie)
{
	// TODO
	void *s_aux = InitS(DimES(s), MAX); // initializare stiva aux
	if (!s_aux)
	{
		DistrS(&s_aux);
		return;
	}
	void *el = malloc(DimEQ(s)); // in el se vor extrage pe rand elemente
	if (!el)
	{
		return;
	}

	while (Pop(s, el)) // cat timp se extrag elemente
	{
		if (!fConditie(el)) // daca conditia nu e respectata
			Push(s_aux, el); // adaug elementul in coada aux		
	}

	Rastoarna(s, s_aux); // recreez stiva initiala
	
	// eliberare memorie
	DistrQ(&s_aux);
	free(el);

}

/**
 * Bonus FC3 - 1p
 * Functia primeste ca parametru o COADA si o va împarți in doua structuri de același tip,
 * in funcție de o anumita condiție specificata pe baza unei funcții condiție primita ca parametru:
 * prima structura va conține elementele care respecta condiția, in structura a doua, restul elementelor.
 * pentru prelucrarea unui element, acesta va fi scos din COADA
 * structurile rezultate vor păstra ordinea relativă a elementelor
 */
void FC3(void *c, void *c1, void *c2, TF1 fConditie)
{
	// TODO
	void *el = malloc(DimEQ(c)); // in el se vor extrage pe rand elemente
	if (!el)
	{
		return;
	}

	while (ExtrQ(c, el)) // cat timp se extrag elemente
	{
		if (!fConditie(el)) // daca conditia nu e respectata
			IntrQ(c2, el); // adaug elementul in coada2
		else
			IntrQ(c1, el);
	}
	
	// eliberare memorie
	free(el);
}

/**
 * Bonus FS3 - 1p
 * Functia primeste ca parametru o STIVA si o va împarți in doua structuri de același tip,
 * in funcție de o anumita condiție specificata pe baza unei funcții condiție primita ca parametru:
 * prima structura va conține elementele care respecta condiția, in structura a doua, restul elementelor.
 * pentru prelucrarea unui element, acesta va fi scos din STIVA
 * structurile rezultate vor păstra ordinea relativă a elementelor
 */
void FS3(void *s, void *s1, void *s2, TF1 fConditie)
{
	// TODO:
	void *el = malloc(DimEQ(s)); // in el se vor extrage pe rand elemente
	if (!el)
	{
		return;
	}

	while (Pop(s, el)) // cat timp se extrag elemente
	{
		if (!fConditie(el)) // daca conditia nu e respectata
			Push(s2, el); // adaug elementul in stiva2
		else
			Push(s1, el);
	}
	
	// inversez continutul stivelor
	InvS(s1);
	InvS(s2);

	// eliberare memorie
	free(el);
}

int main()
{
	void *c, *s;
  	int lgc, lgs;

  	c = InitQ(sizeof(TPct), MAX);
	if (!c)
	{
		DistrQ(&c);
		return 1;
	}

  	s = InitS(sizeof(TPct), MAX);
	if (!s)
	{
		DistrS(&s);
		return 1;
	}

  	lgc = GenereazaCoadaPct(c, 8, -5, 5);
  	lgs = GenereazaStivaPct(s, 6, 8, 14);

	printf("\nCoada cu %d puncte\n", lgc);
  	AfiQ("Elemente coada:\n", AfiPct, c);
  	printf("\n\nStiva cu %d puncte\n", lgs);
   	AfiS("Elemente stiva (baza -> varf):\n", AfiPct, s);

 	/* apeluri functii */

	// pb1, coada
	printf("\nPb1 coada: procentul: %lf", FC1(c, fConditie));
	AfiQ("\nPb1 coada initiala:\n", AfiPct, c);

	// pb1, stiva
	printf("\nPb1 stiva: procentul: %lf", FS1(s, fConditie));
	AfiS("\nPb1 coada initiala:\n", AfiPct, s);

	// pb2, coada
	//FC2(c, fConditie);
	//AfiQ("\nPb2 coada noua:\n", AfiPct, c);

	// pb2, stiva
	//FS2(s, fConditie);
	//AfiS("\nPb2 stiva noua:\n", AfiPct, s);

	// bonus, coada
	void *c_aux1 = InitQ(DimEQ(c), MAX); // initializare coada aux cu el care respecta cond
	if (!c_aux1)
	{
		DistrQ(&c_aux1);
		return 1;
	}
	void *c_aux0 = InitQ(DimEQ(c), MAX); // initializare coada aux cu el care nu repsecta cond
	if (!c_aux0)
	{
		DistrQ(&c_aux0);
		return 1;
	}
	FC3(c, c_aux1, c_aux0, fConditie);
	AfiQ("\nBonus coada1:\n", AfiPct, c_aux1);
	AfiQ("\nBonus coada0:\n", AfiPct, c_aux0);

	// bonus, stiva
	void *s_aux1 = InitS(DimES(s), MAX); // initializare stiva aux cu el care respecta cond
	if (!s_aux1)
	{
		DistrS(&s_aux1);
		return 1;
	}
	void *s_aux0 = InitS(DimES(s), MAX); // initializare stiva aux cu el care nu repsecta cond
	if (!s_aux0)
	{
		DistrS(&s_aux0);
		return 1;
	}
	FS3(s, s_aux1, s_aux0, fConditie);
	AfiS("\nBonus stiva1:\n", AfiPct, s_aux1);
	AfiS("\nBonus stiva0:\n", AfiPct, s_aux0);


	DistrQ(&c_aux0);
	DistrQ(&c_aux1);
	DistrS(&s_aux0);
	DistrS(&s_aux1);

	DistrS(&s);
  	DistrQ(&c);

  	return 0;
}
