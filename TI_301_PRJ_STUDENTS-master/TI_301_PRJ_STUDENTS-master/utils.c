#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

/**
 * @brief Convertit un nombre en identifiant alphabétique
 *
 * Convertit un entier positif en une chaîne de caractères représentant
 * un identifiant alphabétique selon le format suivant :
 * 1 -> A, 2 -> B, ..., 26 -> Z, 27 -> AA, 28 -> AB, etc.
 *
 * @param i Entier positif à convertir (1-based)
 * @return Chaîne de caractères allouée dynamiquement contenant l'identifiant
 * @note La mémoire retournée doit être libérée par l'appelant
 * @warning La fonction termine le programme si l'allocation échoue
 *
 * @example
 * char* id = getID(1);   // Retourne "A"
 * char* id = getID(26);  // Retourne "Z"
 * char* id = getID(27);  // Retourne "AA"
 */

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


/**
 * @brief Crée une nouvelle cellule de liste chaînée
 *
 * Cette fonction alloue dynamiquement une nouvelle cellule et initialise
 * ses champs avec les valeurs fournies.
 *
 * @param destination Identifiant du sommet de destination
 * @param probability Probabilité associée à la transition
 * @return Pointeur vers la nouvelle cellule créée
 * @warning La fonction termine le programme si l'allocation échoue
 *
 * @see Cell Structure de données représentant une cellule
 *
 * @note La cellule créée doit être libérée avec free() quand elle n'est plus utilisée
 */
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

/**
 * @brief Crée une nouvelle liste chaînée vide
 *
 * Initialise une nouvelle structure List avec une tête de liste nulle,
 * représentant ainsi une liste vide.
 *
 * @return Une structure List initialisée et vide
 *
 * @see List Structure de données représentant une liste chaînée
 */
List create_empty_list() {
    List l;
    l.head = NULL;
    return l;
}

/**
 * @brief Ajoute une nouvelle cellule à la fin d'une liste chaînée
 *
 * Cette fonction crée une nouvelle cellule avec les paramètres donnés
 * et l'ajoute à la fin de la liste. Si la liste est vide, la nouvelle
 * cellule devient la tête de liste.
 *
 * @param l Pointeur vers la liste à modifier
 * @param destination Identifiant du sommet de destination à ajouter
 * @param probability Probabilité associée à la transition
 *
 * @note La fonction utilise create_cell() pour l'allocation de la nouvelle cellule
 * @warning La liste doit être initialisée avant l'appel
 *
 * @see create_cell
 * @see List
 * @see Cell
 */
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

/**
 * @brief Affiche le contenu d'une liste chaînée
 *
 * Parcourt la liste et affiche pour chaque cellule son sommet de destination
 * et sa probabilité associée au format "-> (destination, probabilité)".
 * Un retour à la ligne est ajouté à la fin de l'affichage.
 *
 * @param l Liste chaînée à afficher
 *
 * @note Le format d'affichage des probabilités utilise 6 décimales
 * @note Si la liste est vide, seul un retour à la ligne est affiché
 *
 * @see List
 * @see Cell
 */
void display_list(List l) {
    Cell* tmp = l.head;
    while (tmp) {
        printf("-> (%d, %.6f) ", tmp->destination, tmp->probability);
        tmp = tmp->next;
    }
    printf("\n");
}

/**
 * @brief Crée une liste d'adjacence vide de taille donnée
 *
 * Alloue dynamiquement un tableau de listes chaînées représentant
 * une liste d'adjacence pour un graphe de 'size' sommets. Chaque liste
 * est initialisée comme une liste vide.
 *
 * @param size Nombre de sommets du graphe
 * @return Une structure AdjacencyList initialisée
 * 
 * @warning La fonction termine le programme si l'allocation échoue
 * @note La mémoire allouée doit être libérée avec la fonction appropriée
 * quand la liste d'adjacence n'est plus utilisée
 *
 * @see AdjacencyList Structure représentant une liste d'adjacence
 * @see create_empty_list
 */
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

