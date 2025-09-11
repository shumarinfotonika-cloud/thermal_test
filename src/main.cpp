#include <iostream>
#include "ConfigReader.h"
#include "DirectSolver.h"
#include "InverseSolver.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <config_file>\n";
        return 1;
    }

    try {
        ConfigReader config(argv[1]);
        std::string task_type = config.getString("", "task_type");

        if (task_type == "direct") {
            DirectSolver solver(config);
            solver.solve();

        } else if (task_type == "inverse") {
            InverseSolver solver(config);
            solver.solve();

        } else {
            std::cerr << "Unknown task type: " << task_type << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
