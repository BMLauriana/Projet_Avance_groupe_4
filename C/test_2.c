#include "heuristiques.h"
#include "lecture_donnees.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    instance_t * instance = malloc(sizeof (instance_t));
    strcpy(instance->type_distance ,"EUC_2D");
    instance->dimension = 3;
    instance->noeuds =malloc((instance->dimension)*sizeof(noeud_t));
    instance->noeuds[0].num = 1;
    instance->noeuds[1].num = 2;
    instance->noeuds[2].num = 3;

    instance->noeuds[0].x = 0;
    instance->noeuds[1].x = 5;
    instance->noeuds[2].x = 10;

    instance->noeuds[0].y = 0;
    instance->noeuds[1].y = 0;
    instance->noeuds[2].y = 0;


    distance_f fonction_distance = choix_distance(instance);

    int** demi_matrice = creer_matrice((*instance), fonction_distance);

    tournee_t * tour1 = plus_proche_voisin(instance,demi_matrice);

    assert(tour1->parcours[0].num==1);
    assert(tour1->parcours[1].num==2);
    assert(tour1->parcours[2].num==3);

    printf("TEST 2 OK\n");

    liberer_tournee(&tour1);
    liberer_matrice(demi_matrice, instance->dimension);
    free(instance->noeuds);
    free(instance);
    

    return 0;
}