#include <stdio.h>
#include <stdlib.h>
#include "lecture_donnees.h"

int main(int argc,  char* argv[]){
    //test matrice 

    char* nom_fichier;
    if(argc == 2){
        nom_fichier = argv[1]; //récuperation du nom de fichier
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
        exit(1);
    }
    //appel de la fonction pour la lecture du fichier
    instance_t *instance =lire_tsplib(nom_fichier);
    if( instance ==NULL){
        fprintf(stderr,"Il y a eu une erreur pendant la lecture du fichier. Ce type de fichier peut ne pas etre gere.\n");
        exit(2);
    }
    //choix de la fonction de distance
    distance_f fonction_distance = choix_distance(instance);
    if(!fonction_distance){
        fprintf(stderr, "il y a une erreur pour le type de distance.\n");
        liberer_instance(&instance);
        exit(EXIT_FAILURE);
    }

    printf("\n");
    //creation de la demi matrice des distances
    int** demi_matrice = creer_matrice((*instance), fonction_distance);

    printf("Matrice des distances de %s avec %d villes : \n" , instance->nom, instance->dimension);
    for(int i = 0; i <instance->dimension; i++){
        for(int j= 0; j< i; j++){
            printf("%5d",demi_matrice[i][j]);
        }
        printf("\n");
    }

    //libération de la matrice
    liberer_matrice(demi_matrice, instance->dimension);
    liberer_instance(&instance);

}