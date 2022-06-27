/*-- testHash.c  */
#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>


typedef struct
{
	char *nume;
	char *prenume;
	float nota;
	int varsta;
} TPers;


int codHash(void *element)
{
	TPers *pers = (TPers*)element;
	char *nume = pers->nume;
	return *nume - 'A';
}


void afisarePers(void *element)
{
	TPers *pers = (TPers*)element;
	printf("%s - %s - %.1lf - %d\n", pers->nume, pers->prenume, pers->nota, pers->varsta);
}


TLG citesteListaPersoane(char *numeFisier) {
	FILE *f;
	size_t len = 0;
	TLG L = NULL;
	char * line = malloc(200);

	f = fopen(numeFisier, "rt");
	if (f == NULL)
		return NULL;

	while (getline(&line, &len, f) != -1) {
		char *nume = strtok(line, ":");
		char *prenume = strtok(NULL, ":");
		float nota = atof(strtok(NULL, ":"));
		int varsta = atoi(strtok(NULL, ":"));


		TPers *pers = malloc(sizeof(TPers));
		if (pers == NULL)
			return L;

		pers->nume = malloc(100);
		pers->prenume = malloc(100);
		
		strcpy(pers->nume, nume);
		strcpy(pers->prenume, prenume);
		pers->nota = nota;
		pers->varsta = varsta;
		
		InsLG(&L, (void *) pers);
    }
	fclose(f);

	return L;
}


// 0 - ac pers, 1 - pers1 trb inserata inainte de pers2, -1 - dupa 
int cmpPers(void *e1, void *e2)
{
	TPers *pers1 = (TPers*)e1;
	TPers *pers2 = (TPers*)e2;

	// daca e ac persoana return 0
	if (strcmp(pers1->nume, pers2->nume) == 0 && strcmp(pers1->prenume, pers2->prenume) == 0)
		return 0;

	// daca cele 2 pers au ac nota, dar varste dif, 1 cand 
	// varsta1 > varsta2, -1 in caz contrar 
	if (pers1->nota == pers2->nota)
	{
		if(pers1->varsta > pers2->varsta)
			return -1;
		else
			return 1;
	}

	// cand nota1 > nota2 return 1, -1 caz contrar
	if (pers1->nota > pers2->nota)
		return 1;
	else
		return -1;
}


TH *GenerareHash(TLG listaPers)
{
    TH *h = NULL;
	TLG p;
    TPers *pers;
	int rez;

	// calcul dimensiuni maxime pt tabela hash
	size_t M = ('Z'-'A');

	// initializare tabela hash
	h = (TH*)IniTH(M, codHash);
	if (h == NULL)
		return NULL;

	for (p = listaPers; p != NULL; p = p->urm) {
        pers = (TPers*)malloc(sizeof(TPers));
        if (pers == NULL)
			return h;

        memcpy(pers, p->info, sizeof(TPers));
		rez = InsTH(h, pers, cmpPers);

        if (!rez) {
			free(pers);
			return h;
		}
	}

	return h;
}


/**
 * Sa se modifice programul primit astfel incat se citesc date despre un numar de persoane dintr-un fisier text
 * si se construieste o tabela hash cu aceste persoane.
 * Functia de hash este data de prima litera din nume. (TODO: modifica functia codHash)
 *
 * Va trebui sa modificati urmatoarele functii:
 * 
 * 1. 3p: Citirea din fisier TODO: citesteListaCarti -> citesteListaPersoane, cmpCarte->cmpPersoana
 * cmpPersoane: DESCRESCATOR dupa note, iar la note egale CRESCATOR dupa varsta
 * 
 * 2. 4p: Inserarea in tabela hash, in lista corespunzatoare unui element se va face in ordine DESCRESCATOARE a notelor;
 * In locul functiei InsLG va trebui sa implementati o functie InsLGDesc, functie pe care o veti apela din InsTH
 * In cazul notelor egale, inserarea se va face in ordine CRESCATOARE a varstelor (4p)
 * TODO: GenerareHash
 * 
 * 3. 2p: Functia de cautare in tabela hash (2p) TODO: void *ExistaTH(TH *a, void *ae, TFCmp f)
 * 
 * 4. 1p: Functia de afisare tabela hash (1p) TODO: afisareCarte -> afisarePersoana
 */

void *ExistaTH(TH *a, void *ae, TFCmp f)
{
	int cod = a->fh(ae);
	TLG el;

	for (el = a->v[cod]; el != NULL; el = el->urm)
	{
		if (f(ae, el->info) == 0)
			return el->info;
	}

	return NULL;
}


int main()
{
	// Citeste o lista de persoane din fisier

	TLG listaPers = citesteListaPersoane("persoane.txt");
	if (listaPers == NULL) {
		printf("Lista nu a putut fi generata\n");
        return 0;
	}

	printf("=========== LISTA Pers ===========\n");
	Afisare(&listaPers, afisarePers);

	TH * h = NULL;
	h = GenerareHash(listaPers);
	if (h == NULL) {
		printf("Tabela hash nu a putut fi generata\n");
        return 0;
	}

	printf("\n\n=========== TABELA HASH ===========\n");
	AfiTH(h, afisarePers);

  	Distruge(&listaPers, free);
	DistrTH(&h, free);

	return 0;
}
