#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include "genetique.h"

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

    tournee_t ** tabTour = random_population(3,instance,demi_matrice);
    assert(tabTour[0]!=NULL);
    assert(tabTour[1]!=NULL);
    assert(tabTour[2]!=NULL);

    int res = ind_max_tournee(tabTour[1],1,tabTour[2], 2);
    if(tabTour[1]->longueur<tabTour[2]->longueur){
        assert(res==2);
    }else{
        assert(res==1);
    }
    printf("TESTS 3 OK\n");
    liberer_matrice(demi_matrice,instance->dimension);
    void free_population(tournee_t **pop, int size);

    free(instance->noeuds);
    free(instance);
}