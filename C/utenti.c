#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "utenti.h"
#include <ctype.h>

#define _CRT_SECURE_NO_WARNINGS
int NOTIFICA_ADMIN = 0;


//DEFINZIONE FUNZIONI
Utente registrazioneUtente() {
	FILE* f;
	char username[USERNAME_LENGTH];
	char password[PASSWORD_LENGTH];
	char password_conferma[PASSWORD_LENGTH];
	Utente user;

    system("cls||clear");
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

    system("cls||clear");
	printf("\n\n\tACCESSO UTENTE\n\n");

	while (flag) {
		printf("\n\n-> Inserire l'username: ");
		fflush(stdin);
        scanf("%s", username);


		printf("\n-> Inserire la password: ");
		fflush(stdin);
        scanf("%s", password);

		f = fopen("utenti.txt", "r");

        if(f==NULL)
        {
            printf("\nNon ci sono utenti registrati, registrarsi\n");
            exit(0);
        }

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

        if(flag == 0)
            exit(0);

        if(flag == 1)
            return loginUtente();
    }
}

void menuUtente(Utente user, t_grafoP* G, t_grafoC** GC, codaAttesa *codaUtenti) {
	int scelta;
    int i;

    system("cls||clear");
    printf("***************** MENU UTENTE *****************\n\nCiao, %s", user.username);
    if (notificaUtente(codaUtenti, user.username)) 
        printf("Una delle tue mete in attesa e' diventata disponibile!");
    
	while (1) {
        printf("\nOpzioni possibili:"
               "\n"
               "\n0 - Mostra tutte le mete possibili"
               "\n1 - Mostra il grafo"
               "\n2 - Prenota un viaggio"
               "\n3 - Logout"
               "\n"
               "\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &scelta);
        while (scelta != 0 && scelta != 1 && scelta != 2 && scelta!=3) {
			printf("Valore inserito non valido! Inserire il valore: ");
			fflush(stdin);
			scanf("%d", &scelta);
		}
		switch (scelta) {
            case 0:
                system("cls||clear");
                printf("\nCitta\' con aeroporti:\n");
                stampaNomiCitta(G, 0);
                printf("\n");

                printf("\nCitta\' con stazioni:\n");
                stampaNomiCitta(G, 1);
                printf("\n");
                break;

            case(1):
                system("cls||clear");
                stampaGrafoPrincipale(G);
                break;
            case 2:
                prenotaViaggio(user, G, GC, codaUtenti);
                break;

            case 3:
                printf("Arrivederci, %s :'(\n", user.username);
                return;

        }
    }
}

void menuAdmin(Utente user, t_grafoP* G, t_grafoC** GC, codaAttesa* codaUtenti) {
	int scelta;
    int i;

    if (codaUtenti)
        NOTIFICA_ADMIN = 1;
    else
        NOTIFICA_ADMIN = 0;

    system("cls||clear");

    printf("Quale menu vuoi visualizzare?"
           "\n"
           "\n0 - Menu Admin "
           "\n1 - Menu Utente"
           "\n"
           "\nInserisci valore: ");
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

    if (NOTIFICA_ADMIN == 1)
    {
        int x;
        system("cls||clear");
        printf("\n\nNotifica urgente: ci sono degli utenti in coda d'attesa per delle mete non raggiungibili. Le seguenti mete sono:\n\n");
        mostraCodaAttesa(codaUtenti);
        printf("\nVuoi renderle disponibili/eliminarle o vuoi occupartene in un altro momento?"
               "\n0 - Le rendo disponibili o le elimino"
               "\n1 - (o qualsiasi altro numero) Me ne occupo un'altra volta"
               "\nInserisci la scelta: ");
        scanf("%d", &x);
        fflush(stdin);
        if (x == 0)
        {
            if (codaUtenti)
            {
                codaAttesa* tmp = codaUtenti;
                while (tmp)
                {
                    codaAttesa* nodoDaEliminare = NULL;
                    int c, d, y;
                    printf("\nDestinazione: %s non raggiungibile da Partenza: %s", tmp->cittaArrivo, tmp->cittaPartenza);
                    if(tmp->aot==0)
                        printf(" con aereo\n");
                    else
                        printf(" con treno\n");
                    printf("\nVuoi renderla disponibile la meta, rimuoverla o rifiutare la richiesta?\n"
                           "\n0 - Renderla Disponibile"
                           "\n1 - Eliminarla"
                           "\n2 - Rifiuta la richiesta"
                           "\nInserire la scelta: ");
                    scanf("%d", &y);
                    if (y == 0)
                    {
                        printf("\nOK, hai scelto di renderla disponibile. Ora:\n");
                        printf("\nInserisci il costo: ");
                        scanf("%d", &c);
                        printf("\nInserisci la distanza: ");
                        scanf("%d", &d);
                        aggiungiArcoGrafoPrincipale(G, tmp->keyPartenza, tmp->keyArrivo, c, d, tmp->aot);
                        aggiungiArcoGrafoPrincipale(G, tmp->keyArrivo, tmp->keyPartenza, c, d, tmp->aot);
                        salvaGrafo(G);
                        nodoDaEliminare = tmp;
                        tmp = tmp->next;
                        free(nodoDaEliminare);
                        printf("OK, aggiunta\n");
                    }

                    if(y==1)
                    {
                        printf("\nOk, hai scelto di eliminare la meta. Procedo...\n");
                        if(tmp->aot == 0)
                        {
                            freeArco(G->adjAereoporti[tmp->keyArrivo]);
                            G->adjAereoporti[tmp->keyArrivo] = NULL;

                            for(i=0; i<G->nv;i++)
                            {
                                if(i==tmp->keyArrivo)
                                    continue;
                                rimuoviArcoGrafoPrincipale(G, i, tmp->keyArrivo, 0);
                            }
                            G->aereoporti[tmp->keyArrivo] = 0;
                            salvaGrafo(G);
                            printf("\nFinisco");
                            nodoDaEliminare = tmp;
                            tmp = tmp->next;
                            free(nodoDaEliminare);
                            continue;
                        }

                        if(tmp->aot == 1)
                        {
                            freeArco(G->adjStazioni[tmp->keyArrivo]);
                            G->adjStazioni[tmp->keyArrivo] = NULL;

                            for(i=0; i<G->nv;i++)
                            {
                                if(i==tmp->keyArrivo)
                                    continue;
                                rimuoviArcoGrafoPrincipale(G, i, tmp->keyArrivo, 1);
                            }
                            G->stazioni[tmp->keyArrivo] = 0;
                            salvaGrafo(G);
                            printf("\nFinisco");
                            nodoDaEliminare = tmp;
                            tmp = tmp->next;
                            free(nodoDaEliminare);
                            continue;
                        }
                    }

                    if(y==2)
                    {
                        printf("\nOK, hai scelto di rifiutarla. Procedo...\n");
                        nodoDaEliminare = tmp;
                        tmp = tmp->next;
                        free(nodoDaEliminare);
                        printf("Rimossa\n");
                    }
                }
                codaUtenti = NULL;
            }
            FILE *fp = fopen("coda.txt", "w");
            fclose(fp);
        }
        else
            system("cls||clear");
    }


	printf("***************** MENU ADMIN *****************\n\nCiao, %s\n\n", user.username);
	while (1) {
        printf("\nOpzioni possibili:\n"
               "\n0 - Mostra tutti i viaggi possibili"
               "\n1 - Mostra tutte le citta' disponibili"
               "\n2 - Mostra mete in attesa"
               "\n3 - Aggiungi arco"
               "\n4 - Rimuovi arco "
               "\n5 - Modifica citta\'"
               "\n6 - Attiva/disattiva aeroporto"
               "\n7 - Attiva/disattiva stazione"
               "\n8 - Rinomina una citta\'"
               "\n9 - Aggiungi una citta\'"
               "\n10 - Logout"
               "\n\nInserire il valore: ");
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
                printf("Mostro tutte le citta' disponibili: \n");
                stampaGrafoCitta(GC, G, 2, G->nv);
				break;
            case 2:
				system("cls||clear");
                mostraCodaAttesa(codaUtenti);
				break;
            case 3:
				system("cls||clear");
				aggiungiArcoMenu(G);
				break;
            case 4:
				system("cls||clear");
				rimuoviArcoMenu(G);
				break;
            case 5:
                system("cls||clear");
                modificaCittaMenu(G, GC);
                break;
            case 6:
                system("cls||clear");
                toggleAeroporto(G);
                break;
            case 7:
                system("cls||clear");
                toggleStazione(G);
                break;
            case 8:
                system("cls||clear");
                rinominaCitta(G);
                break;
            case 9:
                system("cls||clear");
                aggiungiCitta(&G, &GC);
                break;
            case 10:
				system("cls||clear");
				printf("Arrivederci, %s :'(\n", user.username);
				return;
		}
	}   
}

