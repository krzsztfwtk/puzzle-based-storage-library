#include "cli_visualization.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

#include "../solution/solution.h"
#include "../types.h"

using namespace std;

namespace kwi::visualization {

CLIVisualization::CLIVisualization(int x_size, int y_size)
    : x_size(x_size), y_size(y_size), TARGET_COLOR(hexToAnsi("FF0000")), OCCUPIED_COLOR(hexToAnsi("666666")),
      EMPTY_COLOR(hexToAnsi("666666")), MOVED_FROM_COLOR(hexToAnsi("333399")), MOVED_TO_COLOR(hexToAnsi("0000FF")),
      TARGET_MOVED_COLOR(hexToAnsi("FF00FF")), TARGET_TRAIL_COLOR(hexToAnsi("993399")), RESET("\033[0m"),
      TARGET_CHAR("◼ "), OCCUPIED_CHAR("◼ "), EMPTY_CHAR("□ "), MOVED_FROM_CHAR("□ "), MOVED_TO_CHAR("◼ "),
      TARGET_MOVED_CHAR("◼ "), TARGET_TRAIL_CHAR("□ "), width(1), height(1) {
  setupConsole();
}

void CLIVisualization::setupConsole() {
#ifdef _WIN32
  // Ensure UTF-8 output so Unicode glyphs (◼, □) render correctly.
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);

  // Enable ANSI escape sequence processing (colors, cursor movement) on Windows 10+.
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) {
    return;
  }

  DWORD dwMode = 0;
  if (!GetConsoleMode(hOut, &dwMode)) {
    return;
  }

  dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
  SetConsoleMode(hOut, dwMode);
#endif
}

std::string CLIVisualization::repeatString(const std::string &str, int times) {
  std::string result;
  for (int i = 0; i < times; ++i) {
    result += str;
  }
  return result;
}

void CLIVisualization::clearConsole() {
#ifdef _WIN32
  HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
  if (hOut == INVALID_HANDLE_VALUE) {
    return;
  }

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  if (!GetConsoleScreenBufferInfo(hOut, &csbi)) {
    return;
  }

  DWORD cellCount = static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y);
  COORD homeCoords = {0, 0};

  DWORD count;
  FillConsoleOutputCharacterA(hOut, ' ', cellCount, homeCoords, &count);
  FillConsoleOutputAttribute(hOut, csbi.wAttributes, cellCount, homeCoords, &count);
  SetConsoleCursorPosition(hOut, homeCoords);
#else
  std::cout << "\033[2J\033[H"; // Clear screen and move cursor to home
#endif
  std::cout.flush();
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

void CLIVisualization::printGrid(const status::Status &s, const std::vector<std::array<int, 2>> *moved_from,
                                 const std::vector<std::array<int, 2>> *moved_to, bool is_target_move) {
  for (int x = 0; x < x_size; ++x) {
    for (int i = 0; i < height; i++) {
      for (int y = 0; y < y_size; ++y) {
        bool is_moved_to =
            moved_to && std::any_of(moved_to->begin(), moved_to->end(),
                                    [x, y](const std::array<int, 2> &pos) { return pos[0] == x && pos[1] == y; });
        bool is_moved_from =
            moved_from && std::any_of(moved_from->begin(), moved_from->end(),
                                      [x, y](const std::array<int, 2> &pos) { return pos[0] == x && pos[1] == y; });

        int target_x = s.target_coords[0];
        int target_y = s.target_coords[1];

        if (is_moved_to && target_x == x && target_y == y) {
          std::cout << TARGET_MOVED_COLOR << repeatString(TARGET_MOVED_CHAR, width) << RESET; // Target moved
        } else if (is_moved_to) {
          std::cout << MOVED_TO_COLOR << repeatString(MOVED_TO_CHAR, width) << RESET; // Moved-to cell
        } else if (is_moved_from && is_target_move) {
          std::cout << TARGET_TRAIL_COLOR << repeatString(TARGET_TRAIL_CHAR, width) << RESET; // Target trail
        } else if (is_moved_from) {
          std::cout << MOVED_FROM_COLOR << repeatString(MOVED_FROM_CHAR, width) << RESET; // Moved-from cell
        } else if (target_x == x && target_y == y) {
          std::cout << TARGET_COLOR << repeatString(TARGET_CHAR, width) << RESET; // Target
        } else if (s.grid[x][y]) {
          std::cout << OCCUPIED_COLOR << repeatString(OCCUPIED_CHAR, width) << RESET; // Occupied cell
        } else {
          std::cout << EMPTY_COLOR << repeatString(EMPTY_CHAR, width) << RESET; // Empty cell
        }
      }
      std::cout << '\n';
    }
  }
  std::cout << EMPTY_COLOR << "\n\n" << RESET << "\n";
  std::cout.flush();
}

