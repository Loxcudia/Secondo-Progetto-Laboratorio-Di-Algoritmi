#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "utenti.h"
#define _CRT_SECURE_NO_WARNINGS

//DEFINZIONE FUNZIONI
Utente registrazioneUtente() {
	FILE* f;
	char username[USERNAME_LENGTH];
	char password[PASSWORD_LENGTH];
	char password_conferma[PASSWORD_LENGTH];
	Utente user;


	printf("\n\n\tREGISTRAZIONE UTENTE \n");
	printf("\n\n1 - Admin\n\n0 - Utente\n\nInserisci il valore: ");
	fflush(stdin);
	scanf("%d", &user.isAdmin);
	while (user.isAdmin != 0 && user.isAdmin != 1) {
		printf("Valore non valido! Inserisci un valore compreso fra 0 e 1: ");
		fflush(stdin);
		scanf("%d", &user.isAdmin);
	}

	printf("\n\n-> Inserire l'username (MAX %d caratteri): ", USERNAME_LENGTH);
	fflush(stdin);
	scanf("%s", &username);

	printf("\n-> Inserire la password(MAX %d caratteri): ", PASSWORD_LENGTH);
	fflush(stdin);
	scanf("%s", &password);

	printf("\n-> Conferma la password: ");
	fflush(stdin);
	scanf("%s", &password_conferma);


	while (strcmp(password, password_conferma) != 0) {
		printf("\nLe password non corrispondono, reinserisci le password");
		printf("\n-> Inserire la password(MAX %d caratteri): ", PASSWORD_LENGTH);
		fflush(stdin);
		scanf("%s", &password);

		printf("\n-> Conferma la password: ");
		fflush(stdin);
		scanf("%s", &password_conferma);
	}

	strcpy(user.username, username);
	strcpy(user.password, password);
	user.saldo = 1000; //valore di default

	f = fopen("utenti.txt", "a");
	fprintf(f, "%s %s %d %lf\n", user.username, user.password, user.isAdmin, user.saldo);
	fclose(f);

	printf("\nUtente \"%s\" registrato", username);

	return user;
}

