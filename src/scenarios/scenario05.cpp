#include "../heuristic/heuristic.h"
#include "../is_final/is_final.h"
#include "../neighbors/neighbors.h"
#include "../route_finding/a_star.h"
#include "../visualization/visualization.h"
#include <vector>

/**
 * @brief Basic usage of library
 *
 * @param args
 */

void scenario05(const std::vector<int> &args) {
  kwi::status::Status start = {
      {{1, 1, 1, 1, 0},  //
       {1, 1, 0, 1, 0},  //
       {1, 0, 1, 1, 1},  // parking status
       {1, 0, 1, 1, 0},  //
       {1, 1, 0, 1, 1}}, //
      {4, 4}             // target car coords
  };

  std::array<unsigned int, 2> destination = {0, 0}; // IO

  auto isFinal = [&destination](const kwi::status::Status &s) {
    return kwi::is_final::isTargetOnDestination(s, destination);
  };

  auto heuristic = [&destination](const kwi::status::Status &s) { return kwi::heuristic::manhattan(s, destination); };

  auto [solution_moves, distance] = kwi::route_finding::aStar(start, isFinal, kwi::neighbors::orthogonal, heuristic);

  kwi::visualization::print(start, solution_moves);
}
