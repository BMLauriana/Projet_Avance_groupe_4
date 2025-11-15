#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "lecture_donnees.h"
#include "heuristiques.h"
#include "genetique.h"
#define POPULATION_SIZE 30
#define GENERATION  1000
#define MUTATION_RATE 0.10f
#define TOURNAMENT_SIZE  ((int)(0.5*POPULATION_SIZE))
void free_population(tournee_t **pop, int size)
{
    for (int i = 0; i < size; i++) {
        if (pop[i]) {
            free(pop[i]->parcours);
            free(pop[i]);
        }
    }
    free(pop);
}
int main(int argc, char * argv[]) {
    char* nom_fichier;
    if(argc == 5){
        nom_fichier = argv[2]; //récuperation du nom de fichier apres le -f
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
        /*il manque un argument (-f ou la désignation du fichier)*/
        exit(1);
    }
    srand((unsigned)time(NULL));
    // mettre les gestions argv
    instance_t *instance = lire_tsplib(nom_fichier);
    distance_f f_distance = choix_distance(instance);
    //int ** matrice = creer_matrice(*instance, f_distance); Parceque l'instance est un valeur
    int ** matrice = creer_matrice((*instance), f_distance);
    tournee_t **population = random_population(POPULATION_SIZE, instance, matrice);
    if (!population) {
        fprintf(stderr, "Erreur d'allocation population\n");
        exit(EXIT_FAILURE);
    }
    tournee_t *best_individual = population[0];
    for (int gen=0; gen<GENERATION; gen++) {
        //tournee_t *selected = malloc(population_size*(sizeof(tournee_t)));
        tournee_t **selected = tournament_selection(population,POPULATION_SIZE,TOURNAMENT_SIZE);
        if (!selected) {
            fprintf(stderr, "Erreur d'allocation selected\n");
            exit(EXIT_FAILURE);
        }
        tournee_t **offspring = malloc(POPULATION_SIZE * sizeof(tournee_t *));
        if (!offspring) {
            fprintf(stderr, "Erreur d'allocation offspring\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < POPULATION_SIZE; j += 2) {
            tournee_t *child_a = ordered_crossover(population[j],population[j+1],instance->dimension);
            tournee_t *child_b = ordered_crossover(population[j+1],population[j],instance->dimension);

            offspring[j]   = child_a;
            offspring[j+1] = child_b;
        }
        for (int i = 0; i < POPULATION_SIZE; i++) {
            swap_mutation(offspring[i], instance->dimension, MUTATION_RATE);

            offspring[i]->longueur =calculer_longueur_matrice(offspring[i],instance->dimension,matrice);
        }
        // fonction auxiliaire : comparaison
        qsort(offspring,POPULATION_SIZE,sizeof(tournee_t *),compare_tournee);
        free(selected);
        free(population);
        population = offspring;
        int ind_worst = POPULATION_SIZE - 1;
        tournee_t *rnd = marche_aleatoire_matrice(instance, matrice);
        if (!rnd) {
            fprintf(stderr, "Erreur dans marche_aleatoire_matrice\n");
            exit(EXIT_FAILURE);
        }
        population[ind_worst] = rnd;
        population[ind_worst]->longueur =calculer_longueur_matrice(population[ind_worst],instance->dimension,matrice);
        tournee_t *best_generation = population[0];
        ind_worst = 0;
        for (int i = 1; i < POPULATION_SIZE; i++) {
            if (population[i]->longueur > population[ind_worst]->longueur) {
                ind_worst = i;
            }
        }
        if (best_generation->longueur < best_individual->longueur) {
            best_individual = best_generation;
        }
        population[ind_worst] = best_individual;
    } 
    printf("[");
    for (int i = 0; i < instance->dimension; i++) {
        printf("%d", best_individual->parcours[i].num);
        if (i < instance->dimension - 1) {
            printf(",");
        }
    }
    printf("]\n");
    printf("longueur = %f\n", best_individual->longueur);
    liberer_matrice(matrice,instance->dimension);
    free_population(population, POPULATION_SIZE);
    return 0;
}

