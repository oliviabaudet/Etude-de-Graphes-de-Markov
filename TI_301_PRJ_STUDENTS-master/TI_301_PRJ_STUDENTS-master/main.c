#include <stdio.h>
#include "utils.h"

int main() {
    printf("=== Test lecture de fichier ===\n");

    // On testera bientôt readGraph("data/exemple1.txt");
    // Mais pour l'instant, tu peux tester juste la création manuelle :

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

    printf("=== Lecture du graphe à partir d’un fichier ===\n");

    // Lecture automatique du fichier de données
    AdjacencyList graphe = lireGraphe("data/exemple1.txt");

    // Affichage du graphe lu
    display_adjacency_list(graphe);

    return 0;
}
