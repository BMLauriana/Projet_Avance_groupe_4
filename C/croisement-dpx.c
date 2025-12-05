#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "genetique.h"
static int aretes_communes(int position, tournee_t *p1,tournee_t *p2,int dimension)
{
    // pour chaque arête de parent1, vérifier si elle existe dans parent2
    int suivant_a = (position+1)%dimension;
    int num_a_parentA = parent_a->parcours[position].num;
    int num_b_parentA = parent_a->parcours[suivant_a].num;
    // vérifier si l'arête existe dans parent2 (dans un sens ou dans l'autre)        
    for (int j=0; j<dimension; j++) {
        int suivant_b = (j+1)%dimension;
        int num_a_parentB = parent_b->parcours[j].num;
        int num_b_parentB = parent_b->parcours[suivant_b].num;
        // si il existe, la marquer
        if ((num_a_parentB == num_a_parentA && num_b_parentB == num_b_parentA) || (num_a_parentB == num_b_parentA && num_b_parentB == num_a_parentA)) {
            1;  // arête commune donc on ne détruit pas
        }
    }
    return 0;
    // int a = p1->parcours[position].num;
    // int b = p1->parcours[(position + 1) % dimension].num;

    // for (int j = 0; j < dimension; j++) {
    //     int c = p2->parcours[j].num;
    //     int d = p2->parcours[(j + 1) % dimension].num;
    //     if ((c == a && d == b) || (c == b && d == a)) {
    //         return 1; 
    //     }
    // }
    // return 0; 
}

tournee_t* dpx_crossover(tournee_t* parent1, tournee_t* parent2, int dimension, int** matrice, instance_t* inst) {
    
    //1-Copier parent1 dans la fille
    tournee_t* fille = malloc(sizeof(tournee_t));
    fille->parcours = malloc(dimension * sizeof(noeud_t));
    for (int i = 0; i < dimension; i++) {
        fille->parcours[i] = parent1->parcours[i];
    }
    //2-Trouver les arêtes communes entre parent1 et parent2

    //3-Détruire les arêtes NON communes

    int *frag_id = malloc(dimension * sizeof(int));

    int nb_frag = 0;
    frag_id[0] = 0;   
    nb_frag = 1;

    for (int i = 0; i < dimension - 1; i++) {
        if (aretes_communes(i, parent1, parent2, dimension)) {

            frag_id[i+1] = nb_frag - 1;
        } else {
            frag_id[i+1] = nb_frag;
            nb_frag++;
        }
    }


    //4- Reconnecter les fragments déconnectés
    
    //5-Appliquer 2-opt pour améliorer
    tournee_t* fille_optimisee = deux_opt(fille, inst);
    
    free(fille->parcours);
    free(fille);
    free(frag_id);
    
    return fille_optimisee;
}


