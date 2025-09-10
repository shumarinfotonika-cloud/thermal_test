#ifndef VTKSAVER_H
#define VTKSAVER_H

#include <string>
#include <vector>
#include "Grid.h"

class VTKSaver {
public:
    VTKSaver(const Grid &grid);

    void saveTemperature(const std::string &filename);

private:
    const Grid &grid;
};

#endif // VTKSAVER_H
