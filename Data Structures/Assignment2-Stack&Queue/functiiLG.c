/*--- functiiLG.c -- operatii de baza pentru lista simplu inlantuita generica---*/
#include "tlg.h"

// inserare in lista in functie de rezultatul functiei de comparare; 
// returneaza pozitia la care s-a facut inserarea; nu a reusit reusit -> 0 
int InsLG(TLG* aL, void* ae, TFCmp fcmp)
{
	TLG aux = malloc(sizeof(TCelulaG));
	if (!aux)
	    exit(1);

	int pozitie = 1;
	aux->info = ae;
	
	// daca lista e vida sau elementul ce trebuie inserat e mai "mare" 
	// decat primul element al listei -> inserez la inceput
	if (!*aL || fcmp(ae, (*aL)->info) > 0)
	{ 
		aux->urm = *aL;
		*aL = aux;
		return 1;
	}
	// parcurg lista pentru a gasi ultimul element, dupa care inserez
	else
	{
		TLG p = *aL, ant = NULL;
		// caut ultimul element inaintea caruia sa inserez
		while (p->urm && (fcmp(p->info, ae) > 0))
		{
			ant = p;
			p = p->urm;
			pozitie++;
		}

		// daca elementul ce trebuie inserat e mai "mic" decat cel din
		// lista -> inserez dupa 
		if (fcmp(p->info, ae) < 0)
		{
			ant->urm = aux;
			aux->urm = p;
			return pozitie;
		}

		// sfarsit lista -> inserare la finalul listei
		aux->urm = p->urm;
		p->urm = aux; 
		return pozitie + 1;	
	}
	return 0;
}

// inserare la pozitia specificata; reusita (1/0) 
int InsTOP(TLG* aL, void* ae, int poz)
{
	TLG aux = (TLG)malloc(sizeof(TCelulaG));
	if (!aux)
		exit(1);
	
	aux->info = ae;
	
	// daca se insereaza pe pozitia 1
	if (poz == 1)
	{
		aux->urm = *aL;
		*aL = aux;
		
		// daca lista nu era vida
		if ((*aL)->urm)
		{
			// daca sunt mai mult de 10 aparitii in lista 
			// le pastrez pe primele 10
			TLG p = (*aL)->urm;
			int i = 2;

			while (p  && i < 10)
			{
				p = p->urm;
				i++;
			}
			if (i == 10 && p)
				p->urm = NULL;	
		}

		return 1;
	}
	else
	{
		TLG p = (*aL)->urm, ant = *aL;
		int i = 2;

		// caut pozitia pe care trebuie sa inserez
		for (; i < poz; i++)
		{
			ant = p;
			p = p->urm;
		}

		ant->urm = aux;
		aux->urm = p;

		ant = aux; // ant avanseaza pt ca s-a adaugat elementul 
	 	i++; // pozitia ++ pt ca s-a adaugat elementul cerut
		
		// daca elementul a fost adaugat pe pozitia 10 devine
		// sfarsitul listei
		if (i > 10 && p)
		{
			ant->urm = NULL;
			return 1;
		}

		// daca sunt mai mult de 10 aparitii in lista 
		// le pastrez pe primele 10 
		while (p && i < 10)
		{
			p = p->urm;
			i++;
		}
		if (i == 10 && p)
			p->urm = NULL;

		return 1;
	}
	return 0;
}

// extrage elementul cautat din lista data
void* ExtrageLG(TLG* aL, void* nume, TFCmp fcmp)
{
	TLG p, ant;
	for (p = *aL, ant = NULL; p != NULL; ant = p, p = p->urm)
	{
		if (fcmp(p->info, nume) == 0)
		{
			// daca extrag de la inceputul listei
			if (ant == NULL)
				*aL = p->urm;
			else
				ant->urm = p->urm;

			return p->info;
		}
	}
	return NULL;
}

// functie de adaugare sezon intr-o lista; rezultat (0/1)
int AddSezLG(FILE *outputFile, TLG aL, void *nume, TFCmp fcmp, TFC fcitire)
{
	TLG p;
	for (p = aL; p != NULL; p = p->urm)
	{
		if (fcmp(p->info, nume) == 0)
		{
			fcitire(p->info);
			fprintf(outputFile, "Serialul %s are un sezon nou.\n", 
				(char*)nume);
			return 1;
		}
	}

	return 0;
}

// functie de afisare a listei
void Afisare(FILE *outputFile, TLG* aL, TFA afiEL)
{
	fprintf(outputFile, "[");
	for (; *aL; aL = &(*aL)->urm) 
	{
		afiEL(outputFile, (*aL)->info);
		
		if ((*aL)->urm)
			fprintf(outputFile, ", ");
    }
	fprintf(outputFile, "].\n");
}

// distruge lista
void Distruge(TLG* aL, TF free_elem) 
{
	while (*aL != NULL)
    {
        TLG aux = *aL;     
        if (!aux)
            return;

        free_elem(aux->info);  /* elib.spatiul ocupat de element*/
        *aL = aux->urm;    /* deconecteaza celula din lista */
        free(aux);   /* elibereaza spatiul ocupat de celula */
    }
}

// -> numarul de elemente din lista
int LungimeLG(TLG* a) 
{
	int lg = 0;
	TLG p = *a;

	for (; p != NULL; p = p->urm)
        lg++;

	return lg;
}