void aggiungiCitta(t_grafoP** G, t_grafoC*** GC)
{
    char nomeCitta[20];
    char nomeAlbergo[20];
    t_arcoP** tmpAdjaerei;
    t_arcoP** tmpAdjtreni;
    int* tmpAerei;
    int* tmpTreni;
    t_grafoC **tmpGC;
    int i, a, t;
    char** tmpNomi;

    stampaNomiCitta(*G, 2);

    printf("\nInserire il nome della citta\' da inserire: ");
    scanf("%s", nomeCitta);
    fflush(stdin);

    for(i=0; i<(*G)->nv;i++)
    {
        if(strcmp(nomeCitta, (*G)->nomiCitta[i]) == 0)
        {
            printf("\nCitta\' gia\' presente, usare i menu di modifica\n");
            return;
        }
    }

    (*G)->nv = (*G)->nv+1;

    tmpNomi = (char**)calloc((*G)->nv, sizeof(char*));
    for(i=0; i<(*G)->nv-1;i++)
    {
        tmpNomi[i] = (char*)calloc(20, sizeof(char));
        strcpy(tmpNomi[i], (*G)->nomiCitta[i]);
    }
    tmpNomi[(*G)->nv-1] = (char*)calloc(20, sizeof(char));

    strcpy(tmpNomi[(*G)->nv-1], nomeCitta);
    (*G)->nomiCitta = tmpNomi;

    tmpAerei = (int*)realloc((*G)->aereoporti, (*G)->nv);
    (*G)->aereoporti = tmpAerei;

    tmpTreni = (int*)realloc((*G)->stazioni, (*G)->nv);
    (*G)->stazioni = tmpTreni;

    printf("\n%s ha un aeroporto?"
           "\n0 - No"
           "\n1 - Si"
           "\nInserire la scelta: ", nomeCitta);
    scanf("%d", &a);
    fflush(stdin);
    while(a!=0 && a!=1)
    {
        printf("\nValore non valido!"
               "\n%s ha un aeroporto?"
               "\n0 - No"
               "\n1 - Si"
               "\nInserire la scelta: ", nomeCitta);
        scanf("%d", &a);
        fflush(stdin);
    }

    printf("\n%s ha una stazione?"
           "\n0 - No"
           "\n1 - Si"
           "\nInserire la scelta: ", nomeCitta);
    scanf("%d", &t);
    fflush(stdin);

    while(t!=0 && t!=1)
    {
        printf("\nValore non valido!"
               "\n%s ha una stazione?"
               "\n0 - No"
               "\n1 - Si"
               "\nInserire la scelta: ", nomeCitta);
        scanf("%d", &t);
        fflush(stdin);
    }

    (*G)->aereoporti[(*G)->nv-1] = a;
    (*G)->stazioni[(*G)->nv-1] = t;

    tmpAdjaerei = (t_arcoP**)calloc((*G)->nv, sizeof(t_arcoP*));
    tmpAdjtreni= (t_arcoP**)calloc((*G)->nv, sizeof(t_arcoP*));

    for(i=0; i<(*G)->nv-1;i++)
    {
        tmpAdjaerei[i] = (*G)->adjAereoporti[i];
        tmpAdjtreni[i] = (*G)->adjStazioni[i];
    }
    tmpAdjaerei[(*G)->nv-1] = NULL;
    tmpAdjtreni[(*G)->nv-1] = NULL;

    (*G)->adjAereoporti = tmpAdjaerei;
    (*G)->adjStazioni = tmpAdjtreni;


    tmpGC = (t_grafoC**)calloc((*G)->nv, sizeof(t_grafoC*));

    for(i=0; i<(*G)->nv-1;i++)
    {
        tmpGC[i] = (*GC)[i];
    }

    tmpGC[(*G)->nv-1] = creaGrafoCitta(4);

    strcpy(tmpGC[(*G)->nv-1]->nomeAlberghi[0], "Aeroporto");
    strcpy(tmpGC[(*G)->nv-1]->nomeAlberghi[1], "Stazione");

    for(i=2; i<4; i++)
    {
        printf("\nQual e\' il nome dell\'albergo numero %d di %s?"
               "\nInserire il nome: ", i-1, nomeCitta);
        scanf("%s", nomeAlbergo);
        fflush(stdin);
        strcpy(tmpGC[(*G)->nv-1]->nomeAlberghi[i], nomeAlbergo);
    }
    *GC = tmpGC;

    salvaNomiCitta(*G);
    salvaGrafo(*G);
    salvaGrafoCitta(*GC, (*G)->nv);
}

