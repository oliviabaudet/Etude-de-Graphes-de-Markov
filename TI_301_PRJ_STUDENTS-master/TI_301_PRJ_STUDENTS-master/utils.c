#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// =====================================================
// getID : convertit un numéro (1,2,3,...) en A, B, C, ..., Z, AA, AB...
// =====================================================
char* getID(int i) {
    char* buffer = malloc(10 * sizeof(char));
    if (!buffer) { perror("malloc"); exit(EXIT_FAILURE); }

    char temp[10];
    int index = 0;

    i--; // passage à index 0-based
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    for (int j = 0; j < index; j++)
        buffer[j] = temp[index - j - 1];

    buffer[index] = '\0';
    return buffer;
}

// =====================================================
// create_cell : crée une cellule (arête)
// =====================================================
Cell* create_cell(int destination, float probability) {
    Cell* newCell = (Cell*)malloc(sizeof(Cell));
    if (!newCell) {
        fprintf(stderr, "Erreur d allocation memoire.\n");
        exit(EXIT_FAILURE);
    }
    newCell->destination = destination;
    newCell->probability = probability;
    newCell->next = NULL;
    return newCell;
}

// =====================================================
// create_empty_list : initialise une liste vide
// =====================================================
List create_empty_list() {
    List l;
    l.head = NULL;
    return l;
}

// =====================================================
// add_cell : ajoute une cellule à la fin de la liste
// =====================================================
void add_cell(List* l, int destination, float probability) {
    Cell* newCell = create_cell(destination, probability);
    if (!l->head) {
        l->head = newCell;
        return;
    }
    Cell* tmp = l->head;
    while (tmp->next) tmp = tmp->next;
    tmp->next = newCell;
}

// =====================================================
// display_list : affiche une liste
// =====================================================
void display_list(List l) {
    Cell* tmp = l.head;
    while (tmp) {
        printf("-> (%d, %.6f) ", tmp->destination, tmp->probability);
        tmp = tmp->next;
    }
    printf("\n");
}

// =====================================================
// create_adjacency_list : crée une liste d’adjacence vide
// =====================================================
AdjacencyList create_adjacency_list(int size) {
    AdjacencyList adj;
    adj.size = size;
    adj.array = (List*)malloc(size * sizeof(List));
    if (!adj.array) {
        fprintf(stderr, "Erreur d allocation memoire.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
        adj.array[i] = create_empty_list();
    return adj;
}

// =====================================================
// display_adjacency_list : affiche la liste d'adjacence
// =====================================================
void display_adjacency_list(AdjacencyList adj) {
    printf("===== Liste d adjacence =====\n");
    for (int i = 0; i < adj.size; i++) {
        printf("Sommet %d : ", i + 1);
        display_list(adj.array[i]);
    }
}

// =====================================================
// free_adjacency_list : libère la mémoire allouée
// =====================================================
void free_adjacency_list(AdjacencyList adj) {
    for (int i = 0; i < adj.size; i++) {
        Cell* tmp = adj.array[i].head;
        while (tmp) {
            Cell* toFree = tmp;
            tmp = tmp->next;
            free(toFree);
        }
    }
    free(adj.array);
}

// =====================================================
// lireGraphe : crée une liste d’adjacence à partir d’un fichier texte
// =====================================================
AdjacencyList lireGraphe(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rt");
    if (!fichier) {
        perror("Impossible d ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    int nbSommets, sommetDepart, sommetArrivee;
    float probabilite;

    if (fscanf(fichier, "%d", &nbSommets) != 1) {
        fprintf(stderr, "Erreur : impossible de lire le nombre de sommets.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    AdjacencyList graphe = create_adjacency_list(nbSommets);

    while (fscanf(fichier, "%d %d %f", &sommetDepart, &sommetArrivee, &probabilite) == 3) {
        if (sommetDepart < 1 || sommetDepart > nbSommets || sommetArrivee < 1 || sommetArrivee > nbSommets) {
            fprintf(stderr, "Erreur : sommet hors limites (%d -> %d)\n", sommetDepart, sommetArrivee);
            continue;
        }
        add_cell(&graphe.array[sommetDepart - 1], sommetArrivee, probabilite);
    }

    fclose(fichier);
    return graphe;
}

// =====================================================
// verifierGrapheMarkov : vérifie les probabilités sortantes
// =====================================================
void verifierGrapheMarkov(AdjacencyList graphe) {
    int estMarkov = 1;
    for (int i = 0; i < graphe.size; i++) {
        float somme = 0.0f;
        Cell* tmp = graphe.array[i].head;
        while (tmp) {
            somme += tmp->probability;
            tmp = tmp->next;
        }
        if (somme < 0.99f || somme > 1.01f) {
            printf("Sommet %d : somme des probabilites = %.6f\n", i + 1, somme);
            estMarkov = 0;
        } else {
            printf("Sommet %d : somme des probabilites = %.6f\n", i + 1, somme);
        }
    }
    printf("\n");
    if (estMarkov)
        printf("Le graphe est un graphe de Markov.\n");
    else
        printf("Le graphe n est pas un graphe de Markov.\n");
}

// =====================================================
// ecrireFichierMermaid : génère un fichier au format Mermaid
// =====================================================
void ecrireFichierMermaid(AdjacencyList graphe, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "wt");
    if (!fichier) {
        perror("Erreur lors de la creation du fichier Mermaid");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier, "---\nconfig:\n  layout: elk\n  theme: neo\n  look: neo\n---\n\n");
    fprintf(fichier, "flowchart LR\n");

    for (int i = 0; i < graphe.size; i++)
        fprintf(fichier, "%s((%d))\n", getID(i + 1), i + 1);
    fprintf(fichier, "\n");

    for (int i = 0; i < graphe.size; i++) {
        Cell *tmp = graphe.array[i].head;
        while (tmp) {
            char* from = getID(i + 1);
            char* to = getID(tmp->destination);
            fprintf(fichier, "%s -->|%.6f|%s\n", from, tmp->probability, to);
            free(from);
            free(to);
            tmp = tmp->next;
        }
    }

    fclose(fichier);
    printf("Fichier Mermaid genere : %s\n", nomFichier);
}
