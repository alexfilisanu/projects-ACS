#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "TCoada.h"
#include "TStiva.h"
#include "tlg.h"


// structura pentru serial
typedef struct 
{
	char *nume;
	double rating;
	int nrSezoane;
	void *coada;
	int durata;
} TSerial;

// structura pentru sezon (serial = coada sezoane)
typedef struct 
{
	int nrEpisoade;
	void *coada;
} TSezon;

// structura pentru episod (sezon = coada episoade)
typedef struct 
{
	int durata;
} TEpisod;


// functie de comparare -> 1 daca prima structura e mai mare;
// -1 daca a 2a e mai mare;
int cmpRating(void *a, void *b)
{
	if ((*(TSerial*)a).rating > (*(TSerial*)b).rating)
		return 1;
	if ((*(TSerial*)a).rating == (*(TSerial*)b).rating)
		return ((-1) * strcmp((*(TSerial*)a).nume, (*(TSerial*)b).nume));
	
	return -1;
}

// 0 -> structuri serial identice
int cmpNume(void *a, void *nume)
{
	return strcmp((*(TSerial*)a).nume, (char*)nume);
}

// afisare serial
void AfiSerial(FILE *outputFile, void *a)
{
	fprintf(outputFile, "(%s, %.1lf)", (*(TSerial*)a).nume, 
		(*(TSerial*)a).rating);
}
	
// citire serial
void* CitireSerial()
{
	char *nume = strtok(NULL, " ");
	double rating = atof(strtok(NULL, " "));
	int nrsez = atoi(strtok(NULL, " "));
	int durata = 0;

	TSerial *serial = (TSerial*)malloc(sizeof(TSerial));
	if (!serial)
		exit(1);
			
	serial->nume = malloc(100);
	if (!serial->nume)
		exit(1);

	strcpy(serial->nume, nume);

	serial->rating = rating;
	serial->nrSezoane = nrsez;
	void *coadaSezon = InitQ(sizeof(TSezon));
	if (!coadaSezon)
		exit(1);

	serial->coada = coadaSezon;

	for (int i = 0; i < nrsez; i++)
	{
		int nrep = atoi(strtok(NULL, " "));
		TSezon *sezon = (TSezon*)malloc(sizeof(TSezon));
		if (!sezon)
			exit(1);

		sezon->nrEpisoade = nrep;
		void *coadaEpisod = InitQ(sizeof(TEpisod));
		if (!coadaEpisod)
			exit(1);
		
		sezon->coada = coadaEpisod;
				
		for (int j = 0; j < nrep; j++)
		{
			int timp = atoi(strtok(NULL, " "));
			TEpisod *episod = (TEpisod*)malloc(sizeof(TEpisod));
			if (!episod)
				exit(1);

			episod->durata = timp;
			durata += timp;
			IntrQ(coadaEpisod, episod);
		}

		IntrQ(coadaSezon, sezon);
	}

	serial->durata = durata;

	return serial;
}

// adauga un sezon serialului specificat
void CitireSezon(void* aux)
{
	TSerial *serial = (TSerial*)aux;
	int nrep = atoi(strtok(NULL, " "));
	TSezon *sezon = (TSezon*)malloc(sizeof(TSezon));
		if (!sezon)
			exit(1);

	sezon->nrEpisoade = nrep;
	void *coadaEpisod = InitQ(sizeof(TEpisod));
	if (!coadaEpisod)
		exit(1);

	sezon->coada = coadaEpisod;

	for (int j = 0; j < nrep; j++)
	{
		int timp = atoi(strtok(NULL, " "));
		TEpisod *episod = (TEpisod*)malloc(sizeof(TEpisod));
		if (!episod)
			exit(1);

		episod->durata = timp;
		serial->durata += timp;
		IntrQ(coadaEpisod, episod);
	}

	IntrQ(serial->coada, sezon);
}


