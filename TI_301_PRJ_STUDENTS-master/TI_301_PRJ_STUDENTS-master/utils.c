#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"


typedef struct cell {
    int destination;
    float probability;
    struct cell *next;
} Cell;

typedef struct {
    Cell *head;
} List;

typedef struct {
    int size;
    List *array;
} adjacencylist;

static char *getID(int i)
{
    // translate from 1,2,3, .. ,500+ to A,B,C,..,Z,AA,AB,...
    static char buffer[10];
    char temp[10];
    int index = 0;

    i--; // Adjust to 0-based index
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    // Reverse the string to get the correct order
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';

    return buffer;
}


Cell* create_cell(int destination, float probability) {
    Cell* newCell = (Cell*) malloc(sizeof(Cell));
    if (newCell == NULL) {
        printf("Memory allocation error.\n");
        return;
    }
    newCell->destination = destination;
    newCell->probability = probability;
    newCell->next = NULL;
    return newCell;

List create_empty_list() {
        List l;
        l.head = NULL;
        return l;
    }

void add_cell(List* l, int destination, float probability) {
    Cell* newCell = create_cell(destination, probability);
    newCell->next = l->head;
    l->head = newCell;
}