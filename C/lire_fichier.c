#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lecture_donnees.h"

instance_t *lire_tsplib(const char *chemin)
{
    if (!chemin) return NULL;

    const char *ext = strrchr(chemin, '.');
    if (!ext || strcmp(ext, ".tsp") != 0) {
        fprintf(stderr, "Erreur : le fichier '%s' n'est pas un .tsp\n", chemin);
        return NULL;
    }

    FILE *f = fopen(chemin, "r");
    if (!f) {
        fprintf(stderr, "Erreur d'ouverture de '%s': %s\n", chemin, strerror(errno));
        return NULL;
    }

    instance_t *inst = malloc(sizeof *inst);
    if (!inst) { fclose(f); return NULL; }

    inst->nom[0] = '\0';
    inst->type_distance[0] = '\0';
    inst->dimension = 0;
    inst->noeuds = NULL;

    char line[512];
    int have_node_section = 0;

    while (fgets(line, sizeof line, f)) {
        if      (strncmp(line, "NAME", 4) == 0)
            sscanf(line, "NAME : %63s", inst->nom);
        else if (strncmp(line, "DIMENSION", 9) == 0)
            sscanf(line, "DIMENSION : %d", &inst->dimension);
        else if (strncmp(line, "EDGE_WEIGHT_TYPE", 16) == 0)
            sscanf(line, "EDGE_WEIGHT_TYPE : %15s", inst->type_distance);
        else if (strncmp(line, "NODE_COORD_SECTION", 18) == 0) {
            have_node_section = 1;
            break;
        }
    }

    if (!have_node_section || inst->dimension <= 0) {
        fprintf(stderr, "Erreur d'en-tête dans %s (DIMENSION/NODE_COORD_SECTION)\n", chemin);
        fclose(f);
        free(inst);
        return NULL;
    }
    if (strcmp(inst->type_distance, "EUC_2D") != 0 &&
        strcmp(inst->type_distance, "GEO")    != 0 &&
        strcmp(inst->type_distance, "ATT")    != 0) {
        fprintf(stderr, "EDGE_WEIGHT_TYPE non supporté: %s (EUC_2D/GEO/ATT)\n",
                inst->type_distance);
        fclose(f);
        free(inst);
        return NULL;
    }

    inst->noeuds = malloc((size_t)inst->dimension * sizeof(noeud_t));
    if (!inst->noeuds) {
        fprintf(stderr, "Erreur d'allocation pour %d noeuds\n", inst->dimension);
        fclose(f);
        free(inst);
        return NULL;
    }

    int count = 0, id; float x, y;
    while (count < inst->dimension && fgets(line, sizeof line, f)) {
        if (strncmp(line, "EOF", 3) == 0) break;
        if (sscanf(line, "%d %f %f", &id, &x, &y) == 3) {
            inst->noeuds[count].num = id;
            inst->noeuds[count].x   = x;
            inst->noeuds[count].y   = y;
            count++;
        }
    }

    fclose(f);

    if (count != inst->dimension) {
        fprintf(stderr, "Erreur : %d coordonnées lues au lieu de %d dans %s\n",
                count, inst->dimension, chemin);
        free(inst->noeuds);
        free(inst);
        return NULL;
    }

    printf("Lecture réussie : %s (%s, %d villes)\n",
           inst->nom, inst->type_distance, inst->dimension);

    return inst;
}
