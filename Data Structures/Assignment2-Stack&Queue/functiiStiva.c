/*-- functiiStiva.c --  elementele cozii sunt memorate folosind liste simplu inlantuite generice --*/
#include "TStiva.h"

// creeaza stiva vida cu elemente de dimensiune d 
void *InitS(size_t d)      
{ 
	ASt s = (ASt)malloc(sizeof(TStiva));
  	if (!s) 
		exit(1);

  	s->dime = d;  
  	s->vf = NULL; 

  	return (void*)s;
}

// pune element in varful stivei
int Push(void *s, void *ae)     
{ 
	ACelSt aux = (ACelSt)malloc(sizeof(TCelSt));
	if (!aux) 
		return 0;
  	
	aux->info = malloc(DIMES(s));
	if (!aux->info)
		exit(1);

	memcpy(aux->info, ae, DIMES(s));
	aux->urm = VF(s);
	VF(s) = aux;

  	return 1;
}

// extrage elementul din varful stivei la adresa ae
int Pop(void *s, void *ae)      
{ 
	if (VIDAS(s))
		return 0;
	
	ACelSt aux = VF(s);
	if (!aux) 
		return 0;
	
	memcpy(ae, aux->info, DIMES(s));
	VF(s) = aux->urm;
	free(aux);

	return 1;
}

// test stiva vida 
int VidaS(void *a)               
{ 
	return VF(a) == NULL; 
}

// numarul de elemente din stiva
int NrES(void *a)          
{ 
	int contor = 0;
	ACelSt p = VF(a);
	while (p)
	{
		contor++;
		p = p->urm;
	}
	return contor; 
}

// dimensiunea elementelor din stiva
size_t DimES(void *a)           
{ 
	return DIMES(a); 
}

// transforma stiva in stiva vida
void ResetS(void *s)          
{ 
	ACelSt aux = VF(s), p;
	while (aux)
	{
		p = aux;
		aux = aux->urm;
		free(p->info);
		free(p);
	}
	VF(s) = NULL; 
}

// elibereaza intregul spatiu ocupat de stiva
void DistrS(void **s)           
{ 
	ResetS(*s);
	free(*s);
	*s = NULL;
}

// prelucreaza elementele, folosind functia f;
// rezultat = numarul de elemente pentru care f != 0
size_t PrelS(FILE *outputFile, void *s, TF1 f)  
{ 
	size_t k = 0;
	ACelSt p = VF(s);
	for (; p != NULL; p = p->urm)
	{
		f(outputFile, p->info);
		if (p->urm != NULL)
			fprintf(outputFile, ", ");

		k++;		/* daca f intoarce rezultat != 0 -> contorizeaza */
	}
  	return k;
}

/* muta stiva sursa peste destinatie */
int Suprapune(void *ad, void *as)   
{ 
  	if (DIMDIFS(as,ad))		/* dimensiuni diferite */ 
		return 0;      		
  	if (VIDAS(as)) 			/* sursa vida */
		return 0;			
  	
	ACelSt u = VF(as);
	while (u)
		u = u->urm;
	
	u->urm = VF(ad);
	VF(ad) = VF(as);
	VF(as) = NULL;

	return 1;
}

// suprapune inversul sursei destinatie
int Rastoarna(void *ad, void *as)  
{ 
  	if (DIMDIFS(as, ad)) 
		return 0;      	/* dimensiuni diferite */
  	if (VIDAS(as)) 
		return 0;           /* sursa vida */
	
	ACelSt p = VF(as), u;
	while (p)
	{
		u = p->urm;
		p->urm = VF(ad);
		VF(ad) = p;
		p = u;
	}
	VF(as) = NULL;
	return 1;
}

// functie de extragere al unui element din stiva a
void* ExtrageS(void *a, void *nume, TFCmp fcmp)
{
	void *el = malloc(DimES(a));
	if (!el)
		exit(1);

	void *aux = NULL;
	void *saux = InitS(DimES(a));
	if (!saux)
		exit(1);

	// parcurg stiva in intregime, mutand fiecare element 
	// intr-o stiva auxiliara, iar daca elementul e cel cautat
	// il returnez si nu-l mai intoduc in stiva auxiliara
	while (Pop(a, el))
	{
		if (fcmp(el, nume) == 0)
		{
			void *serial = malloc(DimES(a));
			if (!serial)
				exit(1);

			memcpy(serial, el, DimES(a));
			aux = serial;
		}
		else
		{
			Push(saux, el);
		}
	}

	// rastorn stiva auxiliara in cea initiala pentru a pastra
	// ordinea initiala a elementelor
	Rastoarna(a, saux);
	free(el);
	DistrS(&saux);
	return aux;
}

// functie de adaugare sezon intr-o stiva; rezultat (0/1)
int AddSezS(FILE *outputFile, void *a, void *nume, TFCmp fcmp, TFC fcitire)
{
	void *el = malloc(DimES(a));
	if (!el)
		exit(1);

	void *saux = InitS(DimES(a));
	if (!el)
		exit(1);

	int check = 0;

	// parcurg stiva in intregime, mutand fiecare element 
	// intr-o stiva auxiliara, iar daca elementul e cel cautat
	// ii adaug noul sezon
	while (Pop(a, el))
	{
		if (fcmp(el, nume) == 0)
		{	
			fcitire(el);
			check = 1;
			fprintf(outputFile, "Serialul %s are un sezon nou.\n", 
				(char*)nume);
		}	
		Push(saux, el);
	}

	// rastorn stiva auxiliara in cea initiala pentru a pastra
	// ordinea initiala a elementelor
	Rastoarna(a, saux);
	free(el);
	DistrS(&saux);

	return check;
}

// afiseaza stiva 
void AfiS(FILE *outputFile, TF1 AfiElem, void *a)
{
	fprintf (outputFile, "[");          
	PrelS(outputFile, a, AfiElem);       
  	fprintf(outputFile, "].\n");
}