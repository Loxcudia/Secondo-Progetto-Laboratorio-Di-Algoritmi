#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <limits.h>
#include "grafi.h"
#include "liste.h"
#include "utenti.h"


void freeArco(t_arcoP* e)
{
    if(e==NULL)
        return;

    freeArco(e->next);
    free(e);
}

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
    int scelta;
    int pin;

	fileGrafo = fopen("grafo.txt", "r");

	if (fileGrafo == NULL)
    {
		printf("Errore nella lettura del grafo\n");
        printf("\nCreare un nuovo grafo?"
               "\n0 - Si\'"
               "\n1 - No\'"
               "\nInserire la scelta: ");
        scanf("%d", &scelta);
        fflush(stdin);
        while(scelta != 0 && scelta != 1)
        {
            printf("\nValore non valido!"
                   "\nCreare un nuovo grafo?"
                   "\n0 - Si\'"
                   "\n1 - No\'"
                   "\nInserire la scelta: ");
            scanf("%d", &scelta);
            fflush(stdin);
        }
        if(scelta == 1)
            exit(0);

        if(scelta == 0)
        {
            printf("\n\nInserire il pin di amministrazione: ");
            scanf("%d", &pin);

            if(pin != 9876)
            {
                printf("\nPin errato, arrivederci");
                exit(0);
            }
            else
            {
                G = creaGrafoSenzaFile();
                return G;
            }
        }

    }
    else
	{
        if(fscanf(fileGrafo, "%d", &nv)==EOF)
        {
            printf("Errore nella lettura del grafo\n");
            printf("\nCreare un nuovo grafo?"
                   "\n0 - Si\'"
                   "\n1 - No\'"
                   "\nInserire la scelta: ");
            scanf("%d", &scelta);
            fflush(stdin);
            while(scelta != 0 && scelta != 1)
            {
                printf("\nValore non valido!"
                       "\nCreare un nuovo grafo?"
                       "\n0 - Si\'"
                       "\n1 - No\'"
                       "\nInserire la scelta: ");
                scanf("%d", &scelta);
                fflush(stdin);
            }
            if(scelta == 1)
                exit(0);

            if(scelta == 0)
            {
                printf("\n\nInserire il pin di amministrazione: ");
                scanf("%d", &pin);

                if(pin != 9876)
                {
                    printf("\nPin errato, arrivederci");
                    exit(0);
                }
                else
                {
                    G = creaGrafoSenzaFile();
                    return G;
                }
            }
        }

		G = creaGrafoPrincipale(nv);

		for (i = 0; i < nv; i++)
		{
			fscanf(fileGrafo, "%d", &presenza);
			G->aereoporti[i] = presenza;

			// printf("%d ", presenza);
		}

		for (i = 0; i < nv; i++)
		{
			fscanf(fileGrafo, "%d", &presenza);
			G->stazioni[i] = presenza;
		}

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

t_grafoP* creaGrafoSenzaFile()
{
    int nv;
    char nomeCitta[20];
    int u, v;
    int i, j;
    t_grafoP* G = NULL;
    int aot;

    printf("\nQuanti vertici avra\'(>0)?"
           "\nInserire il numero: ");
    scanf("%d", &nv);
    fflush(stdin);
    while(nv<=0)
    {
        printf("\nValore non valido!"
               "\nQuanti vertici avra\'(>0)?"
               "\nInserire il numero: ");
        scanf("%d", &nv);
        fflush(stdin);
    }

    G = creaGrafoPrincipale(nv);

    for(i=0; i<nv; i++)
    {
        printf("\nCome si chiama la citta' numero %d? "
               "\nInserire il nome: ", i+1);
        scanf("%s", nomeCitta);
        fflush(stdin);

        strcpy(G->nomiCitta[i], nomeCitta);

        printf("\n%s avra\' un aeroporto?"
               "\n0 - No"
               "\n1 - Si\'"
               "\nInserire il valore: ", nomeCitta);
        scanf("%d", &aot);
        fflush(stdin);

        while(aot!=0 && aot!=1)
        {
            printf("\nValore non valido!"
                   "\n%s avra\' un aeroporto?"
                   "\n0 - No"
                   "\n1 - Si\'"
                   "\nInserire il valore: ", nomeCitta);
            scanf("%d", &aot);
            fflush(stdin);
        }

        G->aereoporti[i] = aot;

        printf("\n%s avra\' una stazione?"
               "\n0 - No"
               "\n1 - Si\'"
               "\nInserire il valore: ", nomeCitta);
        scanf("%d", &aot);
        fflush(stdin);

        while(aot!=0 && aot!=1)
        {
            printf("\nValore non valido!"
                   "\n%s avra\' una stazione?"
                   "\n0 - No"
                   "\n1 - Si\'"
                   "\nInserire il valore: ", nomeCitta);
            scanf("%d", &aot);
            fflush(stdin);
        }

        G->stazioni[i] = aot;
    }

    salvaGrafo(G);
    salvaNomiCitta(G);
    return G;
}

void salvaNomiCitta(t_grafoP* G)
{
    FILE *fp;
    int i;

    fp = fopen("nomicitta.txt", "w");

    for(i=0; i<G->nv; i++)
        fprintf(fp, "%s\n", G->nomiCitta[i]);

    fclose(fp);
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

    fprintf(fileGrafo, "\n");

    for (i = 0; i < G->nv; i++)
		fprintf(fileGrafo, "%d ", G->stazioni[i]);

    fprintf(fileGrafo, "\n");


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
    fprintf(fileGrafo, "%d\n", -2);

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
    fprintf(fileGrafo, "%d\n", -2);

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
            {
				G->nomeAlberghi[i] = (char*)malloc(200 * sizeof(char));
                G->adj[i] = NULL;
            }
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
        for (i = 0; i < G->nv; i++)
        {
            if (G->aereoporti[i] == 0)
                continue;

            printf("Aereoporti adiacenti a %s -> ", G->nomiCitta[i]);
            e = G->adjAereoporti[i];
            while (e)
            {
                printf("%s ", G->nomiCitta[e->key]);
                ne = ne + 1;
                e = e->next;
            }
            printf("\n");
        }

        printf("\n");

        for (i = 0; i < G->nv; i++)
		{
			if (G->stazioni[i] == 0)
				continue;

            printf("Stazioni adiacenti a %s -> ", G->nomiCitta[i]);
			e = G->adjStazioni[i];
			while (e)
			{
                printf("%s ", G->nomiCitta[e->key]);
				ne = ne + 1;
				e = e->next;
			}
			printf("\n");
		}	
    }
}