Utente loginUtente() {
	FILE* f;
	Utente user;
	char username[USERNAME_LENGTH + 1];
	char password[PASSWORD_LENGTH + 1];
	int s = 1, flag = 1;

	printf("\n\n\tACCESSO UTENTE\n\n");

	while (flag) {
		printf("\n\n-> Inserire l'username: ");
		fflush(stdin);
		scanf("%s", &username);


		printf("\n-> Inserire la password: ");
		fflush(stdin);
		scanf("%s", &password);

		f = fopen("utenti.txt", "r");

		while (s && !feof(f)) {
			fscanf(f, "%s %s %d %lf\n", &user.username, &user.password, &user.isAdmin, &user.saldo);

			if (!strcmp(user.username, username) && !strcmp(user.password, password)) return user;

		}

		fclose(f);
		printf("\n\nUtente non trovato. Riprovare?\n\n1 - Si\n0 - No\n\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &flag);
		while (flag != 0 && flag != 1) {
			printf("Valore inserito non valido! Inserire il valore: ");
			fflush(stdin);
			scanf("%d", &flag);
		}
	}
}

void menuUtente(Utente user, t_grafoP* G, t_grafoC** GC, codaAttesa *codaUtenti) {
	int scelta;

	printf("***************** MENU UTENTE *****************\n\nCiao, %s \nSaldo attuale: %.2f", user.username, user.saldo);
	while (1) {
		printf("\nOpzioni possibili:\n\n0 - Mostra tutti i viaggi possibili \n1 - Mostra viaggi in aereo\n2 - Mostra viaggi in treno\n3 - Mostra mete in attesa\n4 - Logout\n\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &scelta);
		while (scelta != 0 && scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4) {
			printf("Valore inserito non valido! Inserire il valore: ");
			fflush(stdin);
			scanf("%d", &scelta);
		}
		switch (scelta) {
		case 0:
			stampaGrafoPrincipale(G);
			printf("-0 Viaggio in aereo\n-1 Viaggio in treno\n");
			int boh;
			scanf("%d", &boh);
			if (boh != 0 && boh != 2)
			{
				printf("Errore\n");
				continue;
			}
			printf("Quale meta vuoi raggiungere?\n");
			char arrivo[20], partenza[20];
			scanf("%s", &arrivo);
			printf("E da dove parti?\n");
			scanf("%s", &partenza);
			int arr, part;
			int modalita;
			printf("Vuoi:\n-0 Viaggiare piu economicamente\n-1 Piu velocemente?\n");
			scanf("%d", &modalita);
			if (modalita != 0 && modalita != 1)
			{
				printf("Errore\n");
				continue;
			}
			if (boh == 0)
			{
				for (int i = 0; i < G->nv; i++)
				{
					if (strcmp(G->nomiCitta[i], "") == 0)
						break;
					if (G->aereoporti[i] == 1)
					{
						if (strcmp(G->nomiCitta[i], arrivo) == 0)
							arr = i;
						if (strcmp(G->nomiCitta[i], partenza) == 0)
							part = i;
					}
				}
                dijkstraGenerico(G, part, arr, modalita);
				printf("Alberghi della citta:\n");
				for (int i = 0; i < GC[arr]->nv; i++)
					printf("%s\n", GC[arr]->nomeAlberghi[i]);

			}
			else if (boh == 1)
			{
				printf("Vuoi:\n-0 Viaggiare piu economicamente\n-1 Piu velocemente?\n");
				scanf("%d", &modalita);
				if (modalita != 0 && modalita != 1)
				{
					printf("Errore\n");
					continue;
				}
				for (int i = 0; i < G->nv; i++)
				{
					if (strcmp(G->nomiCitta[i], "") == 0)
						break;
					if (G->stazioni[i] == 1)
					{
						if (strcmp(G->nomiCitta[i], arrivo) == 0)
							arr = i;
						if (strcmp(G->nomiCitta[i], partenza) == 0)
							part = i;
					}
				}
                dijkstraGenerico(G, part, arr, modalita);
				printf("Alberghi della citta:\n");
				for (int i = 0; i < GC[arr]->nv; i++)
					printf("%s\n", GC[arr]->nomeAlberghi[i]);
			}
			break;
		case 1:
			stampaGrafoPrincipale(G);
			printf("Quale meta vuoi raggiungere?\n");
			scanf("%s", &arrivo);
			printf("E da dove parti?\n");
			scanf("%s", &partenza);
			printf("Vuoi:\n-0 Viaggiare piu economicamente\n-1 Piu velocemente?\n");
			scanf("%d", &modalita);
			if (modalita != 0 && modalita != 1)
			{
				printf("Errore\n");
				continue;
			}
			for (int i = 0; i < G->nv; i++)
			{
				if (strcmp(G->nomiCitta[i], "") == 0)
					break;
				if (G->aereoporti[i] == 1)
				{
					if (strcmp(G->nomiCitta[i], arrivo) == 0)
						arr = i;
					if (strcmp(G->nomiCitta[i], partenza) == 0)
						part = i;
				}
			}
            dijkstraGenerico(G, part, arr, modalita);
			printf("Alberghi della citta:\n");
			for (int i = 0; i < GC[arr]->nv; i++)
				printf("%s\n", GC[arr]->nomeAlberghi[i]);
			break;
		case 2:
			stampaGrafoPrincipale(G);
			printf("Quale meta vuoi raggiungere?\n");
			scanf("%s", &arrivo);
			printf("E da dove parti?\n");
			scanf("%s", &partenza);
			printf("Vuoi:\n-0 Viaggiare piu economicamente\n-1 Piu velocemente?\n");
			scanf("%d", &modalita);
			if (modalita != 0 && modalita != 1)
			{
				printf("Errore\n");
				continue;
			}
			for (int i = 0; i < G->nv; i++)
			{
				if (strcmp(G->nomiCitta[i], "") == 0)
					break;
				if (G->stazioni[i] == 1)
				{
					if (strcmp(G->nomiCitta[i], arrivo) == 0)
						arr = i;
					if (strcmp(G->nomiCitta[i], partenza) == 0)
						part = i;
				}
			}
            dijkstraGenerico(G, part, arr, modalita);
			printf("Alberghi della citta:\n");
			for (int i = 0; i < GC[arr]->nv; i++)
				printf("%s\n", GC[arr]->nomeAlberghi[i]);
			break;
		case 3:
			mostraCodaAttesa(codaUtenti);
			break;
		case 4:
            printf("Arrivederci, %s :'(\n", user.username);
			return;
		}
	}
}

void menuAdmin(Utente user, t_grafoP* G, t_grafoC** GC, codaAttesa* codaUtenti) {
	int scelta;

	printf("Quale menu vuoi visualizzare?\n\n0 - Menu Admin \n1 - Menu Utente\n\nInserisci valore: ");
	fflush(stdin);
	scanf("%d", &scelta);

	while (scelta != 0 && scelta != 1) {
		printf("Valore inserito non valido! Inserire un valore valido: ");
		fflush(stdin);
		scanf("%d", &scelta);
	}
	if (scelta) {
		menuUtente(user, G, GC, codaUtenti);
		return;
	}

	printf("***************** MENU ADMIN *****************\n\nCiao, %s", user.username);
	while (1) {
        printf("\nOpzioni possibili:\n\n0 - Mostra tutti i viaggi possibili\n1 - Mostra tutte le citta' disponibili\n2 - Mostra viaggi in aereo\n3 - Mostra viaggi in treno\n4 - Mostra mete in attesa"
               "\n5 - Aggiungi arco\n6 - Rimuovi arco \n7 - Logout\n\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &scelta);

        while (scelta < 0 || scelta > 7) {
			printf("Valore inserito non valido! Inserire il valore: ");
			fflush(stdin);
			scanf("%d", &scelta);
		}

		switch (scelta) {
		case 0:
            system("cls||clear");
            printf("Mostro tutti i viaggi disponibili: \n");
			stampaGrafoPrincipale(G);
			break;
		case 1:
            system("cls||clear");
            printf("Mostra tutte le citta' disponibili");
			stampaGrafoCitta(GC, 2, G->nv);
			break;
		case 2:
            system("cls||clear");
            printf("il numero e' due\n");
			break;
		case 3:
            system("cls||clear");
            printf("il numero e' tre\n");
			break;
		case 4:
            system("cls||clear");
			printf("Sono 4");
			break;
        case 5:
            system("cls||clear");
            aggiungiArcoMenu(G);
            break;
        case 6:
            system("cls||clear");
            rimuoviArcoMenu(G);
            break;
        case 7:
            system("cls||clear");
            printf("Arrivederci, %s :'(\n", user.username);
			return;
		}
	}   
}

void rimuoviArcoMenu(t_grafoP* G)
{
    int u, v;
    int mode;
    int i;

    printf("\n1 - Rimuovi un arco dagli aeroporti\n2 - Rimuovi un arco dalle stazioni\n3 - Annulla\n\nInserire il valore: ");
    fflush(stdin);
    scanf("%d", &mode);
    while(mode < 0 || mode > 3)
    {
        printf("\nValore inserito non valido! Inserire il valore: ");
        fflush(stdin);
        scanf("%d", &mode);
    }

    if(mode == 3)
        return;

    if(mode == 1)
    {
        stampaGrafoPrincipale(G);

        for(i=0; i<G->nv; i++)
        {
            if(G->aereoporti[i])
               printf("%d - %s\n", i, G->nomiCitta[i]);
        }


        printf("\nInserisci il vertice di partenza: ");
        fflush(stdin);
        scanf("%d", &u);
        while(u < 0 || u > G->nv-1 || G->aereoporti[u]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &u);
        }

        printf("\nInserisci il vertice di arrivo: ");
        fflush(stdin);
        scanf("%d", &v);
        while(v < 0 || v > G->nv-1 || G->aereoporti[v]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &v);
        }

        rimuoviArcoGrafoPrincipale(G, u, v, 0);
        salvaGrafo(G);
    }

    if(mode == 2)
    {
        stampaGrafoPrincipale(G);

        for(i=0; i<G->nv; i++)
        {
            if(G->stazioni[i])
               printf("%d - %s\n", i, G->nomiCitta[i]);
        }


        printf("\nInserisci il vertice di partenza: ");
        fflush(stdin);
        scanf("%d", &u);
        while(u < 0 || u > G->nv-1 || G->stazioni[u]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &u);
        }

        printf("\nInserisci il vertice di arrivo: ");
        fflush(stdin);
        scanf("%d", &v);
        while(v < 0 || v > G->nv-1 || G->stazioni[v]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &v);
        }

        rimuoviArcoGrafoPrincipale(G, u, v, 1);
        salvaGrafo(G);
    }
}



