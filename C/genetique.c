#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "lecture_donnees.h"
#include "heuristiques.h"
#include "genetique.h"
//#define POPULATION_SIZE 30
//#define GENERATION  1000
//#define MUTATION_RATE 0.10f
//#define TOURNAMENT_SIZE  ((int)(0.5*POPULATION_SIZE))

void free_population(tournee_t **pop, int size){
    for (int i = 0; i < size; i++) {
        if (pop[i]) {
            free(pop[i]->parcours);
            free(pop[i]);
        }
    }
    free(pop);
}

void genetique(int population_size, int generation, int mutation_rate, instance_t* instance) {
    clock_t debut_time = clock();   // démarrage du chronométrage
    int tournament_size = (int)(0.5*population_size);
    srand((unsigned)time(NULL));
    distance_f f_distance = choix_distance(instance);
    //int ** matrice = creer_matrice(*instance, f_distance); Parceque l'instance est un valeur
    int ** matrice = creer_matrice((*instance), f_distance);
    tournee_t **population = random_population(population_size, instance, matrice);
    if (!population) {
        fprintf(stderr, "Erreur d'allocation population\n");
        exit(EXIT_FAILURE);
    }
    tournee_t *best_individual = population[0];
    for (int gen=0; gen<generation; gen++) {
        //tournee_t *selected = malloc(population_size*(sizeof(tournee_t)));
        tournee_t **selected = tournament_selection(population,population_size, tournament_size);
        if (!selected) {
            fprintf(stderr, "Erreur d'allocation selected\n");
            exit(EXIT_FAILURE);
        }
        tournee_t **offspring = malloc(population_size * sizeof(tournee_t *));
        if (!offspring) {
            fprintf(stderr, "Erreur d'allocation offspring\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < population_size; j += 2) {
            tournee_t *child_a = ordered_crossover(selected[j],selected[j+1],instance->dimension);
            tournee_t *child_b = ordered_crossover(selected[j+1],selected[j],instance->dimension);

            offspring[j]   = child_a;
            offspring[j+1] = child_b;
        }
        for (int i = 0; i < population_size; i++) {
            swap_mutation(offspring[i], instance->dimension, mutation_rate);

            offspring[i]->longueur =calculer_longueur_matrice(offspring[i],instance->dimension,matrice);
        }
        // fonction auxiliaire : comparaison
        qsort(offspring,population_size,sizeof(tournee_t *),compare_tournee);
        free(selected);
        free(population);
        population = offspring;
        int ind_worst = population_size - 1;
        tournee_t *rnd = marche_aleatoire_matrice(instance, matrice);
        if (!rnd) {
            fprintf(stderr, "Erreur dans marche_aleatoire_matrice\n");
            exit(EXIT_FAILURE);
        }
        population[ind_worst] = rnd;
        population[ind_worst]->longueur =calculer_longueur_matrice(population[ind_worst],instance->dimension,matrice);
        tournee_t *best_generation = population[0];
        ind_worst = 0;
        for (int i = 1; i < population_size; i++) {
            if (population[i]->longueur > population[ind_worst]->longueur) {
                ind_worst = i;
            }
        }
        if (best_generation->longueur < best_individual->longueur) {
            best_individual = best_generation;
        }
        population[ind_worst] = best_individual;
    }
    clock_t fin_time = clock();
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;

    printf("%f ; %f ; ",temps_ecoule, best_individual->longueur);
    printf("[");
    for (int i = 0; i < instance->dimension; i++) {
        printf("%d", best_individual->parcours[i].num);
        if (i < instance->dimension - 1) {
            printf(",");
        }
    }
    printf("]\n");
    liberer_matrice(matrice,instance->dimension);
    free_population(population, population_size);
}

