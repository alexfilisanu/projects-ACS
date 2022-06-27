/* Filisanu Mihai-Alexandru -- 314CB */
#include "trie.h"

// structura pentru carte
typedef struct 
{
	char *titlu;
	char *autor;
	int rating;
	int nrPagini;
} TCarte;

// functie de afisare a titlului dintr-o structura TCarte
void afiTitlu(FILE *outputFile, void *info)
{
	fprintf(outputFile, "%s\n", ((TCarte*)info)->titlu);
}

// functie de afisare a autorului dintr-o structura TCarte
void afiAutor(FILE *outputFile, void *info)
{
	fprintf(outputFile, "%s\n", ((TCarte*)info)->autor);
}

// functie apelata pentru comanda "add_book"; cu datele citite creez o structura de 
// tip TCarte pe care o inserez in arborii T1 si T2
void add_book(TArb arbT1, TArb arbT2)
{
	char *titlu = strtok(NULL, ":");
	char *autor = strtok(NULL, ":");
	int rating = atoi(strtok(NULL, ":"));
	int nrPagini = atoi(strtok(NULL, ":"));

	TCarte *carte = (TCarte*)malloc(sizeof(TCarte));
	if (!carte)
		exit(1);
			
	carte->titlu = malloc(50);
	if (!carte->titlu)
		exit(1);
	strcpy(carte->titlu, titlu);

	carte->autor = malloc(40);
	if (!carte->autor)
		exit(1);
	strcpy(carte->autor, autor);

	carte->rating = rating;
	carte->nrPagini = nrPagini;

	// inserare structura citita in arborii T1 si T2
	InserareT1(arbT1, titlu, carte);
	InserareT2(arbT2, autor, titlu, carte);
}

// functie apelata pentru comanda "search_book" cu titlu intreg;
void search_book(FILE *outputFile, TArb arbT1, char *titlu)
{
	// caut structura de tip TCarte dupa parcurgerea cheii titlu din arborele T1
	TCarte *carte = CautaInfo(arbT1, titlu);

	if (carte == NULL)
		fprintf(outputFile, "Cartea %s nu exista in recomandarile tale.\n", titlu);
	else
		fprintf(outputFile, "Informatii recomandare: %s, %s, %d, %d\n", carte->titlu, 
				carte->autor, carte->rating, carte->nrPagini);	
}

// functie apelata pentru comanda "search_book" cu prefixul titlului; 
void prefix_search_book(FILE *outputFile, TArb arbT1, char *titlu)
{
	titlu[strlen(titlu) - 1] = '\0';
	int parcurs = 1;

	// parcurg in arborele T1 prefixul cheii titlu pana la ultima litera
	while (*titlu)
	{
		arbT1 = arbT1->fiu[indexAlfabet(*titlu)];
		titlu++;
		// prefixul cheii nu a putut fi parcurs in intregime
		if (arbT1 == NULL)
		{
			fprintf(outputFile, "Nicio carte gasita.\n");
			parcurs = 0;
			break;
		}	
	}
	// prefixul cheii a fost parcurs in intregime -> afisez primele 3 titluri
	if (parcurs == 1)
	{
		int numara = 0;
		AfisareInfo3(outputFile, arbT1, &numara, afiTitlu);
	}
}

// functie apelata pentru comanda "list_author" cu autor intreg;
void list_author(FILE *outputFile, TArb arbT2, char *autor)
{
	// caut arborele de tip T1 dupa parcurgerea cheii autor din arborele T2
	TArb arbCarte = CautaInfo(arbT2, autor);

	if (arbCarte == NULL)
		fprintf(outputFile, "Autorul %s nu face parte din recomandarile tale.\n", autor);
	else
		AfisareInfoTot(outputFile, arbCarte, afiTitlu);
}

// functie apelata pentru comanda "list_author" cu prefixul autorului;
void prefix_list_author(FILE *outputFile, TArb arbT2, char *autor)
{
	autor[strlen(autor) - 1] = '\0';
	int parcurs = 1;

	// parcurg prefixul cheii autor pana la ultima litera
	while (*autor)
	{
		arbT2 = arbT2->fiu[indexAlfabet(*autor)];
		autor++;
		// prefixul cheii nu a putut fi parcurs in intregime
		if (arbT2 == NULL)
		{
			fprintf(outputFile, "Niciun autor gasit.\n");
			parcurs = 0;
			break;
		}	
	}
	// prefixul cheii a fost parcurs in intregime -> afisez primii 3 autori
	if (parcurs == 1)
	{
		int numara = 0;
		ContinuarePrefix(outputFile, arbT2, &numara, afiAutor);
	}
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

	// cei 2 arbori
	TArb arbT1 = ConstrNod();
	TArb arbT2 = ConstrNod();

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

		if (strcmp(comanda, "add_book") == 0)
			add_book(arbT1, arbT2);

		if (strcmp(comanda, "search_book") == 0)
		{
			char *titlu = strtok(NULL, ":");
			if (titlu[strlen(titlu) - 1] == '\n')
				titlu[strlen(titlu) - 1] = '\0';
			if (titlu[strlen(titlu) - 1] == '\r')
				titlu[strlen(titlu) - 1] = '\0';

			// daca titlul este prefix -> afisez primele 3 titluri
			if (titlu[strlen(titlu) - 1] == '~')
			{
				prefix_search_book(outputFile, arbT1, titlu);
			}
			// daca titlul este intreg
			else
			{
				search_book(outputFile, arbT1, titlu);
			}
		}

		if (strcmp(comanda, "list_author") == 0)
		{
			char *autor = strtok(NULL, ":");
			if (autor[strlen(autor) - 1] == '\n')
				autor[strlen(autor) - 1] = '\0';
			if (autor[strlen(autor) - 1] == '\r')
				autor[strlen(autor) - 1] = '\0';

			// daca autorul este prefix -> afisez primii 3 autori
			if (autor[strlen(autor) - 1] == '~')
			{
				prefix_list_author(outputFile, arbT2, autor);
			}
			// daca autorul este intrg -> afisez toate titlurile cartilor acestuia
			else
			{
				list_author(outputFile, arbT2, autor);		
			}	
		}

		if (strcmp(comanda, "search_by_author") == 0)
		{
			char *autor = strtok(NULL, ":");
			if (autor[strlen(autor) - 1] == '\n')
				autor[strlen(autor) - 1] = '\0';
			if (autor[strlen(autor) - 1] == '\r')
				autor[strlen(autor) - 1] = '\0';

			// daca autorul este prefix
			if (autor[strlen(autor) - 1] == '~')
			{
				prefix_list_author(outputFile, arbT2, autor);		
			}
			// daca autorul este intreg
			else
			{
				char *titlu = strtok(NULL, ":");
				if (titlu[strlen(titlu) - 1] == '\n')
					titlu[strlen(titlu) - 1] = '\0';
				if (titlu[strlen(titlu) - 1] == '\r')
					titlu[strlen(titlu) - 1] = '\0';

				// caut in T2 arborele de tip T1 al autorului
				TArb arbCarte = CautaInfo(arbT2, autor);

				// daca autorul nu exista
				if (arbCarte == NULL)
				{
					fprintf(outputFile, "Autorul %s nu face parte din recomandarile tale.\n", autor);
				}
				// daca autorul exista
				else
				{
					// daca titlul este prefix
					if (titlu[strlen(titlu) - 1] == '~')
					{
						prefix_search_book(outputFile, arbCarte, titlu);
					}
					// daca autorul exista si titlul este intreg 
					else
					{
						search_book(outputFile, arbT1, titlu);	
					}
				}
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
