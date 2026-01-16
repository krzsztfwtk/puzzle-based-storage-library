#pragma once

#include "../status/status.h"
#include "../types.h"
#include <tuple>
#include <vector>

namespace kwi::solution {

/**
 * @brief Converts history of states into sequence of moves
 * @param solution Vector of Status objects representing the solution path
 * @return Vector of moves
 */
std::vector<Move> statesToMoves(const std::vector<kwi::status::Status> &solution);

/**
 * @brief Converts sequence of moves into to history of states
 * @param start Initial status
 * @param moves Vector of moves to execute
 * @return Vector of statuses representing the solution path
 */
std::vector<kwi::status::Status> movesToStates(const kwi::status::Status &start, const std::vector<Move> &moves);

} // namespace kwi::solution