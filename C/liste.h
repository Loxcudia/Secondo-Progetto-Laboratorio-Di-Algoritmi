#ifndef LISTE_H
#define LISTE_H

typedef struct lista {
    int data;
    struct lista* next;
}t_lista;



void inserimentoInTesta(t_lista** lista, int data);
int inserimentoInPercorso(t_lista** lista, int data);
int estraiMinimo(t_lista** lista, int* d);

#endif


