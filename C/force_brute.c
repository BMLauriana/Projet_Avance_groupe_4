#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "lecture_donnees.h"
#include "force_brute.h"
#include "ctrl_c.h"


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

//calcule la longueur d’une tournée (via indices)
int calculer_longueur_tournee(int *ordre, int n, int **matrice) {
    int longueur = 0;
    for (int i = 0; i < n - 1; i++)
        longueur += recuperer_distance(matrice, ordre[i], ordre[i + 1]);
    longueur += recuperer_distance(matrice, ordre[n - 1], ordre[0]);
    return longueur;
}

static void print_perm_nodes(instance_t *inst, int *perm, int n) {
    for (int i = 0; i < n; ++i) {
        int idx = perm[i];
        printf("%d%s", inst->noeuds[idx].num, (i + 1 < n) ? " " : "");
    }
}

tournee_t force_brute(instance_t *inst, int **matrice) {
    install_ctrl_c_handler();
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
       
       if (interrompre) {
            interrompre = 0;

            printf("\n== Interruption (Ctrl-C) ==\n");
            printf("Permutation courante : ");
            print_perm_nodes(inst, ordre, n);
            printf("\n");
            printf("Meilleure tournée courante : ");
            print_perm_nodes(inst, meilleur_ordre, n);
            printf("\n");
            printf("Longueur meilleure courante : %d\n", meilleure_longueur);
            printf("Souhaitez-vous quitter (q) ou continuer (c) ? ");
            fflush(stdout);

            int ch = getchar();
            if (ch != '\n') {
                int nl = getchar(); (void)nl;
            }

            if (ch == 'q' || ch == 'Q') {
                
                break;
            } else {
                printf("Reprise des calculs...\n");
            }
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


//calcule la longueur d’une tournée (via indices) en utilisant la fonction de distance
tournee_t force_brute2(instance_t *inst, distance_f f_distance) {
    install_ctrl_c_handler();
    int n = inst->dimension;
    int *ordre = malloc(n * sizeof(int));          
    int *meilleur_ordre = malloc(n * sizeof(int)); 

    //tournée canonique : 0 → 1 → 2 → … → n-1
    for (int i = 0; i < n; i++)
        ordre[i] = i;

    //initialiser avec la tournée canonique
    // Créer une tournee_t temporaire pour la tournée canonique
    tournee_t tour_temp;
    tour_temp.parcours = malloc(n * sizeof(noeud_t));
    for (int i = 0; i < n; i++) {
        tour_temp.parcours[i] = inst->noeuds[ordre[i]];
    }
    float meilleure_longueur = longueur_tournee(*inst, tour_temp, f_distance);
    memcpy(meilleur_ordre, ordre, n * sizeof(int));
    free(tour_temp.parcours);  // Libérer la mémoire temporaire

    while (next_permutation(ordre, n)) {
        //créer une tournee_t temporaire pour la permutation courante
        tour_temp.parcours = malloc(n * sizeof(noeud_t));
        for (int i = 0; i < n; i++) {
            tour_temp.parcours[i] = inst->noeuds[ordre[i]];
        }
        float longueur = longueur_tournee(*inst, tour_temp, f_distance);
        if (longueur < meilleure_longueur) {
            meilleure_longueur = longueur;
            memcpy(meilleur_ordre, ordre, n * sizeof(int));
        }
        if (interrompre) {
            interrompre = 0;

            printf("\n== Interruption (Ctrl-C) ==\n");
            printf("Permutation courante : ");
            print_perm_nodes(inst, ordre, n);
            printf("\n");
            printf("Meilleure tournée courante : ");
            print_perm_nodes(inst, meilleur_ordre, n);
            printf("\n");
            printf("Longueur meilleure courante : %f\n", meilleure_longueur);
            printf("Souhaitez-vous quitter (q) ou continuer (c) ? ");
            fflush(stdout);

            int ch = getchar();
            if (ch != '\n') {
                int nl = getchar(); (void)nl;
            }

            if (ch == 'q' || ch == 'Q') {
                
                break;
            } else {
                printf("Reprise des calculs...\n");
            }
    }
        //on libère la mémoire temporaire
        free(tour_temp.parcours);  
    }

    //construction de la structure finale tournee_t (celle qu'on retourne)
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
    printf("\n");

    free(ordre);
    free(meilleur_ordre);

    return meilleure_tournee;
}





