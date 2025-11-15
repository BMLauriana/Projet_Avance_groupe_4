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
tournee_t ordered_crossover(tournee_t parent_a,tournee_t parent_b,int dimension){
    tournee_t child;
    child.parcours = malloc(dimension * sizeof(noeud_t));
    if (!child.parcours) {
        fprintf(stderr, "Erreur d'allocation dans ordered_crossover\n");
        exit(EXIT_FAILURE);
    }
    child.longueur = 0.0f;
    for (int i = 0; i < dimension; i++) {
        child.parcours[i].num = -1;
        child.parcours[i].x   = 0.0f;
        child.parcours[i].y   = 0.0f;
    }
    int start = rand() % dimension;
    int end   = rand() % dimension;
    if (start > end) {
        int tmp = start;
        start = end;
        end   = tmp;
    }
    if (start == end) {         
        if (start == 0){    
            end++;
        } 
        else{
            start--;
        }            
    }
    for (int i = start; i <= end; i++) {
        child.parcours[i] = parent_a.parcours[i];
    }
    for (int j = 0; j < dimension; j++) {
        int candidate_num = parent_b.parcours[j].num;
        int existe = 0;
        for (int k = 0; k < dimension; k++) {
            if (child.parcours[k].num == candidate_num) {
                existe = 1;
                break;
            }
        }
        if (existe) continue;

        for (int k = 0; k < dimension; k++) {
            if (child.parcours[k].num == -1) {
                child.parcours[k] = parent_b.parcours[j];
                break;
            }
        }
    }
    return child;
}


{
    const tournee_t *t1 = (const tournee_t *)a;
    const tournee_t *t2 = (const tournee_t *)b;
    if (t1->longueur < t2->longueur) return -1;
    if (t1->longueur > t2->longueur) return  1;
    return 0;
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