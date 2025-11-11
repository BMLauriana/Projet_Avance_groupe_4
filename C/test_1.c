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
        nom_fichier = argv[2]; //récuperation du nom de fichier apres le -f
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
        /*il manque un argument (-f ou la désignation du fichier)*/
        exit(1);
    }


    /*appel de la fonction pour la lecture du fichier*/
    instance_t *instance =lire_tsplib(nom_fichier);
    if( instance ==NULL){
        fprintf(stderr,"Il y a eu une erreur pendant la lecture du fichier. Ce type de fichier peut ne pas etre gere.\n");
        exit(2);
    }


    /*choix de la fonction de distance*/
    distance_f fonction_distance = choix_distance(instance);
    

    /*creation de la demi matrice des distances*/
    int** demi_matrice = creer_matrice((*instance), fonction_distance);


    /*2. avec la fonction longueur_tournee*/
    /*creation de la tournee*/
    //tournee_t tour_cano;
    //tour_cano.parcours = instance->noeuds;
    /*calcul de la longueur*/
    //float longueur2 = longueur_tournee(*instance,tour_cano, fonction_distance);

    /*affichage du calcul de la longeur tournée canonique*/
    // printf("Longueur pour la tournee canonique (version matrice) : %f\n", longueur2);

    /*(partie0) faire un main C, admettant en paramètre de la ligne de commande, la balise -f suivie d’un nom de
fichier et -c, affichant les données lues et calculant la longueur de la tournée canonique 2.*/

    printf("Instance ; Méthode ; Temps CPU (sec) ; longueur ; Tour\n");
    
    //printf("%s ; rw ; 0.00 ; %f ; \n",instance->nom,longueur2); 
    printf("%s ; %s ; ", instance->nom, argv[4]+1);
    clock_t debut_time = clock();   // démarrage du chronométrage
    tournee_t meilleure_tournee = force_brute(instance,demi_matrice);
    clock_t fin_time = clock();
    
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;/*
    clock_t debut_time = clock();
    tournee_t meilleure_tournee2 = force_brute2(instance, fonction_distance);
    clock_t fin_time = clock();
    
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;*/
    float longueur = longueur_tournee(*instance, meilleure_tournee,fonction_distance);
    printf("%f ; %f ; [",temps_ecoule,longueur);
    //modifier meilleure_tournee /meilleure_tournee2 en fonction
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", meilleure_tournee.parcours[i].num);
    }
    printf("%d]\n",meilleure_tournee.parcours[instance->dimension-1].num);
    /*liberation de la memoire allouee a la matrice*/
    liberer_matrice(demi_matrice, instance->dimension);

    /*liberation de la memoire allouee a l'instance*/
    liberer_instance(&instance);

    /*(partie1) Implémenter un main C pour lire un fichier TSPLIB, sélectionner la fonction de distance adéquate
(GEO, ATT, EUCL_2D, coordonnées ou matrice), lancer la fonction précédente, et afficher le résultat
normalisé (voir annexe)*/
    return 0;
}