#include "liste.h"

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
    int weight;
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
void stampaGrafoCitta(t_grafoC** GC, t_grafoP *G, int stampaStazioni, int nv);
void rimuoviArcoStazioniGrafoPrincipale(t_grafoP* G, int u, int v);
void rimuoviArcoAereoportiGrafoPrincipale(t_grafoP* G, int u, int v);
void rimuoviArcoGrafoCitta(t_grafoC* G, int u, int v);
void aggiungiArcoGrafoPrincipale(t_grafoP* G, int u, int v, int costo, int distanza, int mode);
t_grafoP* leggiGrafo();
void salvaGrafo(t_grafoP* G);
void salvaGrafoCitta(t_grafoC** C, int);
t_grafoC** leggiGrafoCitta(t_grafoP* G);
t_lista* dijkstraAereoportiCosto(t_grafoP* G, int s, int meta);
t_lista* dijkstraAereoportiDistanza(t_grafoP* G, int s, int meta);
t_lista* dijkstraStazioniCosto(t_grafoP* G, int s, int meta);
t_lista* dijkstraStazioniDistanza(t_grafoP* G, int s, int meta);
t_lista* dijkstraGenerico(t_grafoP* G, int s, int meta, int mode);
t_grafoP* leggiNomiCitta(t_grafoP* G);
void stampaCitta(t_grafoC* GC, t_grafoP *G, int key);
void aggiungiArcoGrafoCitta(t_grafoC* G, int i, int key);
void rimuoviArcoGrafoPrincipale(t_grafoP* G, int u, int v, int mode);
void stampaNomiCitta(t_grafoP* G, int mode);
void stampaAlberghi(t_grafoC* GC);
t_lista* dijkstraAlberghi(t_grafoC* GC, int mode, int albergo);
void stampaLista(t_lista* lista, t_grafoC* GC);
void stampaPercorso(t_lista* lista, t_grafoP* G);
t_grafoP* creaGrafoSenzaFile();
t_grafoC** creaGrafoCittaSenzaFile(t_grafoC** GC, t_grafoP* G);
void salvaNomiCitta(t_grafoP* G);
void freeArco(t_arcoP* e);

#endif // GRAFI_H