void aggiungiArcoMenu(t_grafoP* G)
{
    int u, v, distanza, costo;
    int mode;
    int i;

    printf("\n1 - Aggiungi un arco agli aeroporti\n2 - Aggiungi un arco alle stazioni\n3 - Annulla\n\nInserire il valore: ");
    fflush(stdin);
    scanf("%d", &mode);
    while(mode < 0 || mode > 3)
    {
        printf("\nValore inserito non valido! Inserire il valore: ");
        fflush(stdin);
        scanf("%d", &mode);
    }

    if(mode == 3)
        return;

    if(mode == 1)
    {
        stampaGrafoPrincipale(G);

        for(i=0; i<G->nv; i++)
        {
            if(G->aereoporti[i])
               printf("%d - %s\n", i, G->nomiCitta[i]);
        }

        printf("\nInserisci il vertice di partenza: ");
        fflush(stdin);
        scanf("%d", &u);
        while(u < 0 || u > G->nv-1 || G->aereoporti[u]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &u);
        }

        printf("\nInserisci il vertice di arrivo: ");
        fflush(stdin);
        scanf("%d", &v);
        while(v < 0 || v > G->nv-1 || G->aereoporti[v]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &v);
        }

        printf("\nInserisci il costo: ");
        fflush(stdin);
        scanf("%d", &costo);
        while(costo <= 0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &costo);
        }

        printf("\nInserisci la distanza: ");
        fflush(stdin);
        scanf("%d", &distanza);
        while(distanza <= 0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &distanza);
        }

        aggiungiArcoGrafoPrincipale(G, u, v, costo, distanza, 0);
        salvaGrafo(G);
    }

    if(mode == 2)
    {
        stampaGrafoPrincipale(G);

        for(i=0; i<G->nv; i++)
        {
            if(G->stazioni[i])
               printf("%d - %s\n", i, G->nomiCitta[i]);
        }

        printf("\nInserisci il vertice di partenza: ");
        fflush(stdin);
        scanf("%d", &u);
        while(u < 0 || u > G->nv-1 || G->stazioni[u]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &u);
        }

        printf("\nInserisci il vertice di arrivo: ");
        fflush(stdin);
        scanf("%d", &v);
        while(v < 0 || v > G->nv-1 || G->stazioni[v]==0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &v);
        }

        printf("\nInserisci il costo: ");
        fflush(stdin);
        scanf("%d", &costo);
        while(costo <= 0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &costo);
        }

        printf("\nInserisci la distanza: ");
        fflush(stdin);
        scanf("%d", &distanza);
        while(distanza <= 0)
        {
            printf("Valore inserito non valido! Inserisci il valore: ");
            fflush(stdin);
            scanf("%d", &distanza);
        }

        aggiungiArcoGrafoPrincipale(G, u, v, costo, distanza, 1);
        salvaGrafo(G);
    }

}


