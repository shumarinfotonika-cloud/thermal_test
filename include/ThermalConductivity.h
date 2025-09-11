#ifndef THERMAL_CONDUCTIVITY_H
#define THERMAL_CONDUCTIVITY_H

#include <string>
#include "muParser.h"
#include "Grid.h"

class ThermalConductivity {
private:
    mu::Parser parser;
    double x, y;
    bool is_direct;
    bool is_inverse;
    Grid coefficients_grid;

public:
    ThermalConductivity(const std::string &formula);

    ThermalConductivity(int size_x, int size_y, double spacing_x, double spacing_y);

    ThermalConductivity(const ThermalConductivity& other);

    void updateCoefficients(const Grid& new_coefficients);
    double evaluate(double x, double y);
};

#endif // THERMAL_CONDUCTIVITY_H
