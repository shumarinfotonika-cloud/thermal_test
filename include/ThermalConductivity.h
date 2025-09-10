#ifndef THERMAL_CONDUCTIVITY_H
#define THERMAL_CONDUCTIVITY_H

#include <string>
#include "muParser.h"

class ThermalConductivity {
private:
    mu::Parser parser;
    double x, y;

public:
    ThermalConductivity(const std::string &formula);

    double evaluate(double x, double y);
};

#endif // THERMAL_CONDUCTIVITY_H
