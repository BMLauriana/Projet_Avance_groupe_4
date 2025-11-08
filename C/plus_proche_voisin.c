#include "lecture_donnees.h"
#include "heuristiques.h"
#include <stdlib>

tournee_t * plus_proche_voisin(instance_t * inst, int** matrice){
    int n = inst->dimension;
    tournee_t * tournee;
    tournee->parcours =malloc(n*sizeof(noeud_t));
    bool * visite = malloc(n*sizeof(bool));

    for(int i=0; i<n; i++){
        visite[i] = false;
    }
    int noeudCourant = 0;
    tournee->parcours[0] = inst->noeuds[0]; //pour le noeud 1
    visite[0]=false;

    for(int i=1; i<n;i++){
        int plus_proche =-1;
        int distance_min =0;

        for(int j=0; j<n;j++){
            if(visite[j] ==false){
                int distanceCourante = recuperer_distance(matrice,noeudCourant, j);
                if(distanceCourante<distance_min){
                    distance_min = distanceCourante;
                    plus_proche = j;
                }
            }
        }
        
        if(plus_proche!=-1){
            tournee->parcours[i] = inst->noeuds[plus_proche];
            visite[plus_proche] = true;
            noeudCourant = plus_proche;
        }

    }

    // on libere la memoire
    free(nonVisite);
    liberer_matrice(matrice);
}