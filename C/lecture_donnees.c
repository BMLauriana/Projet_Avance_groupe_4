#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lecture_donnees.h"

/**********************************
    Distances EUC_2D, GEO et ATT
***********************************/
int distance_euclidienne(noeud_t point1, noeud_t point2) {
    float xd = point1.x - point2.x;
    float yd = point1.y - point2.y;
    float distance = sqrt(xd*xd + yd*yd);
    return (int)distance;
}

int distance_geographique(noeud_t point1, noeud_t point2) {
    // premier noeud
    int deg = (int)point1.x;
    int min = point1.x - deg;
    float latitude1 = PI * (deg+5.0*min/3.0)/180.0;
    deg = (int)point1.y;
    min = point1.y - deg;
    float longitude1 = PI* (deg+5.0*min/3.0)/180.0;
    
    // deuxième noeud
    deg = (int)point2.x;
    min = point2.x - deg;
    float latitude2 = PI * (deg+5.0*min/3.0)/180.0;
    deg = (int)point2.y;
    min = point2.y - deg;
    float longitude2 = PI* (deg+5.0*min/3.0)/180.0;

    // distance entre deux noeuds
    float rayonTerreste = 6378.388;
    float q1 = cos(longitude1 - longitude2);
    float q2 = cos(latitude1 - latitude2);
    float q3 = cos(latitude1 + latitude2);
    float distance = rayonTerreste * acos(0.5*((1.0+q1)*q2-(1.0-q1)*q3))+1.0;
    return (int)distance;
}

// distance ATT
int distance_euclidienne_att(noeud_t point1, noeud_t point2) {
    int dij =0;
    float xd = point1.x - point2.x;
    float yd = point1.y - point2.y;
    float rij = sqrt((xd*xd+yd*yd)/10.0);
    int tij = (int)rij;
    if (tij<rij) {
        dij = tij+1;
    } else {
        dij = tij;
    }
    return dij;
}

/*********************
Longueur d'une tournée 
**********************/

float longueur_tournee(instance_t instance, tournee_t tour, int(*f_distance)(noeud_t, noeud_t)) {
    float longueur_totale = 0;
    for (int i=0; i<instance.dimension; i++) {
        noeud_t ville_debut = tour.parcours[i];
        // quand on atteint le dernier i, l'indice va donner 0
        // donc on revient au début du parcours 
        // ex : 0 -> 1 -> 2 -> 3 -> 0
        noeud_t ville_fin = tour.parcours[(i+1)%instance.dimension];
        // recuperer les coordonnees des villes
        longueur_totale += f_distance(ville_debut,ville_fin);
    }
    tour.longueur = longueur_totale;
    return longueur_totale; 
}

/***********************************
* choix de la fonction de distance *
************************************/

     distance_f choix_distance(const instance_t *inst){
        /*creation des chaines a comparer*/
        char eucl_2D[] = "EUCL_2D";
        char geo[] = "GEO";
        char att[] = "ATT";
        /*verification*/
        if (strcmp(inst->type_distance,eucl_2D)==0){
            return distance_euclidienne;
        }else if(strcmp(inst->type_distance,geo)==0){
            return distance_geographique;
        }else if(strcmp(inst->type_distance, att)==0){
            return distance_euclidienne_att;
        }else{
            return NULL; /*ce n'est pas un type de distance que l'on va traiter*/
        }
    }

/*******************************
matrice inférieure de distance 
*******************************/

int **creer_matrice(instance_t inst, int(*f_distance)(noeud_t, noeud_t)){
    int n = inst.dimension; //nbr de noeuds
    int **matrice = malloc(n* sizeof(int*)) ;
    if(!matrice){
        perror("Erreur d'allocation de la matrice");
        exit(EXIT_FAILURE);
    }
    //on complete la matrice
    for(int i = 0; i < n ; i++){
        matrice[i] = malloc(i*sizeof(int));
        for(int j = 0; j<i ; j++){
            matrice[i][j] = f_distance(inst.noeuds[i],inst.noeuds[j]);
        }
    }
    return matrice;
}

int recuperer_distance(int **matrice , int i , int j){
    if(i==j){
        return 0.0;
    }
    if(i > j){
        return matrice[i][j];
    }
    return matrice[j][i];
}

int longueur_tour_cano_matrice(instance_t inst, int **matrice){
    int n = inst.dimension;
    int longueur = 0;
    for(int i = 0; i < n-1; i++){
        longueur += recuperer_distance(matrice, i, i+1);
    }
    //retour à la première ville pour fermer la boucle de la tournée
    longueur += recuperer_distance(matrice, n-1,0); 
    return longueur;

}

void liberer_matrice(int **matrice, int n){
    for(int i= 0; i < n; i++){
        free(matrice[i]);
    }
    free(matrice);
}

void liberer_instance(instance_t **inst)
{
    if (!inst || !*inst) return;

    free((*inst)->noeuds);
    (*inst)->noeuds = NULL;

    free(*inst);
    *inst = NULL;
}
