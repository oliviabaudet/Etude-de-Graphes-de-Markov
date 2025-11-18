#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "utils.h"

int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "");

    printf("=== ETAPE 1 : Creation manuelle du graphe ===\n");

    AdjacencyList adj = create_adjacency_list(4);

    // Sommet 1
    add_cell(&adj.array[0], 1, 0.95);
    add_cell(&adj.array[0], 2, 0.04);
    add_cell(&adj.array[0], 3, 0.01);

    // Sommet 2
    add_cell(&adj.array[1], 2, 0.90);
    add_cell(&adj.array[1], 3, 0.05);
    add_cell(&adj.array[1], 4, 0.05);

    // Sommet 3
    add_cell(&adj.array[2], 3, 0.80);
    add_cell(&adj.array[2], 4, 0.20);

    // Sommet 4
    add_cell(&adj.array[3], 1, 1.00);

    printf("\n--- Affichage du graphe cree manuellement ---\n");
    display_adjacency_list(adj);

    printf("\n--- Verification du graphe manuel ---\n");
    verifierGrapheMarkov(adj);

    printf("\n--- Generation du fichier Mermaid pour le graphe manuel ---\n");
    ecrireFichierMermaid(adj, "graphe_manuel_mermaid.txt");

    // Libération mémoire
    free_adjacency_list(adj);

    printf("\n=== Partie 2 ===\n");

    printf("\n===Affichage des classes===\n");
    AdjacencyList graphe = lireGraphe2("data/exemple_valid_step3.txt");



    t_partition partition = tarjan(graphe);

    for(int i = 0; i < partition.count; i++) {
        t_classe c = partition.classes[i];
        printf("Composante %s: {", c.name);
        for(int j = 0; j < c.count; j++) {
            printf("%d", c.vertices[j]);
            if(j < c.count-1) printf(",");
        }
        printf("}\n");
    }

    free_partition(partition);
    free_adjacency_list(graphe);

    return 0;
}
//