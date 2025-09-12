#ifndef BOUNDARY_CONDITION_H
#define BOUNDARY_CONDITION_H

#include <string>
#include "Grid.h"

#include "muParser.h"

// Класс граничных условий
// Считывает переданную формулу через парсер и возвращает значение в заданной точке

class BoundaryCondition {
private:
    int axis; // 0 — x, 1 — y
    int side; //  0 — left, 1 — right
    std::string value;
    double x;
    double y;
    double t;
    mu::Parser parser;

public:
    BoundaryCondition(int axis, int side, const std::string &value);

    void set_x(double new_x) {x = new_x;}
    void set_y(double new_y) {y = new_y;}
    void set_t(double new_t) {t = new_t;}

    void apply(Grid& grid, double time);
};

#endif // BOUNDARY_CONDITION_H
