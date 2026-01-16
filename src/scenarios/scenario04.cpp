#include "../heuristic/heuristic.h"
#include "../is_final/is_final.h"
#include "../neighbors/neighbors.h"
#include "../route_finding/a_star.h"
#include "../types.h"
#include "../visualization/visualization.h"
#include <iostream>
#include <vector>

/**
 * @brief Octilinear movement (exp02)
 *
 * @param args Positional arguments:
 *   - args[0]: x_size of the grid (default: 5)
 *   - args[1]: y_size of the grid (default: 5)
 *   - args[2]: number of I/O points (default: 1)
 *   - args[3]: I/O distribution 0 - vertex, 1 - edge (default: 0)
 *   - args[4]: number of occupied cells (default: 20)
 *   - args[5]: allow diagonal movement (default: 0)
 *   - args[3]: flag to enable/disable visualization (default: 0 = disabled)
 *   - args[4]: delay between visualization frames in milliseconds (default: 1000)
 *
 * @note Outputs number of moves and distance to stdout
 */
void scenario04(const std::vector<int> &args) {
  uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
  uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
  int io_num = (args.size() > 1) ? args[2] : 1;
  int io_edge_distribution = (args.size() > 3) ? args[3] : 0;
  int occupied_cells = (args.size() > 4) ? args[4] : 20;
  int allow_diagonal_movement = (args.size() > 5) ? args[5] : 0;
  int enable_visualization = (args.size() > 6) ? args[6] : 0;
  int visualization_delay_miliseconds = (args.size() > 7) ? args[7] : 1000;

  kwi::status::Status start = kwi::status::generate(x_size, y_size, occupied_cells);

  std::vector<std::array<uint, 2>> destinations;

  switch (io_num) {
  case 1:
    if (io_edge_distribution)
      destinations = {{x_size / 2, 0}};
    else
      destinations = {{0, 0}};
    break;

  case 2:
    if (io_edge_distribution)
      destinations = {{x_size / 2, 0}, {x_size / 2, y_size - 1}};
    else
      destinations = {{0, 0}, {x_size - 1, y_size - 1}};
    break;

  case 3:
    if (io_edge_distribution)
      destinations = {{0, y_size / 2}, {x_size / 2, 0}, {x_size / 2, y_size - 1}};
    else
      destinations = {{0, 0}, {0, y_size - 1}, {x_size - 1, y_size - 1}};
    break;

  case 4:
    if (io_edge_distribution)
      destinations = {{0, y_size / 2}, {x_size / 2, 0}, {x_size / 2, y_size - 1}, {x_size - 1, y_size / 2}};
    else
      destinations = {{0, 0}, {0, y_size - 1}, {x_size - 1, 0}, {x_size - 1, y_size - 1}};
    break;

  default:
    break;
  }

  auto isFinal = [&destinations](const kwi::status::Status &s) {
    return kwi::is_final::isTargetOnDestination(s, destinations);
  };

  auto heuristic = [&destinations](const kwi::status::Status &current_status) {
    return kwi::heuristic::manhattanOccupancyVariant(current_status, destinations);
  };

  auto neighborsFunction = kwi::neighbors::orthogonal;

  if (allow_diagonal_movement)
    neighborsFunction = kwi::neighbors::octilinear;

  auto [moves, distance] = kwi::route_finding::aStar(start, isFinal, neighborsFunction, heuristic);

  if (enable_visualization)
    kwi::visualization::play(start, moves, visualization_delay_miliseconds);

  std::cout << moves.size() << " " << distance << std::endl;
}