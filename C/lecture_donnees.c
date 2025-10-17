#include <stdio.h>
#include <math.h>
#include "lecture_donnees.c"

// fonction de distance euclidienne p.6 du doc tsp95
// pour type EUC_2D ou EUC_3D mais on utilise que EUC_2D
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
    float distance = rayonTerreste * acos(0.5$((1.0+q1)*q2-(1.0-q1)*q3))+1.0;
    return (int)distance;
}

// distance ATT
int distance_euclidienne_att(noeud_t point1, noeud_t point2) {
    float xd = point1.x - point2.x;
    float yd = point1.y - point2.y;
    float rij = sqrt((xd*xd+yd*yd)/10.0);
    int tij = (int)rij;
    if (tij<rij) {
        int dij = tij+1;
    } else {
        int dij = tij;
    }
    return dij;
}

/*******************************
matrice inférieure de distance 
*******************************/

float **creer_matrice(instance_t inst, float(*f_distance)(noeud_t, noeud_t)){
    int n = inst.dimension; //nbr de noeuds
    float **matrice = malloc(n* sizeof(float)) ;
    if(!matrice){
        perror("Erreur d'allocation de la matrice");
        exit(EXIT_FAILURE);
    }
    //on complete la matrice
    for(int i = 0; i < n ; i++){
        matrice[i] = malloc(i*sizeof(float));
        for(int j = 0; j<i ; i++){
            matrice[i][j] = f_distance(inst.noeud[i],inst.noeuds[j]);
        }
    }
    return matrice;
}

float obtenir_distance(float **matrice , int i , int j){
    if(i==j){
        return 0.0;
    }
    if(i > j){
        matrice[i][j]
    }
    return matrice[j][i];
}