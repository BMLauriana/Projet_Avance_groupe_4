#include <stdio.h>
#include "lecture_donnees.c"

// fonction de distance euclidienne p.6 du doc tsp95
// pour type EUC_2D ou EUC_3D mais on utilise que EUC_2D
float distance_euclidienne(noeud_t point1, noeud_t point2) {
    float xd = point1.x - point2.x;
    float yd = point1.y - point2.y;
    float distance = sqrt(xd*xd + yd*yd);
    return distance;
}