#include "VTKSaver.h"
#include <fstream>
#include <iostream>

VTKSaver::VTKSaver(const Grid &grid) : grid(grid) {}

void VTKSaver::saveTemperature(const std::string &filename, const std::string &solve_name) {
    int size_x = grid.get_size_x();
    int size_y = grid.get_size_y();
    double spacing_x = grid.get_spacing_x();
    double spacing_y = grid.get_spacing_y();

    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file << "# vtk DataFile Version 3.0\n";
    file << "Temperature distribution on a 2D grid\n";
    file << "ASCII\n\n";
    file << "DATASET STRUCTURED_POINTS\n";
    file << "DIMENSIONS " << size_x << " " << size_y << " 1\n";
    file << "ORIGIN 0.0 0.0 0.0\n";
    file << "SPACING " << spacing_x << " " << spacing_y << " 1.0\n\n";
    file << "POINT_DATA " << (size_x * size_y) << "\n";
    file << "SCALARS " << solve_name << " double\n";
    file << "LOOKUP_TABLE default\n";

    for (int j = 0; j < size_y; ++j) {
        for (int i = 0; i < size_x; ++i) {
            file << grid.get_value(i, j) << " ";
        }
        file << "\n";
    }

    file.close();
}
