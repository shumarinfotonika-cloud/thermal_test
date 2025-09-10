#include "SourceFunction.h"
#include "ParserConst.h"

SourceFunction::SourceFunction(const std::string &formula)
    : parser() {
    addConstants(parser);

    parser.DefineVar("x", &x);
    parser.DefineVar("y", &y);
    parser.DefineVar("t", &t);

    try {
        parser.SetExpr(formula);
    } catch (mu::Parser::exception_type &e) {
        std::cerr << "Error in source function formula: " << e.GetMsg() << "\n";
        throw;
    }
}

double SourceFunction::evaluate(double new_x, double new_y, double new_t) {
    try {
        x = new_x;
        y = new_y;
        t = new_t;
        parser.DefineVar("x", &x);
        parser.DefineVar("y", &y);
        parser.DefineVar("t", &t);
        return parser.Eval();
    } catch (mu::Parser::exception_type &e) {
        std::cerr << "Error evaluating source function: " << e.GetMsg() << "\n";
        throw;
    }
}
