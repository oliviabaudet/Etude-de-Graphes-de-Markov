#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>
#include <stdlib.h>

typedef struct cellule {
    int arrivee;
    float proba;
    struct cellule *suiv;
} cellule;

typedef struct {
    cellule *head;
} liste;

typedef struct {
    int taille;
    liste *tab;
} liste_adjacence;

#endif