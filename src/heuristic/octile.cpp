#include "../types.h"
#include "heuristic.h"
#include <cmath>
#include <limits>

namespace kwi::heuristic {

constexpr uint sqrt2x100 = 141;

unsigned int octile(const status::Status &s, const std::array<unsigned int, 2> &dest) {
  int dx = abs((int)s.target_coords[0] - (int)dest[0]);
  int dy = abs((int)s.target_coords[1] - (int)dest[1]);
  uint n_diag = std::min(dx, dy);
  uint n_orth = abs(dx - dy);
  return sqrt2x100 * n_diag + 100 * n_orth;
}

uint octile(const status::Status &s, const std::vector<std::array<uint, 2>> &dest) {
  uint min_distance = std::numeric_limits<uint>::max();

  for (const auto &destination : dest) {
    int dx = abs((int)s.target_coords[0] - (int)destination[0]);
    int dy = abs((int)s.target_coords[1] - (int)destination[1]);
    uint n_diag = std::min(dx, dy);
    uint n_orth = abs(dx - dy);
    uint distance = sqrt2x100 * n_diag + 100 * n_orth;

    if (distance < min_distance) {
      min_distance = distance;
    }
  }

  return min_distance;
}

} // namespace kwi::heuristic