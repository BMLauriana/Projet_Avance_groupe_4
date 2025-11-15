#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "lecture_donnees.h"
#include "heuristiques.h"
#include "genetique.h"

#define POPULATION_SIZE 30
#define GENERATION  1000
#define MUTATION_RATE 0.10
#define TOURNAMENT_SIZE  ((int)(0.5*POPULATION_SIZE))

int main(int argc, char * argv[]) {
    // mettre les gestions argv
    instance_t *instance = lire_tsplib();
    distance_f f_distance = choix_distance(instance);
    //int ** matrice = creer_matrice(*instance, f_distance); Parceque l'instance est un valeur
    int ** matrice = creer_matrice(instance, f_distance);
    tournee_t population[] = random_population(POPULATION_SIZE,instance, matrice);
    tournee_t best_individual = population[0];
    for (int i=0; i<GENERATION; i++) {
        //tournee_t *selected = malloc(population_size*(sizeof(tournee_t)));
        tournee_t selected[] = tournament_selection(population, TOURNAMENT_SIZE, f_distance);
        tournee_t offspring[];
        for (int j=0; j<POPULATION_SIZE; j+2) {
            tournee_t child_a = ordered_crossover(selected[j], selected[j+1],instance->dimension);
            tournee_t child_a = ordered_crossover(selected[j+1], selected[j],instance->dimension);
            offspring[i] = child_a;
            offspring[i++] = child_b;
        }
        for (int child = 0; child<offspring; child++) {
            swap_mutation(child, MUTATION_RATE);
        }
        // fonction auxiliaire : comparaison
        //qsort(offspring,POPULATION_SIZE,sizeof(tournee_t),compare_tournee);
        offspring = qsort(offspring, POPULATION_SIZE, sizeof(tournee_t), compare);
        population = offspring;
        tournee_t worste = offspring[POPULATION_SIZE-1];
        int indice = POPULATION_SIZE-1;
        population[indice] = marche_aleatoire_matrice(instance, matrice);
        tournee_t best_generation = offspring[0];
        // implémenter max tournée
        indice = ind_max_tournee(population[POPULATION_SIZE-2],POPULATION_SIZE-2,population[indice],indice);
        tournee_t worste_generation = population[indice];
        if (best_generation.longueur < best_individual.longueur) {
            best_individual = best_generation;
        }
        population[indice] = best_individual;
    }
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", best_individual->parcours[i].num);
    }
    printf("%d]\n",best_individual->parcours[instance->dimension-1].num);
    float longueur = meilleure_tournee->longueur;
    printf("longueur = %d",longueur);
    liberer_matrice(matrice);
    return 0;
}