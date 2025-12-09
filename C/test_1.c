#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lecture_donnees.h"
#include "force_brute.h"
#include "ctrl_c.h"
#include <time.h>

int main(int argc, char* argv[]){
    char* nom_fichier;
    if(argc == 5){
        nom_fichier = argv[2]; 
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
        exit(1);
    }

    instance_t *instance =lire_tsplib(nom_fichier);
    if( instance ==NULL){
        fprintf(stderr,"Il y a eu une erreur pendant la lecture du fichier. Ce type de fichier peut ne pas etre gere.\n");
        exit(2);
    }

    distance_f fonction_distance = choix_distance(instance);
    
    int** demi_matrice = creer_matrice((*instance), fonction_distance);

    printf("Instance ; Méthode ; Temps CPU (sec) ; longueur ; Tour\n");
    
    printf("%s ; %s ; ", instance->nom, argv[4]+1);
    clock_t debut_time = clock();   
    tournee_t * meilleure_tournee = force_brute(instance,demi_matrice);
    clock_t fin_time = clock();
    float longueur = longueur_tournee(*instance, *meilleure_tournee,fonction_distance);
    
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;

    clock_t debut_time2 = clock();
    tournee_t * meilleure_tournee2 = force_brute2(instance, fonction_distance);
    clock_t fin_time2 = clock();
    
    double temps_ecoule2 = ((double)(fin_time2 - debut_time2))/CLOCKS_PER_SEC;
    float longueur2 = longueur_tournee(*instance, *meilleure_tournee2,fonction_distance);

    printf("%f ; %f ; [",temps_ecoule,longueur);
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", meilleure_tournee->parcours[i].num);
    }
    printf("%d]\n",meilleure_tournee->parcours[instance->dimension-1].num);

    printf("Instance ; Méthode2 ; Temps CPU (sec) 2 ; longueur ; Tour\n");
    printf("%s ; %s ; ", instance->nom, argv[4]+1);
    printf("%f ; %f ; [",temps_ecoule2,longueur2);
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", meilleure_tournee2->parcours[i].num);
    }
    printf("%d]\n",meilleure_tournee2->parcours[instance->dimension-1].num);  

    liberer_matrice(demi_matrice, instance->dimension);
    liberer_instance(&instance);

    return 0;
}