#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lecture_donnees.c"


int lire_tsplib(const char *chemin, instance_t *inst)
{
    if (!chemin || !inst) return -1;

    FILE *f = fopen(chemin, "r");
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }


    inst->nom[0] = '\0';
    inst->type_distance[0] = '\0';
    inst->dimension = 0;
    inst->noeuds = NULL;

    char line[512];
    int have_node_section = 0;
   while (fgets(line, sizeof line, f)) {
        if (strncmp(line, "NAME", 4) == 0)
            sscanf(line, "NAME : %63s", inst->nom);
        else if (strncmp(line, "DIMENSION", 9) == 0)
            sscanf(line, "DIMENSION : %d", &inst->dimension);
        else if (strncmp(line, "EDGE_WEIGHT_TYPE", 16) == 0)
            sscanf(line, "EDGE_WEIGHT_TYPE : %15s", inst->type_distance);
        else if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) {
            have_node_section = 1;
            break;
        }
    }


    if (!have_node_section || inst->dimension <= 0) {
        fprintf(stderr, "Erreur d'en-tête \n", chemin);
        fclose(f);
        return -1;
    }

    inst->noeuds = malloc((size_t)inst->dimension * sizeof(noeud_t));
    if (!inst->noeuds) {
        fprintf(stderr, "Erreur d'allocation mémoire pour %d noeuds\n", inst->dimension);
        fclose(f);
        return -1;
    }
    int count = 0, id;
    float x, y;
    while (count < inst->dimension && fgets(line, sizeof line, f)) {
        if (strncmp(line, "EOF", 3) == 0) break;
        if (sscanf(line, "%d %f %f", &id, &x, &y) == 3) {
            inst->noeuds[count].num = id;
            inst->noeuds[count].x   = x;
            inst->noeuds[count].y   = y;
            count++;
        }
    }

    fclose(f);

    if (count != inst->dimension) {
        fprintf(stderr, "Erreur : %d coordonnées lues au lieu de %d dans %s\n",
                count, inst->dimension, chemin);
        free(inst->noeuds);
        inst->noeuds = NULL;
        return -1;
    }


    printf("Lecture réussie : %s (%s, %d villes)\n",
           inst->nom, inst->type_distance, inst->dimension);

    return 0;
}

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

void liberer_matrice(float **matrice, int n){
    for(int i= 0; i < n; i++){
        free(matrice[i]);
    }
    free(matrice);
}