/**
 * @brief Affiche le contenu complet d'une liste d'adjacence
 *
 * Affiche un en-tête suivi de chaque sommet et de sa liste de successeurs.
 * Pour chaque sommet i, affiche "Sommet i : " suivi de sa liste d'adjacence.
 *
 * @param adj Structure de liste d'adjacence à afficher
 *
 * @see display_list
 * @see AdjacencyList
 */
void display_adjacency_list(AdjacencyList adj) {
    printf("===== Liste d adjacence =====\n");
    for (int i = 0; i < adj.size; i++) {
        printf("Sommet %d : ", i + 1);
        display_list(adj.array[i]);
    }
}

/**
 * @brief Libère la mémoire allouée pour une liste d'adjacence
 *
 * Libère récursivement toutes les cellules de chaque liste d'adjacence,
 * puis libère le tableau des listes.
 *
 * @param adj Structure de liste d'adjacence à libérer
 */
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

/**
 * @brief Crée une liste d'adjacence à partir d'un fichier texte
 *
 * Le format du fichier attendu est :
 * - Première ligne : nombre de sommets n
 * - Lignes suivantes : triplets "depart arrivee probabilite"
 *
 * @param nomFichier Chemin du fichier à lire
 * @return Structure AdjacencyList construite à partir du fichier
 *
 * @warning Termine le programme en cas d'erreur de lecture/ouverture du fichier
 */
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
/**
 * @brief Variante de lecture de graphe depuis un fichier
 *
 * Similaire à lireGraphe() mais utilise des poids unitaires (1.0)
 * au lieu des probabilités du fichier.
 *
 * @param nomFichier Chemin du fichier à lire
 * @return Structure AdjacencyList construite à partir du fichier
 */
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

/**
 * @brief Vérifie si le graphe est un graphe de Markov
 *
 * Vérifie pour chaque sommet si la somme des probabilités sortantes
 * est égale à 1 (à une tolérance de ±0.01 près).
 * Affiche les résultats pour chaque sommet et la conclusion finale.
 *
 * @param graphe Structure de liste d'adjacence à vérifier
 */
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

/**
 * @brief Génère une représentation Mermaid du graphe
 *
 * Crée un fichier au format Mermaid contenant une représentation visuelle
 * du graphe orienté avec les probabilités sur les arcs.
 *
 * @param graphe Structure de liste d'adjacence à convertir
 * @param nomFichier Nom du fichier de sortie à créer
 *
 * @warning Termine le programme en cas d'erreur d'écriture du fichier
 */
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

/**
 * @brief Initialise un tableau de sommets pour l'algorithme de Tarjan
 *
 * @param n Nombre de sommets à initialiser
 * @return Tableau de t_tarjan_vertex initialisé
 * 
 * @note Les sommets sont numérotés de 1 à n
 * @warning Termine le programme en cas d'échec d'allocation
 */
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

/**
 * @brief Crée une nouvelle classe d'équivalence
 *
 * @param name Nom de la classe à créer
 * @return Structure t_classe initialisée
 */
t_classe create_classe(const char* name) {
    t_classe c;
    strcpy(c.name, name);
    c.count = 0;
    c.capacity = 4;
    c.vertices = malloc(c.capacity * sizeof(int)); // <-- int et non t_tarjan_vertex*
    return c;
}

/**
 * @brief Ajoute un sommet à une classe d'équivalence
 *
 * @param c Pointeur vers la classe à modifier
 * @param v Sommet à ajouter
 *
 * @note Redimensionne automatiquement si nécessaire
 * @warning Termine le programme en cas d'échec de réallocation
 */
void classe_add_vertex(t_classe* c, t_tarjan_vertex* v) {
    if (c->count == c->capacity) {
        c->capacity *= 2;
        c->vertices = realloc(c->vertices, c->capacity * sizeof(int));
        if (!c->vertices) { perror("realloc"); exit(EXIT_FAILURE); }
    }
    c->vertices[c->count++] = v->id;
}

/**
 * @brief Crée une nouvelle partition vide
 *
 * @return Structure t_partition initialisée
 */
t_partition create_partition() {
    t_partition p;
    p.count = 0;
    p.capacity = 4;
    p.classes = malloc(p.capacity * sizeof(t_classe));
    return p;
}

