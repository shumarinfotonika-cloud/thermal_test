#ifndef SOLVER_H
#define SOLVER_H

#include "Grid.h"
#include "BoundaryConditions.h"
#include "ThermalConductivity.h"
#include "SourceFunction.h"

class Solver {
private:
    Grid& grid;
    double dt;
    ThermalConductivity& thermal_conductivity;
    SourceFunction& source_function;
    std::vector<BoundaryCondition>& boundary_conditions;

    void apply_boundary_conditions(double time);
    void perform_x_step(double time);
    void perform_y_step(double time);

public:
    Solver(Grid& grid, double dt,
           ThermalConductivity& thermal_conductivity,
           SourceFunction& source_function,
           std::vector<BoundaryCondition>& boundary_conditions);

    void solve_one_step(double time);
};

#endif // SOLVER_H
