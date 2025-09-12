#ifndef DIRECT_SOLVER_H
#define DIRECT_SOLVER_H

#include "ConfigReader.h"
#include "ThermalConductivity.h"

class DirectSolver {
private:
    const ConfigReader& config;
    ThermalConductivity coeffs;

public:
    DirectSolver(const ConfigReader& config);

    DirectSolver(const ConfigReader& config, const ThermalConductivity& new_coeffs);

    Grid solve();
};

#endif // DIRECT_SOLVER_H