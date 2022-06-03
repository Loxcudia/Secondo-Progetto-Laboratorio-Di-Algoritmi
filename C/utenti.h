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
    int keyPartenza;
    int keyArrivo;
    int aot; //Aereo 0 o Treno 1, 2 se il suo stato è variato da non disponibile a disponibile (e viceversa. Da notificare all'utente).
    struct Coda* next;
} codaAttesa;


//PROTOTIPI FUNZIONE
Utente registrazioneUtente(); //funzione che registra un Utente su un file di testo
Utente loginUtente(); //funzione che effettua il login di un utente
void menuUtente(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti); //mostra menu Utente normale
void menuAdmin(Utente user, t_grafoP* G, t_grafoC **GC, codaAttesa *codaUtenti);  //mostra menu Admin
void aggiungiArcoMenu(t_grafoP *G); //menu per l'aggiunta di un arco al grafo principale
void rimuoviArcoMenu(t_grafoP *G); //menu per la rimozione di un arco
void inserisciCodaAttesa(codaAttesa *codaUtente, Utente user, char* partenza, char* destinazione, int aot, int keyPartenza, int keyArrivo);
codaAttesa* inserisciNodoCodaAttesa(codaAttesa* codaUtente, Utente user, char* partenza, char* destinazione, int aot, int keyPartenza, int keyArrivo);
void mostraCodaAttesa(codaAttesa* codaUtente);
void prenotaViaggio(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti);
void viaggioInAereo(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti);
void viaggioInTreno(Utente user, t_grafoP *G, t_grafoC **GC, codaAttesa *codaUtenti);
void prenotaAlbergo(Utente user, t_grafoP* G, t_grafoC* GC, t_lista* percorso, int mode);
void modificaCittaMenu(t_grafoP* G, t_grafoC **GC);
void toggleAeroporto(t_grafoP* G);
void toggleStazione(t_grafoP* G);
void stampaRicevuta(Utente user, t_lista* percorso, t_lista* strada, t_grafoP* G);


#endif 

