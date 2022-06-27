/*-- functiiM.c --*/
#include "multimeV.h"

/*--- functii de initializare ---*/

/**
 * creeaza multime, alocand dinamic spatiu pentru descriptor
 * si n elemente de dimensiune d; intoarce adr.multime sau NULL
 */
TMultime *InitD(size_t n, size_t d, TFComp fcmp)
{
    char *sf;
    TMultime *m = (TMultime*)calloc(1, sizeof(TMultime));
    if (!m) {    /* alocare esuata */
        return NULL;
    }
    m->p = calloc(n, d);
    if (!m->p) {    /* alocare esuata */
        free(m);
        return NULL;
    }
    m->d = d;
    m->s = m->p;
    sf = (char*)(m->p) + d * n;
    m->t = (void*)sf;
    m->cmp = fcmp;
    
    return m;    /* initializare reusita */
}

/**
 * elibereaza spatiul alocat de structura
 */
void DistrugeM(TMultime **m)
{
    free((*m)->p);
    free(*m);
    *m = NULL;
}

/**
 * numar elemente din mltime
 */
int Cardinal(TMultime *m)
{
    return ((char*)m->s - (char*)m->p) / m->d;
}

/**
 * intoarce 1/0 - elementul x exista/nu exista in multimea m
 *  multimea m nu este sortata
 */
int Apartine(void *x, TMultime *m)
{
    char *pm, *sf = (char*)(m->s);

    for (pm = (char*)(m->p); pm < sf; pm += m->d)
        if (m->cmp(x, (void*)pm) == 0)
            return 1;
    
    return 0;
}

/**
 * obiectiv: adaugarea unui nou element la sfarsitul vectorului;
 * intoarce 1/0/-1 - succes/exista deja/nu exista spatiu
 * multimea m nu este sortata
 */
int Adauga(void *nou, TMultime *m)
{
  char *sf = (char*)m->s;
  if (PlinaM(m)) {    /* multime plina */
      return -1;
  }
  if (Apartine(nou, m)) {    /* nou exista deja in m -> gata */
      return 0;
  }
  memcpy(m->s, nou, m->d);    /* copiaza nou la sfarsit m */
  sf += m->d;                 /* actualizeaza sfarsit */
  m->s = (void*)sf;

  return 1;                   /* elementul a fost adaugat */
}
