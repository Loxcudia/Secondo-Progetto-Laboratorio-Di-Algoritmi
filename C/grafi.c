#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "grafi.h"
#include "liste.h"

t_grafoP* creaGrafoPrincipale(int n)
{
	t_grafoP* G;
	int i;
	G = (t_grafoP*)malloc(sizeof(t_grafoP));
	if (!G)
		printf("Impossibile allocare memoria per il grafo\n");
	else
	{
		G->adjAereoporti = (t_arcoP**)malloc(n * sizeof(t_arcoP*));
		G->adjStazioni = (t_arcoP**)malloc(n * sizeof(t_arcoP*));
		if (!G->adjAereoporti)
			printf("Impossibile allocare memoria per la lista di aeroporti del grafo\n");
		if(!G->adjStazioni)
			printf("Impossibile allocare memoria per la lista di stazioni del grafo\n");
		else
		{
			G->nv = n;
			G->stazioni = (int*)malloc(n * sizeof(int));
			G->aereoporti = (int*)malloc(n * sizeof(int));
			for (i = 0; i < n; i++)
			{
				G->adjAereoporti[i] = NULL;
				G->adjStazioni[i] = NULL;
			}
		}
	}
	return G;
}

t_grafoC* creaGrafoCitta(int n)
{
	t_grafoC* G;
	int i;
	G = (t_grafoC*)malloc(sizeof(t_grafoC));
	if (!G)
		printf("Impossibile allocare memoria per il grafo\n");
	else
	{
		G->adj = (t_luogo**)malloc(n * sizeof(t_luogo*));
		if (!G->adj)
			printf("Impossibile allocare memoria per la lista di stazioni del grafo\n");
		else
		{
			G->nv = n;
			G->nomeAlberghi = (char**)malloc(n * sizeof(char*));
			for (i = 0; i < n; i++)
				G->adj[i] = NULL;
		}
	}
	return G;
}

void stampaGrafoPrincipale(t_grafoP* G)
{
	int i, ne = 0;
	t_arcoP* e;
	if (G)
	{
		printf("Il grafo ha %d vertici\n", G->nv);
		for (i = 0; i < G->nv; i++)
		{
			if (G->stazioni[i] == 0)
				continue;
			printf("Stazioni adiacenti al nodo %d ->", i);
			e = G->adjStazioni[i];
			while (e)
			{
				printf("%d", e->key);
				ne = ne + 1;
				e = e->next;
			}
			printf("\n");
		}
		for (i = 0; i < G->nv; i++)
		{
			printf("Aereoporti adiacenti al nodo %d ->", i);
			e = G->adjAereoporti[i];
			while (e)
			{
				printf("%d", e->key);
				ne = ne + 1;
				e = e->next;
			}
			printf("\n");
		}
	}
}

/*
void dijkstraGraphPesatoLista(t_grafoP *G, int s)
{
    int *d;
    int *pi;
    int i, u;
    t_lista *lista = NULL;
    t_arcoP *e;

    d = (int*)calloc(sizeof(int), G->nv);
    pi = (int*)calloc(sizeof(int), G->nv);

    for(i=0; i<G->nv; i++)
    {
        d[i] = UINT_MAX;
        pi[i] = -1;
    }

    d[s] = 0;

    for(i=0; i<G->nv; i++)
        inserimentoInTesta(&lista, i);

    while(lista != NULL)
    {
        u = estraiMinimo(&lista, d);
        e = G->adj[u];

        while(e!=NULL)
        {
            if(d[e->key] == UINT_MAX || d[e->key] > d[u] + e->weight)
            {
                pi[e->key] = u;
                d[e->key] = d[u] + e->weight;
            }
            e = e->next;
        }
    }

    for(i=0; i<G->nv; i++)
        printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);

    return;
}

int estraiMinimo(t_lista **lista, int *d)
{
    t_lista *p, *pmin, *prec = NULL, *precmin = NULL;
    int u;

    pmin = *lista;
    p = *lista;

    while(p!=NULL)
    {
        if((d[p->data] != UINT_MAX && d[p->data] < d[pmin->data]) || d[pmin->data] == UINT_MAX)
        {
            pmin = p;
            precmin = prec;
        }
        prec = p;
        p = p->next;
    }
    u = pmin->data;

    if(precmin == NULL)
        *lista = (*lista)->next;
    else
        precmin->next = pmin->next;

    free(pmin);

    return u;
}
*/

