#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include "grafi.h"
#include "liste.h"

void freeLista(t_lista* lista)
{
    if(lista == NULL)
        return;

    freeLista(lista->next);
    free(lista);
}

void inserimentoInTesta(t_lista** L, int data)
{
	t_lista* tmp = NULL;

	tmp = (t_lista*)malloc(sizeof(t_lista));

	if (tmp == NULL) {
		printf("Allocazione fallita\n");
		return;
	}

	tmp->data = data;
	tmp->next = NULL;

	if (L == NULL) {
		*L = tmp;
	}
	else {
		tmp->next = *L;
		*L = tmp;
	}
}

int inserimentoInPercorso(t_lista** L, int data)
{
    if (*L == NULL)
    {
        t_lista* tmp = NULL;

        tmp = (t_lista*)malloc(sizeof(t_lista));

        if (tmp == NULL) {
            printf("Allocazione fallita\n");
            return 0;
        }

        tmp->data = data;
        tmp->next = NULL;

        *L = tmp;
        return 1;
    }

    if((*L)->data == data)
        return 0;

    if((*L)->next == NULL)
    {
        t_lista* tmp = NULL;

        tmp = (t_lista*)malloc(sizeof(t_lista));

        if (tmp == NULL) {
            printf("Allocazione fallita\n");
            return 0;
        }

        tmp->data = data;
        tmp->next = NULL;
        (*L)->next = tmp;
        return 1;
    }

    return inserimentoInPercorso(&(*L)->next, data);
}


int estraiMinimo(t_lista** lista, int* d)
{
	t_lista* p, * pmin, * prec = NULL, * precmin = NULL;
	int u;
	pmin = *lista;
	p = *lista;
	while (p != NULL)
	{
        if ((d[p->data] != UINT_MAX && d[p->data] < d[pmin->data]) || d[pmin->data] == UINT_MAX )
		{
			pmin = p;
			precmin = prec;
		}
		prec = p;
		p = p->next;
	}
	u = pmin->data;
	if (precmin == NULL)
		*lista = (*lista)->next;
	else
		precmin->next = pmin->next;
	free(pmin);
	return u;
}

void stampaLista(t_lista* lista, t_grafoC* GC)
{
    if (!lista)
		return;

    if(lista->next == NULL)
    {
        printf("%s\n", GC->nomeAlberghi[lista->data]);
        return;
    }

    printf("%s -> ", GC->nomeAlberghi[lista->data]);
    stampaLista(lista->next, GC);
}

void stampaPercorso(t_lista* lista, t_grafoP* G) {
    if (!lista)
        return;

    if(lista->next == NULL)
    {
        printf("%s -> ", G->nomiCitta[lista->data]);
        return;
    }

    stampaPercorso(lista->next, G);
    printf("%s -> ", G->nomiCitta[lista->data]);

}





