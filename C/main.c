#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lecture_donnees.h"
#include "force_brute.h"
#include "ctrl_c.h"
#include "heuristiques.h"
#include <time.h>
#include <getopt.h>
#include <string.h>

int main(int argc, char* argv[]){
    char* nom_fichier;
    char* methode;
    int opt;

    while ((opt = getopt(argc, argv, "f:cm:")) != -1) {
        switch (opt) {
        case 'f':
            nom_fichier = optarg;
            break;
        case 'm':
            methode = optarg;
            break;
        case 'c':
            methode = "tournee_canonique";
            break;
        default:
            fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
            /*il manque un argument (-f ou la désignation du fichier)*/
            exit(1);
        }
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

    if(strcmp(methode,"tournee_canonique")==0){

        /*calcul longueur tournee canonique*/
        /*1. avec la matrice*/
        int longueur1 = longueur_tour_cano_matrice(*instance,demi_matrice);

        /*2. avec la fonction longueur_tournee*/
        /*creation de la tournee*/
        tournee_t tour_cano;
        tour_cano.parcours = instance->noeuds;
        /*calcul de la longueur*/
        float longueur2 = longueur_tournee(*instance,tour_cano, fonction_distance);

        printf("Instance ; Méthode ; Temps CPU (sec); longueur ; Tour\n");
        printf("%s ; .. ; 0.00 ; %d ; ",instance->nom,longueur1); 
        /*affichage Tour canonique*/
        printf("[ ");
        for(int i=0;i< instance->dimension -1;i++){ //on s'arrete a l'avant dernier noeud
            printf("%d, ",instance->noeuds[i].num);
        }
        printf("%d ]\n", instance->noeuds[instance->dimension-1].num); //affichage du dernier noeud sans la virgule
        fflush(stdout);
        /*affichage du calcul de la longeur tournée canonique*/
        printf("Longueur pour la tournee canonique (version matrice) : %d\n", longueur1);
        printf("Longueur pour la tournee canonique (version fonction) : %f\n",longueur2);

        /*liberation de la memoire allouee a la matrice*/
        liberer_matrice(demi_matrice, instance->dimension);

        /*liberation de la memoire allouee a l'instance*/
        liberer_instance(&instance);
        return 0;
    }

    printf("Instance ; Méthode ; Temps CPU (sec) ; longueur ; Tour\n");
    printf("%s ; %s ; ", instance->nom, argv[4]+1);
    tournee_t meilleure_tournee;

    if(strcmp(methode,"-bf")==0){

        clock_t debut_time = clock();   // démarrage du chronométrage
        meilleure_tournee = force_brute(instance,demi_matrice);
        clock_t fin_time = clock();
        
        double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;
        float longueur = meilleure_tournee.longueur;
        printf("%f ; %f ; [",temps_ecoule,longueur);
        for (int i = 0; i < instance->dimension-1; i++){
            printf("%d,", meilleure_tournee.parcours[i].num);
        }
        printf("%d]\n",meilleure_tournee.parcours[instance->dimension-1].num);
    }
    if(strcmp(methode,"-nn")==0||strcmp(methode,"-2optnn")==0){
        clock_t debut_time = clock();   // démarrage du chronométrage
        tournee_t * meilleure_tournee2 = plus_proche_voisin(instance,demi_matrice);
        clock_t fin_time = clock();
        double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;
        float longueur = meilleure_tournee2->longueur;

        if(strcmp(methode,"-2optnn")==0){
            clock_t debut_time2 = clock();
            meilleure_tournee2 = deux_opt(meilleure_tournee2, demi_matrice, instance);
            clock_t fin_time2 = clock();
            double temps_ecoule2 = ((double)(fin_time2 - debut_time2))/CLOCKS_PER_SEC;
            temps_ecoule = temps_ecoule + temps_ecoule2;
            longueur = meilleure_tournee2->longueur;
        }

        printf("%f ; %f ; [",temps_ecoule,longueur);
        for (int i = 0; i < instance->dimension-1; i++){
            printf("%d,", meilleure_tournee2->parcours[i].num);
        }
        printf("%d]\n",meilleure_tournee2->parcours[instance->dimension-1].num);
        fflush(stdout);
        liberer_tournee(&meilleure_tournee2);
    }
    if(strcmp(methode,"-rw")==0||strcmp(methode,"-2optrw")==0){
        clock_t debut_time = clock();   // démarrage du chronométrage
        meilleure_tournee = marche_aleatoire_matrice(instance,demi_matrice);
        clock_t fin_time = clock();
        double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;
        float longueur = meilleure_tournee.longueur;
        

        if(strcmp(methode,"-2optrw")==0){
            clock_t debut_time2 = clock();
            meilleure_tournee = *deux_opt(&meilleure_tournee, demi_matrice, instance);
            clock_t fin_time2 = clock();
            double temps_ecoule2 = ((double)(fin_time2 - debut_time2))/CLOCKS_PER_SEC;
            temps_ecoule = temps_ecoule + temps_ecoule2;
            longueur = meilleure_tournee.longueur;
        } 

        printf("%f ; %f ; [",temps_ecoule,longueur);
        for (int i = 0; i < instance->dimension-1; i++){
            printf("%d,", meilleure_tournee.parcours[i].num);
        }
        printf("%d]\n",meilleure_tournee.parcours[instance->dimension-1].num);
        fflush(stdout);
    }

    /*liberation de la memoire allouee a la matrice*/
    liberer_matrice(demi_matrice, instance->dimension);

    /*liberation de la memoire allouee a l'instance*/
    liberer_instance(&instance);

    return 0;

}


