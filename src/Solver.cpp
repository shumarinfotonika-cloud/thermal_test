#include "Solver.h"
#include <iostream>

Solver::Solver(Grid& grid, double dt,
               ThermalConductivity& thermal_conductivity,
               SourceFunction& source_function,
               std::vector<BoundaryCondition>& boundary_conditions)
    : grid(grid), dt(dt),
      thermal_conductivity(thermal_conductivity),
      source_function(source_function),
      boundary_conditions(boundary_conditions) {}

void Solver::apply_boundary_conditions(double time) {
    for (auto& bc : boundary_conditions) {
        bc.apply(grid, time);
    }
}

void Solver::perform_x_step(double time) {
    int size_x = grid.get_size_x();
    int size_y = grid.get_size_y();
    double spacing_x = grid.get_spacing_x();

    std::vector<double> alpha(size_x, 0.0);
    std::vector<double> beta(size_x, 0.0);

    for (int j = 1; j < size_y - 1; ++j) {
        beta[0] = grid.get_value(0, j);
        for (int i = 1; i < size_x - 1; ++i) {
            double x = i * spacing_x;
            double y = j * grid.get_spacing_y();

            double a_prev = thermal_conductivity.evaluate(x - spacing_x / 2, y);
            double a_next = thermal_conductivity.evaluate(x + spacing_x / 2, y);

            double A = -dt * a_prev / (spacing_x * spacing_x);
            double B = 1 + dt * (a_prev + a_next) / (spacing_x * spacing_x);
            double C = -dt * a_next / (spacing_x * spacing_x);

            double source = source_function.evaluate(x, y, time);

            double F = grid.get_value(i, j) + dt * source;

            alpha[i] = -C / (B + A * alpha[i - 1]);
            beta[i] = (F - A * beta[i - 1]) / (B + A * alpha[i - 1]);
        }

        for (int i = size_x - 2; i >= 1; --i) {
            double u_next = alpha[i] * grid.get_value(i + 1, j) + beta[i];
            grid.set_value(i, j, u_next);
        }
    }
}

void Solver::perform_y_step(double time) {
    int size_x = grid.get_size_x();
    int size_y = grid.get_size_y();
    double spacing_y = grid.get_spacing_y();

    std::vector<double> alpha(size_y, 0.0);
    std::vector<double> beta(size_y, 0.0);

    for (int i = 1; i < size_x - 1; ++i) {
        beta[0] = grid.get_value(i, 0);
        for (int j = 1; j < size_y - 1; ++j) {
            double x = i * grid.get_spacing_x();
            double y = j * spacing_y;

            double a_prev = thermal_conductivity.evaluate(x, y - spacing_y / 2);
            double a_next = thermal_conductivity.evaluate(x, y + spacing_y / 2);

            double A = -dt * a_prev / (spacing_y * spacing_y);
            double B = 1 + dt * (a_prev + a_next) / (spacing_y * spacing_y);
            double C = -dt * a_next / (spacing_y * spacing_y);

            double source = source_function.evaluate(x, y, time);

            double F = grid.get_value(i, j) + dt * source;

            alpha[j] = -C / (B + A * alpha[j - 1]);
            beta[j] = (F - A * beta[j - 1]) / (B + A * alpha[j - 1]);
        }

        for (int j = size_y - 2; j >= 1; --j) {
            double u_next = alpha[j] * grid.get_value(i, j + 1) + beta[j];
            grid.set_value(i, j, u_next);
        }
    }
}


void Solver::solve_one_step(double time) {
    apply_boundary_conditions(time);
    perform_x_step(time);
    perform_y_step(time);
}
