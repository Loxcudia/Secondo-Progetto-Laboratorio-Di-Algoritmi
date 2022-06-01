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

void menuUtente(Utente user, t_grafoP* G, t_grafoC **GC) {
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
			printf("il numero e' zero\n");
			stampaGrafoPrincipale(G);
			break;
		case 1:
			printf("il numero e' uno\n");
            stampaGrafoCitta(GC, 2, G->nv);
			break;
		case 2:
			printf("il numero e' due\n");
			break;
		case 3:
			printf("il numero e' tre\n");
			break;
		case 4:
			printf("Arrivederci, %s :'(", user.username);
			return;
		}
	}
}

void menuAdmin(Utente user, t_grafoP* G, t_grafoC **GC) {
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
		menuUtente(user, G, GC);
		return;
	}

	printf("***************** MENU ADMIN *****************\n\nCiao, %s", user.username);
	while (1) {
		printf("\nOpzioni possibili:\n\n0 - Mostra tutti i viaggi possibili\n1 - Mostra tutte le citta' disponibili\n2 - Mostra viaggi in aereo\n3 - Mostra viaggi in treno\n4 - Mostra mete in attesa\n5 - Logout\n\nInserire il valore: ");
		fflush(stdin);
		scanf("%d", &scelta);
		while (scelta != 0 && scelta != 1 && scelta != 2 && scelta != 3 && scelta != 4 && scelta != 5) {
			printf("Valore inserito non valido! Inserire il valore: ");
			fflush(stdin);
			scanf("%d", &scelta);
		}

		switch (scelta) {
		case 0:
			printf("Mostro tutti i viaggi disponibili: ");
			stampaGrafoPrincipale(G);
			break;
		case 1:
			printf("Mostra tutte le citta' disponibili");
            stampaGrafoCitta(GC, 2, G->nv);
			break;
		case 2:
			printf("il numero e' due\n");
			break;
		case 3:
			printf("il numero e' tre\n");
			break;
		case 4:
			printf("Sono 4");
			break;
		case 5:
			printf("Arrivederci, %s :'(", user.username);
			return;
		}
	}

}
