#include "matrix.h"
#include <math.h>



t_matrix createEmptyMatrix(int n) {
    t_matrix M;
    M.rows = n; //nombre de ligne de la matrice vide
    M.cols = n;
    M.data = malloc(n * sizeof(double*)); //on alloue la place pour y mettre les futurs probas de type double*
    for (int i = 0; i < n; i++) {
        M.data[i] = calloc(n, sizeof(double)); // on parcourt et on initialise à 0
    }
    return M;
}

t_matrix matrixAdjacencyList(AdjacencyList adj){
    int n = adj.size;                   // nombre d'états
    t_matrix M = createEmptyMatrix(n);  // matrice créée et initialisée à 0
    for (int i = 0; i < n; i++) {
        Cell *tmp = adj.array[i].head; //début de la liste
        while (tmp != NULL) {
            int j = tmp->destination - 1; //car commence à 0
            if (j >= 0 && j < n) {
                M.data[i][j] = tmp->probability; // on met la proba à sa place dans la matrice
                }
            tmp = tmp->next;
        }
    }
    return M;
}

void copyMatrix(t_matrix dest, t_matrix src) {
    // on vérifie les tailles
    if (dest.rows != src.rows || dest.cols != src.cols) {
        printf("Erreur: copie impossible: matrices de tailles differentes.\n");
        return;
    }
    int n = src.rows; //nombre de ligne de la matrice à copier pour ensuite parcourrir
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            dest.data[i][j] = src.data[i][j]; // copie
        }
    }
}

int multiplyMatrices(t_matrix A,t_matrix B, t_matrix C) {
    // Vérifier que les tailles sont compatibles
    if (A.rows != B.rows || A.cols != B.cols || C.rows != A.rows || C.cols != A.cols) {
        printf("Erreur: copie impossible: matrices de tailles differentes.\n");
        return -1;
    }
    int n = A.rows; // nombre de ligne de la 1ère matrice à multiplier
    // Multiplication à faire : C[i][j] = somme(A[i][k] * B[k][j])
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double sum = 0.0;
            for (int k = 0; k < n; k++) {
                sum += A.data[i][k] * B.data[k][j];// k varie d'abord sur tout i pour A et j pour B puis on incrémente
            }
            C.data[i][j] = sum; //somme faite -> donnée à C
        }
    }
    return 0;
}

double diffMatrices(t_matrix A,t_matrix B) {
    if (A.rows != B.rows || A.cols != B.cols) {
        printf("Erreur: copie impossible: matrices de tailles differentes.\n");
        return -1.0;
    }
    int n = A.rows;
    double diff = 0.0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            diff +=fabs(A.data[i][j] - B.data[i][j]);
        }
    }
    return diff;
}

void freeMatrix(t_matrix A) {
    for(int i = 0; i < A.rows; i++)
        free(A.data[i]);
    free(A.data);
}


t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index)//Extracts a submatrix corresponding to a specific component of a graph partition
{
    // 1) Récupérer la classe
    t_classe classe = part.classes[compo_index];
    int k = classe.count;

    // 2) Créer une matrice k×k correctement initialisée
    t_matrix sub = createEmptyMatrix(k);

    // 3) Construire la sous-matrice
    for (int i = 0; i < k; i++) {
        int original_row = classe.vertices[i] - 1;  // -1 car identifiants 1..n
        for (int j = 0; j < k; j++) {
            int original_col = classe.vertices[j] - 1;
            sub.data[i][j] = matrix.data[original_row][original_col];
        }
    }

    return sub;
}

t_matrix limitPower(t_matrix M)
{
    int n = M.rows;
    t_matrix A = createEmptyMatrix(n);
    t_matrix B = createEmptyMatrix(n);
    copyMatrix(A, M);

    double epsilon = 1e-12;
    int maxIter = 1000;
    int iter = 0;
    double diff = epsilon + 1; // pour démarrer la boucle

    // ← Ici tu remplaces while(1)
    while(diff > epsilon && iter < maxIter)
    {
        multiplyMatrices(A, M, B); // B = A * M

        // Normaliser les lignes
        for (int i = 0; i < n; i++) {
            double sum = 0;
            for (int j = 0; j < n; j++)
                sum += B.data[i][j];
            if (sum > 0)
                for (int j = 0; j < n; j++)
                    B.data[i][j] /= sum;
        }

        diff = diffMatrices(A, B);
        copyMatrix(A, B);
        iter++;
    }

    if(iter == maxIter)
        printf("Attention : puissance limite non convergente après %d itérations.\n", maxIter);

    freeMatrix(B);
    return A;
}






void printStationary(t_matrix limit)
{
    int n = limit.rows;
    printf("Distribution stationnaire : ");
    for (int j = 0; j < n; j++) {
        printf("%.6f ", limit.data[0][j]);
    }
    printf("\n");
}




