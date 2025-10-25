#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lecture_donnees.h"
#include "force_brute.h"

/***************************************************
 * Génération de la permutation suivante (itératif)
 ***************************************************/

 /*principe : 
 on cherche i tel que a[i] < a[i+1] en parant de droite à gauche
 on chercher ensuite j tel que a[j] > a[i] en prenant le plus grand j à droite
 on echange ensuite a[i] et a [j]
 inverser ensuite la sous liste */

void permuter(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

//a c'est un tableau d'entier et n c'est la taille du tableau
int next_permutation(int *tab, int n) {
    int i = n - 2;
    while (i >= 0 && tab[i] >= tab[i + 1]){
        i--;
        if (i < 0){
            return 0;
        }
    }
    int j = n - 1;
    while (tab[j] <= tab[i])
        j--;

    permuter(&tab[i], &tab[j]);

    int k = i + 1, l = n - 1;
    while (k < l) {
        permuter(&tab[k], &tab[l]);
        k++;
        l--;
    }
    return 1;
}


/***************************************************
 * Algorithme de force brute — teste toutes les permutations
 * Retourne la meilleure tournée trouvée
 ***************************************************/
tournee_t force_brute(instance_t *inst, int **matrice) {
    int n = inst->dimension;
    int *ordre = malloc(n * sizeof(int));          
    int *meilleur_ordre = malloc(n * sizeof(int)); 

    //tournée canonique : 0 → 1 → 2 → … → n-1
    for (int i = 0; i < n; i++)
        ordre[i] = i;

    //initialiser avec la tournée canonique, on essaie toutes les permutations possibles
    int meilleure_longueur = calculer_longueur_tournee(ordre, n, matrice);
    memcpy(meilleur_ordre, ordre, n * sizeof(int));
    while (next_permutation(ordre, n)) {
        int longueur = calculer_longueur_tournee(ordre, n, matrice);
        if (longueur < meilleure_longueur) {
            meilleure_longueur = longueur;
            memcpy(meilleur_ordre, ordre, n * sizeof(int));
        }
    }

    //construction de la structure finale tournee_t
    tournee_t meilleure_tournee;
    meilleure_tournee.longueur = meilleure_longueur;
    meilleure_tournee.parcours = malloc(n * sizeof(noeud_t));

    for (int i = 0; i < n; i++){
        meilleure_tournee.parcours[i] = inst->noeuds[meilleur_ordre[i]];
    }
    //affichage du résultat final
    printf("\n=== Force brute ===\n");
    printf("Instance : %s\n", inst->nom);
    printf("Nombre de villes : %d\n", n);
    printf("Longueur optimale : %.0f\n", meilleure_tournee.longueur);
    printf("Tournée optimale : ");
    for (int i = 0; i < n; i++){
        printf("%d ", meilleure_tournee.parcours[i].num);
    }
    free(ordre);
    free(meilleur_ordre);

    return meilleure_tournee;
}





