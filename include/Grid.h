#ifndef GRID_H
#define GRID_H

#include <vector>
#include <stdexcept>

class Grid {
private:
    int size_x, size_y;
    double spacing_x, spacing_y;
    std::vector<std::vector<double>> data;

public:
    Grid(int size_x, int size_y, double spacing_x, double spacing_y);

    void initialize(double initial_value = 0.0);

    int get_size_x() const { return size_x; }
    int get_size_y() const { return size_y; }

    double get_spacing_x() const { return spacing_x; }
    double get_spacing_y() const { return spacing_y; }

    double get_value(int x, int y) const;

    void set_value(int x, int y, double value);

    void print() const;
};

#endif // GRID_H
