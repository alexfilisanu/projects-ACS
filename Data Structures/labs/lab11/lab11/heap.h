#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef __HEAP_H__
#define __HEAP_H__

#define NodInex -1

typedef int (*TRel)(int, int);  /* relatie intre 2 informatii */

typedef struct Heap {
	int nMax, ne;   /* numar maxim de elemente posibil, respectiv numar efectiv de elemente din heap */
	int *v;         /* adresa vectorului (alocat dinamic) prin care este reprezentat heapul */
	TRel rel;       /* functie de comparatie a doua elemente de tip int - folosita pentru implementarea max/min heap */
} *THeap;


THeap AlocaHeap(int nMax, TRel rel);

void DistrugeHeap(THeap *ah);

void AfisareHeap(THeap h, int);

#endif /* __HEAP_H__ */
