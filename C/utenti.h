#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "grafi.h"
#define USERNAME_LENGTH 100
#define PASSWORD_LENGTH 100

typedef struct utente {
	char username[USERNAME_LENGTH];
	char password[PASSWORD_LENGTH];
	int isAdmin;
	double saldo;
}Utente;

//PROTOTIPI FUNZIONE
Utente registrazioneUtente(); //funzione che registra un Utente su un file di testo
Utente loginUtente(); //funzione che effettua il login di un utente
void menuUtente(Utente user, t_grafoP *G, t_grafoC **GC); //mostra menu Utente normale
void menuAdmin(Utente user, t_grafoP* G, t_grafoC **GC);  //mostra menu Admin