void rinominaCitta(t_grafoP* G)
{
    int i;
    int citta;
    char nomeCitta[20];

    for(i=0;i<G->nv;i++)
        printf("%d - %s\n", i, G->nomiCitta[i]);

    printf("\nQuale citta\' vuoi rinominare?"
           "\nInserire l\'indice: ");
    scanf("%d", &citta);
    fflush(stdin);

    if(citta<0 || citta > G->nv-1)
    {
        printf("\nIndice non valido");
        return;
    }

    printf("\nInserire il nuovo nome di %s: ", G->nomiCitta[citta]);
    scanf("%s", nomeCitta);
    fflush(stdin);

    strcpy(G->nomiCitta[citta], nomeCitta);

    salvaNomiCitta(G);
    return;
}

void modificaCittaMenu(t_grafoP* G, t_grafoC **GC)
{
    int i;
    int key = -1;
    int scelta;
    int u, v;
    int albergo;
    char nomeAlbergo[200];

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
               "\n3 - Modifica nome albergo"
               "\n4 - Esci"
               "\nInserire la scelta: ");
        scanf("%d", &scelta);
        fflush(stdin);

        switch(scelta)
        {
            case(0):
                system("cls||clear");
                stampaCitta(GC[key], G, key);
                break;
            case(1):
                system("cls||clear");
                stampaCitta(GC[key], G, key);

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
                stampaCitta(GC[key], G, key);

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
            case 3:
                system("cls||clear");
                for(i=2;i<4; i++)
                    printf("\n%d - %s\n", i-1, GC[key]->nomeAlberghi[i]);
                printf("\nInserire l\'indice dell\'albergo da modificare: ");
                scanf("%d", &albergo);
                fflush(stdin);
                if(albergo!=1 && albergo!=2)
                    printf("\nValore invalido");
                else
                {
                    printf("\nInserire il nuovo nome di %s: ", GC[key]->nomeAlberghi[albergo+1]);
                    scanf("%s", nomeAlbergo);
                    strcpy(GC[key]->nomeAlberghi[albergo+1], nomeAlbergo);
                    salvaGrafoCitta(GC, G->nv);
                }
                break;
            case(4):
                return;
            default:
                printf("\nValore non valido!\n");
                break;
        }

    }
}

