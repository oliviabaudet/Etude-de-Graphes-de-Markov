#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include "utils.h"
#include "matrix.h"
#include <math.h>


int main() {
    system("chcp 65001 > nul");
    setlocale(LC_ALL, "");
    //bien lire les fichier.txt de data PARTIE 3
    setlocale(LC_NUMERIC, "C");
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
    AdjacencyList g = lireGraphe2("data/exemple_valid_step3.txt");
    t_partition partition = tarjan(g);
    // 1. affichage des classes
    printf("\n=== Les Classes ===\n");
    for (int i = 0; i < partition.count; i++) {
        printf("%s : {", partition.classes[i].name);
        for (int j = 0; j < partition.classes[i].count; j++) {
            printf("%d", partition.classes[i].vertices[j]);
            if (j < partition.classes[i].count - 1) printf(",");
        }
        printf("}\n");
    }
    // 2. Construire tableau sommet → classe
    int *classOf = build_class_index(partition, g.size);
    // 3. Construire liens entre classes
    printf("\n=== Liens entre classes ===\n");
    build_class_links(g, partition, classOf);
    // ====================================================
    // ÉTAPE 3 : caracteristiques du graphe (TRANSITOIRE / PERSISTANTE / ABSORBANTE / IRRÉDUCTIBLE)
    // ====================================================
    printf("\n=== Etape 3 : Caractéristiques du graphe ===\n");
    caracteristiques_graphe(g, partition, classOf);
    // Libération
    free(classOf);
    free_partition(partition);
    free_adjacency_list(g);




    // PARTIE 3:
    printf("\n=== PARTIE 3 ===\n");
    // Transformer la liste d'adjacence en matrice
    AdjacencyList adjmeteo = lireGraphe("data/exemple1.txt");
    display_adjacency_list(adjmeteo);
    t_matrix M = matrixAdjacencyList(adjmeteo); // transfo d'une adj list en matrice
    //afficher la matrice
    printf("\nMatrice M :\n");
    for(int i = 0; i < M.rows; i++) {
        for(int j = 0; j < M.cols; j++) {
            printf("%.2f ", M.data[i][j]);
        }
        printf("\n");
    }

    // Calcul de M^2
    t_matrix M2 = createEmptyMatrix(M.rows);
    multiplyMatrices(M, M, M2);
    printf("\nM^2 :\n");
    for(int i = 0; i < M2.rows; i++) {
        for(int j = 0; j < M2.cols; j++) {
            printf("%.2f ", M2.data[i][j]);
        }
        printf("\n");
    }

    // Création des matrices
    t_matrix M7 = createEmptyMatrix(M.rows);  // résultat final
    copyMatrix(M7, M);                         // initialise M7 = M
    // Calcul de M^7
    for(int k = 1; k < 7; k++) {              // multiplier 6 fois de plus
        t_matrix tmp = createEmptyMatrix(M.rows);
        multiplyMatrices(M7, M, tmp);         // tmp = M7 * M
        copyMatrix(M7, tmp);                   // <-- copie tmp dans M7
        freeMatrix(tmp);                        // <-- libère tmp si malloc
    }
    // Affichage de M^7
    printf("\nM^7 :\n");
    for(int i = 0; i < M7.rows; i++) {
        for(int j = 0; j < M7.cols; j++) {
            printf("%.2f ", M7.data[i][j]);
        }
        printf("\n");
    }


    // Création des matrices pour faire une boucle
    t_matrix Mprev = createEmptyMatrix(M.rows);
    copyMatrix(Mprev, M);  // Mprev = M initial
    t_matrix Mcurr = createEmptyMatrix(M.rows);
    int k = 1;
    double diff;
    do {
        multiplyMatrices(Mprev, M, Mcurr);       // Mcurr = Mprev * M
        diff = diffMatrices(Mprev, Mcurr);       // diff = somme des différences
        printf("\nDifference entre M^%d et M^%d = %.6f\n", k, k+1, diff);
        copyMatrix(Mprev, Mcurr);                // Mprev = Mcurr pour prochaine itération
        k++;
    } while(diff >= 0.01 && k < 1000);          // limite pour éviter boucle infinie

    // Affichage de la matrice stationnaire
    if (k<1000) {
        printf("\nMatrice stationnaire M^%d :\n", k);
        for(int i = 0; i < Mcurr.rows; i++) {
            for(int j = 0; j < Mcurr.cols; j++)
                printf("%.2f ", Mcurr.data[i][j]);
            printf("\n");
        }
    }
    if(diff < 0.01)
        printf("La matrice a atteint l'etat stationnaire.\n");
    else
        printf("La matrice n'a pas atteint l'etat stationnaire apres %d multiplications.\n", k);


    // Libération mémoire des matrices
    freeMatrix(Mprev);
    freeMatrix(Mcurr);
    freeMatrix(M2);
    freeMatrix(M7);

//étape 2:
    t_partition p = tarjan(adjmeteo);
    int *classof = build_class_index(p, adjmeteo.size);

    for (int c = 0; c < p.count; c++) {
        printf("\nClasse %s (taille %d) :\n",
               p.classes[c].name, p.classes[c].count);

        // 1. Extraire la sous-matrice
        t_matrix sub = subMatrix(M, p, c);

        printf("Sous-matrice :\n");
        for(int i = 0; i < sub.rows; i++){
            for(int j = 0; j < sub.cols; j++)
                printf("%.3f ", sub.data[i][j]);
            printf("\n");
        }

        // 2. Calcul de la puissance limite
        t_matrix limit = limitPower(sub);
        printf("Puissance limite :\n");
        for(int i = 0; i < limit.rows; i++){
            for(int j = 0; j < limit.cols; j++)
                printf("%.6f ", limit.data[i][j]);
            printf("\n");
        }


        // 3. Distribution stationnaire
        printStationary(limit);

        freeMatrix(sub);
        freeMatrix(limit);

    }
    freeMatrix(M);
    free(classof);
    free_partition(p);
}

