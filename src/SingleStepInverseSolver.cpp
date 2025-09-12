#include "SingleStepInverseSolver.h"
#include <cmath>

SingleStepInverseSolver::SingleStepInverseSolver(
    const ConfigReader& config, const SourceFunction& source_function,
    const std::vector<BoundaryCondition>& boundary_conditions,
    double lambda, double learning_rate)
    : config(config), source_function(source_function),
      boundary_conditions(boundary_conditions),
      lambda(lambda), learning_rate(learning_rate) {}

ThermalConductivity SingleStepInverseSolver::solve(ThermalConductivity& current_coeffs, double dt, int iter) {
    bool verbose = config.getString("", "verbose") == "true";
    auto [nx, ny] = config.getPairInt("grid", "size", ',');
    auto [spacing_x, spacing_y] = config.getPairDouble("grid", "spacing", ',');
    double initial_value = config.getDouble("grid", "initial_value");

    ThermalConductivity perturbed_coeffs = current_coeffs;
    Grid perturbed_coeffs_g(nx, ny, spacing_x, spacing_y);
    double delta = 1e-2;
    double max_a = 0.0;
    double learning_rate_new = learning_rate / (1.0 + iter);

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            double old_value = current_coeffs.evaluate(i, j, spacing_x, spacing_y, 0.0, 0.0);
            perturbed_coeffs_g.set_value(i, j, old_value + delta);
        }
    }

    perturbed_coeffs.updateCoefficients(perturbed_coeffs_g);

    DirectSolver direct_solver(config, current_coeffs);
    DirectSolver perturbed_solver(config, perturbed_coeffs);

    Grid grid = direct_solver.solve();
    Grid perturbed_grid = perturbed_solver.solve();

    std::string solution_path = config.getString("solution", "path");
    ExactSolution exact_solution(solution_path, nx, ny, spacing_x, spacing_y);

    ThermalConductivity updated_coeffs = current_coeffs;
    Grid updated_coeffs_g(nx, ny, spacing_x, spacing_y);

    double min_a = initial_value;

    for (int i = 0; i < nx; ++i) {
        for (int j = 0; j < ny; ++j) {
            double computed = grid.get_value(i, j);
            double exact = exact_solution.get_value(i, j, exact_solution.get_steps() - 1);
            double computed_p = perturbed_grid.get_value(i, j);
            double R_a = (computed - exact) * (computed - exact);
            double R_a_plus_delta = (computed_p - exact) * (computed_p - exact);
            double grad_R = (R_a_plus_delta - R_a) / delta;
            // lambda = lambda / (1.0 + grad_R * grad_R);

            double old_value = current_coeffs.evaluate(i, j, spacing_x, spacing_y, 0.0, 0.0);
            double grad_reg = 0.0;

            if (i > 0) grad_reg += 2.0 * (old_value - current_coeffs.evaluate(i - 1, j, spacing_x, spacing_y, 0.0, 0.0)) / (spacing_x * spacing_x);
            if (i < nx - 1) grad_reg += 2.0 * (old_value - current_coeffs.evaluate(i + 1, j, spacing_x, spacing_y, 0.0, 0.0)) / (spacing_x * spacing_x);
            if (j > 0) grad_reg += 2.0 * (old_value - current_coeffs.evaluate(i, j - 1, spacing_x, spacing_y, 0.0, 0.0)) / (spacing_y * spacing_y);
            if (j < ny - 1) grad_reg += 2.0 * (old_value - current_coeffs.evaluate(i, j + 1, spacing_x, spacing_y, 0.0, 0.0)) / (spacing_y * spacing_y);

            double grad_J = grad_R + lambda * grad_reg;
            double new_value = old_value - learning_rate_new * grad_J;
            updated_coeffs_g.set_value(i, j, new_value);
            if (new_value < min_a) {
                min_a = new_value;
            }
        }
    }
    if (verbose) {
        std::cout << "\tLambda = " << lambda << "\n\tAlpha = " << learning_rate_new << "\n\tDelta = " << delta << "\n\tMin Coeff = " << min_a << "\n";
    }

    updated_coeffs.updateCoefficients(updated_coeffs_g);

    return updated_coeffs;
}

