#ifndef THERMAL_CONDUCTIVITY_H
#define THERMAL_CONDUCTIVITY_H

#include <string>
#include "muParser.h"
#include "Grid.h"

// Класс коэффициентов
// Реализует считывание функции, обработку через парсер в случаае прямой задачи.
// Реализует считывание и хранение таблицы коэффициентов в случае обратной задачи.
// Возвращает коэффициент в запрашиваемой точке

class ThermalConductivity {
private:
    mu::Parser parser;
    double x, y;
    bool is_direct;
    bool is_inverse;
    Grid coefficients_grid;

public:
    ThermalConductivity(const std::string &formula);

    ThermalConductivity(int size_x, int size_y, double spacing_x, double spacing_y, double init);

    ThermalConductivity(const ThermalConductivity& other);

    void updateCoefficients(const Grid& new_coefficients);
    double evaluate(int ind_x, int ind_y, double spacing_x, double spacing_y, double half_step_x, double half_step_y);
    Grid get_grid() {return coefficients_grid;}
};

#endif // THERMAL_CONDUCTIVITY_H
