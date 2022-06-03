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
void stampaGrafoCitta(t_grafoC** G, int stampaStazioni, int nv);
void rimuoviArcoStazioniGrafoPrincipale(t_grafoP* G, int u, int v);
void rimuoviArcoAereoportiGrafoPrincipale(t_grafoP* G, int u, int v);
void rimuoviArcoGrafoCitta(t_grafoC* G, int u, int v);
void aggiungiArcoGrafoPrincipale(t_grafoP* G, int u, int v, int costo, int distanza, int mode);
t_grafoP* leggiGrafo();
void salvaGrafo(t_grafoP* G);
void salvaGrafoCitta(t_grafoC** C, int);
t_grafoC** leggiGrafoCitta(int nv);
int dijkstraAereoportiCosto(t_grafoP* G, int s, int meta);
int dijkstraAereoportiDistanza(t_grafoP* G, int s, int meta);
int dijkstraStazioniCosto(t_grafoP* G, int s, int meta);
int dijkstraStazioniDistanza(t_grafoP* G, int s, int meta);
int dijkstraGenerico(t_grafoP* G, int s, int meta, int mode);
t_grafoP* leggiNomiCitta(t_grafoP* G);
void stampaCitta(t_grafoC* G);
void aggiungiArcoGrafoCitta(t_grafoC* G, int i, int key);
void rimuoviArcoGrafoPrincipale(t_grafoP* G, int u, int v, int mode);
int verificaRaggiungibile();

#endif // GRAFI_H
