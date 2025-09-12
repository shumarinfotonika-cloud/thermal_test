#ifndef SINGLE_STEP_INVERSE_SOLVER_H
#define SINGLE_STEP_INVERSE_SOLVER_H

#include "Grid.h"
#include "ThermalConductivity.h"
#include "SourceFunction.h"
#include "BoundaryConditions.h"
#include "DirectSolver.h"
#include "ExactSolution.h"

class SingleStepInverseSolver {
private:
    const ConfigReader& config;
    SourceFunction source_function;
    std::vector<BoundaryCondition> boundary_conditions;
    double lambda;
    double learning_rate;

public:
    SingleStepInverseSolver(const ConfigReader& config, const SourceFunction& source_function,
                            const std::vector<BoundaryCondition>& boundary_conditions,
                            double lambda, double learning_rate);

    ThermalConductivity solve(ThermalConductivity& current_coeffs, double dt, int iter);
};

#endif // SINGLE_STEP_INVERSE_SOLVER_H
