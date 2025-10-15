#ifndef __LECTURE_DONNEES_H__
#define __LECTURE_DONNEES_H__

#define PI 3.141592

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

#endif
