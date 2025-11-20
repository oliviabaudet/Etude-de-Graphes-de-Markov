#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

// =====================================================
// getID : convertit un numéro (1,2,3,...) en A, B, C, ..., Z, AA, AB...
// =====================================================
char* getID(int i) {
    char* buffer = malloc(10 * sizeof(char));
    if (!buffer) { perror("malloc"); exit(EXIT_FAILURE); }

    char temp[10];
    int index = 0;

    i--; // passage à index 0-based
    while (i >= 0) {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }

    for (int j = 0; j < index; j++)
        buffer[j] = temp[index - j - 1];

    buffer[index] = '\0';
    return buffer;
}

// ======================================================
// create_cell : crée une cellule (arête)
// =====================================================
Cell* create_cell(int destination, float probability) {
    Cell* newCell = (Cell*)malloc(sizeof(Cell));
    if (!newCell) {
        fprintf(stderr, "Erreur d allocation memoire.\n");
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
// add_cell : ajoute une cellule à la fin de la liste
// =====================================================
void add_cell(List* l, int destination, float probability) {
    Cell* newCell = create_cell(destination, probability);
    if (!l->head) {
        l->head = newCell;
        return;
    }
    Cell* tmp = l->head;
    while (tmp->next) tmp = tmp->next;
    tmp->next = newCell;
}

// =====================================================
// display_list : affiche une liste
// =====================================================
void display_list(List l) {
    Cell* tmp = l.head;
    while (tmp) {
        printf("-> (%d, %.6f) ", tmp->destination, tmp->probability);
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
    if (!adj.array) {
        fprintf(stderr, "Erreur d allocation memoire.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
        adj.array[i] = create_empty_list();
    return adj;
}

// =====================================================
// display_adjacency_list : affiche la liste d'adjacence
// =====================================================
void display_adjacency_list(AdjacencyList adj) {
    printf("===== Liste d adjacence =====\n");
    for (int i = 0; i < adj.size; i++) {
        printf("Sommet %d : ", i + 1);
        display_list(adj.array[i]);
    }
}

// =====================================================
// free_adjacency_list : libère la mémoire allouée
// =====================================================
void free_adjacency_list(AdjacencyList adj) {
    for (int i = 0; i < adj.size; i++) {
        Cell* tmp = adj.array[i].head;
        while (tmp) {
            Cell* toFree = tmp;
            tmp = tmp->next;
            free(toFree);
        }
    }
    free(adj.array);
}

// =====================================================
// lireGraphe : crée une liste d’adjacence à partir d’un fichier texte
// =====================================================
AdjacencyList lireGraphe(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rt");
    if (!fichier) {
        perror("Impossible d ouvrir le fichier");
        exit(EXIT_FAILURE);
    }

    int nbSommets, sommetDepart, sommetArrivee;
    float probabilite;

    if (fscanf(fichier, "%d", &nbSommets) != 1) {
        fprintf(stderr, "Erreur : impossible de lire le nombre de sommets.\n");
        fclose(fichier);
        exit(EXIT_FAILURE);
    }

    AdjacencyList graphe = create_adjacency_list(nbSommets);

    while (fscanf(fichier, "%d %d %f", &sommetDepart, &sommetArrivee, &probabilite) == 3) {
        if (sommetDepart < 1 || sommetDepart > nbSommets || sommetArrivee < 1 || sommetArrivee > nbSommets) {
            fprintf(stderr, "Erreur : sommet hors limites (%d -> %d)\n", sommetDepart, sommetArrivee);
            continue;
        }
        add_cell(&graphe.array[sommetDepart - 1], sommetArrivee, probabilite);
    }

    fclose(fichier);
    return graphe;
}
AdjacencyList lireGraphe2(const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "rt");
    if (!fichier) { perror("Impossible d'ouvrir le fichier"); exit(EXIT_FAILURE); }

    int nbSommets;
    if (fscanf(fichier, "%d\n", &nbSommets) != 1) {
        fprintf(stderr, "Erreur lecture nbSommets\n");
        exit(EXIT_FAILURE);
    }

    AdjacencyList graphe = create_adjacency_list(nbSommets);

    char line[128];
    while (fgets(line, sizeof(line), fichier)) {
        int depart, arrivee;
        float poids;
        if (sscanf(line, "%d %d %f", &depart, &arrivee, &poids) != 3) continue;

        if (depart < 1 || depart > nbSommets || arrivee < 1 || arrivee > nbSommets) continue;

        add_cell(&graphe.array[depart-1], arrivee, 1.0f); // poids fictif
    }

    fclose(fichier);
    return graphe;
}


// =====================================================
// verifierGrapheMarkov : vérifie les probabilités sortantes
// =====================================================
void verifierGrapheMarkov(AdjacencyList graphe) {
    int estMarkov = 1;
    for (int i = 0; i < graphe.size; i++) {
        float somme = 0.0f;
        Cell* tmp = graphe.array[i].head;
        while (tmp) {
            somme += tmp->probability;
            tmp = tmp->next;
        }
        if (somme < 0.99f || somme > 1.01f) {
            printf("Sommet %d : somme des probabilites = %.6f\n", i + 1, somme);
            estMarkov = 0;
        } else {
            printf("Sommet %d : somme des probabilites = %.6f\n", i + 1, somme);
        }
    }
    printf("\n");
    if (estMarkov)
        printf("Le graphe est un graphe de Markov.\n");
    else
        printf("Le graphe n est pas un graphe de Markov.\n");
}

// =====================================================
// ecrireFichierMermaid : génère un fichier au format Mermaid
// =====================================================
void ecrireFichierMermaid(AdjacencyList graphe, const char *nomFichier) {
    FILE *fichier = fopen(nomFichier, "wt");
    if (!fichier) {
        perror("Erreur lors de la creation du fichier Mermaid");
        exit(EXIT_FAILURE);
    }

    fprintf(fichier, "---\nconfig:\n  layout: elk\n  theme: neo\n  look: neo\n---\n\n");
    fprintf(fichier, "flowchart LR\n");

    for (int i = 0; i < graphe.size; i++)
        fprintf(fichier, "%s((%d))\n", getID(i + 1), i + 1);
    fprintf(fichier, "\n");

    for (int i = 0; i < graphe.size; i++) {
        Cell *tmp = graphe.array[i].head;
        while (tmp) {
            char* from = getID(i + 1);
            char* to = getID(tmp->destination);
            fprintf(fichier, "%s -->|%.6f|%s\n", from, tmp->probability, to);
            free(from);
            free(to);
            tmp = tmp->next;
        }
    }

    fclose(fichier);
    printf("Fichier Mermaid genere : %s\n", nomFichier);
}

t_tarjan_vertex* init_tarjan_vertice(int n) {
    t_tarjan_vertex *tab = (t_tarjan_vertex*) malloc(n * sizeof(t_tarjan_vertex));
    if (!tab) {
        fprintf(stderr, "Erreur d'allocation memoire pour TarjanVertex.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        tab[i].id = i + 1;    // identifiant 1..n
        tab[i].num = -1;      // numéro de découverte
        tab[i].lowlink = -1;  // numéro accessible
        tab[i].onStack = 0;   // pas dans la pile
    }

    return tab;
}


t_classe create_classe(const char* name) {
    t_classe c;
    strcpy(c.name, name);
    c.count = 0;
    c.capacity = 4;
    c.vertices = malloc(4 * sizeof(int)); // <-- int et non t_tarjan_vertex*
    return c;
}


void classe_add_vertex(t_classe* c, t_tarjan_vertex* v) {
    if (c->count == c->capacity) {
        c->capacity *= 2;
        c->vertices = realloc(c->vertices, c->capacity * sizeof(t_tarjan_vertex*));
    }
    c->vertices[c->count++] = v;
}

t_partition create_partition() {
    t_partition p;
    p.count = 0;
    p.capacity = 4;
    p.classes = malloc(4 * sizeof(t_classe));
    return p;
}

void partition_add_classe(t_partition* p, t_classe c) {
    if (p->count == p->capacity) {
        p->capacity *= 2;
        p->classes = realloc(p->classes, p->capacity * sizeof(t_classe));
    }
    p->classes[p->count++] = c;
}


Stack* stack_create(int capacity) {
    Stack* s = malloc(sizeof(Stack));
    s->data = malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

void stack_push(Stack* s, int v) {
    s->data[++s->top] = v;
}

int stack_pop(Stack* s) {
    return s->data[s->top--];
}

int stack_top(Stack* s) {
    return s->data[s->top];
}

int stack_empty(Stack* s) {
    return s->top == -1;
}
// Comparateur pour qsort (mettre en haut du fichier)
int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Fonction parcours corrigée
void parcours(int v_index, AdjacencyList graph, t_tarjan_vertex *tab,
              t_partition *partition, t_tarjan_vertex **stack, int *stackTop, int *num) {

    t_tarjan_vertex *v = &tab[v_index];
    v->num = *num;
    v->lowlink = *num;
    (*num)++;

    // Empiler le sommet
    stack[(*stackTop)++] = v;
    v->onStack = 1;

    // Parcours des successeurs
    Cell *tmp = graph.array[v_index].head;
    while (tmp != NULL) {
        int w_index = tmp->destination - 1;
        t_tarjan_vertex *w = &tab[w_index];

        if (w->num == -1) {
            // Successeur non visité : récursion
            parcours(w_index, graph, tab, partition, stack, stackTop, num);
            v->lowlink = (v->lowlink < w->lowlink) ? v->lowlink : w->lowlink;
        } else if (w->onStack) {
            // Successeur dans la pile
            v->lowlink = (v->lowlink < w->num) ? v->lowlink : w->num;
        }

        tmp = tmp->next;
    }

    // Racine d'une composante fortement connexe
    if (v->lowlink == v->num) {
        t_classe c = create_classe("");  // crée une nouvelle classe propre
        sprintf(c.name, "C%d", partition->count + 1);

        t_tarjan_vertex *w;
        do {
            w = stack[--(*stackTop)];
            w->onStack = 0;

            // Agrandissement du tableau si nécessaire
            if (c.count == c.capacity) {
                c.capacity *= 2;
                c.vertices = realloc(c.vertices, c.capacity * sizeof(int));
            }

            // Stocker l'id du sommet, pas le pointeur
            c.vertices[c.count++] = w->id;

        } while (w->id != v->id);  // comparer par id, pas par pointeur

        // Tri des sommets pour un affichage propre
        qsort(c.vertices, c.count, sizeof(int), cmp_int);

        // Ajouter la classe à la partition
        partition_add_classe(partition, c);
    }
}

t_partition tarjan(AdjacencyList graph) {
    int n = graph.size;

    // 1. Initialiser les sommets pour Tarjan
    t_tarjan_vertex *tab = init_tarjan_vertice(n);

    // 2. Initialiser la partition vide
    t_partition partition = create_partition();

    // 3. Initialiser la pile
    t_tarjan_vertex **stack = malloc(n * sizeof(t_tarjan_vertex*));
    if (!stack) {
        fprintf(stderr, "Erreur d'allocation memoire pour la pile.\n");
        exit(EXIT_FAILURE);
    }
    int stackTop = 0;

    // 4. Initialiser le compteur num
    int num = 0;

    // 5. Parcourir tous les sommets
    for (int i = 0; i < n; i++) {
        if (tab[i].num == -1) {
            parcours(i, graph, tab, &partition, stack, &stackTop, &num);
        }
    }

    // 6. Libérer la pile et le tableau temporaire
    free(stack);
    free(tab);

    // 7. Retourner la partition finale
    return partition;
}


void free_partition(t_partition p) {
    for (int i = 0; i < p.count; i++) {
        free(p.classes[i].vertices);  // libère le tableau des sommets de chaque classe
    }
    free(p.classes);  // libère le tableau des classes
}

int* build_class_index(t_partition partition, int nbSommets) {
    int *classOf = malloc((nbSommets + 1) * sizeof(int));

    for (int i = 0; i < partition.count; i++) {
        t_classe c = partition.classes[i];
        for (int j = 0; j < c.count; j++) {
            int v = c.vertices[j];   // identifiant réel
            classOf[v] = i;          // i = index de la classe
        }
    }

    return classOf;
}

void build_class_links(AdjacencyList g, t_partition p, int *classOf) {
    int nC = p.count;

    int **link = malloc(nC * sizeof(int*));
    for (int i = 0; i < nC; i++) {
        link[i] = calloc(nC, sizeof(int));
    }

    for (int u = 1; u <= g.size; u++) {
        int cu = classOf[u]; // classe de u

        Cell *tmp = g.array[u-1].head;
        while (tmp) {
            int v = tmp->destination;
            int cv = classOf[v];

            if (cu != cv) {
                link[cu][cv] = 1; // lien entre classes
            }
            tmp = tmp->next;
        }
    }

    // affichage :
    for (int i = 0; i < nC; i++) {
        for (int j = 0; j < nC; j++) {
            if (link[i][j]) {
                printf("%s -> %s\n", p.classes[i].name, p.classes[j].name);
            }
        }
    }
}

// =====================================================
// ÉTAPE 3 : Caractéristiques du graphe
// =====================================================
int est_absorbant(AdjacencyList g, int sommet) {
    Cell *tmp = g.array[sommet-1].head;
    if (!tmp) return 0; // aucun lien sortant -> pas absorbant

    int boucle_sur_soi = 0;

    while (tmp) {
        if (tmp->destination == sommet && tmp->probability > 0.0f)
            boucle_sur_soi = 1; // il peut rester sur lui-même
        else if (tmp->probability > 0.0f)
            return 0; // lien sortant vers un autre état => pas absorbant
        tmp = tmp->next;
    }

    return boucle_sur_soi; // vrai si boucle sur lui-même et pas de lien vers d'autres


    }

void afficher_etats_absorbants(AdjacencyList g) {
    printf("• Etats absorbants : ");
    int found = 0;

    for (int i = 1; i <= g.size; i++) {
        if (est_absorbant(g, i)) {
            printf("%d ", i);
            found = 1;
        }
    }

    if (!found) printf("Aucun");
    printf("\n");


    if (!found) printf("Aucun");
    printf("\n");
}
void classes_transitoires_persistantes(AdjacencyList g, t_partition p, int *classOf) {

    int nC = p.count;

    // matrice de lien entre classes
    int **link = malloc(nC * sizeof(int*));
    for (int i = 0; i < nC; i++)
        link[i] = calloc(nC, sizeof(int));

    // remplissage
    for (int u = 1; u <= g.size; u++) {
        int cu = classOf[u];
        for (Cell *tmp = g.array[u-1].head; tmp; tmp = tmp->next) {
            int cv = classOf[tmp->destination];
            if (cu != cv)
                link[cu][cv] = 1;
        }
    }

    // affichage
    printf("Nature des classes :\n");
    for (int i = 0; i < nC; i++) {
        int sort = 0;
        for (int j = 0; j < nC; j++)
            if (i != j && link[i][j])
                sort = 1;

        if (sort)
            printf("  %s : TRANSITOIRE\n", p.classes[i].name);
        else
            printf("  %s : PERSISTANTE\n", p.classes[i].name);
    }

    for (int i = 0; i < nC; i++) free(link[i]);
    free(link);
}
void tester_irreductibilite(t_partition p) {
    if (p.count == 1)
        printf("Le graphe est IRRÉDUCTIBLE\n");
    else
        printf("Le graphe n'est PAS irreductible (%d classes)\n", p.count);
}
void caracteristiques_graphe(AdjacencyList g, t_partition p, int *classOf) {

    printf("1) Etats absorbants :\n");
    afficher_etats_absorbants(g);

    printf("\n2) Classes transitoires / persistantes :\n");
    classes_transitoires_persistantes(g, p, classOf);

    printf("\n3) Irreductibilite :\n");
    tester_irreductibilite(p);

    printf("\n");
}


//