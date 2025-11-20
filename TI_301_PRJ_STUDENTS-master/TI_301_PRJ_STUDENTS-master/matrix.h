//
// Created by louis on 19/11/2025.
//
#ifndef MATRIX_H
#define MATRIX_H

#include "utils.h"
#include <stdio.h>

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



#endif


