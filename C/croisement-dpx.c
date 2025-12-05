#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "genetique.h"

tournee_t* dpx_crossover(tournee_t* parent1, tournee_t* parent2, int dimension, int** matrice, instance_t* inst) {
    
    //1-Copier parent1 dans la fille
    tournee_t* fille = malloc(sizeof(tournee_t));
    fille->parcours = malloc(dimension * sizeof(noeud_t));
    for (int i = 0; i < dimension; i++) {
        fille->parcours[i] = parent1->parcours[i];
    }
    //2-Trouver les arêtes communes entre parent1 et parent2

    //3-Détruire les arêtes NON communes
    
    //4- Reconnecter les fragments déconnectés
    
    //5-Appliquer 2-opt pour améliorer
    tournee_t* fille_optimisee = deux_opt(fille, inst);
    
    free(fille->parcours);
    free(fille);
    
    return fille_optimisee;
}