void inserisciCodaAttesa(codaAttesa *codaUtente, Utente user, char* partenza, char* destinazione, int aot) {
	if (codaUtente == NULL) {
		printf("SONO NULL CODA");
		codaUtente = inserisciNodoCodaAttesa(codaUtente, user, partenza, destinazione, aot);
	}
		
	else {
		int boolean = 0;
		codaAttesa* p = codaUtente;
		while (p != NULL) {
			if (!strcmp(partenza, p->cittaPartenza) && !strcmp(destinazione, p->cittaArrivo)) {
				for (int i = 0; i < 100; i++) {
					if (p->utenti[i].isAdmin != 0 && p->utenti[i].isAdmin != 1 && !strcmp(p->utenti[i].username, user.username)) {
						p->utenti[i] = user;
						return;
					}
				}
				boolean = 1;
			}
			if(p->next != NULL)
				p = p->next;
		}
		p = codaUtente;
		while (!boolean && p != NULL) {
			p = p->next;
			if (p->next == NULL) {
				p->next = inserisciNodoCodaAttesa(codaUtente, user, partenza, destinazione, aot);
				boolean = 1;
			}
		}
	}

}

codaAttesa* inserisciNodoCodaAttesa(codaAttesa* codaUtente, Utente user, char* partenza, char* destinazione, int aot)
{
	puts(partenza);
	codaAttesa* coda = NULL;
	coda = (codaAttesa*)malloc(sizeof(codaAttesa));
	coda->next = NULL;
	coda->utenti[0] = user;
	coda->cittaArrivo = (char*)malloc(100 * sizeof(char));
	coda->cittaPartenza = (char*)malloc(100 * sizeof(char));

	for (int i = 1; i < 100; i++)
		coda->utenti[i].isAdmin = 3;

	strcpy(coda->cittaArrivo, destinazione);
	strcpy(coda->cittaPartenza, partenza);
	coda->aot = aot;
	return coda;
}

void mostraCodaAttesa(codaAttesa* codaUtente) {
	if (codaUtente == NULL) {
		printf("Nessuna meta presente nella coda.");
		return;
	}

	codaAttesa* p = codaUtente;
	while (p != NULL) {
		printf("Utenti in attesa per la meta %s partendo da %s in aereo:\n", p->cittaArrivo, p->cittaPartenza);
		for (int i = 0; i < 100; i++) {
			printf("%s \n", p->utenti->username);
		}
		p = p->next;
	}
}








