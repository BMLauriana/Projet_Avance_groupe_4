#include <stdio.h>
#include "lecture_donnees.h"


int main(int argc, char* argv[]){

    if(argc == 3){
        char* nom_fichier = argv[2] ; //récuperation du nom de fichier apres le -f
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); /*il manque un argument (-f ou la désignation du fichier)*/
        exit(1);
    }
    /*creation d'une instance pour stocker les donnees du fichier*/
    instance_t instance;

    /*appel de la fonction pour la lecture du fichier*/
    
    /*initialisation des champs de l'instance (faite par la fonction de lecture du fichier ?)*/

    /*fonction de selection de la fonction de distance en fonction de l'instance*/
    float(*f_distance)(noeud_t,noeud_t) choix_distance(instance_t inst){
        /*A FAIRE*/
    }

    float(*f_distance)(noeud_t,noeud_t) type_distance = choix_distance(inst);
    

    /*creation de la demi matrice des distances*/
    float** demi_matrice = creer_matrice(instance, type_distance);
/**/

    /*calcul tournee canonique*/



    /*affichage resultat calcul de la tournee canonique*/



    /*liberation de la memoire allouee a la matrice*/
    void liberer_matrice(demi_matrice, int n);
/**/



    /*faire un main C, admettant en paramètre de la ligne de commande, la balise -f suivie d’un nom de
fichier et -c, affichant les données lues et calculant la longueur de la tournée canonique 2.*/
    return 0;
}
