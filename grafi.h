#ifndef GRAFI_H
#define GRAFI_H

typedef struct arcoPrincipale
{
    int key;
    int costo;
    int distanza;
    struct arcoPrincipale *next;
}t_arcoP;

typedef struct grafoPrincipale
{
    int nv;
    int *stazioni;
    int *aereoporti;
    char **nomiCitta;
    t_arcoP **adjStazioni;
    t_arcoP **adjAereoporti;
}t_grafoP;

typedef struct arcoLuogo
{
    int key;
    struct arcoLuogo *next;
}t_luogo;

typedef struct grafoCitta
{
    int nv;
    char **nomeAlberghi;
    t_luogo **adj;
}t_grafoC;

#endif // GRAFI_H
