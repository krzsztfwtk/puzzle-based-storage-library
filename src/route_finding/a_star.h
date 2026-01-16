#pragma once

#include <functional>
#include <vector>

#include "../heuristic/heuristic.h"
#include "../status/status.h"
#include "../types.h"

namespace kwi::route_finding {

/**
 * @brief A* algorithm to find the shortest path (sequence of moves)
 * @param start_status The initial status to start the search from
 * @param isFinal Function to check if a status is final
 * @param getNeighbors Function to generate neighbors with status, cost, and move ID
 * @param heuristic Heuristic function to estimate cost to goal
 * @return Pair containing the move sequence (vector of moves) and the total cost
 */
std::pair<std::vector<Move>, unsigned int>
aStar(const status::Status &start_status, const std::function<bool(const status::Status &)> &isFinal,
      const std::function<std::vector<std::tuple<status::Status, Move, unsigned int>>(const status::Status &)>
          &getNeighbors,
      const std::function<unsigned int(const status::Status &)> &heuristic);

} // namespace kwi::route_finding