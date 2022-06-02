#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <limits.h>
#include "grafi.h"
#include "liste.h"

void aggiungiArcoGrafoPrincipale(t_grafoP* G, int u, int v, int costo, int distanza, int mode)
{
	t_arcoP* nuovo, * e;

	//manca il check del grafo non inizializzato

	if (u<0 || u>G->nv - 1 || v<0 || v>G->nv - 1)
	{
		printf("\nArco non aggiunto, vertici richiesti invalidi(valori validi: 0 a %d)", G->nv - 1);
		return;
	}

	if (costo < 0)
	{
		printf("\nArco non aggiunto, costo invalido");
		return;
	}

	if (distanza < 0)
	{
		printf("\nArco non aggiunto, costo invalido");
		return;
	}

	nuovo = (t_arcoP*)malloc(sizeof(t_arcoP));

	if (nuovo == NULL)
		printf("\nAllocazione fallita");
	else
	{
		nuovo->key = v;
		nuovo->costo = costo;
		nuovo->distanza = distanza;
		nuovo->next = NULL;

		if (mode == 0)
		{
			if (G->adjAereoporti[u] == NULL)
				G->adjAereoporti[u] = nuovo;
			else
			{
				e = G->adjAereoporti[u];
				while (e->next != NULL)
				{
					if (e->key == nuovo->key)
					{
						printf("\nArco gia\' presente");
						free(nuovo);
						return;
					}
					e = e->next;
				}
				e->next = nuovo;
			}
		}

		if (mode == 1)
		{
			if (G->adjStazioni[u] == NULL)
				G->adjStazioni[u] = nuovo;
			else
			{
				e = G->adjStazioni[u];
				while (e->next != NULL)
				{
					if (e->key == nuovo->key)
					{
						printf("\nArco gia\' presente");
						free(nuovo);
						return;
					}
					e = e->next;
				}
				e->next = nuovo;
			}
		}
	}
}

t_grafoP* leggiGrafo()
{
	FILE* fileGrafo = NULL;
	int nv;
	t_grafoP* G = NULL;
	int i = 0;
	int presenza;
	int v, costo, distanza;

	fileGrafo = fopen("grafo.txt", "r");

	if (fileGrafo == NULL)
		printf("Errore nella lettura del grafo\n");
	else
	{
		fscanf(fileGrafo, "%d", &nv);
		G = creaGrafoPrincipale(nv);

		for (i = 0; i < nv; i++)
		{
			fscanf(fileGrafo, "%d", &presenza);
			G->aereoporti[i] = presenza;

			// printf("%d ", presenza);
		}

		printf("\n");

		for (i = 0; i < nv; i++)
		{
			fscanf(fileGrafo, "%d", &presenza);
			G->stazioni[i] = presenza;

			// printf("%d ", presenza);
		}

		printf("\n");

		i = 0;

		fscanf(fileGrafo, "%d", &v);

		while (v != -2) //-2 flag per segnalare la fine degli archi di una lista di adiacenza
		{
			if (v == -1) //-1 flag per segnalare la fine degli archi di un vertice
			{
				i++;
				fscanf(fileGrafo, "%d", &v);
				continue;
			}
			else
			{
				fscanf(fileGrafo, "%d %d ", &costo, &distanza);
				aggiungiArcoGrafoPrincipale(G, i, v, costo, distanza, 0); //0 vuol dire aggiungilo alla lista di adiacenza degli aereoporti
			}

			fscanf(fileGrafo, "%d", &v);
		}

		i = 0;

		fscanf(fileGrafo, "%d ", &v);

		while (v != -2)
		{
			if (v == -1)
			{
				i++;
				fscanf(fileGrafo, "%d", &v);
				continue;
			}
			else
			{

				fscanf(fileGrafo, "%d %d ", &costo, &distanza);
				aggiungiArcoGrafoPrincipale(G, i, v, costo, distanza, 1); //1 vuol dire aggiungilo alla lista di adiacenza delle stazioni
			}

			fscanf(fileGrafo, "%d ", &v);
		}
	}

	fclose(fileGrafo);

	G = leggiNomiCitta(G);

	return G;
}

t_grafoP* leggiNomiCitta(t_grafoP* G)
{
	FILE* fileNomi = NULL;
	char nomeCitta[20]; //20 è a caso
	int i = 0;

	fileNomi = fopen("nomicitta.txt", "r");

	if (fileNomi == NULL)
		return G;

	while (fscanf(fileNomi, "%s", nomeCitta) != EOF && i < G->nv)
	{
		strcpy(G->nomiCitta[i], nomeCitta);
		i++;
	}

	return G;
}

