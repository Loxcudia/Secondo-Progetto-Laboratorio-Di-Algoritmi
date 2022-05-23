#include <stdio.h>
#include <stdlib.h>

#ifndef GRAFI_H
#define GRAFI_H

typedef struct arcoPrincipale
{
    int key;
    int costo;
    int distanza;
    struct arcoPrincipale* next;
}t_arcoP;

typedef struct grafoPrincipale
{
    int nv;
    int* stazioni;
    int* aereoporti;
    char** nomiCitta;
    t_arcoP** adjAereoporti;
	t_arcoP** adjStazioni;
}t_grafoP;

typedef struct arcoLuogo
{
    int key;
    struct arcoLuogo* next;
}t_luogo;

typedef struct grafoCitta
{
    int nv;
    char** nomeAlberghi;
    t_luogo** adj;
}t_grafoC;

t_grafoP* creaGrafoPrincipale(int n);
t_grafoC* creaGrafoCitta(int n);
void stampaGrafoPrincipale(t_grafoP* G);
void stampaGrafoCitta(t_grafoC* G);
void rimuoviArcoStazioniGrafoPrincipale(t_grafoP* G, int u, int v);
void rimuoviArcoAereoportiGrafoPrincipale(t_grafoP* G, int u, int v);
void rimuoviArcoGrafoCitta(t_grafoC* G, int u, int v);
void aggiungiArcoGrafoPrincipale(t_grafoP *G, int u, int v, int costo, int distanza, int mode);
t_grafoP* leggiGrafo();
void salvaGrafo(t_grafoP *G);
void dijkstraAereoportiCosto(t_grafoP *G, int s);
void dijkstraAereoportiDistanza(t_grafoP *G, int s);
void dijkstraStazioniCosto(t_grafoP *G, int s);
void dijkstraStazioniDistanza(t_grafoP *G, int s);

#endif // GRAFI_H
