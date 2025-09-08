#include "ParserConst.h"

void addConstants(mu::Parser &parser) {
    parser.DefineConst("pi", 3.14159265358979323846);
    parser.DefineConst("e", 2.71828182845904523536);
}
