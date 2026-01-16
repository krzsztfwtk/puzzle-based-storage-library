#include "solution.h"
#include "../types.h"
#include <vector>

namespace kwi::solution {

kwi::status::Status applyMove(const kwi::status::Status &currentStatus, Move move) {
  kwi::status::Status newStatus = currentStatus;

  // Update the grid and target coordinates
  newStatus.grid[move.from_x][move.from_y] = false; // Leave the old position
  newStatus.grid[move.to_x][move.to_y] = true;      // Occupy the new position

  // Update target coordinates if the move involves the target
  if (move.from_x == currentStatus.target_coords[0] && move.from_y == currentStatus.target_coords[1]) {
    newStatus.target_coords[0] = move.to_x;
    newStatus.target_coords[1] = move.to_y;
  }

  return newStatus;
}

std::vector<Move> statesToMoves(const std::vector<kwi::status::Status> &solution) {
  std::vector<Move> moves;

  if (solution.size() <= 1) {
    return moves;
  }

  int x_size = solution[0].grid.size();
  int y_size = solution[0].grid[0].size();

  for (size_t i = 1; i < solution.size(); ++i) {
    const auto &prev = solution[i - 1];
    const auto &curr = solution[i];

    bool found = false;
    // Compare the grids to find the moved-from and moved-to cells
    for (int x = 0; x < x_size && !found; ++x) {
      for (int y = 0; y < y_size && !found; ++y) {
        if (prev.grid[x][y] && !curr.grid[x][y]) {
          // Found the moved-from cell, now find the moved-to cell
          for (int nx = 0; nx < x_size && !found; ++nx) {
            for (int ny = 0; ny < y_size && !found; ++ny) {
              if (!prev.grid[nx][ny] && curr.grid[nx][ny]) {
                // Found the moved-to cell
                Move move;
                move.from_x = static_cast<uint8_t>(x);
                move.from_y = static_cast<uint8_t>(y);
                move.to_x = static_cast<uint8_t>(nx);
                move.to_y = static_cast<uint8_t>(ny);
                moves.push_back(move);
                found = true;
              }
            }
          }
        }
      }
    }
  }

  return moves;
}

std::vector<kwi::status::Status> movesToStates(const kwi::status::Status &start, const std::vector<Move> &moves) {
  std::vector<kwi::status::Status> solution;
  solution.push_back(start);

  kwi::status::Status currentStatus = start;
  for (const auto &move : moves) {
    currentStatus = applyMove(currentStatus, move);
    solution.push_back(currentStatus);
  }

  return solution;
}

} // namespace kwi::solution