#ifndef EXACT_SOLUTION_H
#define EXACT_SOLUTION_H

#include "Grid.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>

// Класс чтения решения из текстового файла
// Считывает текстовый файл и создает структуру, хранящую полное решение задачи

class ExactSolution {
private:
    std::vector<Grid> data;
    int size_x;
    int size_y;
    int steps;
    double spacing_x;
    double spacing_y;

public:
    ExactSolution(const std::string& file_path, int size_x, int size_y, double spacing_x, double spacing_y);

    double get_value(int x, int y, int step) const;

    int get_size_x() const { return size_x; }
    int get_size_y() const { return size_y; }
    int get_steps() const { return steps; }
};

#endif // EXACT_SOLUTION_H
