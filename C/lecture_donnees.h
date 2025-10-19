#ifndef __LECTURE_DONNEES_H__
#define __LECTURE_DONNEES_H__

#define PI 3.141592
#define MAX_TAB_NOM 64
#define MAX_TAB_DISTANCE 16

/*************************
 * Structure d'un noeud
 *************************/
typedef struct noeud_s{
    int num; /*numero du noeud*/
    float x; /*permiere coordonnee du noeud*/
    float y; /*deuxiemene coordonnee du neoud*/
} noeud_t;

/***************************
 * Structure d'une tournee
 ***************************/

typedef struct tournee_s{
    float longueur; /*longueur de la tournee*/
    int *parcours; /*tableau de noeuds dans l'ordre de la tournee*/
} tournee_t;

//elles sont écrites au format texte dans les fichiers 
/***************************
 * Structure d'une instance (jeux de donnée)
 ***************************/
typedef struct instance_s{
    char nom[MAX_TAB_NOM];  // nom de l'instance
    char type_distance[MAX_TAB_DISTANCE]; //EUc_2D, GEO, ATT
    int dimension; //nombre de ville (points)
    noeud_t *noeuds; // tableau contenant les coordonnées de chaque villes
}instance_t;

//lecture de fichier
int lire_tsplib(const char *chemin, instance_t *inst);


/********************************************
    Fonctions des distances et de longueur
*********************************************/
int distance_euclidienne(noeud_t point1, noeud_t point2);
int distance_geographique(noeud_t point1, noeud_t point2);
int distance_euclidienne_att(noeud_t point1, noeud_t point2);
float longueur_tournee(instance_t instance,tournee_t tour, float(*f_distance)(noeud_t, noeud_t));

/*******************************
matrice inférieure de distance 
*******************************/ 

float **creer_matrice(instance_t inst, float(*f_distance)(noeud_t, noeud_t));
float récuperer_distance(float **matrice , int i , int j);
void liberer_matrice(float **matrice, int n);

#endif
