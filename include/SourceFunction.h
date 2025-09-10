#ifndef SOURCE_FUNCTION_H
#define SOURCE_FUNCTION_H

#include <string>
#include "muParser.h"

class SourceFunction {
private:
    mu::Parser parser;
    double x, y, t;

public:
    SourceFunction(const std::string &formula);

    double evaluate(double new_x, double new_y, double new_t);
};

#endif // SOURCE_FUNCTION_H
