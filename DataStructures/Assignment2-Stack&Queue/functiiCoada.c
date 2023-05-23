/*-- functiiCoada.c -- elementele cozii sunt memorate folosind liste simplu inlantuite generice --*/
#include "TCoada.h"

/* creeaza coada vida cu elemente de dimensiune d */
void* InitQ(size_t d)
{ 
	AQ c = (AQ)malloc(sizeof(TCoada));
  	if (!c) 
		exit(1);

 	c->dime = d;
  	c->ic = c->sc = NULL; 

  	return (void*)c;
}

// adauga element la sfarsitul cozii 
int IntrQ(void *c, void *ae)   
{ 
	ACel aux = (ACel)malloc(sizeof(TCel));
	if (!aux) 
		exit(1);
	
	aux->info = malloc(DIMEQ(c));
	if (!aux->info)
		exit(1);

	memcpy(aux->info, ae, DIMEQ(c));
	aux->urm = NULL;

	// coada vida
	if (VIDAQ(c))
		IC(c) = aux;
	else
		SC(c)->urm = aux;

	SC(c) = aux;
  	
  	return 1;
}

// extrage primul element din coada la adresa ae 
int ExtrQ (void *c, void *ae)  
{ 
	if (VIDAQ(c)) 
		return 0;
	
	ACel aux = IC(c);
	if (!aux)
		return 0;
	
	IC(c) = aux->urm;
	memcpy(ae, aux->info, DIMEQ(c));  

  	// coada vida 
	if (IC(c) == NULL)
		SC(c) = NULL;
	
  	return 1;
}

// test coada vida 
int VidaQ(void *a)         
{ 
	return VIDAQ(a); 
}

// numarul de elemente din coada
int NrEQ(void *a)       
{ 
	int contor = 0;
	ACel p = IC(a);
	while (p)
	{
		contor++;
		p = p->urm;
	}
	return contor; 
}

// dimensiunea elementelor din coada
size_t DimEQ(void *a)      
{ 
	return DIMEQ(a); 
}

// transforma coada in coada vida
void ResetQ(void *c)       
{ 
	ACel aux = IC(c), p;
	while (aux)
	{
		p = aux;
		aux = aux->urm;
		free(p->info);
		free(p);
	}
	IC(c) = SC(c) = NULL;
}  

// elibereaza intregul spatiu ocupat de coada
void DistrQ(void **c)     
{ 
	ResetQ(*c);
	free(*c);
	*c = NULL;
}

// prelucreaza elementele, folosind functia f;
// rezultat = numarul de elemente pentru care f != 0
size_t PrelQ(FILE *outputFile, void *c, TF1 f)   
{ 
	size_t k = 0;                  
	ACel p = IC(c);
  	for (; p != NULL; p = p->urm)
	{  
    	f(outputFile, p->info);
		if (p->urm != NULL)
			fprintf(outputFile, ", ");
		
		k++;		/* daca f intoarce rezultat != 0 -> contorizeaza */
	}
	return k;
}

// concatenare (muta toate elementele din sursa) 
int ConcatQ(void *ad, void *as) 
{ 
	if (DIMDIFQ(as, ad))	// dimensiuni diferite
		return 0;  		 
  	if (VIDAQ(as)) 	// sursa vida
		return 0;        	

	// coada destinatie vida
	if (VIDAQ(ad))
	{
		IC(ad) = IC(as);
		SC(ad) = SC(as);
		IC(as) = SC(as) = NULL;
		return 1;
	}

	SC(ad)->urm = IC(as);
	SC(ad) = SC(as);
	IC(as) = SC(as) = NULL;

  	return 1;
}

// functie de extragere al unui element din coada a
void* ExtrageQ(void *a, void *nume, TFCmp fcmp)
{
	void *el = malloc(DimEQ(a));
	if	(!el)
		exit(1);
	void *aux = NULL;
	int nr = NrEQ(a), i = 0;

	// parcurg coada in intregime, mutand fiecare element de
	// la inceputul la sfarsitul cozii, iar daca elementul e 
	// cel cautat il returnez si nu il mai introduc in coada
	while (i < nr)
	{
		ExtrQ(a, el);
		if (fcmp(el, nume) == 0)
		{
			void *serial = malloc(DimEQ(a));
			if (!serial)
				exit(1);

			memcpy(serial, el, DimEQ(a));
			aux = serial;
		}
		else
		{
			IntrQ(a, el);
		}
		i++;
	}
	
	free(el);
	return aux;
}

// functie de adaugare sezon intr-o coada; rezultat (0/1)
int AddSezQ(FILE *outputFile, void *a, void *nume, TFCmp fcmp, TFC fcitire)
{
	void *el = malloc(DimEQ(a));
	if (!el)
		exit(1);
	int nr = NrEQ(a), i = 0, check = 0;

	// parcurg coada in intregime, mutand fiecare element de
	// la inceputul la sfarsitul cozii, iar daca elementul e 
	// cel cautat ii adaug noul sezon
	while (i < nr)
	{
		ExtrQ(a, el);
		if (fcmp(el, nume) == 0)
		{
			fcitire(el);
			check = 1;
			fprintf(outputFile, "Serialul %s are un sezon nou.\n", 
				(char*)nume);
		}
		IntrQ(a, el);
		i++;
	}

	return check;
}

// afiseaza coada
void AfiQ(FILE *outputFile, TF1 AfiElem, void *a)
{
	fprintf(outputFile, "[");
    PrelQ(outputFile, a, AfiElem);       	
  	fprintf(outputFile, "].\n");
}
