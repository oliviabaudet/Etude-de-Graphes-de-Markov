//
// Created by louis on 19/11/2025.
//
#ifndef MATRIX_H
#define MATRIX_H

#include "utils.h"
#include <stdio.h>

/**
 * @brief Structure représentant une matrice de nombres réels
 *
 * @struct t_matrix
 * @var t_matrix::rows Nombre de lignes de la matrice
 * @var t_matrix::cols Nombre de colonnes de la matrice
 * @var t_matrix::data Tableau 2D contenant les éléments de la matrice
 */
typedef struct {
    int rows;
    int cols;
    double **data;
} t_matrix;

t_matrix createEmptyMatrix(int n);
t_matrix matrixAdjacencyList(AdjacencyList adj);
void copyMatrix(t_matrix dest, t_matrix src);
int multiplyMatrices(t_matrix A, t_matrix B, t_matrix C);
double diffMatrices(t_matrix A, t_matrix B);
void freeMatrix(t_matrix M);
t_matrix subMatrix(t_matrix matrix, t_partition part, int compo_index);
t_matrix limitPower(t_matrix M);
void printStationary(t_matrix limit);



#endif


