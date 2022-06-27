/* FILISANU Mihai-Alexandru - 314CB */
/*-- tema1.c --  */
#include <stdio.h>
#include "tlg.h"
#include "thash.h"
#include <string.h>


typedef struct
{
	char *key;
	char *value;
} Tdns;


// calculeaza suma literelor fiecarui cuvant,
// codul Hash urmand a fi aflat prin "% nr totoal"
int codHash(void *element)
{
    int sum = 0;
    Tdns *dns = (Tdns*)element;
    char *key = dns->key;

    for (; *key != '\0'; key++)
        sum += *key;

    return sum;
}


// functie de afisare
void afisareDns(FILE *outputFile, void *element)
{
	Tdns *dns = (Tdns*)element;
	fprintf(outputFile, "%s ", dns->value);
}


// functie de comparare
// 0 - ac valoare; >0 - dns1 > dns2; <0 - dns1 < dns2 
int cmpDns(void *e1, void *e2)
{
	Tdns *dns1 = (Tdns*)e1;
	Tdns *dns2 = (Tdns*)e2;

	return strcmp(dns1->key, dns2->key);
}


// functia pentru comanda put
void PutFunction(TH *h)
{
	char *key = strtok(NULL, " ");
	char *value = strtok(NULL, " ");
		
	if (value[strlen(value) - 1] == '\n')
        value[strlen(value) - 1] = '\0';
    if (value[strlen(value) - 1] == '\r')
        value[strlen(value) - 1] = '\0';

	Tdns *dns = malloc(sizeof(Tdns));
	if (!dns)
	{
		free(dns);
		return;
	}

	dns->key = malloc(100);
	if (!dns->key)
	{
		free(dns->key);
		free(dns);
		return;	
	}
	dns->value = malloc(100);
	if (!dns->value)
	{
		free(dns->value);
		free(dns);
		return;
	}
	
	strcpy(dns->key, key);
	strcpy(dns->value, value);

	InsTH(h, dns, cmpDns);
}


// functia pentru comanda find
void FindFunction(TH *h, FILE *outputFile)
{
	char *key = strtok(NULL, " ");
		
	if (key[strlen(key) - 1] == '\n')
        key[strlen(key) - 1] = '\0';
    if (key[strlen(key) - 1] == '\r')
        key[strlen(key) - 1] = '\0';
			
	Tdns *dns = malloc(sizeof(Tdns));
	if (!dns)
	{
		free(dns);
		return;
	}

	dns->key = malloc(100);
	if (!dns->key)
	{
		free(dns->key);
		free(dns);
		return;	
	}
	strcpy(dns->key, key);

	if (ExistaTH(h, (void*)dns, cmpDns) == NULL)
		fprintf(outputFile, "False\n");
	else
		fprintf(outputFile, "True\n");
}


// functia pentru comanda get
void GetFunction(TH *h, FILE *outputFile)
{
	char *key = strtok(NULL, " ");
			
	if (key[strlen(key) - 1] == '\n')
        key[strlen(key) - 1] = '\0';
	if (key[strlen(key) - 1] == '\r')
        key[strlen(key) - 1] = '\0';
			
	Tdns *dns = malloc(sizeof(Tdns));
	if (!dns)
	{
		free(dns);
		return;
	}
	
	dns->key = malloc(100);
	if (!dns->key)
	{
		free(dns->key);
		free(dns);
		return;	
	}
	strcpy(dns->key, key);

	Tdns *rez = (Tdns*)(ExistaTH(h, (void*)dns, cmpDns));
	if (rez == NULL)
		fprintf(outputFile, "NULL\n");
	else
		fprintf(outputFile, "%s\n", rez->value);
}


// functia pentru comanda remove
void RemoveFunction(TH *h)
{
	char *key = strtok(NULL, " ");
		
	if (key[strlen(key) - 1] == '\n')
        key[strlen(key) - 1] = '\0';
    if (key[strlen(key) - 1] == '\r')
		key[strlen(key) - 1] = '\0';
			
	Tdns *dns = malloc(sizeof(Tdns));
	if (!dns)
	{
		free(dns);
		return;
	}
	
	dns->key = malloc(100);
	if (!dns->key)
	{
		free(dns->key);
		free(dns);
		return;	
	}
	strcpy(dns->key, key);
			
	RemoveTH(h, dns, cmpDns);
}


int main(int argc, char *argv[])
{
	// verific ca nr de parametri sa fie cel cerut
	if (argc != 4)
		return 0;

	size_t M = atoi(argv[1]);
    char *inputFileName = strdup(argv[2]);
    char *outputFileName = strdup(argv[3]);

	FILE *inputFile = fopen(inputFileName, "rt");
	FILE *outputFile = fopen(outputFileName, "wt");

 	TH *h = NULL;
	h = (TH*)IniTH(M, codHash); // initializare tabela hash

	if (!h)
	{
		free(h);
		return 0;
	}

	size_t len = 0;
	char *line = malloc(200);
	if (!line)
	{
		free(line);
		return 0;
	}

	// parcurg pe rand fiecare comanda
	while (getline(&line, &len, inputFile) != -1) 
	{
		char *comanda = strtok(line, " ");

		if (comanda[strlen(comanda) - 1] == '\n')
            comanda[strlen(comanda) - 1] = '\0';
        if (comanda[strlen(comanda) - 1] == '\r')
            comanda[strlen(comanda) - 1] = '\0';	

		if (strcmp(comanda, "put") == 0)
			PutFunction(h);

		if (strcmp(comanda, "print") == 0)
			AfiTH(outputFile, h, afisareDns);

		if (strcmp(comanda, "find") == 0)
			FindFunction(h, outputFile);

		if (strcmp(comanda, "get") == 0)
			GetFunction(h, outputFile);

		if (strcmp(comanda, "remove") == 0)
			RemoveFunction(h);

		if (strcmp(comanda, "print_bucket") == 0)
		{
			int number = atoi(strtok(NULL, " "));
			AfiTH_v2(outputFile, h, number, afisareDns);
		}
	}
	free(line);
	free(inputFileName);
	free(outputFileName);

	fclose(inputFile);
	fclose(outputFile);

	return 0;
}