void salvaGrafo(t_grafoP* G)
{
	FILE* fileGrafo = NULL;
	int i;
	t_arcoP* a;

	fileGrafo = fopen("grafo.txt", "w");

	if (fileGrafo == NULL)
	{
		printf("Errore nell\'apertura del file per il salvataggio del grafo\n");
		return;
	}

	fprintf(fileGrafo, "%d\n", G->nv);

	for (i = 0; i < G->nv; i++)
		fprintf(fileGrafo, "%d ", G->aereoporti[i]);
	printf("\n");

	for (i = 0; i < G->nv; i++)
		fprintf(fileGrafo, "%d ", G->stazioni[i]);
	printf("\n");

	for (i = 0; i < G->nv; i++)
	{
		a = G->adjAereoporti[i];

		while (a != NULL)
		{
			fprintf(fileGrafo, "%d %d %d ", a->key, a->costo, a->distanza);
			a = a->next;
		}
		fprintf(fileGrafo, "%d\n", -1);
	}
	fprintf(fileGrafo, "\n%d", -2);

	for (i = 0; i < G->nv; i++)
	{
		a = G->adjStazioni[i];

		while (a != NULL)
		{
			fprintf(fileGrafo, "%d %d %d ", a->key, a->costo, a->distanza);
			a = a->next;
		}
		fprintf(fileGrafo, "%d\n", -1);
	}
	fprintf(fileGrafo, "\n%d", -2);

	fclose(fileGrafo);
}

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
		if (!G->adjStazioni)
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

		G->nomiCitta = (char**)malloc(n * sizeof(char*));

		for (i = 0; i < n; i++)
			G->nomiCitta[i] = (char*)malloc(20 * sizeof(char));



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
				G->nomeAlberghi[i] = (char*)malloc(200 * sizeof(char));
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

			printf("Stazioni adiacenti a %s ->", G->nomiCitta[i]);
			e = G->adjStazioni[i];
			while (e)
			{
				printf("%s", G->nomiCitta[e->key]);
				ne = ne + 1;
				e = e->next;
			}
			printf("\n");
		}

		printf("\n");

		for (i = 0; i < G->nv; i++)
		{
			if (G->aereoporti[i] == 0)
				continue;

			printf("Aereoporti adiacenti a %s ->", G->nomiCitta[i]);
			e = G->adjAereoporti[i];
			while (e)
			{
				printf("%s", G->nomiCitta[e->key]);
				ne = ne + 1;
				e = e->next;
			}
			printf("\n");
		}
	}
}

void stampaGrafoCitta(t_grafoC** G, int stampaStazioni, int nv)
{
	//se stampaStazioni è 1 stampa le stazioni, se è 0 stampa gli aereoporti, se è 2 stampa entrambi
	int i;
	t_luogo* e;

	if (G)
	{
		printf("Il grafo ha %d vertici\n", nv);

		for (i = 0; i < nv; i++)
			stampaCitta(G[i]);
	}
}

void stampaCitta(t_grafoC* G)
{
	int i;
	t_luogo* l;
	for (i = 0; i < G->nv; i++)
	{
		l = G->adj[i];

		if (i == 0)
		{
			printf("Luoghi adiacenti all'aereoporto: ");
			while (l != NULL)
			{
				printf("%s", G->nomeAlberghi[i]);
				l = l->next;
			}
			printf("\n");
			continue;
		}

		if (i == 1)
		{
			printf("Luoghi adiacenti alla stazione: ");
			while (l != NULL)
			{
				printf("%s", G->nomeAlberghi[i]);
				l = l->next;
			}
			printf("\n");
			continue;
		}

		printf("Luoghi adiacenti a %s: ", G->nomeAlberghi[i]);
		while (l != NULL)
		{
			printf("%s", G->nomeAlberghi[i]);
			l = l->next;
		}
		printf("\n");

	}
}

void rimuoviArcoStazioniGrafoPrincipale(t_grafoP* G, int u, int v)
{
	t_arcoP* prev;
	t_arcoP* e;
	e = G->adjStazioni[u];
	if (e->key == v)
		G->adjStazioni[u] = e->next;
	else
	{
		prev = e;
		while (prev->next->key != v)
			prev = prev->next;
		e = prev->next;
		prev->next = e->next;
	}
}