/**
 * @brief Ajoute une classe à une partition
 *
 * @param p Pointeur vers la partition à modifier
 * @param c Classe à ajouter
 *
 * @note Redimensionne automatiquement si nécessaire
 */
void partition_add_classe(t_partition* p, t_classe c) {
    if (p->count == p->capacity) {
        p->capacity *= 2;
        p->classes = realloc(p->classes, p->capacity * sizeof(t_classe));
    }
    p->classes[p->count++] = c;
}

/**
 * @brief Crée une nouvelle pile
 *
 * @param capacity Capacité initiale de la pile
 * @return Pointeur vers la structure Stack créée
 */
Stack* stack_create(int capacity) {
    Stack* s = malloc(sizeof(Stack));
    s->data = malloc(capacity * sizeof(int));
    s->top = -1;
    s->capacity = capacity;
    return s;
}

/**
 * @brief Empile un élément
 *
 * @param s Pointeur vers la pile
 * @param v Valeur à empiler
 */
void stack_push(Stack* s, int v) {
    s->data[++s->top] = v;
}

/**
 * @brief Dépile et retourne l'élément au sommet
 *
 * @param s Pointeur vers la pile
 * @return Valeur dépilée
 */
int stack_pop(Stack* s) {
    return s->data[s->top--];
}

/**
 * @brief Consulte l'élément au sommet sans le retirer
 *
 * @param s Pointeur vers la pile
 * @return Valeur au sommet
 */
int stack_top(Stack* s) {
    return s->data[s->top];
}

/**
 * @brief Vérifie si la pile est vide
 *
 * @param s Pointeur vers la pile
 * @return 1 si la pile est vide, 0 sinon
 */
int stack_empty(Stack* s) {
    return s->top == -1;
}

/**
 * @brief Fonction de comparaison pour le tri d'entiers
 *
 * @param a Pointeur vers le premier entier
 * @param b Pointeur vers le second entier
 * @return Différence entre les deux entiers
 */
int cmp_int(const void *a, const void *b) {
    return (*(int*)a - *(int*)b);
}

// Comparateur pour qsort (mettre en haut du fichier)
// Fonction parcours corrigée

/**
 * @brief Implémente l'algorithme de Tarjan pour trouver les composantes fortement connexes
 *
 * Cette fonction récursive implémente les étapes principales de l'algorithme de Tarjan :
 * 1. Initialisation des numéros de découverte et lowlink
 * 2. Empilement du sommet courant
 * 3. Parcours des successeurs
 * 4. Identification et création des composantes fortement connexes
 *
 * @param v_index Index du sommet courant (base 0)
 * @param graph Liste d'adjacence du graphe
 * @param tab Tableau des sommets avec leurs attributs Tarjan
 * @param partition Pointeur vers la partition à construire
 * @param S Pile utilisée par l'algorithme
 * @param num Pointeur vers le compteur de numérotation
 *
 * @note Les sommets dans chaque composante sont triés par ordre croissant
 * @warning La fonction suppose que les indices des sommets sont valides
 *
 * @see t_tarjan_vertex
 * @see t_partition
 * @see Stack
 */
