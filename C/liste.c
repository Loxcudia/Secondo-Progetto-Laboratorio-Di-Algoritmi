#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "grafi.h"
#include "liste.h"

void inserimentoInTesta(t_lista **L, int data)
{
	t_lista *tmp = NULL;
	
	tmp = (t_lista*)malloc(sizeof(t_lista));
	
	if(tmp == NULL) {
		printf("Allocazione fallita\n");
		return;
	}
	
	tmp->data = data;
	tmp->next = NULL;
	
	if(L == NULL) {
		*L = tmp;
	}
	else {
		tmp->next = *L;
		*L = tmp;
	}
}

int estraiMinimo(t_lista **lista, int *d)
{
	t_lista *p, *pmin, *prec = NULL, *precmin = NULL;
	int u;
	pmin = *lista;
	p = *lista;
	while(p!=NULL)
	{
		if((d[p->data] != UINT_MAX && d[p->data] < d[pmin->data]) || d[pmin->data] == UINT_MAX)
		{
			pmin = p;
			precmin = prec;
		}
		prec = p;
		p = p->next;
	}
	u = pmin->data;
	if(precmin == NULL)
		*lista = (*lista)->next;
	else
		precmin->next = pmin->next;
	free(pmin);
	return u;
}
