#include "ExactSolution.h"
#include <iostream>

ExactSolution::ExactSolution(const std::string& file_path, int size_x, int size_y, double spacing_x, double spacing_y)
    : size_x(size_x), size_y(size_y), spacing_x(spacing_x), spacing_y(spacing_y) {
    std::ifstream file("../" + file_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + file_path);
    }

    std::string line;
    Grid* current_grid = nullptr;
    int current_step = -1;
    int current_row = 0;

    while (std::getline(file, line)) {
        if (!line.empty() && line.find_first_not_of("0123456789") == std::string::npos) {
            if (current_grid) {
                data.push_back(*current_grid);
                delete current_grid;
            }
            current_step = std::stoi(line);
            current_grid = new Grid(size_x, size_y, spacing_x, spacing_y);
            current_row = 0;
        } 
        else if (!line.empty()) {
            std::vector<double> row;
            std::istringstream iss(line);
            double value;
            while (iss >> value) {
                row.push_back(value);
            }
            for (int j = 0; j < row.size(); ++j) {
                current_grid->set_value(current_row, j, row[j]);
            }
            ++current_row;
        }
    }

    if (current_grid) {
        data.push_back(*current_grid);
        delete current_grid;
    }

    file.close();

    steps = data.size();
    if (steps == 0) {
        throw std::runtime_error("File is empty or contains no valid data.");
    }
}

double ExactSolution::get_value(int x, int y, int step) const {
    if (step < 0 || step >= steps) {
        throw std::out_of_range("Step index out of range.");
    }
    if (x < 0 || x >= size_x || y < 0 || y >= size_y) {
        throw std::out_of_range("Grid index out of range.");
    }
    return data[step].get_value(x, y);
}