void parcours(int v_index, AdjacencyList graph, t_tarjan_vertex *tab,
              t_partition *partition, Stack *S, int *num) {

    t_tarjan_vertex *v = &tab[v_index];

    /* 1. initialiser num et lowlink */
    v->num = *num;
    v->lowlink = *num;
    (*num)++;

    /* 2. empiler v (on empile l'index 0-based) */
    stack_push(S, v_index);
    v->onStack = 1;

    /* 3. parcourir les successeurs */
    Cell *tmp = graph.array[v_index].head;
    while (tmp != NULL) {
        int w_index = tmp->destination - 1; /* destination est 1-based */
        if (w_index < 0 || w_index >= graph.size) {
            fprintf(stderr, "Warning: destination hors limites: %d\n", tmp->destination);
            tmp = tmp->next;
            continue;
        }
        t_tarjan_vertex *w = &tab[w_index];

        if (w->num == -1) {
            /* successeur non visité : récursion */
            parcours(w_index, graph, tab, partition, S, num);
            if (v->lowlink > w->lowlink) v->lowlink = w->lowlink;
        } else if (w->onStack) {
            /* successeur dans la pile : back-edge */
            if (v->lowlink > w->num) v->lowlink = w->num;
        }
        tmp = tmp->next;
    }

    /* 4. si v est racine d'une SCC, dépiler pour former la classe */
    if (v->lowlink == v->num) {
        t_classe c = create_classe("");
        sprintf(c.name, "C%d", partition->count + 1);

        int w_idx;
        do {
            w_idx = stack_pop(S);
            t_tarjan_vertex *w = &tab[w_idx];
            w->onStack = 0;

            /* garantir espace pour insérer l'id */
            if (c.count == c.capacity) {
                int newcap = c.capacity * 2;
                int *tmpv = realloc(c.vertices, newcap * sizeof(int));
                if (!tmpv) { perror("realloc c.vertices"); exit(EXIT_FAILURE); }
                c.vertices = tmpv;
                c.capacity = newcap;
            }

            /* stocker l'id (1-based) du sommet dans la classe */
            c.vertices[c.count++] = w->id;
        } while (w_idx != v_index);

        /* trier pour affichage propre */
        qsort(c.vertices, c.count, sizeof(int), cmp_int);

        /* ajouter la classe à la partition */
        partition_add_classe(partition, c);
    }
}

/* Fonction tarjan complète qui prépare tout et appelle parcours sur chaque sommet non visité */

/**
 * @brief Implémente l'algorithme de Tarjan pour trouver les composantes fortement connexes d'un graphe
 *
 * Cette fonction initialise les structures nécessaires et lance l'algorithme de Tarjan
 * pour identifier toutes les composantes fortement connexes du graphe.
 * Elle effectue :
 * - L'initialisation des structures de données
 * - Le lancement du parcours pour chaque sommet non visité
 * - Le nettoyage des ressources allouées
 *
 * @param graph Liste d'adjacence du graphe à analyser
 * @return Une partition contenant toutes les composantes fortement connexes
 *
 * @note La mémoire allouée pour la partition retournée doit être libérée par l'appelant
 *
 * @see t_partition
 * @see parcours
 * @see init_tarjan_vertice
 */
t_partition tarjan(AdjacencyList graph) {
    int n = graph.size;
    t_tarjan_vertex *tab = init_tarjan_vertice(n);
    t_partition partition = create_partition();
    Stack *S = stack_create(16);
    int num = 0;

    for (int i = 0; i < n; i++) {
        if (tab[i].num == -1) {
            parcours(i, graph, tab, &partition, S, &num);
        }
    }

    /* cleanup */
    free(S->data);
    free(S);
    free(tab); /* si tu veux garder tab pour debug, n'appelles pas free ici */

    return partition;
}

/**
 * @brief Libère la mémoire allouée pour une partition
 *
 * Libère récursivement :
 * - Les tableaux de sommets de chaque classe
 * - Le tableau des classes de la partition
 *
 * @param p Partition à libérer
 *
 * @note Cette fonction doit être appelée lorsque la partition n'est plus nécessaire
 */
void free_partition(t_partition p) {
    for (int i = 0; i < p.count; i++) {
        free(p.classes[i].vertices);  // libère le tableau des sommets de chaque classe
    }
    free(p.classes);  // libère le tableau des classes
}

/**
 * @brief Construit un tableau d'index des classes pour chaque sommet
 *
 * @param partition Partition contenant les classes
 * @param nbSommets Nombre total de sommets dans le graphe
 * @return Tableau où classOf[i] contient l'index de la classe du sommet i
 */
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

/**
 * @brief Construit la matrice des liens entre les classes
 *
 * @param g Graphe d'origine
 * @param p Partition des classes
 * @param classOf Tableau d'index des classes
 * @return Matrice d'adjacence entre les classes
 */
