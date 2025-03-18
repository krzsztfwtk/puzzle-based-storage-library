#include "cli_visualization.h"

#include <iostream>
#include <tuple>
#include <string>
#include <sstream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <algorithm>
#include <fstream>


#include "solution.h" // Ensure this header is included and available

using namespace std;

namespace kwi::visualization {

// Constructor
CLIVisualization::CLIVisualization(int x_size, int y_size)
    : x_size(x_size), y_size(y_size),
      TARGET_COLOR(hexToAnsi("FF0000")),
      OCCUPIED_COLOR(hexToAnsi("000000")),
      EMPTY_COLOR(hexToAnsi("000000")),
      MOVED_FROM_COLOR(hexToAnsi("0000FF")),
      MOVED_TO_COLOR(hexToAnsi("0000FF")),
      TARGET_MOVED_COLOR(hexToAnsi("FF00FF")),
      RESET("\033[0m"),
      TARGET_CHAR("◼ "),
      OCCUPIED_CHAR("◼ "),
      EMPTY_CHAR("□ "),
      MOVED_FROM_CHAR("□ "),
      MOVED_TO_CHAR("◼ "),
      TARGET_MOVED_CHAR("◼ "),
      width(1),
      height(1)
{
    std::cout << "Visualization init";
}

// Helper function to repeat a string
std::string CLIVisualization::repeatString(const std::string &str, int times) {
    std::string result;
    for (int i = 0; i < times; ++i) {
        result += str;
    }
    return result;
}

void CLIVisualization::clearConsole() {
    std::cout << "\033[2J\033[1;1H"; // ANSI escape sequence for clearing the screen
}

std::string CLIVisualization::hexToAnsi(const std::string &hexColor) {
    if (hexColor.length() != 6)
        return "\033[0m"; // Reset color if invalid hex

    // Convert hex to RGB
    int r, g, b;
    std::istringstream(hexColor.substr(0, 2)) >> std::hex >> r;
    std::istringstream(hexColor.substr(2, 2)) >> std::hex >> g;
    std::istringstream(hexColor.substr(4, 2)) >> std::hex >> b;

    // Return ANSI escape code for 24-bit RGB color
    std::ostringstream oss;
    oss << "\033[38;2;" << r << ";" << g << ";" << b << "m";
    return oss.str();
}

void CLIVisualization::printGrid(const kwi::status::Status &s, const std::vector<std::array<int, 2>> *moved_from,
                              const std::vector<std::array<int, 2>> *moved_to) {
    for (int x = 0; x < x_size; ++x) {
        for (int i = 0; i < height; i++) {
            for (int y = 0; y < y_size; ++y) {
                bool is_moved_to = moved_to && std::any_of(moved_to->begin(), moved_to->end(),
                                                           [x, y](const std::array<int, 2> &pos) {
                                                               return pos[0] == x && pos[1] == y;
                                                           });
                bool is_moved_from = moved_from && std::any_of(moved_from->begin(), moved_from->end(),
                                                               [x, y](const std::array<int, 2> &pos) {
                                                                   return pos[0] == x && pos[1] == y;
                                                               });

                int target_x = s.target_coords[0];
                int target_y = s.target_coords[1];

                if (is_moved_to && target_x == x && target_y == y) {
                    std::cout << TARGET_MOVED_COLOR << repeatString(TARGET_MOVED_CHAR, width) << RESET; // Target moved
                } else if (is_moved_to) {
                    std::cout << MOVED_TO_COLOR << repeatString(MOVED_TO_CHAR, width) << RESET; // Moved-to cell
                } else if (is_moved_from) {
                    std::cout << MOVED_FROM_COLOR << repeatString(MOVED_FROM_CHAR, width) << RESET; // Moved-from cell
                } else if (target_x == x && target_y == y) {
                    std::cout << TARGET_COLOR << repeatString(TARGET_CHAR, width) << RESET; // Target
                } else if (s.are_occupied_grid[x][y]) {
                    std::cout << OCCUPIED_COLOR << repeatString(OCCUPIED_CHAR, width) << RESET; // Occupied cell
                } else {
                    std::cout << EMPTY_COLOR << repeatString(EMPTY_CHAR, width) << RESET; // Empty cell
                }
            }
            std::cout << '\n';
        }
    }
    std::cout << EMPTY_COLOR << "\n\n" << RESET << "\n";
}

void CLIVisualization::printSolutionPath(const std::vector<kwi::status::Status> &solution) {
    std::cout << "Solution Path:\n";

    // Decode the solution into moves
    auto decoded_solution = kwi::solution::decode(solution);

    for (size_t i = 0; i < solution.size(); ++i) {
        const auto &status = solution[i];

        if (i == 0) {
            // First state: No highlights
            printGrid(status);
        } else {
            const auto &[unused, moved_from, moved_to] = decoded_solution[i - 1];
            printGrid(status, &moved_from, &moved_to);
        }
    }
}

void CLIVisualization::playSolutionPath(const std::vector<kwi::status::Status> &solution, int delay_milliseconds) {
    // Decode the solution into moves
    auto decoded_solution = kwi::solution::decode(solution);

    for (size_t i = 0; i < solution.size(); ++i) {
        clearConsole();

        const auto &status = solution[i];

        if (i == 0) {
            // First state: No highlights
            printGrid(status);
        } else {
            const auto &[unused, moved_from, moved_to] = decoded_solution[i - 1];
            printGrid(status, &moved_from, &moved_to);
        }

        // Wait for the specified delay in milliseconds
        std::this_thread::sleep_for(std::chrono::milliseconds(delay_milliseconds));
    }
}
void CLIVisualization::saveSolutionToFile(const std::vector<kwi::status::Status> &solution, const std::string &filename) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return;
    }

    // Decode the solution into moves
    auto decoded_solution = kwi::solution::decode(solution);

    for (size_t i = 0; i < solution.size(); ++i) {
        const auto &status = solution[i];
        int target_x = status.target_coords[0];
        int target_y = status.target_coords[1];

        outFile << "Step " << i << ":\n";
        outFile << "Target coordinates: (" << target_x << ", " << target_y << ")\n";
        outFile << "Grid status (occupied = 1, empty = 0):\n";

        for (const auto &row : status.are_occupied_grid) {
            for (int cell : row) {
                outFile << cell << " ";
            }
            outFile << "\n";
        }

        if (i > 0) {
            const auto &[unused, moved_from, moved_to] = decoded_solution[i - 1];
            outFile << "Moved from: ";
            for (const auto &pos : moved_from) {
                outFile << "(" << pos[0] << ", " << pos[1] << ") ";
            }
            outFile << "\nMoved to: ";
            for (const auto &pos : moved_to) {
                outFile << "(" << pos[0] << ", " << pos[1] << ") ";
            }
            outFile << "\n";
        }

        outFile << "\n";
    }

    outFile.close();
    std::cout << "Solution saved to " << filename << std::endl;
}

}