#include "../types.h"
#include "heuristic.h"
#include <cmath>
#include <limits>

namespace kwi::heuristic {

unsigned int chebyshev(const status::Status &s, const std::array<unsigned int, 2> &dest) {
  int dx = abs((int)s.target_coords[0] - (int)dest[0]);
  int dy = abs((int)s.target_coords[1] - (int)dest[1]);
  return std::max(dx, dy) * 100;
}

uint chebyshev(const status::Status &s, const std::vector<std::array<uint, 2>> &dest) {
  uint min_distance = std::numeric_limits<uint>::max();

  for (const auto &destination : dest) {
    int dx = abs((int)s.target_coords[0] - (int)destination[0]);
    int dy = abs((int)s.target_coords[1] - (int)destination[1]);
    uint distance = std::max(dx, dy);

    if (distance < min_distance) {
      min_distance = distance;
    }
  }

  return min_distance * 100;
}

} // namespace kwi::heuristic