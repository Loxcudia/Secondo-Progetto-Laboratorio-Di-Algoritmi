#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "utenti.h"
#include <ctype.h>

#define _CRT_SECURE_NO_WARNINGS
int NOTIFICA_ADMIN;


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
    scanf("%s", username);

	printf("\n-> Inserire la password(MAX %d caratteri): ", PASSWORD_LENGTH);
	fflush(stdin);
    scanf("%s", password);

	printf("\n-> Conferma la password: ");
	fflush(stdin);
    scanf("%s", password_conferma);


	while (strcmp(password, password_conferma) != 0) {
		printf("\nLe password non corrispondono, reinserisci le password");
		printf("\n-> Inserire la password(MAX %d caratteri): ", PASSWORD_LENGTH);
		fflush(stdin);
        scanf("%s", password);

		printf("\n-> Conferma la password: ");
		fflush(stdin);
        scanf("%s", password_conferma);
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
        scanf("%s", username);


		printf("\n-> Inserire la password: ");
		fflush(stdin);
        scanf("%s", password);

		f = fopen("utenti.txt", "r");

		while (s && !feof(f)) {
            fscanf(f, "%s %s %d %lf\n", user.username, user.password, &user.isAdmin, &user.saldo);

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
    int i;
    t_lista* dijkstra = NULL;

	printf("***************** MENU UTENTE *****************\n\nCiao, %s \nSaldo attuale: %.2f", user.username, user.saldo);
	while (1) {
        printf("\nOpzioni possibili:\n\n0 - Mostra tutte le mete possibili\n1 - Mostra citta con aeroporti\n2 - Mostra citta con stazioni"
               "\n3 - Prenota un viaggio\n4 - Logout\n\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &scelta);
		while (scelta != 0 && scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4) {
			printf("Valore inserito non valido! Inserire il valore: ");
			fflush(stdin);
			scanf("%d", &scelta);
		}
		switch (scelta) {
            case 0:
                system("cls||clear");
                printf("Citta\' con aeroporto: \n");
                stampaNomiCitta(G, 0);
                printf("Citta\' con stazione: \n");
                stampaNomiCitta(G, 1);
                break;

            case 1:
                system("cls||clear");
                stampaNomiCitta(G, 0);
                break;

            case 2:
                system("cls||clear");
                stampaNomiCitta(G, 1);
                break;

            case 3:
                prenotaViaggio(user, G, GC, codaUtenti);
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
		if (NOTIFICA_ADMIN == 1)
		{
			int x;
			printf("Notifica urgente: ci sono degli utenti in coda d'attesa per delle mete non raggiungibili. Le seguenti mete sono:\n");
			mostraCodaAttesa(codaUtenti);
			printf("Vuoi renderle disponibili? (No significa eliminarle)\n-0 Sì\n-1 No\n");
			scanf("%d", &x);
			if (x == 1)
			{
				if (codaUtenti)
				{
					codaAttesa* tmp = codaUtenti;
					while (tmp)
					{
						int c, d;
						printf("Destinazione: %s non raggiungibile da Arrivo: %s\n");
						printf("Inserisci il costo:\n");
						scanf("%d", &c);
						printf("Inserisci la distanza: \n");
						scanf("%d", &d);
						aggiungiArcoGrafoPrincipale(G, tmp->keyPartenza, tmp->keyArrivo, c, d, tmp->aot);
						tmp = tmp->next;
					}
				}
			}
			else
			{
				printf("OK, procedo con l'eliminazione\n");
				NOTIFICA_ADMIN = 0;
				if (codaUtenti)
				{
					codaAttesa* tmp = codaUtenti;
					while (tmp)
					{
						rimuoviArcoGrafoPrincipale(G, tmp->keyPartenza, tmp->keyArrivo, tmp->aot);
						printf("Meta %s eliminata\n", tmp->cittaArrivo);
						tmp = tmp->next;
					}
				}
			}
		}
        printf("\nOpzioni possibili:\n"
               "\n0 - Mostra tutti i viaggi possibili"
               "\n1 - Mostra tutte le citta' disponibili"
               "\n2 - Mostra viaggi in aereo\n3 - Mostra viaggi in treno"
               "\n4 - Mostra mete in attesa"
               "\n5 - Aggiungi arco\n6 - Rimuovi arco "
               "\n7 - Modifica struttura citta"
               "\n8 - Attiva/disattiva aeroporto"
               "\n9 - Attiva/disattiva stazione"
               "\n10 - Logout\n\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &scelta);

        while (scelta < 0 || scelta > 10) {
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
                modificaCittaMenu(G, GC);
                break;
            case 8:
                toggleAeroporto(G);
                break;
            case 9:
                toggleStazione(G);
                break;
            case 10:
				system("cls||clear");
				printf("Arrivederci, %s :'(\n", user.username);
				return;
		}
	}   
}

void modificaCittaMenu(t_grafoP* G, t_grafoC **GC)
{
    int i;
    int key = -1;
    int scelta;
    int u, v;

    while(1)
    {
        for(i=0; i<G->nv; i++)
            printf("\n%d - %s", i, G->nomiCitta[i]);

        printf("\n\nInserire la chiave della citta\' da modificare: ");
        scanf("%d", &key);
        fflush(stdin);

        while(key<0 || key>G->nv-1)
        {

            printf("Valore non valido!\nInserire la chiave della citta\' da modificare: ");
            scanf("%d", &key);
            fflush(stdin);
        }

        printf("\n0 - Visualizza citta\'"
               "\n1 - Aggiungi arco"
               "\n2 - Rimuovi arco"
               "\n3 - Esci"
               "\nInserire la scelta: ");
        scanf("%d", &scelta);
        fflush(stdin);

        switch(scelta)
        {
            case(0):
                system("cls||clear");
                stampaCitta(GC[key]);
                break;
            case(1):
                system("cls||clear");
                stampaCitta(GC[key]);

                printf("\nInserire il vertice di partenza: ");
                scanf("%d", &u);
                fflush(stdin);

                printf("\nInserire il vertice di arrivo: ");
                scanf("%d", &v);
                fflush(stdin);

                if(u<0 || u>GC[key]->nv-1 || v<0 || v>GC[key]->nv-1)
                    printf("\n\nVertici Invalidi\n");
                else
                {
                    aggiungiArcoGrafoCitta(GC[key], u, v);
                    salvaGrafoCitta(GC, G->nv);
                }
                break;
            case(2):
                system("cls||clear");
                stampaCitta(GC[key]);

                printf("\nInserire il vertice di partenza: ");
                scanf("%d", &u);
                fflush(stdin);

                printf("\nInserire il vertice di arrivo: ");
                scanf("%d", &v);
                fflush(stdin);

                if(u<0 || u>GC[key]->nv-1 || v<0 || v>GC[key]->nv-1)
                    printf("\n\nVertici Invalidi\n");
                else
                {
                    rimuoviArcoGrafoCitta(GC[key], u, v);
                    salvaGrafoCitta(GC, G->nv);
                }
                break;
            case(3):
                return;
        }

    }
}

void toggleAeroporto(t_grafoP* G)
{
    int i;
    int key;

    for(i=0; i<G->nv; i++)
    {
        printf("%s: aeroporto:%d\n", G->nomiCitta[i], G->aereoporti[i]);
    }

    printf("\nInserire la chiave dell'aeroporto: ");
    scanf("%d", &key);
    fflush(stdin);

    if(key<0 || key>G->nv-1)
    {
        printf("\nChiave invalida\n");
        return;
    }

    if(G->aereoporti[key]==0)
        G->aereoporti[key] = 1;
    else
        G->aereoporti[key] = 0;
}

void toggleStazione(t_grafoP* G)
{
    int i;
    int key;

    for(i=0; i<G->nv; i++)
    {
        printf("%s: stazione:%d\n", G->nomiCitta[i], G->stazioni[i]);
    }

    printf("\nInserire la chiave della stazione: ");
    scanf("%d", &key);
    fflush(stdin);

    if(key<0 || key>G->nv-1)
    {
        printf("\nChiave invalida\n");
        return;
    }

    if(G->stazioni[key]==0)
        G->stazioni[key] = 1;
    else
        G->stazioni[key] = 0;

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


void inserisciCodaAttesa(codaAttesa *codaUtente, Utente user, char* partenza, char* destinazione, int aot, int keyPartenza, int keyArrivo) {
	if (codaUtente == NULL) {
		printf("SONO NULL CODA");
		codaUtente = inserisciNodoCodaAttesa(codaUtente, user, partenza, destinazione, aot, keyPartenza, keyArrivo);
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
				p->next = inserisciNodoCodaAttesa(codaUtente, user, partenza, destinazione, aot, keyPartenza, keyArrivo);
				boolean = 1;
			}
		}
	}

}

codaAttesa* inserisciNodoCodaAttesa(codaAttesa* codaUtente, Utente user, char* partenza, char* destinazione, int aot, int keyPartenza, int keyArrivo)
{
	puts(partenza);
	codaAttesa* coda = NULL;
	coda = (codaAttesa*)malloc(sizeof(codaAttesa));
	coda->next = NULL;
	coda->utenti[0] = user;
	coda->cittaArrivo = (char*)malloc(100 * sizeof(char));
	coda->cittaPartenza = (char*)malloc(100 * sizeof(char));
	coda->keyPartenza = keyPartenza;
	coda->keyArrivo = keyArrivo;
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


void prenotaViaggio(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti)
{
    int mode;

    printf("0 - Viaggio in aereo\n1 - Viaggio in treno\nInserire la scelta: ");
    scanf("%d", &mode);
    fflush(stdin);
    while (mode < 0 || mode > 1) {
        printf("Valore non valido! Inserire la scelta: ");
        scanf("%d", &mode);
        fflush(stdin);
    }

    switch(mode)
    {
        case 0:
            viaggioInAereo(user, G, GC, codaUtenti);
            break;

        case 1:
            viaggioInTreno(user, G, GC, codaUtenti);
            break;
    }
}

void viaggioInAereo(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti)
{
    char arrivo[20], partenza[20];
    int keyArrivo = -1;
    int keyPartenza = -1;
    int i;
    int mode;
    t_lista* percorso = NULL;

    stampaNomiCitta(G, 0);
    printf("Dove ti trovi?\nInserire il nome della citta\': ");
    scanf("%s", partenza);
    fflush(stdin);

    printf("Quale meta vuoi raggiungere?\nInserire il nome della citta\': ");
    scanf("%s", arrivo);
    fflush(stdin);

    for(i=0; i<G->nv; i++)
    {
        if(strcmp(G->nomiCitta[i], partenza)==0 && G->aereoporti[i] == 1)
            keyPartenza = i;
        if(strcmp(G->nomiCitta[i], arrivo)==0 && G->aereoporti[i] == 1)
            keyArrivo = i;
    }

    if(keyPartenza == -1)
    {
        printf("Citta di partenza\' inserita non valida\n");
        return;
    }

    //inserire aggiunta della meta

    printf("0 - Viaggio economico\n1 - Viaggio breve\nInserire la scelta: ");
    scanf("%d", &mode);
    fflush(stdin);

    while(mode < 0 || mode > 1)
    {
        printf("Valore non valido!\nInserire la scelta: ");
        scanf("%d", &mode);
        fflush(stdin);
    }

    if(mode == 0)
        percorso = dijkstraGenerico(G, keyPartenza, keyArrivo, 0);

    if(mode == 1)
        percorso = dijkstraGenerico(G, keyPartenza, keyArrivo, 1);

    if(percorso == NULL)
    {
        int x;
        printf("Meta non raggiungibile.\n-0 Non inserirti in una coda di attesa\n-1 Inserisciti in una coda di attesa\n");
        scanf("%d", &x);
        if (x == 1)
        {
            NOTIFICA_ADMIN = 1;
            inserisciCodaAttesa(codaUtenti, user, arrivo, partenza, 0, keyPartenza, keyArrivo);
            printf("Aggiunto!\n");
        }
        else
            printf("Non aggiunto\n");
        return;
    }

    prenotaAlbergo(user, G, GC[keyArrivo], percorso, 0);
}

void viaggioInTreno(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti)
{
    char arrivo[20], partenza[20];
    int keyArrivo = -1;
    int keyPartenza = -1;
    int i;
    int mode;
    t_lista* percorso = NULL;

    stampaNomiCitta(G, 1);
    printf("Dove ti trovi?\nInserire il nome della citta\': ");
    scanf("%s", partenza);
    fflush(stdin);

    printf("Quale meta vuoi raggiungere?\nInserire il nome della citta\': ");
    scanf("%s", arrivo);
    fflush(stdin);

    for(i=0; i<G->nv; i++)
    {
        if(strcmp(G->nomiCitta[i], partenza)==0 && G->stazioni[i]==1)
            keyPartenza = i;
        if(strcmp(G->nomiCitta[i], arrivo)==0 && G->stazioni[i]==1)
            keyArrivo = i;
    }

    if(keyPartenza == -1)
    {
        printf("Citta di partenza\' inserita non valida\n");
        return;
    }

    printf("0 - Viaggio economico\n1 - Viaggio breve\nInserire la scelta: ");
    scanf("%d", &mode);
    fflush(stdin);

    while(mode < 0 || mode > 1)
    {
        printf("Valore non valido!\nInserire la scelta: ");
        scanf("%d", &mode);
        fflush(stdin);
    }

    if(mode == 0)
        percorso = dijkstraGenerico(G, keyPartenza, keyArrivo, 2);

    if(mode == 1)
        percorso = dijkstraGenerico(G, keyPartenza, keyArrivo, 3);

    if(percorso == NULL)
    {
        int x;
        printf("Meta non raggiungibile.\n-0 Non inserirti in una coda di attesa\n-1 Inserisciti in una coda di attesa\n");
        scanf("%d", &x);
        if (x == 1)
        {
            NOTIFICA_ADMIN = 1;
            inserisciCodaAttesa(codaUtenti, user, arrivo, partenza, 1, keyPartenza, keyArrivo);
            printf("Aggiunto!\n");
        }
        else
            printf("Non aggiunto\n");
        return;
    }

    prenotaAlbergo(user, G, GC[keyArrivo], percorso, 1);
}

void prenotaAlbergo(Utente user, t_grafoP* G, t_grafoC* GC, t_lista* percorso, int mode)
{
    char nomeAlbergo[20];
    int keyAlbergo = -1;
    int i;
    char conferma;
    t_lista* strada = NULL;

    stampaAlberghi(GC);

    printf("In quale albergo vuole risiedere?\nInserire il nome: ");
    scanf("%s", nomeAlbergo);
    fflush(stdin);

    for(i=0; i<GC->nv; i++)
    {
        if(strcmp(GC->nomeAlberghi[i], nomeAlbergo) == 0)
        {
            keyAlbergo = i;
            break;
        }
    }

    while(keyAlbergo<2 || keyAlbergo>4)
    {
        printf("Nome non valido!\n");

        printf("In quale albergo vuole risiedere?\nInserire il nome: ");
        scanf("%s", nomeAlbergo);
        fflush(stdin);

        for(i=0; i<GC->nv; i++)
        {
            if(strcmp(GC->nomeAlberghi[i], nomeAlbergo) == 1)
            {
                keyAlbergo = i;
                break;
            }
        }
    }

    strada = dijkstraAlberghi(GC, mode, keyAlbergo);


    system("cls||clear");
    stampaLista(percorso);
    printf("\n");
    stampaLista(strada);
    printf("\n");

    printf("\nConfermare(y/n)?\nInserire la scelta: ");
    scanf("%c", &conferma);
    fflush(stdin);

    while(toupper(conferma) != 'Y' && toupper(conferma) != 'N')
    {
        printf("\nRisposta non valida!\nConfermare(y/n)?\nInserire la scelta: ");
        scanf("%c", &conferma);
        fflush(stdin);
    }

    if(toupper(conferma) == 'Y')
        stampaRicevuta(user, percorso, strada, G);
    else
        return;
}

void stampaRicevuta(Utente user, t_lista* percorso, t_lista* strada, t_grafoP* G)
{
    FILE *fp = NULL;
    char nomeFile[20];

    sprintf(nomeFile, "%s.txt", user.username);
    fp = fopen(nomeFile, "a");
    fprintf(fp, "Buon Viaggio!\n");

    while(percorso->next!=NULL)
    {
        fprintf(fp, "%s -> ", G->nomiCitta[percorso->data]);
        percorso = percorso->next;
    }
    fprintf(fp, "%s\n ", G->nomiCitta[percorso->data]);

    while(strada->next!=NULL)
    {
        fprintf(fp, "%s -> ", G->nomiCitta[strada->data]);
        strada = strada->next;
    }
    fprintf(fp, "%s\n ", G->nomiCitta[strada->data]);

}




