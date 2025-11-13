#ifndef __FORCE_BRUTE_H__
#define __FORCE_BRUTE_H_
#include "lecture_donnees.h"

int next_permutation(int *tab, int n);
tournee_t *force_brute (instance_t *inst, int **matrice);
tournee_t *force_brute2(instance_t *inst, distance_f f_distance) ;

#endif