#ifndef VTKSAVER_H
#define VTKSAVER_H

#include <string>
#include <vector>
#include "Grid.h"

// Класс сохранения в формат VTK
// Реализует сохранение данных с сетки в формат VTK.
// Крайне удобен для просмотра в ParaView

class VTKSaver {
public:
    VTKSaver(const Grid &grid);

    void saveTemperature(const std::string &filename, const std::string &solve_name);

private:
    const Grid &grid;
};

#endif // VTKSAVER_H
