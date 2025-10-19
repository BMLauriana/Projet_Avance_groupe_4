#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "lecture_donnees.h"

//chemin is the file path 
int lire_tsplib(const char *chemin, instance_t *inst)
{
    //Error handling
    if (!chemin || !inst) return -1;
    
    const char *ext = strrchr(chemin, '.'); 
    if (!ext || strcmp(ext, ".tsp") != 0) {
        fprintf(stderr, "Erreur : le fichier '%s' n'est pas un fichier .tsp\n", chemin);
        return -1;
    }

    
    FILE *f = fopen(chemin, "r");
    //Error handling
    if (!f) {
        perror("Erreur d'ouverture du fichier");
        return -1;
    }

    inst->nom[0] = '\0';
    inst->type_distance[0] = '\0';
    inst->dimension = 0;
    inst->noeuds = NULL;
//Temp Buffer
    char line[512];
    int have_node_section = 0;
    //Reading the file
   while (fgets(line, sizeof line, f)) {
    //The name of the instance
        if (strncmp(line, "NAME", 4) == 0)
            sscanf(line, "NAME : %63s", inst->nom);
        //Reading the number of cities
        else if (strncmp(line, "DIMENSION", 9) == 0)
            sscanf(line, "DIMENSION : %d", &inst->dimension);
        //What kind of distence is used AKA seperating the files 
        //QUESTION:Shoild i write a function to seperate the type too?
        else if (strncmp(line, "EDGE_WEIGHT_TYPE", 16) == 0)
            sscanf(line, "EDGE_WEIGHT_TYPE : %15s", inst->type_distance);
        //This line is for when we start reading the coordinations
            else if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) {
            have_node_section = 1;
            break;
        }
    }

//Error handling to see if we really found the coordinates
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

/*********************
Longueur d'une tournée 
**********************/

float longueur_tournee(instance_t instance, tournee_t tour, int(*f_distance)(noeud_t, noeud_t)) {
    float longueur_totale = 0;
    for (int i=0; i<instance.dimension; i++) {
        int debut = tour.parcours[i];
        // quand on atteint le dernier i, l'indice va donner 0
        // donc on revient au début du parcours 
        // ex : 0 → 1 → 2 → 3 → 0
        int fin = tour.parcours[(i+1)%instance.dimension];
        // recuperer les coordonnees des villes
        noeud_t ville_debut = instance.noeuds[debut-1];
        noeud_t ville_fin = instance.noeuds[fin-1];
        longueur_totale += f_distance(ville_debut,ville_fin);
    }
    tour.longueur = longueur_totale;
    return longueur_totale; 
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

void liberer_matrice(int **matrice, int n){
    for(int i= 0; i < n; i++){
        free(matrice[i]);
    }
    free(matrice);
}

/****************
Tournée canonique  
*****************/
void generer_tournee_canonique(tournee_t *t, int n){
    t->parcours = malloc(n*sizeof(int));
    for(int i = 0; i < n ; i++){
        t->parcours[i] = i+1;
    }
}



