
#include "../heuristic/heuristic.h"
#include "../is_final/is_final.h"
#include "../neighbors/neighbors.h"
#include "../route_finding/a_star.h"
#include "../types.h"
#include "../visualization/visualization.h"
#include <vector>

/**
 * @brief Octilinear movement with 4 I/O points with visualization
 *
 * @param args Positional arguments:
 *   - args[0]: x_size of the grid (default: 5)
 *   - args[1]: y_size of the grid (default: 5)
 *   - args[2]: number of occupied cells in the grid (default: 20)
 */
void scenario03(const std::vector<int> &args) {
  uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
  uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
  int num_occupied = args.size() > 2 ? args[2] : 20;

  kwi::status::Status start = kwi::status::generate(x_size, y_size, num_occupied);

  std::vector<std::array<uint, 2>> destinations = {{0, 0}, {0, y_size - 1}, {x_size - 1, 0}, {x_size - 1, y_size - 1}};

  auto isFinal = [&destinations](const kwi::status::Status &s) {
    return kwi::is_final::isTargetOnDestination(s, destinations);
  };

  auto heuristic = [&destinations](const kwi::status::Status &current_status) {
    return kwi::heuristic::manhattan(current_status, destinations);
  };

  auto [moves, distance] = kwi::route_finding::aStar(start, isFinal, kwi::neighbors::orthogonal, heuristic);

  kwi::visualization::play(start, moves);
}