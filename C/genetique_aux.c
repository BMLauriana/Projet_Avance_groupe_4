#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "genetique.h"

tournee_t *tournament_selection(tournee_t *population , int population_size, int tournament_size){
    tournee_t *selected = malloc(population_size * sizeof(tournee_t));
    if (!selected) {
        fprintf(stderr, "Erreur d'allocation de selected\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < population_size; i++) {
        int meilleur_index = rand() % population_size;
        float meilleur_longueur = population[meilleur_index].longueur;
        for (int j = 1; j < tournament_size; j++) {
            int indice = rand() % population_size;
            if (population[indice].longueur < meilleur_longueur) {
                meilleur_index = indice;
                meilleur_longueur = population[indice].longueur;
            }
        }
        selected[i] = population[meilleur_index];
    }

    return selected;

}