void stampaGrafoCitta(t_grafoC** GC, t_grafoP *G, int stampaStazioni, int nv)
{
	//se stampaStazioni è 1 stampa le stazioni, se è 0 stampa gli aereoporti, se è 2 stampa entrambi
	int i;
	t_luogo* e;

	if (G)
	{
		for (i = 0; i < nv; i++)
            stampaCitta(GC[i], G, i);
        printf("\n");

	}
}

void stampaCitta(t_grafoC* GC, t_grafoP* G, int key)
{
	int i;
	t_luogo* l;

    printf("\n%s: \n", G->nomiCitta[key]);

    for(i = 0; i < GC->nv; i++)
	{
        l = GC->adj[i];

		if (i == 0)
		{
            printf("0 - Luoghi adiacenti all'aereoporto: ");
			while (l != NULL)
			{
                printf("%s ", GC->nomeAlberghi[l->key]);
				l = l->next;
			}
			printf("\n");
			continue;
		}

		if (i == 1)
		{
            printf("1 - Luoghi adiacenti alla stazione: ");
			while (l != NULL)
			{
                printf("%s ", GC->nomeAlberghi[l->key]);
				l = l->next;
			}
			printf("\n");
			continue;
		}

        printf("%d - Luoghi adiacenti a %s: ", i , GC->nomeAlberghi[i]);
		while (l != NULL)
		{
            printf("%s ", GC->nomeAlberghi[l->key]);
			l = l->next;
		}
		printf("\n");

	}
}

