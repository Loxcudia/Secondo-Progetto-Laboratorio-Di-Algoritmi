#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "grafi.h"


#define USERNAME_LENGTH 100
#define PASSWORD_LENGTH 100

#ifndef UTENTI_H
#define UTENTI_H

typedef struct utente {
	char username[USERNAME_LENGTH];
	char password[PASSWORD_LENGTH];
	int isAdmin;
	double saldo;
}Utente;

typedef struct Coda {
    Utente utenti[100];
    char* cittaPartenza;
    char* cittaArrivo;
    int aot; //Aereo 0 o Treno 1, 2 se il suo stato � variato da non disponibile a disponibile (e viceversa. Da notificare all'utente).
    struct Coda* next;
} codaAttesa;


//PROTOTIPI FUNZIONE
Utente registrazioneUtente(); //funzione che registra un Utente su un file di testo
Utente loginUtente(); //funzione che effettua il login di un utente
void menuUtente(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti); //mostra menu Utente normale
void menuAdmin(Utente user, t_grafoP* G, t_grafoC **GC, codaAttesa *codaUtenti);  //mostra menu Admin
void aggiungiArcoMenu(t_grafoP *G); //menu per l'aggiunta di un arco al grafo principale
void rimuoviArcoMenu(t_grafoP *G); //menu per la rimozione di un arco
void inserisciCodaAttesa(codaAttesa *codaUtente, Utente user, char* partenza, char* destinazione, int aot);
codaAttesa* inserisciNodoCodaAttesa(codaAttesa* codaUtente, Utente user, char* partenza, char* destinazione, int aot);
void mostraCodaAttesa(codaAttesa* codaUtente);

#endif 