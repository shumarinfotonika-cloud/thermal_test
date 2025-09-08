#include "Grid.h"
#include <iostream>
#include <iomanip>

Grid::Grid(int size_x, int size_y, double spacing_x, double spacing_y)
    : size_x(size_x), size_y(size_y),
      spacing_x(spacing_x), spacing_y(spacing_y),
      data(size_y, std::vector<double>(size_x, 0.0)) {}

void Grid::initialize(double initial_value) {
    for (int y = 0; y < size_y; ++y) {
        for (int x = 0; x < size_x; ++x) {
            data[y][x] = initial_value;
        }
    }
}

double Grid::get_value(int x, int y) const {
    if (x < 0 || x >= size_x || y < 0 || y >= size_y) {
        throw std::out_of_range("Grid indices out of range");
    }
    return data[y][x];
}

void Grid::set_value(int x, int y, double value) {
    if (x < 0 || x >= size_x || y < 0 || y >= size_y) {
        throw std::out_of_range("Grid indices out of range");
    }
    data[y][x] = value;
}

void Grid::print() const {
    for (int y = size_y - 1; y >= 0; --y) {
        for (int x = 0; x < size_x; ++x) {
            std::cout << std::setw(8) << data[y][x] << " ";
        }
        std::cout << "\n";
    }
}