void rimuoviArcoGrafoCitta(t_grafoC* G, int u, int v)
{
    t_luogo* prev, *current;
	t_luogo* e;

    e = G->adj[u];

    if(e == NULL)
        printf("\nArco assente");

	if (e->key == v)
		G->adj[u] = e->next;
	else
	{
        {
            current = e;
            prev = NULL;

            while (current != NULL && current->key!=v) {
                prev = current;
                current = current->next;
            }

            if (current == NULL)
            {
                printf("\nArco assente");
                return;
            }

            e = prev->next;
            prev->next = e->next;
        }
        free(e);
    }
}


t_lista* dijkstraAereoportiCosto(t_grafoP* G, int s, int meta)
{
	int* d;
	int* pi;
	int i, u;
    int prec;
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

    if(pi[meta] == -1)
    {
        printf("\nLa meta %s non e\' raggiungibile da %s con un aereo\n", G->nomiCitta[meta], G->nomiCitta[s]);
        return NULL;
    }

	/*for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
    ^for da eliminare, tenuto solo per debug*/

    prec = meta;

    t_lista* percorso = NULL;

    while(prec != -1)
    {
        //printf("%s <- ", G->nomiCitta[prec]);
        if(inserimentoInPercorso(&percorso, prec) == 0)
        {
            freeLista(percorso);
            return NULL;
        }
        else
            prec = pi[prec];
    }

    return percorso;
}

t_lista* dijkstraAereoportiDistanza(t_grafoP* G, int s, int meta)
{
	int* d;
	int* pi;
	int i, u;
    int prec;
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

    if(pi[meta] == -1)
    {
        printf("\nLa meta %s non e\' raggiungibile da %s con un aereo", G->nomiCitta[meta], G->nomiCitta[s]);
		//pushCoda(G->nomiCitta[meta], G->nomiCitta[s]);
        return NULL;
    }

    /*
    for (i = 0; i < G->nv; i++)
        printf("\nd[%d] = %3d, pi[%d] = %3d\n", i, d[i], i, pi[i]);
    ^for da eliminare, tenuto solo per debug
*/

    prec = meta;

    t_lista *percorso = NULL;

    while(prec != -1)
    {
        //printf("%s <- ", G->nomiCitta[prec]);

        if(inserimentoInPercorso(&percorso, prec) == 0)
        {
            freeLista(percorso);
            return NULL;
        }
        else
            prec = pi[prec];
    }

    return percorso;
}

t_lista* dijkstraStazioniCosto(t_grafoP* G, int s, int meta)
{
	int* d;
	int* pi;
	int i, u;
    int prec;
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

    if(pi[meta] == -1)
    {
        printf("\nLa meta %s non e\' raggiungibile da %s con un treno", G->nomiCitta[meta], G->nomiCitta[s]);
        return NULL;
    }

    /*
	for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
    ^for da eliminare, tenuto solo per debug
*/

    prec = meta;

    t_lista *percorso = NULL;

    while(prec != -1)
    {
        //printf("%s <- ", G->nomiCitta[prec]);
        if(inserimentoInPercorso(&percorso, prec) == 0)
        {
            freeLista(percorso);
            return NULL;
        }
        else
            prec = pi[prec];
    }

    return percorso;
}

t_lista* dijkstraStazioniDistanza(t_grafoP* G, int s, int meta)
{
	int* d;
	int* pi;
	int i, u;
    int prec;
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

    if(pi[meta] == -1)
    {
        printf("\nLa meta %s non e\' raggiungibile da %s con un treno", G->nomiCitta[meta], G->nomiCitta[s]);
        return NULL;
    }

    /*
	for (i = 0; i < G->nv; i++)
		printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
    ^for da eliminare, tenuto solo per debug
*/

    prec = meta;

    t_lista *percorso = NULL;

    while(prec != -1)
    {
        //printf("%s <- ", G->nomiCitta[prec]);
        if(inserimentoInPercorso(&percorso, prec) == 0)
        {
            freeLista(percorso);
            return NULL;
        }
        else
            prec = pi[prec];
    }

    return percorso;
}

t_lista* dijkstraGenerico(t_grafoP* G, int s, int meta, int mode)
{
	switch (mode) {
	case(0):
        return dijkstraAereoportiCosto(G, s, meta);
		break;

	case(1):
        return dijkstraAereoportiDistanza(G, s, meta);
		break;

	case(2):
        return dijkstraStazioniCosto(G, s, meta);
		break;

	case(3):
        return dijkstraStazioniDistanza(G, s, meta);
		break;

    default:
        return NULL;
	}
}

