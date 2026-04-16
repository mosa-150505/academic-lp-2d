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
    double a, b;
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

//  Functions
static bool is_constraint_satisfied (const Constraint* c, double x, double y) {
    return (c->a * x + c->b * y) <= (c->d + EPSILON); //  EPSILON sert à la tolérance des valeurs, car les double sont souvent imprécis
}

static bool is_feasible (const LinearProgram* lp, double x, double y) {
    int i;

    for (i = 0; i < lp->num_constraints; i++)
        if (!is_constraint_satisfied(&lp->constraints[i], x, y))
            return false;

    if (x < -EPSILON || y < -EPSILON)
        return false;

    return true;
}
