#include "heap.h"

#define NMAX 25
static char desen[20][81], cri[] = "xxx";   /* caractere reprezentare arbore, info */

THeap AlocaHeap(int nMax, TRel rel)
{
	THeap h = (THeap)malloc(sizeof(struct Heap));
	if(!h)
		return NULL;

	h->v = (int*)malloc(nMax * sizeof(int));
	if (!h->v) {
		free(h);
		return NULL;
	}

	h->nMax = nMax;
	h->ne = 0;
	h->rel = rel;

	return h;
}

void DistrugeHeap(THeap *ah)
{
	free((*ah)->v);
	free(*ah);
	*ah = NULL;
}

/**
 * Functie auxiliara pentru desenarea unui arbore cu radacina in centrul liniei
 * dif - distanta pe orizontala intre radacina si cei doi descendenti
 *	Exemple:  dif = 5	|	 dif = 2	|
 *			    xxx			   xxx
 *		     ___/ \___		   / \
 */
void AfiHeapAux(THeap h, int k, int centru, int linie, int dif)
{
	int s, d;

	desen[linie][0] = ':';

	if (dif <= 1) {	 /* spatiu de afisare insuficient -> ... */
		memset (desen[linie] + centru, '.', 3);
		return;
	}

	desen[linie][centru] = '|';	 /* legatura spre radacina */
	sprintf (cri, "%3i", h->v[k]);  /* converteste valoare info la 3 caractere */
	d = (cri[0] == ' ') ? 2 : 1;	/* decalaj fata de centru */
	memcpy(desen[linie + 1] + centru - d, cri, 3);  /* copiaza cele 3 caractere in desen */
	desen[linie + 1][0] = ':';
	desen[linie + 2][0] = ':';

	if ((s = k * 2 + 1) < h->ne) {
		desen[linie + 2][centru - 1] = '/';
		memset(desen[linie + 2] + centru - dif + 1, '_', dif - 2);
		AfiHeapAux(h, s, centru - dif, linie + 3, (dif + 1) / 2 );
	}

	if (s + 1 < h->ne) {
		desen[linie + 2][centru + 1] = '\\';
		memset (desen[linie + 2] + centru + 2, '_', dif - 2);
		AfiHeapAux(h, s + 1, centru + dif, linie + 3, (dif + 1) / 2);
	}
}

/**
 * Afiseaza arborele h in partea superioara a ecranului
 */
void AfiHeap(THeap h, int linie)
{
	int j;
	for (j = 0; j < 20; ++j)
		memset(desen[j], ' ', 79);

 	if (h->ne == 0) {
		printf("\t\t\t\t-=- Arbore VID -=-\n");
	} else {
		printf("\n");
		AfiHeapAux(h, 0, 40, 0, 19);	/* afiseaza incepand de la mijlocul primei linii de pe ecran */

		for (j = 0; j < 16 && desen[j][0] == ':'; ++j) {
			desen[j][79] = '\0';
			printf("%s\n", desen[j] + 1);
		}
	}

	printf("\n");
}

/**
 * Intoarce 1 daca @valParinte < @valCopil, 0 altfel
 */
int RelMinHeap(int valParinte, int valCopil)
{
	return valParinte < valCopil;
}

/**
 * Intoarce 1 daca @valParinte > @valCopil, 0 altfel
 */
int RelMaxHeap(int valParinte, int valCopil)
{
	return valParinte > valCopil;
}

/**
 * Intoarce indicele parintelui pentru nodul @indCopil sau NodInex daca parintele nu exista
 */
int Parinte(THeap h, int indCopil)
{
	return indCopil > 0 ? (indCopil - 1) / 2 : NodInex;
}

/**
 * Intoarce indicele fiului stanga al nodului @indParinte sau NodInex daca fiul nu exista
 */
int FiuStanga(THeap h, int indParinte)
{
	int fiuSt = indParinte * 2 + 1;
	return fiuSt < h->ne ? fiuSt : NodInex;
}

/**
 * Intoarce indicele fiului dreapta al nodului @indParinte sau NodInex daca fiul nu exista
 */
int FiuDreapta(THeap h, int indParinte)
{
	int fiuDr = indParinte * 2 + 2;
	return fiuDr < h->ne ? fiuDr : NodInex;
}

/**
 * Insereaza valoarea @val in heapul @h
 */
void PushHeap(THeap h, int val)
{
	// TODO: problema 1, 2p
	h->v[h->ne] = val;
	h->ne++;

	int position = h->ne - 1;
	while (h->v[Parinte(h, position)] > h->v[position])
	{	
		int aux = h->v[Parinte(h, position)];
		h->v[Parinte(h, position)] = h->v[position];
		h->v[position] = aux;

		position = Parinte(h, position);
	}
}