void salvaGrafoCitta(t_grafoC** C, int nv) {
	FILE* fp;
	t_luogo* arcoLuogo = NULL;
	int i, j;

	fp = fopen("citta.txt", "w");

	for (i = 0; i < nv; i++)
	{
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

t_grafoC** leggiGrafoCitta(t_grafoP* G)
{
	FILE* fp;
    int key, i, j, k, verticiCitta;
	char nomealbergo[20];
	t_grafoC** GC = NULL;
	t_luogo* arcoLuogo = NULL;
    int scelta;
    int pin;

    GC = (t_grafoC**)malloc(sizeof(t_grafoC*) * G->nv);

    for (i = 0; i < G->nv; i++)
	{
		GC[i] = NULL;
	}

	fp = fopen("citta.txt", "r");
	if (!fp) {
        printf("\nIl file dei grafi delle citta\' non esiste!");
        printf("\nCreare un nuovo grafo?"
                "\n0 - Si\'"
                "\n1 - No\'"
                "\nInserire la scelta: ");
        scanf("%d", &scelta);
        fflush(stdin);
        while(scelta != 0 && scelta != 1)
        {
            printf("\nValore non valido!"
            "\nCreare un nuovo grafo?"
            "\n0 - Si\'"
            "\n1 - No\'"
            "\nInserire la scelta: ");
            scanf("%d", &scelta);
            fflush(stdin);
        }
        if(scelta == 1)
            exit(0);

        if(scelta == 0)
        {
            printf("\n\nInserire il pin di amministrazione: ");
            scanf("%d", &pin);
            if(pin != 9876)
            {
                printf("\nPin errato, arrivederci");
                exit(0);
            }
            else
            {
                GC = creaGrafoCittaSenzaFile(GC, G);
                return GC;
            }
        }

	}

    for (i = 0; i < G->nv; i++)
    {
        verticiCitta = 4;

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
        strcpy(GC[i]->nomeAlberghi[1], "Stazione");


        for (k = 2; k < j; k++)
        {
            fscanf(fp, "%s", nomealbergo);
            strcpy(GC[i]->nomeAlberghi[k], nomealbergo);
		}
	}


	fclose(fp);

	return GC;
}

t_grafoC** creaGrafoCittaSenzaFile(t_grafoC** GC, t_grafoP* G)
{
    int i, j;
    char nomeAlbergo[20];

    for(i=0; i<G->nv; i++)
    {
        GC[i] = (t_grafoC*)malloc(sizeof(t_grafoC));
        GC[i]->nv = 4;
        GC[i]->adj = (t_luogo**)malloc(GC[i]->nv*sizeof(t_luogo*));
        GC[i]->nomeAlberghi = (char**)malloc(GC[i]->nv*sizeof(char*));

        for(j=0; j<4; j++)
        {
            GC[i]->adj[j] = NULL;
            GC[i]->nomeAlberghi[j] = (char*)malloc(200*sizeof(char));

            if(j==0)
            {
                strcpy(GC[i]->nomeAlberghi[j], "Aeroporto");
                continue;
            }

            if(j==1)
            {
                strcpy(GC[i]->nomeAlberghi[j], "Stazione");
                continue;
            }

            printf("\nInserire il nome dell'albergo numero %d di %s: ", j-1, G->nomiCitta[i]);
            scanf("%s", nomeAlbergo);
            fflush(stdin);
            strcpy(GC[i]->nomeAlberghi[j], nomeAlbergo);
        }
    }
    salvaGrafoCitta(GC, G->nv);
    return GC;
}

void aggiungiArcoGrafoCitta(t_grafoC* G, int i, int key)
{
	t_luogo* nuovo, * e;

	if (i<0 || i>G->nv - 1 || key<0 || key>G->nv - 1)
	{
        printf("\nArco non aggiunto, vertici richiesti invalidi(valori validi: 0 a %d), %d %d invalidi", G->nv-1, i, key);
		return;
	}

	nuovo = (t_luogo*)malloc(sizeof(t_luogo));

	if (nuovo == NULL)
		printf("\nAllocazione fallita");
	else
	{
		nuovo->key = key;
		nuovo->weight = 1;
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

void stampaNomiCitta(t_grafoP* G, int mode)
{
    int i;

    //0 per stampare solo le città che hanno aeroporti, 1 per le stazioni, 2 tutto

    if(mode == 0)
        for(i=0; i<G->nv; i++)
        {
            if(G->aereoporti[i])
                printf("%s\n", G->nomiCitta[i]);
        }

    if(mode == 1)
        for(i=0; i<G->nv; i++)
        {
            if(G->stazioni[i])
                printf("%s\n", G->nomiCitta[i]);
        }

    if(mode == 2)
        for(i=0; i<G->nv; i++)
            printf("%s\n", G->nomiCitta[i]);
}

void rimuoviArcoGrafoPrincipale(t_grafoP* G, int u, int v, int mode)
{
    t_arcoP *prev, *current, *e;

    if(u<0 || u>G->nv-1 || v<0 || v>G->nv-1)
    {
        printf("\nArco non rimosso, vertici richiesti invalidi");
        return;
    }

    if(mode == 0)
    {
        e = G->adjAereoporti[u];

        if(e==NULL)
            printf("\nArco assente");

        if(e->key == v)
            G->adjAereoporti[u] = e->next;
        else
        {
            current = e;
            prev = NULL;

            while (current != NULL) {
                if(current->key==v)
                    break;

                prev = current;
                current = current->next;
            }

            if (current == NULL)
            {
                printf("\nArco assente");
                return;
            }

            if(prev)

            e = prev->next;
            prev->next = e->next;
        }
        free(e);
        e = NULL;
    }

    if(mode == 1)
    {
        e = G->adjStazioni[u];

        if(e==NULL)
            printf("\nArco assente");

        if(e->key == v)
            G->adjStazioni[u] = e->next;
        else
        {
            current = e;
            prev = NULL;

            while (current != NULL) {
                if(current->key==v)
                    break;

                prev = current;
                current = current->next;
            }

            if (current == NULL)
            {
                printf("\nArco assente");
                return;
            }

            e = prev->next;
            prev->next = e->next;
        }
        free(e);
        e = NULL;
    }
}

void stampaAlberghi(t_grafoC* GC)
{
    int i;

    for(i=2; i<GC->nv; i++)
        printf("%s\n", GC->nomeAlberghi[i]);

    printf("\n");

}

t_lista* dijkstraAlberghi(t_grafoC* GC, int mode, int albergo)
{
    int* d;
    int* pi;
    int i, u;
    int prec;
    t_lista* lista = NULL;
    t_luogo* e;

    d = (int*)calloc(sizeof(int), GC->nv);
    pi = (int*)calloc(sizeof(int), GC->nv);

    for (i = 0; i < GC->nv; i++)
    {
        d[i] = UINT_MAX;
        pi[i] = -1;
    }

    d[mode] = 0;

    for (i = 0; i < GC->nv; i++)
        inserimentoInTesta(&lista, i);

    while (lista != NULL)
    {
        u = estraiMinimo(&lista, d);
        e = GC->adj[u];

        while (e != NULL)
        {
            if (d[e->key] == UINT_MAX || d[e->key] > d[u] + e->weight)
            {
                pi[e->key] = u;
                d[e->key] = d[u] + e->weight;
            }
            e = e->next;
        }
    }

    if(pi[albergo] == -1)
    {
        printf("\nL'albergo %s non e\' raggiungibile da %s\n", GC->nomeAlberghi[albergo], GC->nomeAlberghi[mode]);
        //puts(G->nomiCitta[s]);
        //inserisciCodaAttesa(codaUtenti, user, G->nomiCitta[s], G->nomiCitta[meta], 0);
        return NULL;
    }

    /*for (i = 0; i < G->nv; i++)
        printf("\nd[%d] = %3d, pi[%d] = %3d", i, d[i], i, pi[i]);
    ^for da eliminare, tenuto solo per debug*/

    prec = albergo;

    t_lista* strada = NULL;

    while(prec != -1)
    {
        //printf("%s <- ", G->nomiCitta[prec]);
        inserimentoInTesta(&strada, prec);
        prec = pi[prec];
    }

    return strada;
}
