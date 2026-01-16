#include "../heuristic/heuristic.h"
#include "../is_final/is_final.h"
#include "../neighbors/neighbors.h"
#include "../route_finding/a_star.h"
#include "../types.h"
#include "../visualization/visualization.h"
#include <iostream>
#include <vector>

/**
 * @brief Octilinear movement (exp01.2)
 *
 * @param args Positional arguments:
 *   - args[0]: x_size of the grid (default: 5)
 *   - args[1]: y_size of the grid (default: 5)
 *   - args[2]: number of occupied cells in the grid (default: 20)
 *   - args[3]: flag to enable/disable visualization (default: 0 = disabled)
 *   - args[4]: delay between visualization frames in milliseconds (default: 1000)
 *
 * @note Outputs number of moves and distance to stdout
 */
void scenario02(const std::vector<int> &args) {
  uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
  uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
  int num_occupied = args.size() > 2 ? args[2] : 20;
  int enable_visualization = (args.size() > 3) ? args[3] : 0;
  int visualization_delay_miliseconds = (args.size() > 4) ? args[4] : 1000;

  kwi::status::Status start = kwi::status::generate(x_size, y_size, num_occupied);

  std::array<uint, 2> destination = {0, 0};

  auto isFinal = [&destination](const kwi::status::Status &s) {
    return kwi::is_final::isTargetOnDestination(s, destination);
  };

  auto heuristic = [&destination](const kwi::status::Status &current_status) {
    return kwi::heuristic::manhattanOccupancyVariant(current_status, destination);
  };

  auto [moves, distance] = kwi::route_finding::aStar(start, isFinal, kwi::neighbors::octilinear, heuristic);

  if (enable_visualization) {
    kwi::visualization::play(start, moves, visualization_delay_miliseconds);
  }

  std::cout << moves.size() << " " << distance << std::endl;
}