/**
 * Functia intoarce valoarea nodului extras din heapul @h sau NodInex daca heapul este gol
 */
int PopHeap(THeap h)
{
	// TODO: problema 2, 3p
	int rez = h->v[0];
	h->v[0] = h->v[h->ne - 1];
	h->ne--;

	int position = 0;
	while (1)
	{
		int minim_index = 0;
		if (FiuStanga(h, position) != -1 && h->v[FiuStanga(h, position)] < h->v[position])
			minim_index = FiuStanga(h, position);
		
		if (FiuDreapta(h, position) != -1 && h->v[FiuDreapta(h, position)] < h->v[FiuStanga(h, position)])
			minim_index = FiuDreapta(h, position);

		if (minim_index)
		{
			int aux = h->v[minim_index];
			h->v[minim_index] = h->v[position];
			h->v[position] = aux;

			position = minim_index;		
		}
		else
			break;
	}

	return rez;
}

/**
 * Functia restabileste structura de tip Heap pentru @h fara a folosi memorie auxiliara
 * Nu se vor folosi operatii de PushHeap sau PopHeap
 */
void MakeHeap(THeap h)
{
	// TODO: problema 3, 3p
	for (int i = h->ne / 2; i >= 0; i--)
	{
		int position = i;
		while (1)
		{
			int minim_index = 0;
			if (FiuStanga(h, position) != -1 && h->v[FiuStanga(h, position)] < h->v[position])
				minim_index = FiuStanga(h, position);
			
			if (FiuDreapta(h, position) != -1 && h->v[FiuDreapta(h, position)] < h->v[FiuStanga(h, position)])
				minim_index = FiuDreapta(h, position);

			if (minim_index)
			{
				int aux = h->v[minim_index];
				h->v[minim_index] = h->v[position];
				h->v[position] = aux;

				position = minim_index;		
			}
			else
				break;
		}
	}
}	

/**
 * Sorteaza vectorul @v, de dimensiune @nv, folosind functia de comparatie @rel
 */
void HeapSort(int *v, int nv, TRel rel)
{
	// TODO: problema 4, 2p
	THeap h = AlocaHeap(NMAX, rel);
	if (!h)
		return;

	for (int i = 0; i < nv; ++i)
		PushHeap(h, v[i]);

	for (int i = 0; i < nv; ++i)
		v[i] = PopHeap(h);
}

/** 
 * Bonus: 2p, void PushHeap(THeap h, void *info) Inserare în heap generic (definit pe baza unui vector generic).
 * Se va folosi un vector cu elemente generice (se modifică functia de alocare si eliberare heap).
 */

int main()
{
	THeap h;
	int testv[] = { 8, 7, 9, 10, 6, 5, 11, 4, 12, 3, 13, 14, 2, 1, 15 },
		nv = sizeof(testv) / sizeof(testv[0]), i;

	h = AlocaHeap(NMAX, RelMinHeap);
	if (!h)
		return 1;

#if 0 // Demo structura min-heap
	int heapv[] = {2, 5, 3, 11, 9, 13, 8},
		nh = sizeof(heapv) / sizeof(heapv[0]);
	printf("Demo structura de tip min-heap:\n");
	for (i = 0; i < nh; ++i)
		h->v[h->ne++] = heapv[i];
	AfiHeap(h, 13);
	h->ne = 0;
#endif

#if 0 // Testeaza PushHeap
	for (i = 0; i < nv; ++i)
		PushHeap(h, testv[i]);
	printf("Heap dupa crearea folosind PushHeap:\n");
	AfiHeap(h, 13);
#endif

#if 0 // Testeaza PopHeap
	for (i = 0; i < nv; ++i)
	{
		printf("Pasul %d la PopHeap\n", i);
		PopHeap(h);
		AfiHeap(h,13);
	}
	printf("Heap vid dupa eliminarea folosind PopHeap:\n");
	AfiHeap(h, 13);
#endif

#if 0 // Testeaza MakeHeap
	for (i = 0; i < nv; ++i)
		h->v[h->ne++] = testv[i];
	AfiHeap(h, 13);
	MakeHeap(h);
	printf("Heap dupa operatia de MakeHeap:\n");
	AfiHeap(h, 13);
#endif

#if 1 // Testeaza HeapSort
	HeapSort(testv, nv, RelMinHeap);
	printf("Vector sortat crescator folosind HeapSort: ");
	for (i = 0; i < nv; ++i)
		printf("%d ", testv[i]);
	printf("\n");
#endif

//	DistrugeHeap(&h);

	return 0;
}
