#ifndef __LECTURE_DONNEES_H__
#define __LECTURE_DONNEES_H__

typedef struct noeud_s noeud_t;
typedef struct tournee_s tournee_t;
typedef struct instance_s instance_t;

//lecture de fichier
instance_t lire_tsplib(const char *chemin);
void liberer_instance(instance_t ** inst);

/********************************************
    Fonctions des distances et de longueur
*********************************************/
int distance_euclidienne(noeud_t point1, noeud_t point2);
int distance_geographique(noeud_t point1, noeud_t point2);
int distance_euclidienne_att(noeud_t point1, noeud_t point2);
float longueur_tournee(instance_t instance,tournee_t tour, int(*f_distance)(noeud_t, noeud_t));

/*fonction de selection de la fonction de distance en fonction de l'instance*/
int(*f_distance)(noeud_t,noeud_t) choix_distance(instance_t inst);


/*******************************
matrice inférieure de distance 
*******************************/ 
int **creer_matrice(instance_t inst, int(*f_distance)(noeud_t, noeud_t));
int recuperer_distance(int **matrice , int i , int j);
/*calcule la longueur de la tournee canonique a l'aide de la matrice*/
int longueur_tour_cano_matrice(instance_t inst, int ** matrice);
void liberer_matrice(int **matrice, int n);


#endif
