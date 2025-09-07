#include <iostream>
#include "ConfigReader.h"

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

        if (verbose) {
            std::cout << "Grid size: " << nx << "x" << ny << "\n";
            std::cout << "Origin: (" << origin_x << ", " << origin_y << ")\n";
            std::cout << "Spacing: (" << spacing_x << ", " << spacing_y << ")\n";
        }

        auto boundaries = config.getSubsections("boundary_conditions.boundary");
        if (verbose) {
            std::cout << "Boundary conditions:\n";
            for (const auto& boundary : boundaries) {
                int axis = std::stoi(boundary.at("axis"));
                int side = std::stoi(boundary.at("side"));
                double value = std::stod(boundary.at("value"));
                std::cout << "Axis: " << axis << ", Side: " << side << ", Value: " << value << "\n";
            }
        }

        auto savers = config.getSubsections("savers.saver");
        if (verbose) {
            std::cout << "Savers:\n";
            for (const auto& saver : savers) {
                std::string name = saver.at("name");
                std::string path = saver.at("path");
                int save_frequency = std::stoi(saver.at("save"));

                path = ConfigReader::replacePlaceholder(path, "%g", id);

                std::cout << "Name: " << name << "\n";
                std::cout << "Path: " << path << "\n";
                std::cout << "Save frequency: " << save_frequency << "\n";
            }
        }


        // solver

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
