#include <stdio.h>
#include "utils.h"

int main() {
    printf("=== Test de création manuelle ===\n");

    AdjacencyList adj = create_adjacency_list(4);

    add_cell(&adj.array[0], 1, 0.95);
    add_cell(&adj.array[0], 2, 0.04);
    add_cell(&adj.array[0], 3, 0.01);

    add_cell(&adj.array[1], 2, 0.90);
    add_cell(&adj.array[1], 3, 0.05);
    add_cell(&adj.array[1], 4, 0.05);

    add_cell(&adj.array[2], 3, 0.80);
    add_cell(&adj.array[2], 4, 0.20);

    add_cell(&adj.array[3], 1, 1.00);

    display_adjacency_list(adj);

    printf("\n=== Lecture du graphe à partir d’un fichier ===\n");

    AdjacencyList graphe = lireGraphe("data/exemple1.txt");

    display_adjacency_list(graphe);

    printf("\n=== Vérification du graphe ===\n");
    verifierGrapheMarkov(graphe);

    return 0;
}
