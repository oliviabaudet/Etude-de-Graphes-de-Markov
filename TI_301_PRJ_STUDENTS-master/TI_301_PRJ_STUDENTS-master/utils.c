#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}


Cell* create_cell(int destination, float probability) {
    Cell* newCell = (Cell*) malloc(sizeof(Cell));
    if (newCell == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    newCell->destination = destination;
    newCell->probability = probability;
    newCell->next = NULL;
    return newCell;

List create_empty_list() {
        List l;
        l.head = NULL;
        return l;
    }

void add_cell(List* l, int destination, float probability) {
        Cell* newCell = create_cell(destination, probability);
        newCell->next = l->head;
        l->head = newCell;
    }
void displayListe(Liste l) {
    Cellule* tmp = l.head;
    while (tmp != NULL) {
        printf("-> (%d, %.2f) ", tmp->sommet, tmp->proba);
        tmp = tmp->suiv;
    }
    printf("\n");
}
void AdjListe(Liste l) {
    Cellule* tmp = l.head;
    while (tmp != NULL) {
        printf("-> (%d, %.2f) ", tmp->sommet, tmp->proba);
        tmp = tmp->suiv;
    }
    printf("\n");
}
    ListeAdj creerListeAdj(int taille) {
    ListeAdj ladj;
    ladj.taille = taille;

    // allocation du tableau de listes
    ladj.tab = (Liste*)malloc(taille * sizeof(Liste));
    if (ladj.tab == NULL) {
        fprintf(stderr, "Erreur d’allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    // initialisation de chaque liste à vide
    for (int i = 0; i < taille; i++) {
        ladj.tab[i] = creerListe();
    }

    return ladj;
}
void displayListeAdj(ListeAdj ladj) {
    printf("===== Liste d’adjacence =====\n");
    for (int i = 0; i < ladj.taille; i++) {
        printf("Sommet %d : ", i);
        afficherListe(ladj.tab[i]);
    }
}