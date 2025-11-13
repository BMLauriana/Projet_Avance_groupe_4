#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "lecture_donnees.h"
#include "heuristiques.h"

int calculer_longueur_matrice(tournee_t *tournee, int n, int **matrice) {
    int longueur = 0;
    for (int i = 0; i < n - 1; i++)
        longueur += recuperer_distance(matrice, tournee->parcours[i].num - 1, tournee->parcours[i + 1].num - 1);
    longueur += recuperer_distance(matrice, tournee->parcours[n - 1].num - 1, tournee->parcours[0].num - 1);
    return longueur;
}

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

    // créer une tournée canonique : [0,1,2,...,n-1]
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

    //mékange aléatoire complet : Fisher-Yates standard
    //=> la première ville devient aussi aléatoire
    srand((unsigned)time(NULL));
    for (int i = n - 1; i > 0; i--) {
        int k = rand() % (i + 1);   // k est entre 0 et i
        permuter(&tour_cano[i], &tour_cano[k]);
    }

    //construire la tournée avec les noeuds réels
    for (int i = 0; i < n; i++) {
        tournee->parcours[i] = inst->noeuds[tour_cano[i]];
    }

    //calculer la longueur de la tournée avec la matrice
    tournee->longueur = calculer_longueur_matrice(tournee, n, matrice);

    free(tour_cano);
    return tournee;
}


