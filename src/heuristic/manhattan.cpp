#include "../types.h"
#include "heuristic.h"
#include <cmath>
#include <limits>


namespace kwi::heuristic {

uint manhattan(const status::Status &s, const std::array<uint, 2> &dest) {
  int dx = abs((int)s.target_coords[0] - (int)dest[0]);
  int dy = abs((int)s.target_coords[1] - (int)dest[1]);
  return (dx + dy) * 100;
}

uint manhattan(const status::Status &s, const std::vector<std::array<uint, 2>> &dest) {
  uint min_distance = std::numeric_limits<uint>::max();

  for (const auto &destination : dest) {
    int dx = abs((int)s.target_coords[0] - (int)destination[0]);
    int dy = abs((int)s.target_coords[1] - (int)destination[1]);
    uint distance = dx + dy;

    if (distance < min_distance) {
      min_distance = distance;
    }
  }

  return min_distance * 100;
}

uint manhattanOccupancyVariant(const status::Status &s, const std::array<unsigned int, 2> &dest) {
  unsigned int dx = (unsigned int)abs((int)s.target_coords[0] - (int)dest[0]);
  unsigned int dy = (unsigned int)abs((int)s.target_coords[1] - (int)dest[1]);

  unsigned int distance = (dx + dy) * 100;

  // Add a penalty for occupied cells between target and destination
  int start_x = (int)s.target_coords[0];
  int end_x = (int)dest[0];
  int step_x = (end_x > start_x) ? 1 : -1;
  int start_y = (int)s.target_coords[1];
  int end_y = (int)dest[1];
  int step_y = (end_y > start_y) ? 1 : -1;

  for (int x = start_x; x != end_x; x += step_x) {
    for (int y = start_y; y != end_y; y += step_y) {
      if (s.grid[x][y]) {
        distance += 50;
      }
    }
  }
  return distance;
}

uint manhattanOccupancyVariant(const status::Status &s, const std::vector<std::array<uint, 2>> &dest) {
  uint min_distance = std::numeric_limits<uint>::max();

  for (const auto &destination : dest) {
    uint distance = manhattanOccupancyVariant(s, destination);

    if (distance < min_distance) {
      min_distance = distance;
    }
  }

  return min_distance;
}

} // namespace kwi::heuristic