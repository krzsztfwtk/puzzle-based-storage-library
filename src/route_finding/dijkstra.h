#pragma once

#include "../status/status.h"
#include "../types.h"
#include <functional>
#include <vector>

namespace kwi::route_finding {

/**
 * @brief Dijkstra's algorithm to find the shortest path
 * @param start_status The initial status to start the search from
 * @param isFinal Function to check if a status is final
 * @param get_neighbors Function to generate neighbors
 * @return Sequence of moves
 */
std::pair<std::vector<Move>, unsigned int>
dijkstra(const status::Status &start_status, const std::function<bool(const status::Status &)> &isFinal,
         const std::function<std::vector<std::tuple<status::Status, Move, unsigned int>>(const status::Status &)>
             &getNeighbors);
} // namespace kwi::route_finding