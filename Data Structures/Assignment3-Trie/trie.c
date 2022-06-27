/* Filisanu Mihai-Alexandru -- 314CB */
/*--- trie.c ---*/
#include "trie.h"

// returneaza indexul fiecarui caracter primit conform "alfabetului" indicat
int indexAlfabet(char aux)
{
	if (aux >= 'a' && aux <= 'z')
		return aux - 'a';

	if (aux >= 'A' && aux <= 'Z')
		return aux - 'A' + 27;

	if (aux >= '0' && aux <= '9')
		return aux - '0' + 54;
	
	if (aux == '.')
		return 64;
	if (aux == '-')
		return 65;
	if (aux == '\'')
		return 66;
	if (aux == '?')
		return 67;
	if (aux == '!')
		return 68;
	if (aux == ' ')
		return 69;

	return -1;
}

// creeaza un nod pentru arbore si ii returneaza adresa
TArb ConstrNod()
{
	TArb aux = (TArb)malloc(sizeof(TNod));
	if (!aux)
		exit(1);
	
	aux->info = NULL;
	for (int i = 0; i < ALPHABET_SIZE; i++)
		aux->fiu[i] = NULL;

	return aux;
}

// inserare informatie intr-un arbore T1
void InserareT1(TArb aa, char *cheie, void *info)
{
    while (*cheie)
    {
		// daca nodul nu exista -> il creez
		if (aa->fiu[indexAlfabet(*cheie)] == NULL)
			aa->fiu[indexAlfabet(*cheie)] = ConstrNod();
		
		// avansare
		aa = aa->fiu[indexAlfabet(*cheie)];
		cheie++;
    }
    // inserare 
    aa->info = info;
}

// inserare informatie intr-un arbore T2
void InserareT2(TArb aa, char *cheie1, char *cheie2, void *info)
{
    while (*cheie1)
    {
		// daca nodul nu exista -> il creez
		if (aa->fiu[indexAlfabet(*cheie1)] == NULL)
			aa->fiu[indexAlfabet(*cheie1)] = ConstrNod();
		
		// avansare
		aa = aa->fiu[indexAlfabet(*cheie1)];
		cheie1++;
    }
    // daca nodul nu exista in frunza arborelui T2 -> il creez
	if (aa->info == NULL)
		aa->info = ConstrNod();
	
	// inserare (inceputul unui arbore T1, cu cheia specificata)
	InserareT1(aa->info, cheie2, info); 
}

// returneaza informatia aflata in frunza arborelui dupa parcurgerea cheii 
// acestuia (structura TCarte pt arbore T1 si arbore T1 pt arbore T2)
void* CautaInfo(TArb aa, char *cheie)
{
	if (aa == NULL)
		return NULL;
	
	// parcurg cheia pana la ultimul nod
	while (*cheie)
	{
		aa = aa->fiu[indexAlfabet(*cheie)];
		if (aa == NULL)
			return NULL;

		cheie++;	
	}

	return aa->info;
}

// parcurg recursiv arborele ramas si afisez primele 3 informatii cerute
void AfisareInfo3(FILE *outputFile, TArb aa, int *numara, TAfi afi)
{
	// daca exista informatie in nod -> frunza -> afisez
	if (aa->info && *numara < 3)
	{
		afi(outputFile, aa->info);
		(*numara)++;
	}

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (aa->fiu[i])
			AfisareInfo3(outputFile, aa->fiu[i], numara, afi);
	}
}

// afiseaza toate informatia dintr-un arbore, specificata prin functia TAfi
void AfisareInfoTot(FILE *outputFile, TArb aa, TAfi afi)
{
	// daca exista informatie in nod -> frunza
	if (aa->info)
		afi(outputFile, aa->info);
	
	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		// parcurg recursiv pana la frunza
		if (aa->fiu[i])
			AfisareInfoTot(outputFile, aa->fiu[i], afi);
	}
}

// parcurg recursiv un arbore intreg si afisez primele 3 informatii cerute fara sa se repete
void AfisareInfoDif3(FILE *outputFile, TArb aa, int *numara, int *verifica, TAfi afi)
{
	// daca exista informatie in nod -> frunza -> afisez
	// am grija sa nu afisez de 2 ori aceeasi informatie prin contorul verifica
	if (aa->info && *numara < 3 && *verifica < 1)
	{
		afi(outputFile, aa->info);
		(*numara)++;
		(*verifica)++;
	}

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (aa->fiu[i])
			AfisareInfoDif3(outputFile, aa->fiu[i], numara, verifica, afi);
	}
}

// parcurg arborele ramas pana la arborele aflat in frunza
void ContinuarePrefix(FILE *outputFile, TArb aa, int *numara, TAfi afi)
{
	// daca este informatie in nod -> frunza -> parcurg arborele pentru informatie
	if (aa->info && *numara < 3)
	{
		int verifica = 0;
		AfisareInfoDif3(outputFile, aa->info, numara, &verifica, afi);
	}

	for (int i = 0; i < ALPHABET_SIZE; i++)
	{
		if (aa->fiu[i])
			ContinuarePrefix(outputFile, aa->fiu[i], numara, afi);
	}
}
