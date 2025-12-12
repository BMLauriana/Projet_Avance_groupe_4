#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "genetique.h"
#include "heuristiques.h"
#include "lecture_donnees.h"

/* ---------- arêtes communes (sur les villes, via .num) ---------- */
static int aretes_communes(int position, tournee_t *p1, tournee_t *p2, int n){
    int a = p1->parcours[position].num;
    int b = p1->parcours[(position + 1) % n].num;

    for (int j = 0; j < n; j++) {
        int c = p2->parcours[j].num;
        int d = p2->parcours[(j + 1) % n].num;
        if ((c == a && d == b) || (c == b && d == a)) return 1;
    }
    return 0;
}


tournee_t* dpx_crossover(tournee_t* parent1, tournee_t* parent2,int n, instance_t* inst, int **matrice){
    if (n <= 1) {
        tournee_t* fille_triv = malloc(sizeof(tournee_t));
        fille_triv->parcours = malloc(n * sizeof(noeud_t));
        for (int i = 0; i < n; i++) fille_triv->parcours[i] = parent1->parcours[i];
        return deux_opt(fille_triv, inst);
    }

    /* construire frag_id : fragments = suites d'aretes communes dans l'ordre de parent1 */
    int *frag_id = malloc(n * sizeof(int));
    if (!frag_id) {
         perror("malloc frag_id"); 
         exit(EXIT_FAILURE); 
        }

    int nb_frag = 1;
    frag_id[0] = 0;

    for (int i = 0; i < n - 1; i++) {
        if (aretes_communes(i, parent1, parent2, n)) {
            frag_id[i + 1] = nb_frag - 1;   
        } else {
            frag_id[i + 1] = nb_frag;      
            nb_frag++;
        }
    }

    /* calculer start/len de chaque fragment (indices dans parent1) */
    int *frag_start = malloc(nb_frag * sizeof(int));
    int *frag_len   = malloc(nb_frag * sizeof(int));
    if (!frag_start || !frag_len) {
         fprintf(stderr, "malloc frag_start/frag_len\n"); 
         exit(EXIT_FAILURE); 
        }

    for (int f = 0; f < nb_frag; f++) {
        frag_start[f] = -1;
        frag_len[f] = 0;
    }

    frag_start[ frag_id[0] ] = 0;
    int curr = frag_id[0];

    for (int i = 1; i < n; i++) {
        int f = frag_id[i];
        if (f != curr) {
            frag_len[curr] = i - frag_start[curr];
            curr = f;
            frag_start[curr] = i;
        }
    }
    frag_len[curr] = n - frag_start[curr];

    
    int max_num = 0;
    for (int i = 0; i < n; i++) {
        if (inst->noeuds[i].num > max_num) max_num = inst->noeuds[i].num;
    }
    int taille_map = max_num + 1;
    int *num_vers_indice = malloc(taille_map * sizeof(int));
    if (!num_vers_indice) { 
        perror("malloc num_vers_indice");
         exit(EXIT_FAILURE); 
        }
    for (int i = 0; i < taille_map; i++) num_vers_indice[i] = -1;
    for (int i = 0; i < n; i++) {
        int num = inst->noeuds[i].num;
        if (num >= 0 && num < taille_map) num_vers_indice[num] = i;
    }

    /* Preparer les extremites des fragments en indices */
    int *frag_debut_idx = malloc(nb_frag * sizeof(int));
    int *frag_fin_idx   = malloc(nb_frag * sizeof(int));
    bool *frag_utilise  = calloc(nb_frag, sizeof(bool));
    if (!frag_debut_idx || !frag_fin_idx || !frag_utilise) {
        fprintf(stderr, "malloc frag endpoints\n");
        exit(EXIT_FAILURE);
    }

    for (int f = 0; f < nb_frag; f++) {
        int s = frag_start[f];
        int l = frag_len[f];
        int num_debut = parent1->parcours[s].num;
        int num_fin   = parent1->parcours[s + l - 1].num;

        if (num_debut < 0 || num_debut >= taille_map || num_vers_indice[num_debut] < 0 ||
            num_fin   < 0 || num_fin   >= taille_map || num_vers_indice[num_fin]   < 0) {
            frag_debut_idx[f] = s;
            frag_fin_idx[f]   = s + l - 1;
        } else {
            frag_debut_idx[f] = num_vers_indice[num_debut];
            frag_fin_idx[f]   = num_vers_indice[num_fin];
        }
    }

    /* Construire la tournee fille par reconnexion plus proche voisin des fragments */
    noeud_t *nouveau_parcours = malloc(n * sizeof(noeud_t));
    if (!nouveau_parcours) { 
        perror("malloc nouveau_parcours"); 
        exit(EXIT_FAILURE); 
    }

    int idx = 0;
    frag_utilise[0] = true;
    for (int t = 0; t < frag_len[0]; t++) {
        nouveau_parcours[idx++] = parent1->parcours[frag_start[0] + t];
    }

    int num_courant = nouveau_parcours[idx - 1].num;
    int ville_courante_idx;

        if(num_courant >= 0 && num_courant < taille_map && num_vers_indice[num_courant] >= 0){
             ville_courante_idx =num_vers_indice[num_courant];
        }

        else{(ville_courante_idx =idx - 1);}

    for (int step = 1; step < nb_frag; step++) {
        int meilleur_f = -1;
        int meilleure_d = INT_MAX;
        bool inverser = false;

        for (int f = 0; f < nb_frag; f++) {

            if (!frag_utilise[f]) {

                int d1 = recuperer_distance(matrice, ville_courante_idx, frag_debut_idx[f]);
                if (meilleur_f == -1 || d1 < meilleure_d) {
                    meilleure_d = d1;
                    meilleur_f = f;
                    inverser = false;
                }

                int d2 = recuperer_distance(matrice, ville_courante_idx, frag_fin_idx[f]);
                if (d2 < meilleure_d) {
                    meilleure_d = d2;
                    meilleur_f = f;
                    inverser = true;
                }
            }
        }

        if (meilleur_f < 0) exit (1);
        frag_utilise[meilleur_f] = true;

        int start = frag_start[meilleur_f];
        int len   = frag_len[meilleur_f];

        if (!inverser) {
            for (int t = 0; t < len; t++) {
                nouveau_parcours[idx++] = parent1->parcours[start + t];
            }
        } else {
            for (int t = len - 1; t >= 0; t--) {
                nouveau_parcours[idx++] = parent1->parcours[start + t];
            }
        }

        num_courant = nouveau_parcours[idx - 1].num;

            if (num_courant >= 0 && num_courant < taille_map && num_vers_indice[num_courant] >= 0)
            {
                   ville_courante_idx = num_vers_indice[num_courant];
            }else{ ville_courante_idx =idx - 1;}

    }

    if (idx != n) {
        for (int i = 0; i < n; i++) nouveau_parcours[i] = parent1->parcours[i];
    }

    tournee_t *fille = malloc(sizeof(tournee_t));
    fille->parcours = nouveau_parcours;

    tournee_t *fille_opt = deux_opt(fille, inst);

    free(fille->parcours);
    free(fille);
    free(frag_id);
    free(frag_start);
    free(frag_len);
    free(num_vers_indice);
    free(frag_debut_idx);
    free(frag_fin_idx);
    free(frag_utilise);
    return fille_opt;
}
