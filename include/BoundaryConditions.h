#ifndef BOUNDARY_CONDITION_H
#define BOUNDARY_CONDITION_H

#include "Grid.h"

class BoundaryCondition {
private:
    int axis; // 0 — x, 1 — y
    int side; //  0 — left, 1 — right
    double value;

public:
    BoundaryCondition(int axis, int side, double value);

    void apply(Grid& grid) const;
};

#endif // BOUNDARY_CONDITION_H