void CLIVisualization::printSolutionPath(const std::vector<status::Status> &solution) {
  std::cout << "Solution Path:\n";

  auto moves = kwi::solution::statesToMoves(solution);

  for (size_t i = 0; i < solution.size(); ++i) {
    const auto &status = solution[i];

    if (i == 0) {
      std::cout << "Step " << i << ": Initial state of the parking\n";
      std::cout << "Target vehicle at position: (" << status.target_coords[0] << ", " << status.target_coords[1]
                << ")\n\n";
      printGrid(status);
    } else {
      const auto &move = moves[i - 1];
      const auto &prev_status = solution[i - 1];
      bool is_target_move =
          (move.from_x == prev_status.target_coords[0] && move.from_y == prev_status.target_coords[1]);

      std::cout << "Step " << i << ": ";
      if (is_target_move) {
        std::cout << "Target vehicle";
      } else {
        std::cout << "Vehicle";
      }
      std::cout << " moved from (" << static_cast<int>(move.from_y) << ", " << static_cast<int>(move.from_x) << ")";
      std::cout << " to (" << static_cast<int>(move.to_x) << ", " << static_cast<int>(move.to_y) << ")\n\n";

      std::vector<std::array<int, 2>> moved_from = {{move.from_x, move.from_y}};
      std::vector<std::array<int, 2>> moved_to = {{move.to_x, move.to_y}};
      printGrid(status, &moved_from, &moved_to, is_target_move);
    }
  }

  std::cout << "Solution completed in " << moves.size() << " moves.\n";
}

void CLIVisualization::playSolutionPath(const std::vector<status::Status> &solution, int delay_milliseconds) {
  // Decode the solution into moves
  auto moves = kwi::solution::statesToMoves(solution);

  for (size_t i = 0; i < solution.size(); ++i) {
    clearConsole();

    const auto &status = solution[i];

    if (i == 0) {
      // First state: No highlights
      printGrid(status);
    } else {
      const auto &move = moves[i - 1];
      const auto &prev_status = solution[i - 1];
      bool is_target_move =
          (move.from_x == prev_status.target_coords[0] && move.from_y == prev_status.target_coords[1]);
      std::vector<std::array<int, 2>> moved_from = {{move.from_x, move.from_y}};
      std::vector<std::array<int, 2>> moved_to = {{move.to_x, move.to_y}};
      printGrid(status, &moved_from, &moved_to, is_target_move);
    }

    // Wait for the specified delay in milliseconds
    std::this_thread::sleep_for(std::chrono::milliseconds(delay_milliseconds));
  }
}
void CLIVisualization::saveSolutionToFile(const std::vector<status::Status> &solution, const std::string &filename) {
  std::ofstream outFile(filename);
  if (!outFile.is_open()) {
    std::cerr << "Error: Unable to open file " << filename << std::endl;
    return;
  }

  // Decode the solution into moves
  auto moves = kwi::solution::statesToMoves(solution);

  for (size_t i = 0; i < solution.size(); ++i) {
    const auto &status = solution[i];
    int target_x = status.target_coords[0];
    int target_y = status.target_coords[1];

    outFile << "Step " << i << ":\n";
    outFile << "Target coordinates: (" << target_x << ", " << target_y << ")\n";
    outFile << "Grid status (occupied = 1, empty = 0):\n";

    for (const auto &row : status.grid) {
      for (int cell : row) {
        outFile << cell << " ";
      }
      outFile << "\n";
    }

    if (i > 0) {
      const auto &move = moves[i - 1];
      outFile << "Moved from: (" << static_cast<int>(move.from_x) << ", " << static_cast<int>(move.from_y) << ")\n";
      outFile << "Moved to: (" << static_cast<int>(move.to_x) << ", " << static_cast<int>(move.to_y) << ")\n";
    }

    outFile << "\n";
  }

  outFile.close();
  std::cout << "Solution saved to " << filename << std::endl;
}

} // namespace kwi::visualization