#include "ThermalConductivity.h"
#include "ParserConst.h"

ThermalConductivity::ThermalConductivity(const std::string &formula) {
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

double ThermalConductivity::evaluate(double new_x, double new_y) {
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
}
