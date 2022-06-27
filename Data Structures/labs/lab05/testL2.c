#include "lista2.h"

void AfiInt(void *ax)
{
	printf("%d ", *(int*)ax);
}

void CitireLDI(TLDI s, int *lg)
{
	int *ax, x;
 	char ch;

  	printf("se citesc valori numerice, terminate cu caracter nenumeric\n");
	for (*lg = 0; scanf("%i", &x) == 1; )
  	{
        ax = (int*)malloc(sizeof(int));
		if (!ax)
			return;
	 	*ax = x;
		InsDupa(s->pre, ax);
    	(*lg)++;
  	}
  	while ((ch=getchar()) != EOF && ch != '\n');
}

/**
 * Problema 1 - 5p
 * Sa se scrie o functie care primeste o lista dublu inlantuita circulara cu santilena.
 * Sa se verifice ca lista este formata din 2 jumatati egale (elementele egale pot fi pe pozitii diferite).
 * Returneaza 1 daca da, 0 daca nu.
 * Daca lista are numar impar de elemente, elementul de la mijloc nu se ia in calcul la verificare.
 * Ex: 1 2 3 2 1 3 => da,       Ex: 1 2 3 1 2 3 => da,    Ex: 1 2 3 2 4 1 => nu
 */

int JumatatiIdentice(TLDI s)
{
	int count = 0;
	TLDI bck, fwd;
	for (TLDI p = s->urm; p != s; p = p->urm)
		count++;
	
	TLDI h1 = s->urm;
	for (int i = 0; i < count / 2; i++)
	{
		TLDI h2 = s->pre;
		int check = 0;
		for (int j = 0; j < count / 2; j++)
		{
			if(*(int*)h1->info == *(int*)h2->info)
				check = 1;

			h2 = h2->pre;
		}
		if (check == 0)
			return 0;
	
		h1 = h1->urm;
	}

	return 1;
}

/**
 * Problema 2 - 5p
 * Se defineste o structura TPers { char *nume; float nota; int varsta; }
 * a). Se construieste o lista dublu inlantuita circulara cu elemente de tip TPers
 *     (numele, notele si varstele se citesc de la tastatura).
 * b). Sa se scrie o functie care primeste ca parametru o lista dublu inlantuita circulara si construieste o noua lista
 * prin copierea elementelor consecutive cu note diferite.
 */

typedef struct pers
{
	char *nume;
	float nota;
	int varsta;
} TPers;


void afisarePers(void *element)
{
	TPers *pers = (TPers*)element;
	printf("%s - %.1lf - %d\n", pers->nume, pers->nota, pers->varsta);
}


TLDI CitirePersoane() 
{
	// TODO: Create lista dublu inlantuita de persoane citite de la tastatura (sau fisier)
	FILE *f = fopen("persoane.txt", "rt");
	if (f == NULL)
		return NULL;
	
	size_t len = 0;
	TLDI s = InitLDI();
	char *line = malloc(200);

	while (getline(&line, &len, f) != -1) {
		char *nume = strtok(line, ":");
		float nota = atof(strtok(NULL, ":"));
		int varsta = atoi(strtok(NULL, ":"));

		TPers *pers = malloc(sizeof(TPers));
		if (pers == NULL)
			return s;

		pers->nume = malloc(100);
		
		strcpy(pers->nume, nume);
		pers->nota = nota;
		pers->varsta = varsta;
		
		InsDupa(s, (void*)pers);
    }
	fclose(f);

	return s;
}


TLDI CopiazaConsecutiveDiferite(TLDI s)
{
	// TODO: Creaza o noua lista copiind celulele consecutive cu note diferite
	
	TLDI noualista = InitLDI();
	TLDI p;
	
	for (p = s->urm; p->urm != s; p = p->urm)
	{
		TPers *pers = (TPers*)p->info;
		TPers *pers_urm = (TPers*)p->urm->info;
		
		if (pers->nota != pers_urm->nota)
			InsDupa(noualista->pre, p->info);
	}

	// se verifica daca ult persoana are ac nota cu prima dupa santinela
	if (((TPers*)p->info)->nota != ((TPers*)s->urm->info)->nota)
		InsDupa(noualista->pre, p->info); 

	return noualista;
}


/**
 * Bonus - 2p
 * Să se scrie o funcție care sortează crescator in-place (fără ajutorul vreunei alte liste suplimentare)
 * o listă dublu înlănțuită circulară cu santinelă.
 */
void Sorteaza(TLDI s)
{
	TLDI A, B;
    for(A = s->urm; A != s; A = A->urm)
    {
        for(B = A->urm; B != s; B = B->urm)
        {
            if(*(int*)B->info < *(int*)A->info)
            {
                int aux = *(int*)A->info;
                *(int*)A->info = *(int*)B->info;
                *(int*)B->info = aux;
            }
        }
    }
}

int main()
{
	TLDI s;
	int lg;

	for (;;)
	{
		s = InitLDI();
		CitireLDI(s, &lg);
		AfisareLDI(s, AfiInt);

		/* apeluri functii */
		
		// Problema 1
		printf("%d\n", JumatatiIdentice(s));

		// Problema 2
		TLDI lista1 = CitirePersoane();
		printf("Lista pb 2, a:\n");
		AfisareLDI(lista1, afisarePers);

		TLDI lista2 = CopiazaConsecutiveDiferite(lista1);
		printf("Lista pb 2, b:\n");
		AfisareLDI(lista2, afisarePers);

		// Problema Bonus
		Sorteaza(s);
		printf("Bonus:\n");
		AfisareLDI(s, AfiInt);

		DistrLDI(&s);
		DistrLDI(&lista1);
		//DistrLDI(&lista2);

		printf ("\n  Inca un test ? [d/n]");
		if (getchar() == 'n') 
			break;
	}

    return 0;
}
