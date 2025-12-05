#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "lecture_donnees.h"
#include "force_brute.h"
#include "ctrl_c.h"
#include "heuristiques.h"
#include "genetique.h"
#include <time.h>
#include <getopt.h>
#include <string.h>

instance_t * instance;
int ** demi_matrice;
distance_f fonction_distance;
tournee_t *meilleure_tournee;
char* methode;
int nb_individus;
int nb_generations;
int taux_de_mutation;

void tournee_canonique(){
    /*calcul longueur tournee canonique*/
    

    //int longueur1 = longueur_tour_cano_matrice(*instance,demi_matrice);

    /*2. avec la fonction longueur_tournee*/
    /*creation de la tournee*/
    tournee_t tour_cano;
    tour_cano.parcours = instance->noeuds;
    tour_cano.longueur = instance->dimension;
    /*1. avec la matrice*/
        int longueur1 = calculer_longueur_matrice(&tour_cano,tour_cano.longueur,demi_matrice);
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

}

void tournee_bf(){
    clock_t debut_time = clock();   // démarrage du chronométrage
    meilleure_tournee = force_brute(instance,demi_matrice);
    clock_t fin_time = clock();
    
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;
    float longueur = meilleure_tournee->longueur;
    printf("%f ; %f ; [",temps_ecoule,longueur);
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", meilleure_tournee->parcours[i].num);
    }
    printf("%d]\n",meilleure_tournee->parcours[instance->dimension-1].num);
    //libération de la tournée 
    liberer_tournee(&meilleure_tournee);
}

void tournee_nn_ou_2opt(){
    clock_t debut_time = clock();   // démarrage du chronométrage
    meilleure_tournee = plus_proche_voisin(instance,demi_matrice);
    clock_t fin_time = clock();
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;
    float longueur = meilleure_tournee->longueur;

    if(strcmp(methode,"-2optnn")==0){
        clock_t debut_time2 = clock();
        tournee_t * meilleure_tournee2 = deux_opt(meilleure_tournee, instance);
        clock_t fin_time2 = clock();
        double temps_ecoule2 = ((double)(fin_time2 - debut_time2))/CLOCKS_PER_SEC;
        temps_ecoule = temps_ecoule + temps_ecoule2;
        
        if(meilleure_tournee2){
            liberer_tournee(&meilleure_tournee);
            meilleure_tournee = meilleure_tournee2;
        }
        longueur = meilleure_tournee->longueur;
        
    }

    printf("%f ; %f ; [",temps_ecoule,longueur);
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", meilleure_tournee->parcours[i].num);
    }
    printf("%d]\n",meilleure_tournee->parcours[instance->dimension-1].num);
    fflush(stdout);
    liberer_tournee(&meilleure_tournee);
}

void tournee_rw_ou_2opt(){
    clock_t debut_time = clock();   // démarrage du chronométrage
    tournee_t *meilleure_tournee = marche_aleatoire_matrice(instance, demi_matrice);
    clock_t fin_time = clock();
    double temps_ecoule = ((double)(fin_time - debut_time))/CLOCKS_PER_SEC;
    float longueur = meilleure_tournee->longueur;
    

    if(strcmp(methode,"-2optrw")==0){
        clock_t debut_time2 = clock();
        tournee_t *meilleure_tournee2 = deux_opt(meilleure_tournee, instance);
        clock_t fin_time2 = clock();
        double temps_ecoule2 = ((double)(fin_time2 - debut_time2))/CLOCKS_PER_SEC;
        temps_ecoule = temps_ecoule + temps_ecoule2;
        
        if(meilleure_tournee2){
            liberer_tournee(&meilleure_tournee);
            meilleure_tournee = meilleure_tournee2;
        }
        longueur = meilleure_tournee->longueur;
    } 

    printf("%f ; %f ; [",temps_ecoule,longueur);
    for (int i = 0; i < instance->dimension-1; i++){
        printf("%d,", meilleure_tournee->parcours[i].num);
    }
    printf("%d]\n",meilleure_tournee->parcours[instance->dimension-1].num);
    fflush(stdout);
    liberer_tournee(&meilleure_tournee);
}

void tournee_ga(int nb_individus,int nb_generations,int taux_de_mutation,instance_t *instance){
    printf("Instance ; Méthode ; Temps CPU (sec); longueur ; Tour\n");
    printf("%s ; ga ; ",instance->nom);
    genetique(nb_individus,nb_generations,taux_de_mutation,instance);
    
}

int main(int argc, char* argv[]){
    char* nom_fichier;

    int opt;

    while ((opt = getopt(argc, argv, "f:m:c")) != -1) {
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
    instance =lire_tsplib(nom_fichier);

    if( instance ==NULL){
        fprintf(stderr,"Il y a eu une erreur pendant la lecture du fichier. Ce type de fichier peut ne pas etre gere.\n");
        exit(2);
    }    

    if(strcmp(methode,"-ga")==0||strcmp(methode,"-gdpx")==0){
        if(argc-optind ==3){
            nb_individus = atoi(argv[optind]);
            nb_generations = atoi(argv[optind+1]);
            taux_de_mutation = atoi(argv[optind+2]);
        }else{
            fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
            /*il manque un argument*/
            liberer_instance(&instance);
            exit(1);            
        }
        tournee_ga(nb_individus,nb_generations,taux_de_mutation,instance);
        return 0;
    }

    /*choix de la fonction de distance*/
    fonction_distance = choix_distance(instance);
    /*creation de la demi matrice des distances*/
    demi_matrice = creer_matrice((*instance), fonction_distance);

    if(strcmp(methode,"tournee_canonique")==0){
        tournee_canonique();
        return 0;
    }

    printf("Instance ; Méthode ; Temps CPU (sec) ; longueur ; Tour\n");
    printf("%s ; %s ; ", instance->nom, argv[4]+1);
    

    if(strcmp(methode,"-bf")==0){
        tournee_bf();
    }
    if(strcmp(methode,"-nn")==0||strcmp(methode,"-2optnn")==0){
        tournee_nn_ou_2opt();
    }
    if(strcmp(methode,"-rw")==0||strcmp(methode,"-2optrw")==0){
        tournee_rw_ou_2opt();
    }
    if(strcmp(methode,"-all")==0){
        tournee_canonique();
        printf("Instance ; Méthode ; Temps CPU (sec) ; longueur ; Tour\n");
        tournee_nn_ou_2opt();
        printf("Instance ; Méthode ; Temps CPU (sec) ; longueur ; Tour\n");
        tournee_rw_ou_2opt();
        tournee_ga(nb_individus,nb_generations,taux_de_mutation,instance);
    }
    

    /*liberation de la memoire allouee a la matrice*/
    liberer_matrice(demi_matrice, instance->dimension);

    /*liberation de la memoire allouee a l'instance*/
    liberer_instance(&instance);

    return 0;

}


