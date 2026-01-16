#include "visualization.h"
#include "../solution/solution.h"
#include "../types.h"
#include "cli_visualization.h"

namespace kwi::visualization {

void play(const std::vector<kwi::status::Status> &history, int delay_milliseconds) {
  if (history.empty())
    return;
  int x_size = static_cast<int>(history[0].grid.size());
  int y_size = static_cast<int>(history[0].grid[0].size());
  CLIVisualization visualization(x_size, y_size);
  visualization.playSolutionPath(history, delay_milliseconds);
}

void play(const kwi::status::Status &start, const std::vector<Move> &moves, int delay_milliseconds) {
  auto solution_steps = kwi::solution::movesToStates(start, moves);
  play(solution_steps, delay_milliseconds);
}

void print(const std::vector<kwi::status::Status> &history) {
  if (history.empty())
    return;
  int x_size = static_cast<int>(history[0].grid.size());
  int y_size = static_cast<int>(history[0].grid[0].size());
  CLIVisualization visualization(x_size, y_size);
  visualization.printSolutionPath(history);
}

void print(const kwi::status::Status &start, const std::vector<Move> &moves) {
  auto solution_steps = kwi::solution::movesToStates(start, moves);
  print(solution_steps);
}

void save_to_file(const std::vector<kwi::status::Status> &history, const std::string &filename) {
  if (history.empty())
    return;
  int x_size = static_cast<int>(history[0].grid.size());
  int y_size = static_cast<int>(history[0].grid[0].size());
  CLIVisualization visualization(x_size, y_size);
  visualization.saveSolutionToFile(history, filename);
}

void save_to_file(const kwi::status::Status &start, const std::vector<Move> &moves, const std::string &filename) {
  auto solution_steps = kwi::solution::movesToStates(start, moves);
  save_to_file(solution_steps, filename);
}

} // namespace kwi::visualization