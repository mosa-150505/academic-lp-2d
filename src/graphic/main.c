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

bool est_infini (int nb_contrainte, Droite droites[], double c1, double c2, double signe, double z_opt, Point sommet_opt) {
    int i;
    double intervalle, new_z;
    Point new_pt;

    intervalle = 1e6;

    double directions[3][2] = {
        {1.0, 0.0},
        {0.0, 1.0},
        {c1 * signe, c2 * signe}
    };

    for (i = 0; i < 3; i++) {
        new_pt = (Point){
            sommet_opt.x + directions[i][0] * intervalle,
            sommet_opt.y + directions[i][1] * intervalle,
        };

        if (est_valide(nb_contrainte, droites, new_pt)) {
            new_z = (c1 * new_pt.x + c2 * new_pt.y) * signe;

            if (new_z > z_opt + EPSILON)
                return true;
        }
    }

    return false;
}

void afficher_resultats (int nb_sommet_opt, Point sommets_opt[], double z_opt, double signe) {
    printf("\n-- RESULTAT --\n");

    if (nb_sommet_opt == 0) {
        printf("\n*Pas de solution\n");
        return;
    }

    if (nb_sommet_opt > 1) {
        printf("\n*%s atteint entre (%.2f, %.2f) et (%.2f, %.2f)\n",
                signe == 1.0 ? "Max" : "Min", 
                sommets_opt[0].x, sommets_opt[0].y,
                sommets_opt[nb_sommet_opt - 1].x, sommets_opt[nb_sommet_opt - 1].y
        );
    }
    
    else {
        printf("\n*%s atteint en (%.4f, %.4f)\n", signe == 1.0 ? "Max" : "Min", sommets_opt[0].x, sommets_opt[0].y);
    }

    printf("\n\tz%s = %.4f\n", signe == 1.0 ? "max" : "min", z_opt);
}

int main () {
    Droite droites[105];
    Point sommets_opt[100];
    int nb_droite, nb_contrainte, nb_sommet_opt;
    double c1, c2, signe, z_opt;

    nb_droite = saisir_donnees(&signe, &c1, &c2, droites);
    
    nb_sommet_opt = trouver_sommet_opt(nb_droite, droites, c1, c2, signe, &z_opt, sommets_opt);

    nb_contrainte = nb_droite - 2;

    if (nb_sommet_opt > 0 && est_infini(nb_contrainte, droites, c1, c2, signe, z_opt, sommets_opt[0]))
        printf("\n*Ensemble des solutions possibles non borne : z%s = inf\n", signe == 1.0 ? "max" : "min");

    else 
        afficher_resultats(nb_sommet_opt, sommets_opt, z_opt, signe);

    return 0;
}
