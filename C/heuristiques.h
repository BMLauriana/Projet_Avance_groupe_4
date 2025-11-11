#ifndef __HEURISTIQUES_H__
#define __HEURISTIQUES_H__
#include "lecture_donnees.h"

tournee_t * plus_proche_voisin(instance_t * inst, int** matrice);
tournee_t marche_aleatoire_matrice(instance_t *inst, int **matrice);
tournee_t * deux_opt(tournee_t * tournee, int ** matrice, int n);

#endif