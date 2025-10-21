#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdio.h>
#include <stdlib.h>

typedef struct cell {
    int destination;
    float probability;
    struct cell *next;
} Cell;

typedef struct {
    Cell *head;
} list;

typedef struct {
    int size;
    List *array;
} adjacencylist;

Cell* create_cell(int destination, float probability);
List create_empty_list();
void add_cell(List* l, int destination, float probability);
#endif