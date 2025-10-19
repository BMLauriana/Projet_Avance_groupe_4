#include <stdio.h>
#include "lecture_donnees.h"


int main(int argc, char* argv[]){

    if(argc == 3){
        char* nom_fichier = argv[2]; //récuperation du nom de fichier apres le -f
    }else{
        fprintf(stderr,"Usage: %s desigation_fichier[...]\n",argv[0]); 
        /*il manque un argument (-f ou la désignation du fichier)*/
        exit(1);
    }
    /*****************************BEAUCOUP DE CHANGEMENTS A PARTIR D'ICI*******************************/
    /*creation d'une instance pour stocker les donnees du fichier*/
    instance_t *instance = malloc(sizeof(struct instance_s)); /*??????*/
    /*je crois que la fonction lire_tsplib doit retourner l'instance
    et non la prendre en parametre sinon on se retrouve a gerer la memoire
    dans le main ... malloc bizare (on n'est pas sense avoir acces a
    struct instance_s)*/

    /*appel de la fonction pour la lecture du fichier changements possibles ??*/
    int resultat_appel;
    resultat_appel = lire_tsplib(nom_fichier, instance);

    /*affichage des donnees lues*/
    printf("J'ouvre le fichier : %s", instance->nom);

    /*******MODIFICATIONS A FAIRE POUR LE COMMENTAIRE ET LE NOMBRE DE LIGNES LUES*********/
    /*ce sera peut etre pour l'affichage de la partie 1 mais
    il faudra faire des changements sur lire_tsplib du coup
    je ne sais pas si il faut aficher tout ce qui a ete lu 
    et non ce qui est demande d'afficher dans lesdiapos*/
    printf("Commentaire : \n");
    printf(" lignes lues\n");
    printf("Distance %s",instance->type_distance);

    printf("Instance ; Methode ; Temps CPU ; longueur ; Tour\n");
    /*la methode sera ecrite a paertir de la partie 1*/
    printf("%s ; none ; temps CPU ? ; %f ; "); 

    /*fonction de selection de la fonction de distance en fonction de l'instance*/
    float(*f_distance)(noeud_t,noeud_t) choix_distance(instance_t inst){
        /*A FAIRE DANS LE .C ?*/
    }

    float(*f_distance)(noeud_t,noeud_t) type_distance = choix_distance(inst);
    

    /*creation de la demi matrice des distances*/
    float** demi_matrice = creer_matrice(instance, &type_distance);


    /*calcul longueur tournee canonique*/
    /*MODIFICAITON POSSIBLE DU STRUCT DE LA TOURNEE*/
    /*j'utilise la matrice ou bien longeur tournee ?*/


    /*affichage resultat calcul de la tournee canonique*/


    /*liberation de la memoire allouee a la matrice*/
    void liberer_matrice(demi_matrice, int n);

    /*liberation de la memoire allouee a l'instance*/
    /*LIBERER LES NOEUDS AVANT ? 
    ce serait mieux d'avoir une fonction qui libere la memoire
    dans lecture_donnee*/


    /*faire un main C, admettant en paramètre de la ligne de commande, la balise -f suivie d’un nom de
fichier et -c, affichant les données lues et calculant la longueur de la tournée canonique 2.*/
    return 0;
}
