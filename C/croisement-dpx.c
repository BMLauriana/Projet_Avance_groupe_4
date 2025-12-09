#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "genetique.h"


static int aretes_communes(int position, tournee_t *p1, tournee_t *p2, int dimension)
{
    int a = p1->parcours[position].num;
    int b = p1->parcours[(position + 1) % dimension].num;

    for (int j = 0; j < dimension; j++) {
        int c = p2->parcours[j].num;
        int d = p2->parcours[(j + 1) % dimension].num;
        if ((c == a && d == b) || (c == b && d == a)) {
            return 1;
        }
    }
    return 0;
}

tournee_t* dpx_crossover(tournee_t* parent1, tournee_t* parent2, int dimension, instance_t* inst)
{
    if (dimension <= 1) {
        // cas trivial : on clone juste parent1
        tournee_t* fille_triv = malloc(sizeof(tournee_t));
        fille_triv->parcours = malloc(dimension * sizeof(noeud_t));
        for (int i = 0; i < dimension; i++) {
            fille_triv->parcours[i] = parent1->parcours[i];
        }
        return deux_opt(fille_triv, inst);
    }

    tournee_t* fille = malloc(sizeof(tournee_t));
    fille->parcours = malloc(dimension * sizeof(noeud_t));
    for (int i = 0; i < dimension; i++) {
        fille->parcours[i] = parent1->parcours[i];
    }
    // Identifier les fragments d’arêtes communes
    int *frag_id = malloc(dimension * sizeof(int));
    int nb_frag = 0;

    frag_id[0] = 0;
    nb_frag = 1;

    for (int i = 0; i < dimension - 1; i++) {
        if (aretes_communes(i, parent1, parent2, dimension)) {
            frag_id[i + 1] = nb_frag - 1; // même fragment
        } else {
            frag_id[i + 1] = nb_frag;     // nouveau fragment
            nb_frag++;
        }
    }

    // Calculer frag_start et frag_len de chaque fragment (dans parent1)
    int *frag_start = malloc(nb_frag * sizeof(int));
    int *frag_len   = malloc(nb_frag * sizeof(int));

    for (int f = 0; f < nb_frag; f++) {
        frag_start[f] = -1;
        frag_len[f]   = 0;
    }

    // premier fragment commence à 0
    frag_start[ frag_id[0] ] = 0;
    int curr_frag = frag_id[0];

    for (int i = 1; i < dimension; i++) {
        int f = frag_id[i];
        if (f != curr_frag) {
            // on ferme le fragment précédent
            frag_len[curr_frag] = i - frag_start[curr_frag];
            curr_frag = f;
            frag_start[curr_frag] = i;
        }
    }
    // dernier fragment
    frag_len[curr_frag] = dimension - frag_start[curr_frag];

    // Reconnecter les fragments en suivant l’ordre des villes de parent2

    // mapping ville -> position dans parent1
    // num est dans [0..dimension] ou [1..dimension]
    int max_label = dimension + 1;
    int *pos_in_p1 = malloc(max_label * sizeof(int));
    for (int i = 0; i < max_label; i++) {pos_in_p1[i] = -1;}

    for (int i = 0; i < dimension; i++) {
        int v = parent1->parcours[i].num;
        if (v >= 0 && v < max_label) {
            pos_in_p1[v] = i;
        }
    }

    int *ordre_frag = malloc(nb_frag * sizeof(int));
    bool *frag_deja_vu = calloc(nb_frag, sizeof(bool));
    int nb_ordre = 0;

    for (int i = 0; i < dimension; i++) {
        int v = parent2->parcours[i].num;
        if (v < 0 || v >= max_label) continue;
        int pos = pos_in_p1[v];
        if (pos < 0 || pos >= dimension) continue;
        int f = frag_id[pos];
        if (!frag_deja_vu[f]) {
            frag_deja_vu[f] = true;
            ordre_frag[nb_ordre++] = f;
        }
    }

    // Au cas où un fragment n’a pas été rencontré, on l’ajoute à la fin
    for (int f = 0; f < nb_frag; f++) {
        if (!frag_deja_vu[f]) {
            ordre_frag[nb_ordre++] = f;
        }
    }

    // Reconstituer le parcours fille en concaténant les fragments de parent1
    noeud_t *nouveau_parcours = malloc(dimension * sizeof(noeud_t));
    int idx = 0;

    for (int k = 0; k < nb_ordre; k++) {
        int f = ordre_frag[k];
        int start = frag_start[f];
        int len   = frag_len[f];

        for (int t = 0; t < len; t++) {
            nouveau_parcours[idx++] = parent1->parcours[start + t];
        }
    }

    if (idx != dimension) {
        // si ça se passe mal, on retombe sur parent1 tel quel
        for (int i = 0; i < dimension; i++) {
            nouveau_parcours[i] = parent1->parcours[i];
        }
    }

    free(fille->parcours);
    fille->parcours = nouveau_parcours;

    // Améliorer par 2-opt
    tournee_t* fille_optimisee = deux_opt(fille, inst);

    free(fille->parcours);
    free(fille);
    free(frag_id);
    free(frag_start);
    free(frag_len);
    free(pos_in_p1);
    free(ordre_frag);
    free(frag_deja_vu);

    return fille_optimisee;
}
