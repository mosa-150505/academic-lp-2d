#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define EPSILON 1e-9
#define C_MAX 30

// Data types
typedef struct {
    double a, b, d;
} Droite;

typedef struct {
   double x, y;
} Point;

//  Input functions
int saisir_type () {
    int type;

    do {
        printf("\n1. Maximisation\n2. Minimisation\n");

        printf("\nVotre choix : ");
        scanf("%d", &type);
    } while (type != 1 && type != 2);

    return type;
}

void saisir_fonction_objectif (double *c1, double *c2) {
    printf("\nEntrer les coeff de z : ");
    scanf("%lf %lf", c1, c2);
}

int saisir_contraintes (Droite droites[]) {
    int nb_contrainte;

    printf("\nEntrer les contraintes sous forme 'a b d' (0 0 0 pour terminer) :\n");

    nb_contrainte = 0;

    while (nb_contrainte < C_MAX) {
        printf("-- Contrainte %d : ", nb_contrainte + 1);
        scanf("%lf %lf %lf", &droites[nb_contrainte].a, &droites[nb_contrainte].b, &droites[nb_contrainte].d);

        if (droites[nb_contrainte].a == 0 && droites[nb_contrainte].b == 0 && droites[nb_contrainte].d == 0)
            break;

        nb_contrainte ++;
    }

    droites[nb_contrainte ++] = (Droite){1.0, 0.0, 0.0};
    droites[nb_contrainte ++] = (Droite){0.0, 1.0, 0.0};

    return nb_contrainte;
}

int saisir_donnees (double *signe_opt, double *c1, double *c2, Droite droites[]) {
    int nb_contrainte;

   *signe_opt = (saisir_type() == 1) ? 1.0 : -1.0;

   saisir_fonction_objectif(c1, c2);

   nb_contrainte = saisir_contraintes(droites);

   return nb_contrainte;
}

bool est_valide (int nb_contrainte, Droite droites[], Point p) {
    int i;

    if (p.x < -EPSILON || p.y < -EPSILON)
        return false;

    for (i = 0; i < nb_contrainte; i++)
        if (droites[i].a * p.x + droites[i].b * p.y > droites[i].d+ EPSILON)
            return false;

    return true;
}

int trouver_sommet_opt (int nb_droite, Droite droites[], double c1, double c2, double signe, double *z_opt, Point sommets_opt[]) {
    int nb_sommet_opt, i, j, k;
    double det;
    double dx, dy;
    double val_z;
    double distance;
    Point intersection;
    bool doublon;

    nb_sommet_opt = 0;

    *z_opt = -DBL_MAX;

    for (i = 0; i < nb_droite; i++) {
        for (j = i + 1; j < nb_droite; j++) {
            det = droites[i].a * droites[j].b - droites[i].b * droites[j].a;

            if (fabs(det) < EPSILON) 
                continue;

            dx = droites[i].d * droites[j].b - droites[j].d * droites[i].b;
            dy = droites[i].a * droites[j].d - droites[j].a * droites[i].d;

            intersection = (Point){(dx / det), (dy / det)};

            if (est_valide(nb_droite - 2, droites, intersection)) {
                val_z = (c1 * intersection.x + c2 * intersection.y) * signe;

                if (val_z > *z_opt + EPSILON) {
                    *z_opt = val_z;
                    nb_sommet_opt = 0;
                    sommets_opt[nb_sommet_opt ++] = intersection;
                }

                else if (fabs(val_z - *z_opt) < EPSILON) {
                    doublon = false;

                    for (k = 0; k < nb_sommet_opt; k++) {
                        distance = sqrt(pow(sommets_opt[k].x - intersection.x, 2) + pow(sommets_opt[k].y - intersection.y, 2));
                        if (distance < EPSILON)
                            doublon = true;
                    }

                    if (!doublon)
                        sommets_opt[nb_sommet_opt++] = intersection;
                }
            }
        }
    }

    return nb_sommet_opt;
}
