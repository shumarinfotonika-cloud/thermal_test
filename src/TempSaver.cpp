#include <fstream>
#include <iostream>
#include "TempSaver.h"

TempSaver::TempSaver(const Grid& grid)
    : grid(grid) {}

void TempSaver::save_step(int time_step, const std::string &filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        return;
    }

    file << time_step << "\n";

    int size_x = grid.get_size_x();
    int size_y = grid.get_size_y();

    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            file << std::fixed << std::setprecision(12) << grid.get_value(i, j) << " ";
        }
        file << "\n";
    }

    file << "\n";
    file.close();
}
