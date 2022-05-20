#include <stdio.h>
#include "grafi.h"
#include "utenti.h"

int main()
{
	int scelta;
	Utente user;
	
	printf("******************* PROGETTO LASD 2 *******************\n\nClaudio Simonelli\nClaudio Riccio\nGiorgio Longobardo\n\n\n\n");
	printf("1 - Accedi\n0 - Registrati\n\nInserisci un valore: ");
	fflush(stdin);
	scanf("%d", &scelta);
	while(scelta != 0 && scelta != 1) {
		printf("Inserisci un valore valido: ");
		fflush(stdin);
		scanf("%d", &scelta);
	}
	
	if(scelta) user = loginUtente();
	else user = registrazioneUtente();
	
	printf("\nCiao, %s\n\n", user.username);
	if(user.isAdmin) menuAdmin(user); 
	else menuUtente(user);
}
