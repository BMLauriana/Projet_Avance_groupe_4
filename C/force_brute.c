#include <stdio.h>
#include <stdlib.h>
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



