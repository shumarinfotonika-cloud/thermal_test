#include "InverseSolver.h"
#include "DirectSolver.h"
#include "Grid.h"
#include "BoundaryConditions.h"
#include "SourceFunction.h"
#include "Solver.h"
#include "SingleStepInverseSolver.h"
#include "VTKSaver.h"
#include "TempSaver.h"
#include <iostream>

InverseSolver::InverseSolver(const ConfigReader& config)
    : config(config) {}

void InverseSolver::solve() {

    bool verbose = config.getString("", "verbose") == "true";
    std::string id = config.getString("", "id");
    double dt = config.getDouble("", "dt");
    int steps = config.getInt("", "steps");

    std::string solution_path = config.getString("solution", "path");

    int iterations = config.getInt("optimization", "iterations");
    double error_rate = config.getDouble("optimization", "error_rate");
    double learning_rate = config.getDouble("optimization", "learning_rate");
    double lambda = config.getDouble("optimization", "lambda");
    double initial_coef = config.getDouble("optimization", "initial_coef");

    auto [nx, ny] = config.getPairInt("grid", "size", ',');
    auto [spacing_x, spacing_y] = config.getPairDouble("grid", "spacing", ',');
    double initial_value = config.getDouble("grid", "initial_value");

    auto boundaries = config.getSubsections("boundary_conditions.boundary");
    std::vector<BoundaryCondition> boundary_conditions;

    for (const auto& boundary : boundaries) {
        int axis = std::stoi(boundary.at("axis"));
        int side = std::stoi(boundary.at("side"));
        std::string value = boundary.at("value");

        boundary_conditions.emplace_back(axis, side, value);
    }

    std::string source_formula = config.getString("source", "source_formula");
    SourceFunction source(source_formula);

    auto savers = config.getSubsections("savers.saver");

    bool vtk_saver_active = false;
    bool txt_saver_active = false;
    std::string vtk_path;
    std::string txt_path;
    int save_vtk_frequency = 1;
    int save_txt_frequency = 1;

    for (const auto& saver : savers) {
        if (saver.at("name") == "VTKSaver") {
            vtk_saver_active = true;
            vtk_path = saver.at("path");
            save_vtk_frequency = std::stoi(saver.at("save"));
        }
        if (saver.at("name") == "TempSaver") {
            txt_saver_active = true;
            txt_path = saver.at("path");
            save_txt_frequency = std::stoi(saver.at("save"));
        }
    }

    if (verbose) {
        std::cout << "\nStarting inverse task with ID: " << id << "\n\n";

        std::cout << "\t[Solution]\n";
        std::cout << "\t  Path to solution file: " << solution_path << "\n";

        std::cout << "\t[Optimization]\n";
        std::cout << "\t  Iterations: " << iterations << "\n";
        std::cout << "\t  Error rate: " << error_rate << "\n";
        std::cout << "\t  Learning rate: " << learning_rate << "\n";
        std::cout << "\t  Initial coefficient: " << initial_coef << "\n";


        std::cout << "\t[Time]\n";
        std::cout << "\t  Number of steps: " << steps << "\n";
        std::cout << "\t  Step: " << dt << "\n";

        std::cout << "\t[Grid]\n";
        std::cout << "\t  Size: " << nx << "x" << ny << "\n";
        std::cout << "\t  Spacing: (" << spacing_x << ", " << spacing_y << ")\n";
        std::cout << "\t  Initial value: " << initial_value << "\n";

        std::cout << "\t[Boundary conditions]\n";
        for (const auto& boundary : boundaries) {
            int axis = std::stoi(boundary.at("axis"));
            int side = std::stoi(boundary.at("side"));
            std::string value = boundary.at("value");

            std::cout << "\t  Axis: " << axis << ", Side: " << side << ", Value: " << value << "\n";
        }

        std::cout << "\t[Savers]\n";
        for (const auto& saver : savers) {
            std::string name = saver.at("name");
            std::string path = saver.at("path");
            int save_frequency = std::stoi(saver.at("save"));


            if (saver.at("name") == "VTKSaver") {
                std::cout << "\t  VTKSaver is active. Path: " << vtk_path << ", Save frequency: " << save_vtk_frequency << "\n";
            }
            if (saver.at("name") == "TempSaver") {
                std::cout << "\t  TempSaver is active. Path: " << txt_path << ", Save frequency: " << save_txt_frequency << "\n";
            }
        }
        std::cout << "\n";
    }

    ThermalConductivity thermal_conductivity(nx, ny, spacing_x, spacing_y, initial_coef);
    Grid thermal_conductivity_g = thermal_conductivity.get_grid();
    VTKSaver vtk_saver(thermal_conductivity_g);
    TempSaver txt_saver(thermal_conductivity_g);

    SingleStepInverseSolver step_solver(config, source, boundary_conditions, lambda, learning_rate);

    for (int iter = 0; iter < iterations; ++iter) {
        if (vtk_saver_active && iter % save_vtk_frequency == 0) {
            std::string new_path = ConfigReader::replacePlaceholder(vtk_path, "%g", id);
            new_path = ConfigReader::replacePlaceholder(new_path, "%s", std::to_string(iter));

            vtk_saver.saveTemperature("../" + new_path, "temperature");

            if (verbose) {
                std::cout << "Iteration " << iter + 1 << ": VTK saved to " << new_path << "\n";
            }
        }
        if (txt_saver_active && iter % save_txt_frequency == 0) {
            if (txt_saver_active && (iter % save_txt_frequency == 0)) {
            std::string new_path = ConfigReader::replacePlaceholder(txt_path, "%g", id);

            txt_saver.save_step(iter, "../" + new_path);

            if (verbose) {
                std::cout << "Iteration " << iter + 1 << ": Temp saved to " << new_path << "\n";
            }
        }
        }
        if (verbose) {
            std::cout << "Iteration " << iter + 1 << " of " << iterations << "...\n";
        }

        ThermalConductivity new_thermal_conductivity = step_solver.solve(thermal_conductivity, dt, iter);

        double error = 0.0;
        for (int i = 0; i < nx; ++i) {
            for (int j = 0; j < ny; ++j) {
                double old_coeff = thermal_conductivity.evaluate(i, j, spacing_x, spacing_y, 0.0, 0.0);
                double new_coeff = new_thermal_conductivity.evaluate(i, j, spacing_x, spacing_y, 0.0, 0.0);
                error += (new_coeff - old_coeff) * (new_coeff - old_coeff);
            }
        }
        error /= (nx * ny);
        error = std::sqrt(error);
        thermal_conductivity_g = new_thermal_conductivity.get_grid();

        if (verbose) {
            std::cout << "\tError: " << error << "\n";
        }

        if (error < error_rate) {
            if (verbose) {
                std::cout << "Error is below the threshold. Stopping optimization.\n";
            }
            break;
        }

        thermal_conductivity = new_thermal_conductivity;
    }

    if (verbose) {
        std::cout << "\nInverse task with ID " << id << " was completed successfully\n\n";
    } 
}
