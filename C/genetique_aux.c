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

tournee_t[] random_population(const int population_size, const instance_t * inst,const int** mat){
    tournee_t population[population_size];
    for(int i=0;i<population_size;i++){
        population[i] = * marche_aleatoire_matrice(inst,mat);
    }
    return population;
}


ind_max_tournee(tournee_t t1,int ind1,tournee_t t2, int ind2){
    if(t1.longueur<t2.longeur){
        return ind2;
    }else{
        return ind1;
    }
}