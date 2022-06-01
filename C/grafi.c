#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <limits.h>
#include "grafi.h"
#include "liste.h"

void aggiungiArcoGrafoPrincipale(t_grafoP *G, int u, int v, int costo, int distanza, int mode)
{
	 t_arcoP *nuovo, *e;

	//manca il check del grafo non inizializzato

    if(u<0 || u>G->nv-1 || v<0 || v>G->nv-1)
    {
        printf("\nArco non aggiunto, vertici richiesti invalidi(valori validi: 0 a %d)", G->nv-1);
        return;
    }

    if(costo<0)
    {
        printf("\nArco non aggiunto, costo invalido");
        return;
    }
    
    if(distanza<0)
    {
        printf("\nArco non aggiunto, costo invalido");
        return;
    }

    nuovo = (t_arcoP*)malloc(sizeof(t_arcoP));
    
    if(nuovo == NULL)
        printf("\nAllocazione fallita");
    else
    {
        nuovo->key = v;
        nuovo->costo = costo;
        nuovo->distanza = distanza;
        nuovo->next = NULL;
		
		if(mode==0)
		{
			if(G->adjAereoporti[u] == NULL)
	            G->adjAereoporti[u] = nuovo;
	        else
	        {
	            e = G->adjAereoporti[u];
	            while(e->next!=NULL)
	            {
	                if(e->key == nuovo->key)
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
    	
    	if(mode==1)
    	{
			if(G->adjStazioni[u] == NULL)
	            G->adjStazioni[u] = nuovo;
	        else
	        {
	            e = G->adjStazioni[u];
	            while(e->next!=NULL)
	            {
	                if(e->key == nuovo->key)
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
	FILE *fileGrafo = NULL;
	int nv;
	t_grafoP* G = NULL;
	int i = 0;
	int presenza;
	int v, costo, distanza;
	
	fileGrafo = fopen("grafo.txt", "r");
		
	if(fileGrafo == NULL)
		printf("Errore nella lettura del grafo\n");
	else
	{
		fscanf(fileGrafo, "%d", &nv);
		G = creaGrafoPrincipale(nv);
		
		for(i=0; i<nv; i++)
		{
			fscanf(fileGrafo, "%d", &presenza);
			G->aereoporti[i] = presenza;

           // printf("%d ", presenza);
		}
		
        printf("\n");

		for(i=0; i<nv; i++)
		{
			fscanf(fileGrafo, "%d", &presenza);
			G->stazioni[i] = presenza;

           // printf("%d ", presenza);
        }

        printf("\n");
		
		i = 0;
		
        fscanf(fileGrafo, "%d", &v);

		while(v != -2) //-2 flag per segnalare la fine degli archi di una lista di adiacenza
		{
			if(v == -1) //-1 flag per segnalare la fine degli archi di un vertice
			{
				i++;
                fscanf(fileGrafo, "%d", &v);
				continue;
			}
			else
			{
                fscanf(fileGrafo,"%d %d ", &costo, &distanza);
				aggiungiArcoGrafoPrincipale(G, i, v, costo, distanza, 0); //0 vuol dire aggiungilo alla lista di adiacenza degli aereoporti
			}
			
            fscanf(fileGrafo, "%d", &v);
		}
		
        i = 0;

        fscanf(fileGrafo, "%d ", &v);
		
		while(v != -2)
		{
            if(v == -1)
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
	return G;
}

void salvaGrafo(t_grafoP *G)
{
	FILE *fileGrafo = NULL;
	int i;
	t_arcoP *a;
	
	fileGrafo = fopen("grafo.txt", "w");
	
	if(fileGrafo == NULL)
	{
		printf("Errore nell\'apertura del file per il salvataggio del grafo\n");
		return;
	}

	fprintf(fileGrafo, "%d\n", G->nv);
	
	for(i=0; i<G->nv; i++)
		fprintf(fileGrafo, "%d ", G->aereoporti[i]);
	printf("\n");
	
	for(i=0; i<G->nv; i++)
		fprintf(fileGrafo, "%d ", G->stazioni[i]);
	printf("\n");
	
	for(i=0; i<G->nv; i++)
	{
		a = G->adjAereoporti[i];
		
		while(a!=NULL)
		{
			fprintf(fileGrafo, "%d %d %d ", a->key, a->costo, a->distanza);
			a = a->next;
		}
		fprintf(fileGrafo, "%d\n", -1);
	}
	fprintf(fileGrafo, "\n%d", -2);
	
	for(i=0; i<G->nv; i++)
	{
		a = G->adjStazioni[i];
		
		while(a!=NULL)
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
			if (G->aereoporti[i] == 0)
				continue;
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

void stampaGrafoCitta(t_grafoC* G)
{
	int i, ne = 0, j = 0;
	t_luogo* e;
	if (G)
	{
		printf("Il grafo ha %d vertici\n", G->nv);
		for (i = 2; i < G->nv; i++)
		{
			e = G->adj[i];
			if (G->adj[0] != NULL)
			{
				printf("Stazione della citta: %d\n", G->adj[0]->key);
				ne++;
			}
			if (G->adj[1] != NULL)
			{
				printf("Aereoporto della citta: %d\n", G->adj[1]->key);
				ne++;
			}
			printf("Alberghi della città:\n");
			while (e)
			{
				printf("%d - %s", e->key, G->nomeAlberghi[j]);
				ne++;
				e = e->next;
				j++;
			}
			printf("\n");
		}
		printf("Il grafo ha %d archi\n", ne);
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


void dijkstraAereoportiCosto(t_grafoP *G, int s)
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
		e = G->adjAereoporti[u];
		
		while(e!=NULL)
		{
			if(d[e->key] == UINT_MAX || d[e->key] > d[u] + e->costo)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->costo;
			}
			e = e->next;
		}
	}
	
	for(i=0; i<G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
		
	return;
}

void dijkstraAereoportiDistanza(t_grafoP *G, int s)
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
		e = G->adjAereoporti[u];
		
		while(e!=NULL)
		{
			if(d[e->key] == UINT_MAX || d[e->key] > d[u] + e->distanza)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->distanza;
			}
			e = e->next;
		}
	}
	
	for(i=0; i<G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
		
	return;
}

void dijkstraStazioniCosto(t_grafoP *G, int s)
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
		e = G->adjStazioni[u];
		
		while(e!=NULL)
		{
			if(d[e->key] == UINT_MAX || d[e->key] > d[u] + e->costo)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->costo;
			}
			e = e->next;
		}
	}
	
	for(i=0; i<G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
		
	return;
}

void dijkstraStazioniDistanza(t_grafoP *G, int s)
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
		e = G->adjStazioni[u];
		
		while(e!=NULL)
		{
			if(d[e->key] == UINT_MAX || d[e->key] > d[u] + e->distanza)
			{
				pi[e->key] = u;
				d[e->key] = d[u] + e->distanza;
			}
			e = e->next;
		}
	}
	
	for(i=0; i<G->nv; i++)
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

void salvaGrafoCitta(t_grafoC* C) {
	FILE* fp;
	t_luogo *arcoLuogo = C->adj;

	fp = fopen("citta.txt", "a");

	if (!fp) {
		printf("Il file non esiste!");
		return;
	}
	fprintf(fp, "\n%d\n", C->nv);

	for (int i = 0; i < C->nv; i++) {
		fprintf(fp, "%s %d\n", C->nomeAlberghi[i], arcoLuogo->key);
		arcoLuogo = arcoLuogo->next;
	}
	fclose(fp);

	return;
}

t_grafoC* leggiGrafoCitta() {
	FILE* fp;
	int nv, key, nadj;
	t_grafoC* grafoCitta = NULL;
	t_luogo *arcoLuogo = NULL;

	fp = fopen("citta.txt", "r");
	if (!fp) {
		printf("Il file non esiste!");
		return NULL;
	}
	
	fscanf(fp, "%d\n", &nv);
	
	grafoCitta = creaGrafoCitta(nv);
	
	arcoLuogo = (t_luogo*)malloc(sizeof(t_luogo));

	for (int i = 0; i < grafoCitta->nv; i++) {
		fscanf(fp, "%s %d", grafoCitta->nomeAlberghi[i], &nadj);
		printf("Numero adiacenze: %d\n", nadj);
		for (int j = 0; j < nadj; j++) {
			fscanf(fp, "%d", &key);
			printf("Chiave: %d\n", key);
			inserimentoInTesta(&arcoLuogo, key);
		}
		stampaLista(arcoLuogo);
		grafoCitta->adj[i] = arcoLuogo;
		printf("\nStampo: %d", grafoCitta->adj[i]->key);
	}

	fclose(fp);

	return grafoCitta;
}
