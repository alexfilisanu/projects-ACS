/* Filisanu Mihai-Alexandru -- 314CB */
/*--- trie.h ---*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h> 
#include <time.h>

#define ALPHABET_SIZE 70

#ifndef TRIE
#define TRIE

typedef struct nod 
{
	void *info;
	struct nod *fiu[ALPHABET_SIZE];
} TNod, *TArb;

typedef void TAfi(FILE*, void*);

int indexAlfabet(char aux);		/* returneaza indexul caracterului primit */

TArb ConstrNod();		/* returneaza adresa nodului creat */
void InserareT1(TArb, char*c1, void*info);		/* inserare informatie intr-un arbore T1 */
void InserareT2(TArb, char*c1, char*c2, void*info);		/* inserare informatie intr-un arbore T2 */
void* CautaInfo(TArb, char*c1);		/* returneaza informatia aflata in frunza arborelui */
void AfisareInfo3(FILE*, TArb, int *n, TAfi afi);		/* afiseaza primele 3 informatii cerute */
void AfisareInfoTot(FILE*, TArb, TAfi afi);		/* afiseaza toate informatiile dintr-un arbore */
void AfisareInfoDif3(FILE*, TArb, int *n, int *v, TAfi afi);		/* afiseaza primele 3 informatii cerute fara sa se repete */
void ContinuarePrefix(FILE *, TArb, int *n, TAfi afi);		/* parcurg arborele ramas pana la arborele aflat in frunza */

#endif
