#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define EPSILON 1e-9
#define MAX_CONSTRAINTS 30
#define MAX_INTERSECTIONS (MAX_CONSTRAINTS + 2) * (MAX_CONSTRAINTS + 1)/2 // C(n,2) : pour truover le nombre d'intersections possibles par couple de droites : C(n,2) où n = MAX_CONSTRAINTS + 2 i.e : les contraintes finales x1 >= 0 et x2 >= 0 en font partie

typedef enum {
    MAXIMIZATION,
    MINIMIZATION
} OptimizationType; //  définition d'un type spécifique pour le type d'optimisation

//  DS USED
typedef struct {
    double a, b, d; // a*x + b*y <= d
} Constraint;

typedef struct {
    double a, b, d; // a*x + b*y = d
} Line;

typedef struct {
    double x, y;
} Point;

typedef struct {
    double c1, c2;
    OptimizationType type;
    Constraint constraints[MAX_CONSTRAINTS];
    int num_constraints;
    Line lines[MAX_CONSTRAINTS + 2];
    int num_lines;
} LinearProgram;

typedef struct {
    Point p1, p2;
    double z_value;
    bool found, is_segment;
    int num_optimal_points;
} Solution;

//  Internal Functions
static bool is_constraint_satisfied (const Constraint *c, double x, double y) {
    return (c->a * x + c->b * y) <= (c->d + EPSILON); //  EPSILON sert à la tolérance des valeurs, car les double sont souvent imprécis
}

static bool is_feasible (const LinearProgram *lp, double x, double y) {
    int i;

    for (i = 0; i < lp->num_constraints; i++)
        if (!is_constraint_satisfied(&lp->constraints[i], x, y))
            return false;

    if (x < -EPSILON || y < -EPSILON)
        return false;

    return true;
}

static bool calculate_intersection (const Line *d1, const Line *d2, Point *out) {
    double det = d1->a * d2->b - d2->a * d1->b; // déterminant d'ordre 2

    if (fabs(det) < EPSILON)
        return false; // Droites confondues

    out->x = (d1->d * d2->b - d2->d * d1->b) / det;
    out->y = (d1->a  * d2->d - d2->a * d1->d) / det;

    return true;
}

static double calculate_z (double c1, double c2, double x, double y) {
    return c1*x + c2*y;
}

static bool are_z_equal (double z1, double z2) {
    return fabs(z1 - z2) < EPSILON;
}

static double distance (Point p1, Point p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

//  Global Functions
void init_linear_program (LinearProgram *lp, double c1, double c2, OptimizationType type) {
    lp->c1 = c1;
    lp->c2 = c2;
    lp->type = type;
    lp->num_constraints = 0;
    lp->num_lines = 0;
}

void add_constraint (LinearProgram *lp, double a, double b, double d) {
    if (lp->num_constraints >= MAX_CONSTRAINTS) {
        printf("\nNombre limite de contraintes atteint (max %d)\n", MAX_CONSTRAINTS);
        return;
    }

    lp->constraints[lp->num_constraints].a = a;
    lp->constraints[lp->num_constraints].b = b;
    lp->constraints[lp->num_constraints].d = d;

    lp->num_constraints++;
}

void build_lines (LinearProgram *lp) {
    int i;

    for (i = 0; i < lp->num_constraints; i++) {
        lp->lines[i].a = lp->constraints[i].a;
        lp->lines[i].b = lp->constraints[i].b;
        lp->lines[i].d = lp->constraints[i].d;
    }

    lp->num_lines = lp->num_constraints;

    // Axe des abscisses : x >= 0 : x = 0 (contrainte obli)
    lp->lines[lp->num_lines].a = 1.0;
    lp->lines[lp->num_lines].b = 0.0;
    lp->lines[lp->num_lines].d = 0.0;
    lp->num_lines++;

    // Axe des ordonnees : y >= 0 : y = 0 (contrainte obli)
    lp->lines[lp->num_lines].a = 0.0;
    lp->lines[lp->num_lines].b = 1.0;
    lp->lines[lp->num_lines].d = 0.0;
    lp->num_lines++;
}
