#ifndef INVERSE_SOLVER_H
#define INVERSE_SOLVER_H

#include "ConfigReader.h"

// Класс решения обратной задачи
// Реализует полное решение обратной задачи

class InverseSolver {
private:
    const ConfigReader& config;

public:
    InverseSolver(const ConfigReader& config);

    void solve();
};

#endif // INVERSE_SOLVER_H
