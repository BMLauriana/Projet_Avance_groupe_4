#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lecture_donnees.h"
#include "heuristiques.h"

tournee_t * marche_aleatoire_matrice(instance_t *inst, int **matrice) {
    int n = inst->dimension;
    
    tournee_t *tournee = malloc(sizeof(tournee_t));
    if (!tournee) {
        fprintf(stderr, "Memory allocation failed (tournee)\n");
        exit(EXIT_FAILURE);
    }
    tournee->parcours = malloc(n * sizeof(noeud_t));
    if (!tournee->parcours) {
        fprintf(stderr,"Erreur d'allocation de la tournée\n");
        exit(EXIT_FAILURE);
    }

    int *tour_cano = malloc(n * sizeof(int));
    if (!tour_cano) {
        fprintf(stderr,"Erreur d'allocation (tour_cano)\n");
        free(tournee->parcours);
        free(tournee);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        tour_cano[i] = i;
    }

    //mélange aléatoire complet : Fisher-Yates standard
    for (int i = n - 1; i > 0; i--) {
        int k = rand() % (i + 1);  
        permuter(&tour_cano[i], &tour_cano[k]);
    }

    for (int i = 0; i < n; i++) {
        tournee->parcours[i] = inst->noeuds[tour_cano[i]];
    }

    tournee->longueur = calculer_longueur_matrice(tournee, n, matrice);

    free(tour_cano);
    return tournee;
}


