#include "BoundaryConditions.h"

BoundaryCondition::BoundaryCondition(int axis, int side, double value)
    : axis(axis), side(side), value(value) {}

void BoundaryCondition::apply(Grid& grid) const {
    int size_x = grid.get_size_x();
    int size_y = grid.get_size_y();

    if (axis == 0) {
        if (side == 0) {
            for (int y = 0; y < size_y; ++y) {
                grid.set_value(0, y, value);
            }
        } else if (side == 1) {
            for (int y = 0; y < size_y; ++y) {
                grid.set_value(size_x - 1, y, value);
            }
        }
    } else if (axis == 1) {
        if (side == 0) {
            for (int x = 0; x < size_x; ++x) {
                grid.set_value(x, 0, value);
            }
        } else if (side == 1) {
            for (int x = 0; x < size_x; ++x) {
                grid.set_value(x, size_y - 1, value);
            }
        }
    }
}
