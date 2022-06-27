/*--- graf.h --- Graf --*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef GRAF__
#define GRAF__


#define NodInex 0
#define ArcInex 0

void* AlocG(int); /* aloca memorie pt.reprezentare graf cu nr.noduri dat */
void DistrG(void**);  /* elibereaza tot spatiul ocupat de graf */

int CostArc(void*, int, int);
int NrNoduri(void* g);

int CitGraf(void**);  /* citeste descriere graf */
void AfiGrafM(void*); /* afisare ca matrice */
void AfiGrafL(void*); /* afisare liste de adiacente */

#endif
