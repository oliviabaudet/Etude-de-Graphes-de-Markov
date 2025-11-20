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

typedef struct {
    int id;          // identifiant du sommet (1..n)
    int num;         // numéro de découverte (index) : initialement -1
    int lowlink;     // numéro accessible : initialement -1
    int onStack;     // booléen (0 ou 1) : est-il dans la pile ?
} t_tarjan_vertex;

typedef struct {
    char name[10];  // nom de la composante (C1, C2…)
    int *vertices;  // tableau dynamique d’identifiants de sommets
    int count;      // nombre de sommets dans la classe
    int capacity;   // capacité actuelle du tableau
} t_classe;


typedef struct {
    t_classe* classes;
    int count;
    int capacity;
} t_partition;

typedef struct {
    int* data;
    int top;
    int capacity;
} Stack;


// ==== Fonctions ====

Cell* create_cell(int destination, float probability);
List create_empty_list();
void add_cell(List *l, int destination, float probability);
void display_list(List l);

AdjacencyList create_adjacency_list(int size);
void display_adjacency_list(AdjacencyList adj);
void free_adjacency_list(AdjacencyList adj);

AdjacencyList lireGraphe(const char *nomFichier);
AdjacencyList lireGraphe2(const char *nomFichier);

void verifierGrapheMarkov(AdjacencyList graphe);

void ecrireFichierMermaid(AdjacencyList graphe, const char *nomFichier);

char* getID(int i);
// Initialise le tableau des t_tarjan_vertex
t_tarjan_vertex* init_tarjan_vertice(int n);

// Gestion des classes et partitions
t_classe create_classe(const char* name);
void classe_add_vertex(t_classe* c, t_tarjan_vertex* v);
t_partition create_partition();
void partition_add_classe(t_partition* p, t_classe c);
void free_partition(t_partition p);

// Gestion de la pile
Stack* stack_create(int capacity);
void stack_push(Stack* s, int v);
int stack_pop(Stack* s);
int stack_top(Stack* s);
int stack_empty(Stack* s);

// Algorithme de Tarjan
void parcours(int v_index, AdjacencyList graph, t_tarjan_vertex *tab, t_partition *partition,
              t_tarjan_vertex **stack, int *stackTop, int *num);

t_partition tarjan(AdjacencyList graph);
// Construit un tableau qui donne la classe de chaque sommet (1..n)
int* build_class_index(t_partition partition, int nbSommets);

// Construit les liens entre classes (graphe réduit) et les affiche
void build_class_links(AdjacencyList g, t_partition p, int *classOf);
void caracteristiques_graphe(AdjacencyList g, t_partition p, int *classOf);





#endif