void toggleAeroporto(t_grafoP* G)
{
    int i;
    int key;

    for(i=0; i<G->nv; i++)
    {
        printf("%d - %s: aeroporto:%d\n", i, G->nomiCitta[i], G->aereoporti[i]);
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

    salvaGrafo(G);
}

void toggleStazione(t_grafoP* G)
{
    int i;
    int key;

    for(i=0; i<G->nv; i++)
    {
        printf("%d - %s: stazione:%d\n", i, G->nomiCitta[i], G->stazioni[i]);
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

    salvaGrafo(G);
}

void rimuoviArcoMenu(t_grafoP* G)
{
    int u, v;
    int mode;
    int i;
    stampaGrafoPrincipale(G);
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

    stampaGrafoPrincipale(G);
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


void inserisciCodaAttesa(codaAttesa **codaUtente, Utente user, char* partenza, char* destinazione, int aot, int keyPartenza, int keyArrivo) {
    if (*codaUtente == NULL) {
        *codaUtente = inserisciNodoCodaAttesa(*codaUtente, user, partenza, destinazione, aot, keyPartenza, keyArrivo);
        salvaCoda(*codaUtente);
	}
		
	else {
		int flag = 0;
        codaAttesa* p = *codaUtente;
		while (p != NULL) {
			if (!strcmp(partenza, p->cittaPartenza) && !strcmp(destinazione, p->cittaArrivo)) {
				for (int i = 0; i < 100; i++) {
					if (p->utenti[i].isAdmin != 0 && p->utenti[i].isAdmin != 1 && !strcmp(p->utenti[i].username, user.username)) {
						p->utenti[i] = user;
						return;
					}
				}
				flag = 1;
			}
			p = p->next;
		}
        p = *codaUtente;
		while (flag == 0 && p != NULL) {
			if (p->next == NULL) {
                p->next = inserisciNodoCodaAttesa(*codaUtente, user, partenza, destinazione, aot, keyPartenza, keyArrivo);
                salvaCoda(p->next);
				flag = 1;
			}
            p = p->next;
		}
	}

}

codaAttesa* inserisciNodoCodaAttesa(codaAttesa* codaUtente, Utente user, char* partenza, char* destinazione, int aot, int keyPartenza, int keyArrivo)
{
	codaAttesa* coda = NULL;
	coda = (codaAttesa*)malloc(sizeof(codaAttesa));
	coda->next = NULL;
	coda->utenti[0] = user;
	coda->cittaArrivo = (char*)malloc(100 * sizeof(char));
	coda->cittaPartenza = (char*)malloc(100 * sizeof(char));
	coda->keyPartenza = keyPartenza;
	coda->keyArrivo = keyArrivo;
	for (int i = 1; i < 100; i++)
		coda->utenti[i].isAdmin = -1;

	strcpy(coda->cittaArrivo, destinazione);
	strcpy(coda->cittaPartenza, partenza);
	coda->aot = aot;

    return coda;
}

void mostraCodaAttesa(codaAttesa* codaUtente) {
	if (codaUtente == NULL) {
        printf("\nNessuna meta presente nella coda.\n");
		return;
	}

	codaAttesa* p = codaUtente;
	while (p != NULL) {
        if(p->aot == 0)
            printf("Utenti in attesa per la meta %s partendo da %s in aereo:\n", p->cittaArrivo, p->cittaPartenza);

        if(p->aot == 1)
            printf("Utenti in attesa per la meta %s partendo da %s in treno:\n", p->cittaArrivo, p->cittaPartenza);

        for (int i = 0; i < 100; i++) {
            if (p->utenti[i].isAdmin >= 0)
                printf("%s \n", p->utenti[i].username);
            else continue;
		}
		p = p->next;
	}
}

void prenotaViaggio(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti)
{
    int mode;

    system("cls||clear");
    printf("\n0 - Viaggio in aereo\n1 - Viaggio in treno\n\nInserire la scelta: ");
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

    system("cls||clear");
    printf("\n");
    stampaNomiCitta(G, 0);
    printf("\nDove ti trovi?\nInserire il nome della citta\': ");
    scanf("%s", partenza);
    fflush(stdin);

    printf("\nQuale meta vuoi raggiungere?\nInserire il nome della citta\': ");
    scanf("%s", arrivo);
    fflush(stdin);

    for(i=0; i<G->nv; i++)
    {
        if(strcmp(G->nomiCitta[i], partenza)==0 && G->aereoporti[i] == 1)
            keyPartenza = i;
        if(strcmp(G->nomiCitta[i], arrivo)==0 && G->aereoporti[i] == 1)
            keyArrivo = i;
    }

    if(keyPartenza == -1 || keyArrivo == -1)
    {
        printf("Citta\' inserite non valide\n");
        return;
    }


    printf("\n0 - Viaggio economico\n1 - Viaggio breve\nInserire la scelta: ");
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
        printf("\nMeta non raggiungibile."
               "\n0 - Non inserirti in una coda di attesa"
               "\n1 - Inserisciti in una coda di attesa\nInserisci la scelta: ");
        scanf("%d", &x);
        if (x == 1)
        {
            NOTIFICA_ADMIN = 1;
            inserisciCodaAttesa(&codaUtenti, user, arrivo, partenza, 0, keyPartenza, keyArrivo);
            printf("Aggiunto!\n\n");
            mostraCodaAttesa(codaUtenti);
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

    system("cls||clear");
    printf("\n");
    stampaNomiCitta(G, 1);
    printf("\nDove ti trovi?\nInserire il nome della citta\': ");
    scanf("%s", partenza);
    fflush(stdin);

    printf("\nQuale meta vuoi raggiungere?\nInserire il nome della citta\': ");
    scanf("%s", arrivo);
    fflush(stdin);

    for(i=0; i<G->nv; i++)
    {
        if(strcmp(G->nomiCitta[i], partenza)==0 && G->stazioni[i]==1)
            keyPartenza = i;
        if(strcmp(G->nomiCitta[i], arrivo)==0 && G->stazioni[i]==1)
            keyArrivo = i;
    }

    if(keyPartenza == -1 || keyArrivo == -1)
    {
        printf("Citta inserite non valide\n");
        return;
    }

    printf("\n0 - Viaggio economico\n1 - Viaggio breve\nInserire la scelta: ");
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
        printf("\nMeta non raggiungibile.\n0 - Non inserirti in una coda di attesa\n1 - Inserisciti in una coda di attesa\nInserire la scelta: ");
        scanf("%d", &x);
        if (x == 1)
        {
            NOTIFICA_ADMIN = 1;
            inserisciCodaAttesa(&codaUtenti, user, arrivo, partenza, 1, keyPartenza, keyArrivo);
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
    int conferma;
    t_lista* strada = NULL;

    printf("\n\nScelta dell'albergo:\n\n");

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

    if(strada == NULL)
        return;

    system("cls||clear");
    stampaPercorso(percorso, G);
    printf("\n\n");
    stampaLista(strada, GC);
    printf("\n\n");

    printf("\n0 - Conferma"
           "\n1 - Annulla"
           "\nInserire la scelta: ");
    scanf("%d", &conferma);
    fflush(stdin);

    while(conferma != 0 && conferma != 1)
    {
        printf("\nValore non valido!"
               "\n0 - Conferma"
               "\n1 - Annulla"
               "\nInserire la scelta: ");
        scanf("%d", &conferma);
        fflush(stdin);
    }

    if(conferma == 0)
        stampaRicevuta(user, percorso, strada, G, GC);
    else
    {
        freeLista(percorso);
        freeLista(strada);
        return;
    }
}

void stampaRicevuta(Utente user, t_lista* percorso, t_lista* strada, t_grafoP* G, t_grafoC* GC)
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
    fprintf(fp, "%s\n", G->nomiCitta[percorso->data]);

    while(strada->next!=NULL)
    {
        fprintf(fp, "%s -> ", GC->nomeAlberghi[strada->data]);
        strada = strada->next;
    }
    fprintf(fp, "%s\n\n", G->nomiCitta[strada->data]);

    printf("\nRicevuta stampata e visibile in %s\nBuon Viaggio!", nomeFile);

    freeLista(percorso);
    freeLista(strada);
    fclose(fp);
}

codaAttesa* caricaCoda() {
    codaAttesa* coda = NULL;
    FILE* fp;
    codaAttesa* e = NULL;
    codaAttesa* testa = e;
    int c = 0, i;

    fp = fopen("coda.txt", "r");
    if (fp == NULL) {
        printf("\nIl file 'coda.txt' non esiste.\n");
        return e;
    }
    else {
        while(!feof(fp)) {
            coda = (codaAttesa*)malloc(sizeof(codaAttesa));

            if(fscanf(fp, "%d ", &c)==EOF)
                return NULL;
            for (i = 0; i < c; i++) {
                fscanf(fp, "%s ", coda->utenti[i].username);
                coda->utenti[i].isAdmin = 0;
            }
            for (; i < 100; i++) {
                coda->utenti[i].isAdmin = -1;
            }

            coda->cittaArrivo = (char*)malloc(sizeof(char));
            coda->cittaPartenza = (char*)malloc(sizeof(char));
            
            fscanf(fp, "%s %s %d %d %d\n", coda->cittaPartenza, coda->cittaArrivo, &coda->keyPartenza, &coda->keyArrivo, &coda->aot);

            
            coda->next = NULL; 
            e = accodaNodo(e, coda);        
        }

        fclose(fp);
        return e;
    }
    
}

codaAttesa* accodaNodo(codaAttesa* testa, codaAttesa* nodo) {
    if (testa == NULL) {
        testa = nodo;
        return testa;
    }
    else {
        testa->next = accodaNodo(testa->next, nodo);
        return testa;
    }
}

void salvaCoda(codaAttesa* coda) {
    FILE* fp;
    int c = 0;

    fp = fopen("coda.txt", "a");
    if (fp == NULL) {
        printf("Il file 'coda.txt' non esiste.");
        return;
    }
    else {
        for (int i = 0; i < 100; i++) {
            if (coda->utenti[i].isAdmin >= 0)
                c++;
        }

        fprintf(fp, "%d ", c);
        for (int i = 0; i < c; i++) 
            fprintf(fp, "%s ", coda->utenti[i].username);

        fprintf(fp, "%s %s %d %d %d\n", coda->cittaPartenza, coda->cittaArrivo, coda->keyPartenza, coda->keyArrivo, coda->aot);
        fclose(fp);
    }
}

int notificaUtente(codaAttesa* coda, char* username) {
    for (codaAttesa* p = coda; p != NULL; p = p->next) {
        for (int i = 0; i < 100; i++) {
            if (!strcmp(p->utenti[i].username, username) && p->utenti[i].isAdmin >= 0 && p->aot == 2)
                return 1;
        }
    }

    return 0;
}
