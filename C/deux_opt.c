#include "heuristiques.h"
#include <stdio.h>
#include <stdlib.h>

void inverser_segment(noeud_t *parcours, int i, int j) {
    while (i < j) {
        noeud_t temp = parcours[i];
        parcours[i] = parcours[j];
        parcours[j] = temp;
        i++;
        j--;
    }
}

tournee_t * deux_opt(tournee_t * tournee, instance_t* inst){
    int n = inst->dimension;
    distance_f f_distance = choix_distance(inst);

    tournee_t * res = malloc(sizeof(tournee_t));
    res->parcours =malloc(n*sizeof(noeud_t));

    if (!tournee || !res->parcours){
        fprintf(stderr, "Erreur d’allocation mémoire\n");
        exit(EXIT_FAILURE);
    }

    res->longueur = tournee->longueur;

    for (int i = 0; i < n; i++) {
        res->parcours[i] = tournee->parcours[i];
    }

    for(int i=0; i<n-2;i++){
        for(int j=0; j<n-1;j++){
            inverser_segment(res->parcours, i, j);

            float nouvelle_longueur = longueur_tournee(*inst,*res, f_distance);

            if (nouvelle_longueur < res->longueur) {
                res->longueur = nouvelle_longueur;
            } else {
                inverser_segment(res->parcours, i, j);            
            }
        }
    }

    return res;
}
