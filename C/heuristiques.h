#ifndef __HEURISTIQUES_H__
#define __HEURISTIQUES_H__
#include "lecture_donnees.h"

tournee_t * plus_proche_voisin(instance_t * inst, int** matrice);
void permuter(int *a, int *b);
tournee_t marche_aleatoire_matrice(instance_t *inst, int **matrice);

#endif