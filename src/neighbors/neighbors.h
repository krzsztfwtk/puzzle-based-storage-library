#pragma once

#include "../status/status.h"
#include "../types.h"
#include <array>
#include <tuple>
#include <vector>

namespace kwi::neighbors {

/**
 * @brief Generate all neighbors using orthogonal movement type
 *
 * @param s Parking status
 * @return Neighbor status, corresponding move, cost
 */
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>> orthogonal(const kwi::status::Status &s);

/**
 * @brief Generate all neighbors using octilinear movement type
 *
 * @param s Parking status
 * @return Neighbor status, corresponding move, cost
 */
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>> octilinear(const kwi::status::Status &s);

// Helper functions or non-standard scenarios
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>> orthogonalTargetOnly(const kwi::status::Status &s);
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>> octilinearTargetOnly(const kwi::status::Status &s);
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>>
orthogonalFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>>
octilinearFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);
std::vector<std::tuple<kwi::status::Status, Move, unsigned int>>
octilinearFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position);

} // namespace kwi::neighbors
