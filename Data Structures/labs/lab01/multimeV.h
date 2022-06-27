/*-- multimeV.h --*/
/*-- Multimi generice (elemente de orice tip) memorate ca vectori --*/

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef  _TMULTIME_
#define  _TMULTIME_

typedef int(*TFComp)(const void *, const void *);

typedef struct
{
    size_t d;        /* dimensiune elemente */
    void *p, *s, *t; /* adrese vector, sfarsit zona utila, disponibila */
    TFComp cmp;      /* functia care verifica identitatea a doua elemente */
} TMultime;

#define VidaM(m) ((m)->s == (m)->p)
#define PlinaM(m) ((m)->s == (m)->t)

/*--- initializari ---*/

/**
 * creeaza multime, alocand dinamic spatiu pentru descriptor
 * si n elemente de dimensiune d; intoarce adr.multime sau NULL
 */
TMultime *InitD(size_t n, size_t d, TFComp fcmp);

void DistrugeM(TMultime **m);

/*--- operatii asupra multimilor NESORTATE ---*/

int Cardinal(TMultime * m);

/**
 * obiectiv: adaugarea unui nou element la sfarsitul vectorului;
 * intoarce 1/0/-1 - succes/exista deja/lipsa spatiu
 */
int Adauga(void *nou, TMultime *m);

/**
 * intoarce 1/0 - elementul x exista/nu exista in multimea m
 */
int Apartine(void *x, TMultime *m);

#endif
