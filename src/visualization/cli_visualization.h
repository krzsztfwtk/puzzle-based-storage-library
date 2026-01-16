#pragma once

#include "../status/status.h"
#include <array>
#include <string>
#include <vector>

namespace kwi::visualization {

// Class for displaying the solution path in command line interface
class CLIVisualization {
public:
  CLIVisualization(int x_size, int y_size);

  // Print the entire solution path with moves highlighted
  void printSolutionPath(const std::vector<status::Status> &solution);

  // Play the solution path as an animation in the terminal
  void playSolutionPath(const std::vector<status::Status> &solution, int delay_milliseconds = 1000);

  void saveSolutionToFile(const std::vector<status::Status> &solution, const std::string &filename);

private:
  int x_size;
  int y_size;

  void setupConsole();

  // Helper methods
  void printGrid(const status::Status &s, const std::vector<std::array<int, 2>> *moved_from = nullptr,
                 const std::vector<std::array<int, 2>> *moved_to = nullptr, bool is_target_move = false);
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
  const std::string TARGET_TRAIL_COLOR;
  const std::string RESET;

  // Character customization
  const std::string TARGET_CHAR;
  const std::string OCCUPIED_CHAR;
  const std::string EMPTY_CHAR;
  const std::string MOVED_FROM_CHAR;
  const std::string MOVED_TO_CHAR;
  const std::string TARGET_MOVED_CHAR;
  const std::string TARGET_TRAIL_CHAR;

  const int width;
  const int height;
};

} // namespace kwi::visualization