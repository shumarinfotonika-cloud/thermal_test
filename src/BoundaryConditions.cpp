#include <string>
#include "BoundaryConditions.h"
#include "ParserConst.h"

BoundaryCondition::BoundaryCondition(int axis, int side, const std::string &value)
    : axis(axis), side(side), value(value) {
        addConstants(parser);

        parser.DefineVar("x", &x);
        parser.DefineVar("y", &y);

        try {
            parser.SetExpr(value);
        } catch (mu::Parser::exception_type &e) {
            std::cerr << "Error in formula: " << e.GetMsg() << "\n";
            throw;
        }
    }

void BoundaryCondition::apply(Grid &grid) {
    int size_x = grid.get_size_x();
    int size_y = grid.get_size_y();
    double spacing_x = grid.get_spacing_x();
    double spacing_y = grid.get_spacing_y();

    if (axis == 0) {
        int x_index;
        if (side == 0) {
            x_index = 0;
        } else {
            x_index = size_x - 1;
        }

        for (int j = 0; j < size_y; ++j) {
            double current_x = x_index * spacing_x;
            set_x(current_x);
            double current_y = j * spacing_y;
            set_y(current_y);
            try {
                parser.DefineVar("x", &x);
                parser.DefineVar("y", &y);
                double current_value = parser.Eval();
                grid.set_value(x_index, j, current_value);
            } catch (mu::Parser::exception_type &e) {
                std::cerr << "Error in formula: " << e.GetMsg() << "\n";
                throw;
            }
        }
    } else if (axis == 1) {
        int y_index;
        if (side == 0) {
            y_index = 0;
        } else {
            y_index = size_y - 1;
        }

        for (int i = 0; i < size_x; ++i) {
            double current_x = i * spacing_x;
            set_x(current_x);
            double current_y = y_index * spacing_y;
            set_y(current_y);

            try {
                parser.DefineVar("x", &x);
                parser.DefineVar("y", &y);
                double current_value = parser.Eval();
                grid.set_value(i, y_index, current_value);
            } catch (mu::Parser::exception_type &e) {
                std::cerr << "Error in formula: " << e.GetMsg() << "\n";
                throw;
            }
        }
    }
}

