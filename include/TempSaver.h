#ifndef TEMPSAVER_H
#define TEMPSAVER_H

#include <fstream>
#include <iomanip>
#include <string>
#include "Grid.h"

// Класс сохранения в текстовый файл
// Реализует сохранение решения в текстовый файл

class TempSaver {
public:
    TempSaver(const Grid& grid);

    void save_step(int time_step, const std::string &filename);

private:
    const Grid& grid;
};

#endif // TEMPSAVER_H