void rimuoviArcoAereoportiGrafoPrincipale(t_grafoP* G, int u, int v)
{
	t_arcoP* prev;
	t_arcoP* e;
	e = G->adjAereoporti[u];
	if (e->key == v)
		G->adjAereoporti[u] = e->next;
	else
	{
		prev = e;
		while (prev->next->key != v)
			prev = prev->next;
		e = prev->next;
		prev->next = e->next;
	}
}

void rimuoviArcoGrafoCitta(t_grafoC* G, int u, int v)
{
	t_luogo* prev;
	t_luogo* e;
	e = G->adj[u];
	if (e->key == v)
		G->adj[u] = e->next;
	else
	{
		prev = e;
		while (prev->next->key != v)
			prev = prev->next;
		e = prev->next;
		prev->next = e->next;
	}
}


void dijkstraAereoportiCosto(t_grafoP* G, int s)
{
	int* d;
	int* pi;
	int i, u;
	t_lista* lista = NULL;
	t_arcoP* e;

	d = (int*)calloc(sizeof(int), G->nv);
	pi = (int*)calloc(sizeof(int), G->nv);

	for (i = 0; i < G->nv; i++)
	{
		d[i] = UINT_MAX;
		pi[i] = -1;
	}

	d[s] = 0;

	for (i = 0; i < G->nv; i++)
		inserimentoInTesta(&lista, i);

	while (lista != NULL)
	{
		u = estraiMinimo(&lista, d);
		e = G->adjAereoporti[u];

		while (e != NULL)
		{
			if (d[e->key] == UINT_MAX || d[e->key] > d[u] + e->costo)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->costo;
			}
			e = e->next;
		}
	}

	for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);

	return;
}

void dijkstraAereoportiDistanza(t_grafoP* G, int s)
{
	int* d;
	int* pi;
	int i, u;
	t_lista* lista = NULL;
	t_arcoP* e;

	d = (int*)calloc(sizeof(int), G->nv);
	pi = (int*)calloc(sizeof(int), G->nv);

	for (i = 0; i < G->nv; i++)
	{
		d[i] = UINT_MAX;
		pi[i] = -1;
	}

	d[s] = 0;

	for (i = 0; i < G->nv; i++)
		inserimentoInTesta(&lista, i);

	while (lista != NULL)
	{
		u = estraiMinimo(&lista, d);
		e = G->adjAereoporti[u];

		while (e != NULL)
		{
			if (d[e->key] == UINT_MAX || d[e->key] > d[u] + e->distanza)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->distanza;
			}
			e = e->next;
		}
	}

	for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);

	return;
}

void dijkstraStazioniCosto(t_grafoP* G, int s)
{
	int* d;
	int* pi;
	int i, u;
	t_lista* lista = NULL;
	t_arcoP* e;

	d = (int*)calloc(sizeof(int), G->nv);
	pi = (int*)calloc(sizeof(int), G->nv);

	for (i = 0; i < G->nv; i++)
	{
		d[i] = UINT_MAX;
		pi[i] = -1;
	}

	d[s] = 0;

	for (i = 0; i < G->nv; i++)
		inserimentoInTesta(&lista, i);

	while (lista != NULL)
	{
		u = estraiMinimo(&lista, d);
		e = G->adjStazioni[u];

		while (e != NULL)
		{
			if (d[e->key] == UINT_MAX || d[e->key] > d[u] + e->costo)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->costo;
			}
			e = e->next;
		}
	}

	for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);

	return;
}

void dijkstraStazioniDistanza(t_grafoP* G, int s)
{
	int* d;
	int* pi;
	int i, u;
	t_lista* lista = NULL;
	t_arcoP* e;

	d = (int*)calloc(sizeof(int), G->nv);
	pi = (int*)calloc(sizeof(int), G->nv);

	for (i = 0; i < G->nv; i++)
	{
		d[i] = UINT_MAX;
		pi[i] = -1;
	}

	d[s] = 0;

	for (i = 0; i < G->nv; i++)
		inserimentoInTesta(&lista, i);

	while (lista != NULL)
	{
		u = estraiMinimo(&lista, d);
		e = G->adjStazioni[u];

		while (e != NULL)
		{
			if (d[e->key] == UINT_MAX || d[e->key] > d[u] + e->distanza)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->distanza;
			}
			e = e->next;
		}
	}

	for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);

	return;
}

