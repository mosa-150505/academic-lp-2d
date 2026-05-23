#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define EPSILON 1e-9

typedef struct {
    double a, b, d;
} Droite;

typedef struct {
   double x, y;
} Point;

bool est_valide (int nb_contrainte, Droite droites[], Point p) {
    int i;

    if (p.x < -EPSILON || p.y < -EPSILON)
        return false;

    for (i = 0; i < nb_contrainte; i++) {
        if (droites[i].a * p.x + droites[i].b * p.y > droites[i].d + EPSILON)
            return false;
    }
    return true;
}
