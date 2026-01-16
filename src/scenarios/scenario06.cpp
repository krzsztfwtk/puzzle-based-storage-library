#include "../is_final/is_final.h"
#include "../neighbors/neighbors.h"
#include "../route_finding/dijkstra.h"
#include "../visualization/visualization.h"
#include <vector>

/**
 * @brief Dijkstra's algorithm with small parking configuration
 *
 * @param args
 */
void scenario06(const std::vector<int> &args) {
  kwi::status::Status start = {
      {{1, 1, 1, 1},  //
       {1, 0, 1, 1},  //
       {1, 1, 1, 1},  // parking status
       {1, 0, 1, 1},  //
       {1, 1, 1, 1}}, //
      {4, 1}          // target car coords
  };

  std::array<unsigned int, 2> destination = {0, 0}; // IO

  auto isFinal = [&destination](const kwi::status::Status &s) {
    return kwi::is_final::isTargetOnDestination(s, destination);
  };

  auto [solution_moves, distance] = kwi::route_finding::dijkstra(start, isFinal, kwi::neighbors::orthogonal);

  kwi::visualization::print(start, solution_moves);
}