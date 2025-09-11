#include "ThermalConductivity.h"
#include "ParserConst.h"

ThermalConductivity::ThermalConductivity(const std::string &formula)
    : is_direct(true), is_inverse(false), coefficients_grid(0, 0, 0.0, 0.0) {
    
    addConstants(parser);

    parser.DefineVar("x", &x);
    parser.DefineVar("y", &y);

    try {
        parser.SetExpr(formula);
    } catch (mu::Parser::exception_type &e) {
        std::cerr << "Error in thermal conductivity formula: " << e.GetMsg() << "\n";
        throw;
    }
}

ThermalConductivity::ThermalConductivity(int size_x, int size_y, double spacing_x, double spacing_y)
    : is_direct(false), is_inverse(true), coefficients_grid(size_x, size_y, spacing_x, spacing_y) {

    coefficients_grid.initialize(0.0);
}

void ThermalConductivity::updateCoefficients(const Grid& new_coefficients) {
    if (!is_inverse) {
        throw std::runtime_error("Cannot update coefficients for a direct task.");
    }
    coefficients_grid = new_coefficients;
}

ThermalConductivity::ThermalConductivity(const ThermalConductivity& other)
    : is_direct(other.is_direct),
      is_inverse(other.is_inverse),
      coefficients_grid(other.coefficients_grid) {
    if (is_direct) {
        parser = mu::Parser();
        addConstants(parser);
        parser.DefineVar("x", &x);
        parser.DefineVar("y", &y);
        parser.SetExpr(other.parser.GetExpr());
    }
}


double ThermalConductivity::evaluate(double new_x, double new_y) {
    if (is_direct) {
        try {
            x = new_x;
            y = new_y;
            parser.DefineVar("x", &x);
            parser.DefineVar("y", &y);
            return parser.Eval();
        } catch (mu::Parser::exception_type &e) {
            std::cerr << "Error evaluating thermal conductivity: " << e.GetMsg() << "\n";
            throw;
        }
    } else if (is_inverse) {
        int i = static_cast<int>(x / coefficients_grid.get_spacing_x());
        int j = static_cast<int>(y / coefficients_grid.get_spacing_y());
        return coefficients_grid.get_value(i, j);
    } else {
        throw std::runtime_error("Invalid task type in ThermalConductivity.");
    }

}
