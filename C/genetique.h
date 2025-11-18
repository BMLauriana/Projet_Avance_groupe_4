#ifndef __GENETIQUE_H___
#define __GENETIQUE_H___
#include "lecture_donnees.h"
#include "heuristiques.h"

void free_population(tournee_t **pop, int size);
void genetique(int population_size, int generation, int mutation_rate, instance_t* instance);
int ind_max_tournee(tournee_t *t1, int ind1,tournee_t *t2, int ind2);
tournee_t **tournament_selection(tournee_t **population,int population_size,int tournament_size);
tournee_t *ordered_crossover(tournee_t *parent_a,tournee_t *parent_b,int dimension);
tournee_t **random_population(int population_size,instance_t *inst,int **matrice);
int compare_tournee(const void *a, const void *b);
void swap_mutation(tournee_t *tournee, int dimension, float mutation_rate);



#endif