#include <iostream>
#include "ConfigReader.h"
#include "Grid.h"
#include "BoundaryConditions.h"
#include "ParserConst.h"
#include "VTKSaver.h"
#include "TempSaver.h"
#include "ThermalConductivity.h"
#include "SourceFunction.h"
#include "Solver.h"

#include "muParser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>\n";
        return 1;
    }

    try {
        ConfigReader config(argv[1]);

        bool verbose = config.getString("", "verbose") == "true";
        std::string id = config.getString("", "id");
        double dt = config.getDouble("", "dt");
        int steps = config.getInt("", "steps");

        if (verbose) {
            std::cout << "Starting simulation with ID: " << id << "\n";
            std::cout << "Time step: " << dt << ", Number of steps: " << steps << "\n";
        }

        auto [nx, ny] = config.getPairInt("grid", "size", ',');
        auto [origin_x, origin_y] = config.getPairDouble("grid", "origin", ',');
        auto [spacing_x, spacing_y] = config.getPairDouble("grid", "spacing", ',');

        Grid grid(nx, ny, spacing_x, spacing_y);
        grid.initialize(20.0);

        if (verbose) {
            std::cout << "Grid size: " << nx << "x" << ny << "\n";
            std::cout << "Origin: (" << origin_x << ", " << origin_y << ")\n";
            std::cout << "Spacing: (" << spacing_x << ", " << spacing_y << ")\n";
        }

        auto boundaries = config.getSubsections("boundary_conditions.boundary");
        std::vector<BoundaryCondition> boundary_conditions;

        for (const auto& boundary : boundaries) {
            int axis = std::stoi(boundary.at("axis"));
            int side = std::stoi(boundary.at("side"));
            std::string value = boundary.at("value");

            boundary_conditions.emplace_back(axis, side, value);

            if (verbose) {
                std::cout << "Axis: " << axis << ", Side: " << side << ", Value: " << value << "\n";
            }
        }

        for (auto& bc : boundary_conditions) {
            bc.apply(grid, 0.0);
        }

        std::string thermal_conductivity = config.getString("material", "thermal_conductivity");
        std::string source_formula = config.getString("source", "source_formula");
        ThermalConductivity coeffs(thermal_conductivity);
        SourceFunction source(source_formula);

        Solver solver(grid, dt, coeffs, source, boundary_conditions);

        auto savers = config.getSubsections("savers.saver");
        if (verbose) {
            std::cout << "Savers:\n";
            for (const auto& saver : savers) {
                std::string name = saver.at("name");
                std::string path = saver.at("path");
                int save_frequency = std::stoi(saver.at("save"));

                std::cout << "Name: " << name << "\n";
                std::cout << "Path: " << path << "\n";
                std::cout << "Save frequency: " << save_frequency << "\n";
            }
        }

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

        if (vtk_saver_active && verbose) {
            std::cout << "VTKSaver is active. Path: " << vtk_path << ", Save frequency: " << save_vtk_frequency << "\n";
        }

        if (txt_saver_active && verbose) {
            std::cout << "TempSaver is active. Path: " << txt_path << ", Save frequency: " << save_txt_frequency << "\n";
        }

        VTKSaver vtk_saver(grid);
        TempSaver txt_saver(grid);

        for (int t = 0; t < steps; ++t) {
            if (vtk_saver_active && (t % save_vtk_frequency == 0)) {
                std::string new_path = ConfigReader::replacePlaceholder(vtk_path, "%g", id);
                new_path = ConfigReader::replacePlaceholder(new_path, "%s", std::to_string(t));

                vtk_saver.saveTemperature(new_path);

                if (verbose) {
                    std::cout << "Step " << t << ": VTK saved to " << new_path << "\n";
                }
            }

            if (txt_saver_active && (t % save_txt_frequency == 0)) {
                std::string new_path = ConfigReader::replacePlaceholder(txt_path, "%g", id);

                txt_saver.save_step(t, new_path);

                if (verbose) {
                    std::cout << "Step " << t << ": Temp saved to " << new_path << "\n";
                }
            }
            solver.solve_one_step(t);
        }


        // if (verbose) {
        //     std::cout << "\nGrid after applying boundary conditions:\n";
        //     grid.print();
        // }

        // solver

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
