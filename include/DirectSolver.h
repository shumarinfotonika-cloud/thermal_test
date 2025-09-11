#ifndef DIRECT_SOLVER_H
#define DIRECT_SOLVER_H

#include "ConfigReader.h"

class DirectSolver {
private:
    const ConfigReader& config;

public:
    DirectSolver(const ConfigReader& config);

    void solve();
};

#endif // DIRECT_SOLVER_H