#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"

/* numar maxim elemente coada / stiva */
#define MAX 256

int fConditie(void *a) {
	// TODO: returneaza 1 daca pct.x == pct.y
	return 0;
}

int GenereazaCoada(void *c, size_t N, int v1, int v2)
{
  	int n = 0;

  	srand(time(NULL));
  	for (; N > 0; N--)
  	{
    	int value = v1 + rand() % (v2-v1 + 1);

		if (!IntrQ(c, &value))
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

int AfiInt(void *a)
{
	printf("%d ", *(int*)a);
	return 1;
}

/**
 * Infix Expression	        Prefix Expression        Postfix Expression
 *  A + B * C + D            + + A * B C D            A B C * + D +
 *  (A + B) * (C + D)        * + A B + C D            A B + C D + *
 *  A * B + C * D            + * A B * C D            A B * C D * +
 *  A + B + C + D            + + + A B C D            A B + C + D +
 */

/**
 * Problema 1 - 5p
 * Sa se scrie o functie care primeste ca parametru un string reprezentand o expresie aritmetica in forma postfixata
 * si realizeaza evaluarea expresiei.
 */
float evaluare_postfix(char *postfix)
{
	// Se vor folosi implementarile de stive si cozi din scheletul de cod - nu alte variante.

	void *s = InitS(sizeof(float), MAX);
	if (!s)
	{
		DistrS(&s);
		return 0.0;
	}

	int i = 0;
	float *element = malloc(sizeof(float));
	if (!element)
	{
		free(element);
		return 0.0;
	}
	float *element1 = malloc(sizeof(float));
	if (!element1)
	{
		free(element1);
		return 0.0;
	}
	float *element2 = malloc(sizeof(float));
	if (!element2)
	{
		free(element2);
		return 0.0;
	}

	for (; postfix[i] != '\0'; i++)
	{
		if (postfix[i] >= '0' && postfix[i] <= '9')
		{
			float number = postfix[i] - '0';
			Push(s, (void*)&number);
		}
		else
		{
			if (postfix[i] == '+')
			{
				Pop(s, element1);
				Pop(s, element2);
				*element = *(float*)element1 + *(float*)element2;
				Push(s, (void*)element);
			}
			if (postfix[i] == '-')
			{
				Pop(s, element1);
				Pop(s, element2);
				*element = *(float*)element2 - *(float*)element1;
				Push(s, (void*)element);
			}
			if (postfix[i] == '*')
			{
				Pop(s, element1);
				Pop(s, element2);
				*element = *(float*)element1 * *(float*)element2;
				Push(s, (void*)element);
			}
			if (postfix[i] == '/')
			{
				Pop(s, element1);
				Pop(s, element2);
				*element = *(float*)element2 / *(float*)element1;
				Push(s, (void*)element);
			}
		}
	}

	Pop(s, element);
	float rez = *element;

	free(element);
	free(element1);
	free(element2);
	DistrS(&s);

	return rez;
}

/**
 * Problema 2 - 5p
 * Sa se sorteze crescator coada c fara a utiliza spatiu suplimentar.
 * Nu aveti voie sa folositi alta structura de date auxiliara.
 */
void sortare_coada(void *c)
{
	// Se vor folosi implementarile de stive si cozi din scheletul de cod - nu alte variante.

	int len = NrEQ(c), i, j, k;
	int *element1 = malloc(sizeof(float));
	if (!element1)
	{
		free(element1);
		return;
	}
	int *element2 = malloc(sizeof(float));
	if (!element2)
	{
		free(element2);
		return;
	}
	
	for (i = 0; i < len; i++)
	{
		ExtrQ(c, element1);
		int min = *(int*)element1;
		
		for (j = 1; j < len - i; j++)
		{
			ExtrQ(c, element2);
			if (*(int*)element2 < min)
			{
				min = *(int*)element2;
				IntrQ(c, element1);
			}
			else
				IntrQ(c, element2);
		}

		for (k = len - i; k < len; k++)
		{
			ExtrQ(c, element1);
			IntrQ(c, element1);	
		}

		IntrQ(c, (void*)&min);
	}

	free(element1);
	free(element2);
}

int main()
{

#if 0
	// 2+3*4-1 -> 234*+1-
	float ret = evaluare_postfix("234*+1-");
	if (ret != 13) {
		printf("Test 1 failed: %f != 13\n", ret);
	} else {
		printf("Test 1 succeded\n");
	}

	// (1+2)*(5-2) -> 12+52-*
	ret = evaluare_postfix("12+52-*");
	if (ret != 9) {
		printf("Test 2 failed: %f != 9\n", ret);
	} else {
		printf("Test 2 succeded\n");
	}

	// (2*3)+(3*4) -> 23*34*+
	ret = evaluare_postfix("23*34*+");
	if (ret != 18) {
		printf("Test 3 failed: %f != 18\n", ret);
	} else {
		printf("Test 3 succeded\n");
	}

	// 2+6/3+5*2 -> 263/+52*+
	ret = evaluare_postfix("263/+52*+");
	if (ret != 14) {
		printf("Test 4 failed: %f != 14\n", ret);
	} else {
		printf("Test 4 succeded\n");
	}
#else
	void *c;
  	int lgc;

  	c = InitQ(sizeof(int), MAX);
	if (!c) {
		DistrQ(&c);
		return 1;
	}

  	lgc = GenereazaCoada(c, 8, -5, 5);

	printf("\nCoada cu %d elemente, \n", lgc);
  	AfiQ("Coada initiala: ", AfiInt, c);
	sortare_coada(c);
	AfiQ("Coada dupa sortare: ", AfiInt, c);

  	DistrQ(&c);
#endif

  	return 0;
}