int** build_class_links(AdjacencyList g, t_partition p, int *classOf) {
    int nC = p.count;

    int **link = malloc(nC * sizeof(int*));
    for (int i = 0; i < nC; i++) {
        link[i] = calloc(nC, sizeof(int));
    }

    for (int u = 1; u <= g.size; u++) {
        int cu = classOf[u];

        Cell *tmp = g.array[u-1].head;
        while (tmp) {
            int v = tmp->destination;
            int cv = classOf[v];

            if (cu != cv)
                link[cu][cv] = 1;

            tmp = tmp->next;
        }
    }

    return link;
}




// =====================================================
// ÉTAPE 3 : Caractéristiques du graphe
// =====================================================

/**
 * @brief Vérifie si un état est absorbant
 *
 * Un état est absorbant s'il n'a que des transitions vers lui-même
 *
 * @param g Graphe à analyser
 * @param sommet Numéro du sommet à tester
 * @return 1 si le sommet est absorbant, 0 sinon
 */
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

/**
 * @brief Affiche les états absorbants du graphe
 *
 * @param g Graphe à analyser
 */
void afficher_etats_absorbants(AdjacencyList g) {
    printf(" Etats absorbants : ");
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

/**
 * @brief Identifie et affiche les classes transitoires et persistantes
 *
 * Une classe est transitoire si elle a des liens sortants vers d'autres classes,
 * persistante sinon
 *
 * @param g Graphe d'origine
 * @param p Partition des classes
 * @param classOf Tableau d'index des classes
 */
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

/**
 * @brief Teste et affiche si le graphe est irréductible
 *
 * Un graphe est irréductible s'il n'a qu'une seule classe
 *
 * @param p Partition à analyser
 */
void tester_irreductibilite(t_partition p) {
    if (p.count == 1)
        printf("Le graphe est IRRÉDUCTIBLE\n");
    else
        printf("Le graphe n'est PAS irreductible (%d classes)\n", p.count);
}

/**
 * @brief Analyse et affiche les caractéristiques principales du graphe
 *
 * Affiche :
 * 1. Les états absorbants
 * 2. La nature des classes (transitoires/persistantes)
 * 3. L'irréductibilité
 *
 * @param g Graphe à analyser
 * @param p Partition des classes
 * @param classOf Tableau d'index des classes
 */
void caracteristiques_graphe(AdjacencyList g, t_partition p, int *classOf) {

    printf("1) Etats absorbants :\n");
    afficher_etats_absorbants(g);

    printf("\n2) Classes transitoires / persistantes :\n");
    classes_transitoires_persistantes(g, p, classOf);

    printf("\n3) Irreductibilite :\n");
    tester_irreductibilite(p);

    printf("\n");
}


/**
 * @brief Génère une représentation Mermaid du diagramme de Hasse du graphe
 *
 * @param filename Nom du fichier de sortie
 * @param p Partition des classes
 * @param links Matrice des liens entre classes
 */
void generate_mermaid_hasse(const char *filename, t_partition p, int **links) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        printf("Erreur ouverture fichier\n");
        return;
    }

    // --- Header Mermaid ---
    fprintf(f,
        "---\n"
        "config:\n"
        "   layout: elk\n"
        "   theme: mc\n"
        "   look: classic\n"
        "---\n\n"
        "flowchart LR\n"
    );

    // 1. Écrire les noeuds
    for (int i = 0; i < p.count; i++) {
        char *ID = getID(i + 1);  // <-- utilisation ici
        fprintf(f, "%s[\"{", ID);

        for (int j = 0; j < p.classes[i].count; j++) {
            fprintf(f, "%d", p.classes[i].vertices[j]);
            if (j < p.classes[i].count - 1) fprintf(f, ",");
        }

        fprintf(f, "}\"]\n");
        free(ID);
    }

    // 2. Écrire les liens
    for (int i = 0; i < p.count; i++) {
        for (int j = 0; j < p.count; j++) {
            if (links[i][j]) {
                char *ID1 = getID(i + 1);
                char *ID2 = getID(j + 1);

                fprintf(f, "%s --> %s\n", ID1, ID2);

                free(ID1);
                free(ID2);
            }
        }
    }

    fclose(f);
    printf("Fichier Mermaid genere un graphe de hasse: %s\n", filename);
}