int main(int argc, char *argv[])
{
	// verific ca nr de parametri sa fie cel cerut
	if (argc != 3)
		return 0;

	char *inputFileName = strdup(argv[1]);
	char *outputFileName = strdup(argv[2]);

	FILE *inputFile = fopen(inputFileName, "rt");
	if (!inputFile)
		exit(1);
	
	FILE *outputFile = fopen(outputFileName, "wt");
	if (!outputFile)
		exit(1);
	
	// cele 4 liste: 
	TLG lista1 = NULL;
	TLG lista2 = NULL;
	TLG lista3 = NULL;
	TLG listatop = NULL;

	// initiliazare coada si stive cerute
	void *coadaLater = InitQ(sizeof(TSerial));
	if (!coadaLater)
		exit(1);

	void *stivaWatch = InitS(sizeof(TSerial));
	if (!stivaWatch)
		exit(1);

	void *stivaHistory = InitS(sizeof(TSerial));
	if (!stivaHistory)
		exit(1);

	size_t len = 0;
	char *line  = malloc(200);
	if (!line)
		exit(1);

	// parcurg pe rand fiecare comanda
	while (getline(&line, &len, inputFile) != -1)
	{
		char *comanda = strtok(line, " ");

		if (comanda[strlen(comanda) - 1] == '\n')
			comanda[strlen(comanda) - 1] = '\0';
		if (comanda[strlen(comanda) - 1] == '\r')
			comanda[strlen(comanda) - 1] = '\0';


		if (strcmp(comanda, "add") == 0)
		{
			int id = atoi(strtok(NULL, " "));

			// creare structura de tip serial din datele citite
			TSerial *serial = CitireSerial();
			if (!serial)
				exit(1);
			
			// structura serial se insereaza in lista ceruta
			switch (id)
			{
				case 1:
					fprintf(outputFile, "Serialul %s a fost adaugat la pozitia %d.\n", 
						serial->nume, InsLG(&lista1, serial, cmpRating));
					break;

				case 2:
					fprintf(outputFile, "Serialul %s a fost adaugat la pozitia %d.\n", 
						serial->nume, InsLG(&lista2, serial, cmpRating));
					break;

				case 3:
					fprintf(outputFile, "Serialul %s a fost adaugat la pozitia %d.\n", 
						serial->nume, InsLG(&lista3, serial, cmpRating));
					break;
			}
		}


		if (strcmp(comanda, "add_top") == 0)
		{
			int pozitie = atoi(strtok(NULL, " "));

			// creare structura de tip serial din datele citite
			TSerial *serial = CitireSerial();
			if (!serial)
				exit(1);

			// inserare in lista top
			InsTOP(&listatop, serial, pozitie);
			
			fprintf(outputFile, "Categoria top10: ");
			Afisare(outputFile, &listatop, AfiSerial);
		}


		if (strcmp(comanda, "add_sez") == 0)
		{
			char *nume = strtok(NULL, " ");


			if (nume[strlen(nume) - 1] == '\n')
				nume[strlen(nume) - 1] = '\0';
			if (nume[strlen(nume) - 1] == '\r')
				nume[strlen(nume) - 1] = '\0';

			int check = 0;

			// caut elementul in stiva Watch, in coada Later si in liste pana il gasesc  
			// si ii adaug serialul cerut 			
			if (!check)
				check = AddSezLG(outputFile, lista1, nume, cmpNume, CitireSezon);

			if (!check)
				check = AddSezLG(outputFile, lista2, nume, cmpNume, CitireSezon);

			if (!check)
				check = AddSezLG(outputFile, lista3, nume, cmpNume, CitireSezon);

			if (!check)
				check = AddSezLG(outputFile, listatop, nume, cmpNume, CitireSezon);

			if (!check)
				check = AddSezQ(outputFile, coadaLater, nume, cmpNume, CitireSezon);

			if (!check)
				check = AddSezS(outputFile, stivaWatch, nume, cmpNume, CitireSezon);
		}


		if (strcmp(comanda, "later") == 0)
		{
			char *nume = strtok(NULL, " ");
			
			if (nume[strlen(nume) - 1] == '\n')
				nume[strlen(nume) - 1] = '\0';
			if (nume[strlen(nume) - 1] == '\r')
				nume[strlen(nume) - 1] = '\0';

			TSerial *aux = NULL; 

			// caut pe rand in cele 4 liste pana gasesc elementul
			if (!aux)
				aux = ExtrageLG(&lista1, nume, cmpNume);
			
			if (!aux)
				aux = ExtrageLG(&lista2, nume, cmpNume);
			
			if (!aux)
				aux = ExtrageLG(&lista3, nume, cmpNume);
			
			if (!aux)
				aux = ExtrageLG(&listatop, nume, cmpNume);

			// introduc elementul in coada Later
			IntrQ(coadaLater, aux);

			fprintf(outputFile, "Serialul %s se afla in coada de asteptare pe pozitia %d.\n",
				aux->nume, NrEQ(coadaLater));
		}

		if (strcmp(comanda, "show") == 0)
		{
			char *categorie = strtok(NULL, " ");

			if (categorie[strlen(categorie) - 1] == '\n')
				categorie[strlen(categorie) - 1] = '\0';
			if (categorie[strlen(categorie) - 1] == '\r')
				categorie[strlen(categorie) - 1] = '\0';

			fprintf(outputFile, "Categoria %s: ", categorie);

			// afisez ce se cere
			if (strcmp(categorie, "1") == 0)
				Afisare(outputFile, &lista1, AfiSerial);

			if (strcmp(categorie, "2") == 0)
				Afisare(outputFile, &lista2, AfiSerial);	

			if (strcmp(categorie, "3") == 0)
				Afisare(outputFile, &lista3, AfiSerial);

			if (strcmp(categorie, "top10") == 0)
				Afisare(outputFile, &listatop, AfiSerial);

			if (strcmp(categorie, "later") == 0)
				AfiQ(outputFile, AfiSerial, coadaLater);

			if (strcmp(categorie, "watching") == 0)
				AfiS(outputFile, AfiSerial, stivaWatch);
		
			if (strcmp(categorie, "history") == 0)
				AfiS(outputFile, AfiSerial, stivaHistory);
		}

		if (strcmp(comanda, "watch") == 0)
		{
			char *nume = strtok(NULL, " ");
			int durata = atoi(strtok(NULL, " "));
			TSerial *aux = NULL;

			// caut elementul in stiva Watch, in coada Later si in liste pana il gasesc  
			// si il extrag 
			if (!aux)
				aux = ExtrageS(stivaWatch, nume, cmpNume);
			
			if (!aux)
				aux = ExtrageQ(coadaLater, nume, cmpNume);
				
			if (!aux)
				aux = ExtrageLG(&lista1, nume, cmpNume);
				
			if (!aux)
				aux = ExtrageLG(&lista2, nume, cmpNume);
		
			if (!aux)
				aux = ExtrageLG(&lista3, nume, cmpNume);

			if (!aux)
				aux = ExtrageLG(&listatop, nume, cmpNume);
				
			// daca nu viozionez intreg serialul calculez cat mai ramane de 
			// de vizionat si il adaug in stiva Watch
			if (aux->durata > durata)
			{
				aux->durata -= durata;
				Push(stivaWatch, aux);
			}
			// daca il vizionez in intregime il adaug in stiva History
			else
			{
				aux->durata = 0;
				Push(stivaHistory, aux);
				fprintf(outputFile, "Serialul %s a fost vizionat integral.\n", aux->nume);
			}
		}
	}

	free(line);
	free(inputFileName);
	free(outputFileName);


	fclose(inputFile);
	fclose(outputFile);
  	return 0;
}
