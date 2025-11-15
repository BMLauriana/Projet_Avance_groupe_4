#ifndef __GENETIQUE_H___
#define __GENETIQUE_H___
#include "lecture_donnees.h"
#include "heuristiques.h"

tournee_t *tournament_selection(tournee_t *population , int population_size, int tournament_size);
tournee_t[] random_population(const int population_size, const instance_t * inst,const int** mat);
ind_max_tournee(tournee_t t1,int ind1,tournee_t t2, int ind2);

#endif