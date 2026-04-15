# academic-lp-2d
A lightweight, interactive C implementation for solving two-dimensional linear programming (LP) problems using the **vertex enumeration method**. Designed primarily for academic and educational purposes in Operations Research and Optimization courses.

## Overview
This solver handles both **maximization** and **minimization** problems with linear constraints in the form `a·x + b·y ≤ d`, alongside non-negativity constraints (`x ≥ 0, y ≥ 0`). It interactively guides the user through problem definition, computes all constraint intersections, filters feasible vertices, and identifies the optimal solution(s), including cases where the optimum lies on an entire edge (segment).

## Vertex Enumeration Method
The algo proceeds in 3 deterministc steps :
1. **Enumeration**: Computes all pairwise intersections `I_k = D_i ∩ D_j` between constraint lines and boundary axes (`x=0`, `y=0`).
2. **Feasibility Filtering**: Retains only intersection points that satisfy all inequality constraints and non-negativity conditions (`I_k ∈ R`).
3. **Optimization**: Evaluates the objective function `z = c₁x + c₂y` on the feasible vertices and selects the vertex (or segment) that maximizes/minimizes `z`.

## 
