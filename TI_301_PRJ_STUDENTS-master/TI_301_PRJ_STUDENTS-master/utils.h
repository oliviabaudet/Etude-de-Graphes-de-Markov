#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>

// ==== Structures ====

typedef struct cell {
    int destination;
    float probability;
    struct cell *next;
} Cell;

typedef struct {
    Cell *head;
} List;

typedef struct {
    int size;
    List *array;
} AdjacencyList;

// ==== Fonctions ====

Cell* create_cell(int destination, float probability);
List create_empty_list();
void add_cell(List *l, int destination, float probability);
void display_list(List l);

AdjacencyList create_adjacency_list(int size);
void display_adjacency_list(AdjacencyList adj);
void free_adjacency_list(AdjacencyList adj);

AdjacencyList lireGraphe(const char *nomFichier);

void verifierGrapheMarkov(AdjacencyList graphe);

void ecrireFichierMermaid(AdjacencyList graphe, const char *nomFichier);

char* getID(int i);

#endif
