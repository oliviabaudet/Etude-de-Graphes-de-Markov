#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// =====================================================
// getID : convertit un numéro (1,2,3,...) en A, B, C, ..., Z, AA, AB...
// =====================================================
char* getID(int i) {
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // passe à un index 0-based
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    for (int j = 0; j < index; j++) {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';
    return buffer;
}

// =====================================================
// create_cell : crée une cellule (arête)
// =====================================================
Cell* create_cell(int destination, float probability) {
    Cell* newCell = (Cell*)malloc(sizeof(Cell));
    if (newCell == NULL) {
        fprintf(stderr, "Erreur d'allocation mémoire.\n");
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
// add_cell : ajoute une cellule au début de la liste
// =====================================================
void add_cell(List* l, int destination, float probability) {
    Cell* newCell = create_cell(destination, probability);
    newCell->next = l->head;
    l->head = newCell;
}

// =====================================================
// display_list : affiche une liste
// =====================================================
void display_list(List l) {
    Cell* tmp = l.head;
    while (tmp != NULL) {
        printf("-> (%d, %.2f) ", tmp->destination, tmp->probability);
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
    if (adj.array == NULL) {
        fprintf(stderr, "Erreur d’allocation mémoire.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        adj.array[i] = create_empty_list();
    }

    return adj;
}

// =====================================================
// display_adjacency_list : affiche la liste d'adjacence
// =====================================================
void display_adjacency_list(AdjacencyList adj) {
    printf("===== Liste d’adjacence =====\n");
    for (int i = 0; i < adj.size; i++) {
        printf("Sommet %d : ", i + 1);
        display_list(adj.array[i]);
    }
}

// =====================================================
// lireGraphe : crée une liste d’adjacence à partir d’un fichier texte
// =====================================================
AdjacencyList lireGraphe(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rt"); // ouverture en lecture texte
    if (fichier == NULL) {
        perror("Impossible d’ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    int nbSommets, sommetDepart, sommetArrivee;
    float probabilite;

    // Lecture du nombre de sommets (première ligne du fichier)
    if (fscanf(fichier, "%d", &nbSommets) != 1) {
        fprintf(stderr, "Erreur : impossible de lire le nombre de sommets.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    // Création de la liste d’adjacence vide
    AdjacencyList graphe = create_adjacency_list(nbSommets);

    // Lecture des arêtes (lignes suivantes)
    while (fscanf(fichier, "%d %d %f", &sommetDepart, &sommetArrivee, &probabilite) == 3) {
        add_cell(&graphe.array[sommetDepart - 1], sommetArrivee, probabilite);
    }

    fclose(fichier);
    return graphe;
}

// =====================================================
// verifierGrapheMarkov : vérifie que la somme des probabilités sortantes
// de chaque sommet est comprise entre 0.99 et 1.01
// =====================================================
void verifierGrapheMarkov(AdjacencyList graphe) {
    int estMarkov = 1;  // indicateur global

    for (int i = 0; i < graphe.size; i++) {
        float somme = 0.0;
        Cell* tmp = graphe.array[i].head;

        // Parcours de la liste des arêtes sortantes
        while (tmp != NULL) {
            somme += tmp->probability;
            tmp = tmp->next;
        }

        // Vérification de la somme
        if (somme < 0.99 || somme > 1.01) {
            printf("Sommet %d : somme des probabilités = %.2f ❌\n", i + 1, somme);
            estMarkov = 0;
        } else {
            printf("Sommet %d : somme des probabilités = %.2f ✅\n", i + 1, somme);
        }
    }

    printf("\n");
    if (estMarkov)
        printf("✅ Le graphe est un graphe de Markov.\n");
    else
        printf("❌ Le graphe n’est pas un graphe de Markov.\n");
}
