#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <float.h>

#define EPSILON 1e-9
#define MAX_CONSTRAINTS 30
#define MAX_INTERSECTIONS (MAX_CONSTRAINTS + 2) * (MAX_CONSTRAINTS + 1)/2

typedef enum {
    MAXIMIZATION,
    MINIMIZATION
} optimization_type;
