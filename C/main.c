#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lecture_donnees.h"
#include "force_brute.h"
#include "ctrl_c.h"

int main(int argc, char* argv[]){
    /**********************************DEBUT PARTIE 0**********************************/
    char* nom_fichier;
    if(argc == 4){
        nom_fichier = argv[2]; //récuperation du nom de fichier apres le -f
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
        /*il manque un argument (-f ou la désignation du fichier)*/
        exit(1);
    }

    printf("*********************Debut de l'affichage de la partie 0*********************\n");

    /*appel de la fonction pour la lecture du fichier*/
    instance_t *instance =lire_tsplib(nom_fichier);
    if( instance ==NULL){
        fprintf(stderr,"Il y a eu une erreur pendant la lecture du fichier. Ce type de fichier peut ne pas etre gere.\n");
        exit(2);
    }
    install_ctrl_c_handler();

    /*choix de la fonction de distance*/
    distance_f fonction_distance = choix_distance(instance);
    

    /*creation de la demi matrice des distances*/
    int** demi_matrice = creer_matrice((*instance), fonction_distance);


    /*calcul longueur tournee canonique*/
    /*1. avec la matrice*/
    int longueur1 = longueur_tour_cano_matrice(*instance,demi_matrice);

    /*2. avec la fonction longueur_tournee*/
    /*creation de la tournee*/
    tournee_t tour_cano;
    tour_cano.parcours = instance->noeuds;
    /*calcul de la longueur*/
    float longueur2 = longueur_tournee(*instance,tour_cano, fonction_distance);

    /*affichage du calcul de la longeur tournée canonique*/
    printf("Longueur pour la tournee canonique (version matrice) : %d\n", longueur1);
    printf("Longueur pour la tournee canonique (version fonction) : %f\n",longueur2);

    /*affichage Tour*/
    printf("Tournee canonique : [");
    for(int i=0;i< instance->dimension -1;i++){ //on s'arrete a l'avant dernier noeud
        printf("%d, ",instance->noeuds[i].num);
    }
    printf("%d ]\n", instance->noeuds[instance->dimension-1].num); //affichage du dernier noeud sans la virgule


    /*(partie0) faire un main C, admettant en paramètre de la ligne de commande, la balise -f suivie d’un nom de
fichier et -c, affichant les données lues et calculant la longueur de la tournée canonique 2.*/


/**********************************DEBUT PARTIE 1**********************************/

    printf("\n\n*********************Debut de l'affichage de la partie 1*********************\n");

    printf("Instance ; Méthode ; Temps CPU (sec); longueur ; Tour\n");
    /*la methode sera ecrite a partir de la partie 1*/
    printf("%s ; rw ; 0.00 ; %d ; \n",instance->nom,longueur1); 
    tournee_t meilleure_tournee = force_brute(instance,demi_matrice);

    /*liberation de la memoire allouee a la matrice*/
    liberer_matrice(demi_matrice, instance->dimension);

    /*liberation de la memoire allouee a l'instance*/
    liberer_instance(&instance);

    /*(partie1) Implémenter un main C pour lire un fichier TSPLIB, sélectionner la fonction de distance adéquate
(GEO, ATT, EUCL_2D, coordonnées ou matrice), lancer la fonction précédente, et afficher le résultat
normalisé (voir annexe)*/
    return 0;
}
