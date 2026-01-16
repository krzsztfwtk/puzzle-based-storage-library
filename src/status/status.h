#pragma once

#include <array>
#include <cstdint>
#include <functional>

namespace kwi::status {

/**
 * @brief Representation of parking status
 *
 */
struct Status {
  std::vector<std::vector<bool>> grid;
  unsigned int target_coords[2];

  bool operator==(const Status &other) const {
    return grid == other.grid && target_coords[0] == other.target_coords[0] &&
           target_coords[1] == other.target_coords[1];
  }
};

/**
 * @brief Custom status hash for unordered_map
 *
 */
struct Hash {
  size_t operator()(const Status &s) const {
    size_t hash = 0;

    for (auto row_it = s.grid.begin(); row_it != s.grid.end(); ++row_it) {
      for (auto cell_it = row_it->begin(); cell_it != row_it->end(); ++cell_it) {
        hash = (hash << 1) ^ *cell_it;
      }
    }
    hash = ((hash << 5) - hash) ^ s.target_coords[0];
    hash = ((hash << 5) - hash) ^ s.target_coords[1];

    return hash;
  }
};

/**
 * @brief Generates random parking status
 *
 * @param x_size
 * @param y_size
 * @param num_occupied Number of occupied cells
 * @return Status
 */
Status generate(int x_size, int y_size, int num_occupied);

} // namespace kwi::status