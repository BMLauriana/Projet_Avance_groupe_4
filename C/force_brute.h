#ifndef __FORCE_BRUTE_H__
#define __FORCE_BRUTE_H_
#include "lecture_donnees.h"

int next_permutation(int *tab, int n);
double brute(
    int nb_nodes,                         // nombre d'éléments à permuter
    int nb_ressources,                    // 0 pour le TSP (réservé pour d'autres problèmes)
    int *best_perm,                       // (sortie) meilleure permutation trouvée
    unsigned long long *count_best,       // (sortie) coût de cette permutation
    void *(*cout)(void *perm, int n)      // fonction coût générique
);
tournee_t *force_brute (instance_t *inst, int **matrice);
tournee_t *force_brute2(instance_t *inst, distance_f f_distance) ;

#endif