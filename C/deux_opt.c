#include "heuristiques.h"
#include <stdio.h>
#include <stdlib.h>

tournee_t * deux_opt(tournee_t * tournee, int ** matrice, int n){
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

    float nouvelle_longueur;

    for(int i=0; i<n-2;i++){
        for(int j=0; i<n-1;j++){
            /*etape */
        }
    }

    return res;
}
