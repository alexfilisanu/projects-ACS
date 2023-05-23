/*-- tlg.h --- LISTA SIMPLU INLANTUITA GENERICA ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celulag
{
    void* info;           /* adresa informatie */
    struct celulag *urm;   /* adresa urmatoarei celule */
} TCelulaG, *TLG;         /* tipurile Celula, Lista */

typedef int (*TFCmp)(void*, void*);    /* functie de comparare doua elemente */
typedef void (*TFA)(FILE*, void*);     /* functie afisare/eliberare un element */
typedef void (*TF)(void*);      /* functie de eliminare element */           
typedef void TFC(void*);        /* functie de citire */

/* functii lista generica */

int InsLG(TLG*, void*, TFCmp fcmp);  /* inserare in lista; returneaza pozitia */ 
int InsTOP(TLG*, void*, int poz);   /* inserare la pozitia specificata; reusita (1/0) */
void* ExtrageLG(TLG*, void*, TFCmp fcmp);   /* extrage elementul cautat din lista data */
int AddSezLG(FILE*, TLG, void*, TFCmp fcmp, TFC fc);   /* adauga un sezon in lista; reusita (0/1) */
void Afisare(FILE*, TLG*, TFA fAfi);  /* afiseaza elementele din lista */
void Distruge(TLG*, TF fe); /* distruge lista */
int LungimeLG(TLG*);   /* numarul de elemente din lista */

#endif
