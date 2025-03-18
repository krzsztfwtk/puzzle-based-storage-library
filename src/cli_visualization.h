#pragma once

#include <vector>
#include <string>
#include "status.h"

namespace kwi::visualization {

// CLI Visualization class for displaying the solution path in command line interface
class CLIVisualization {
public:
    // Constructor accepting the grid dimensions
    CLIVisualization(int x_size, int y_size);

    // Print the entire solution path with moves highlighted
    void printSolutionPath(const std::vector<kwi::status::Status> &solution);

    // Play the solution path as an animation in the terminal
    void playSolutionPath(const std::vector<kwi::status::Status> &solution, int delay_milliseconds = 1000);

    void saveSolutionToFile(const std::vector<kwi::status::Status> &solution, const std::string &filename);

private:
    int x_size;
    int y_size;

    // Private helper methods
    void printGrid(const kwi::status::Status &s, const std::vector<std::array<int, 2>> *moved_from = nullptr,
                   const std::vector<std::array<int, 2>> *moved_to = nullptr);
    void clearConsole();
    std::string hexToAnsi(const std::string &hexColor);
    std::string repeatString(const std::string &str, int times);

    // Color customization
    const std::string TARGET_COLOR;
    const std::string OCCUPIED_COLOR;
    const std::string EMPTY_COLOR;
    const std::string MOVED_FROM_COLOR;
    const std::string MOVED_TO_COLOR;
    const std::string TARGET_MOVED_COLOR;
    const std::string RESET;

    // Character customization
    const std::string TARGET_CHAR;
    const std::string OCCUPIED_CHAR;
    const std::string EMPTY_CHAR;
    const std::string MOVED_FROM_CHAR;
    const std::string MOVED_TO_CHAR;
    const std::string TARGET_MOVED_CHAR;

    const int width;
    const int height;
};

}