void dijkstraGenerico(t_grafoP* G, int s, int mode)
{
	switch (mode) {
	case(0):
		dijkstraAereoportiCosto(G, s);
		break;

	case(1):
		dijkstraAereoportiDistanza(G, s);
		break;

	case(2):
		dijkstraStazioniCosto(G, s);
		break;

	case(3):
		dijkstraStazioniDistanza(G, s);
		break;
	}
}

void salvaGrafoCitta(t_grafoC** C, int nv) {
	FILE* fp;
	t_luogo* arcoLuogo = NULL;
	int i, j;

	fp = fopen("citta.txt", "w");

	for (i = 0; i < nv; i++)
	{
		fprintf(fp, "%d\n", C[i]->nv);

		for (j = 0; j < C[i]->nv;j++)
		{
			arcoLuogo = C[i]->adj[j];
			while (arcoLuogo != NULL)
			{
				fprintf(fp, "%d ", arcoLuogo->key);
				arcoLuogo = arcoLuogo->next;
			}
			fprintf(fp, "-1\n");
		}
		fprintf(fp, "-2\n");

		for (j = 2; j < C[i]->nv; j++)
		{
			fprintf(fp, "%s\n", C[i]->nomeAlberghi[j]);
		}
	}

	fprintf(fp, "-5\n");

	fclose(fp);
}

t_grafoC** leggiGrafoCitta(int nv) 
{
	FILE* fp;
	int key, i, j, verticiCitta;
	char nomealbergo[20];
	t_grafoC** GC = NULL;
	t_luogo* arcoLuogo = NULL;

	GC = (t_grafoC**)malloc(sizeof(t_grafoC*) * nv);

	for (i = 0; i < nv; i++)
	{
		GC[i] = NULL;
	}

	fp = fopen("citta.txt", "r");
	if (!fp) {
		printf("Il file non esiste!");
		return NULL;
	}

	for (i = 0; i < nv; i++)
	{
		fscanf(fp, "%d", &verticiCitta);

		if (verticiCitta == -5)
			return GC;

		GC[i] = (t_grafoC*)malloc(sizeof(t_grafoC));
		GC[i]->nv = verticiCitta;
		GC[i]->adj = (t_luogo**)malloc(verticiCitta * sizeof(t_luogo*));

		GC[i]->nomeAlberghi = (char**)calloc(verticiCitta, sizeof(char*));

		for (j = 0; j < verticiCitta; j++)
		{
			GC[i]->adj[j] = NULL;
			GC[i]->nomeAlberghi[j] = (char*)calloc(20, sizeof(char));
		}

		j = 0;

		while (fscanf(fp, "%d", &key))
		{
			if (key == -1)
			{
				j++;
				continue;
			}

			if (key == -2)
				break;


			aggiungiArcoGrafoCitta(GC[i], j, key);
		}

		strcpy(GC[i]->nomeAlberghi[0], "Aereoporto");
		strcpy(GC[i]->nomeAlberghi[1], "Stazioni");


		for (j = 2; j < verticiCitta; j++)
		{
			fscanf(fp, "%s", nomealbergo);
			strcpy(GC[i]->nomeAlberghi[j], nomealbergo);
		}
	}


	fclose(fp);

	return GC;
}

void aggiungiArcoGrafoCitta(t_grafoC* G, int i, int key)
{
	t_luogo* nuovo, * e;

	printf("provo ad aggiungere %d %d\n", i, key);

	if (i<0 || i>G->nv - 1 || key<0 || key>G->nv - 1)
	{
		printf("\nArco non aggiunto, vertici richiesti invalidi(valori validi: 0 a %d)", G->nv - 1);
		return;
	}

	nuovo = (t_luogo*)malloc(sizeof(t_luogo));

	if (nuovo == NULL)
		printf("\nAllocazione fallita");
	else
	{
		nuovo->key = key;
		nuovo->next = NULL;

		if (G->adj[i] == NULL)
			G->adj[i] = nuovo;
		else
		{
			e = G->adj[i];

			while (e->next != NULL)
			{
				if (e->key == key)
				{
					free(nuovo);
					return;
				}
				e = e->next;
			}

			if (e->key == key)
			{
				free(nuovo);
				return;
			}
			e->next = nuovo;
		}
	}
}
