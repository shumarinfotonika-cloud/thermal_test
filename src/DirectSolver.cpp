#include "DirectSolver.h"
#include "Grid.h"
#include "BoundaryConditions.h"
#include "ThermalConductivity.h"
#include "SourceFunction.h"
#include "Solver.h"
#include "VTKSaver.h"
#include "TempSaver.h"
#include <iostream>

DirectSolver::DirectSolver(const ConfigReader& config)
    : config(config), coeffs(config.getString("material", "thermal_conductivity")) {}

DirectSolver::DirectSolver(const ConfigReader& config, const ThermalConductivity& new_coeffs)
    : config(config), coeffs(new_coeffs) {}

Grid DirectSolver::solve() {

    bool verbose = config.getString("", "verbose") == "true";
    bool direct = config.getString("", "task_type") == "direct";
    std::string id = config.getString("", "id");
    double dt = config.getDouble("", "dt");
    int steps = config.getInt("", "steps");

    auto [nx, ny] = config.getPairInt("grid", "size", ',');
    auto [spacing_x, spacing_y] = config.getPairDouble("grid", "spacing", ',');
    double initial_value = config.getDouble("grid", "initial_value");

    Grid grid(nx, ny, spacing_x, spacing_y);
    grid.initialize(initial_value);

    auto boundaries = config.getSubsections("boundary_conditions.boundary");
    std::vector<BoundaryCondition> boundary_conditions;

    for (const auto& boundary : boundaries) {
        int axis = std::stoi(boundary.at("axis"));
        int side = std::stoi(boundary.at("side"));
        std::string value = boundary.at("value");

        boundary_conditions.emplace_back(axis, side, value);
    }

    for (auto& bc : boundary_conditions) {
        bc.apply(grid, 0.0);
    }

    std::string source_formula = config.getString("source", "source_formula");
    SourceFunction source(source_formula);

    Solver solver(grid, dt, coeffs, source, boundary_conditions);

    auto savers = config.getSubsections("savers.saver");

    bool vtk_saver_active = false;
    bool txt_saver_active = false;
    std::string vtk_path;
    std::string txt_path;
    int save_vtk_frequency = 1;
    int save_txt_frequency = 1;

    VTKSaver vtk_saver(grid);
    TempSaver txt_saver(grid);

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

    if (verbose && direct) {
        std::cout << "\nStarting simulation with ID: " << id << "\n\n";

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

    for (int t = 0; t < steps; ++t) {
        if (vtk_saver_active && (t % save_vtk_frequency == 0) && direct) {
            std::string new_path = ConfigReader::replacePlaceholder(vtk_path, "%g", id);
            new_path = ConfigReader::replacePlaceholder(new_path, "%s", std::to_string(t));

            vtk_saver.saveTemperature("../" + new_path, "temperature");

            if (verbose && direct) {
                std::cout << "Step " << t << ": VTK saved to " << new_path << "\n";
            }
        }

        if (txt_saver_active && (t % save_txt_frequency == 0) && direct) {
            std::string new_path = ConfigReader::replacePlaceholder(txt_path, "%g", id);

            txt_saver.save_step(t, "../" + new_path);

            if (verbose && direct) {
                std::cout << "Step " << t << ": Temp saved to " << new_path << "\n";
            }
        }
        if (t == steps - 1) break;
        solver.solve_one_step(t);
    }
    if (verbose && direct) {
        std::cout << "\nSimulation with ID " << id << " was completed successfully\n\n";
    }
    return grid;
}