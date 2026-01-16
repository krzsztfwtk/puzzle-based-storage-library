#include <array>
#include <functional>
#include <tuple>
#include <vector>

#include "../types.h"
#include "neighbors.h"

constexpr uint sqrt2x100 = 141;

namespace kwi::neighbors {

const std::vector<std::array<int, 2>> orthogonal_directions = {{{1, 0}}, {{-1, 0}}, {{0, 1}}, {{0, -1}}};
const std::vector<std::array<int, 2>> diagonal_directions = {{{1, 1}}, {{1, -1}}, {{-1, 1}}, {{-1, -1}}};

// Helper function to generate moves for the target only

std::vector<std::tuple<kwi::status::Status, Move, uint>>
generateMovesForTarget(const kwi::status::Status &s, const std::vector<std::array<int, 2>> &directions, uint cost,
                       std::function<bool(const kwi::status::Status &, uint, uint)> isValidMove = nullptr) {

  std::vector<std::tuple<kwi::status::Status, Move, uint>> moves;
  const int x_size = s.grid.size();
  const int y_size = s.grid[0].size();
  const uint from_x = s.target_coords[0];
  const uint from_y = s.target_coords[1];

  for (const auto &dir : directions) {
    const int dx = dir[0];
    const int dy = dir[1];

    int new_x = from_x + dx;
    int new_y = from_y + dy;

    // Check if the move is within bounds
    if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size && !s.grid[new_x][new_y]) {

      // Check if the move is valid
      if (isValidMove && !isValidMove(s, new_x, new_y)) {
        continue;
      }

      // Update status
      kwi::status::Status new_status = s;
      new_status.grid[from_x][from_y] = false;
      new_status.grid[new_x][new_y] = true;
      new_status.target_coords[0] = new_x;
      new_status.target_coords[1] = new_y;

      Move move = {(uint8_t)from_x, (uint8_t)from_y, (uint8_t)new_x, (uint8_t)new_y};
      moves.emplace_back(new_status, move, cost);
    }
  }

  return moves;
}

std::vector<std::tuple<kwi::status::Status, Move, uint>> orthogonalTargetOnly(const kwi::status::Status &s) {
  return generateMovesForTarget(s, orthogonal_directions, 100);
}

std::vector<std::tuple<kwi::status::Status, Move, uint>> octilinearTargetOnly(const kwi::status::Status &s) {
  std::vector<std::tuple<kwi::status::Status, Move, uint>> moves = orthogonalTargetOnly(s);

  auto diagonal_moves =
      generateMovesForTarget(s, diagonal_directions, sqrt2x100, [](const kwi::status::Status &s, int new_x, int new_y) {
        // Ensure intermediate cells are free for diagonal moves
        const int from_x = s.target_coords[0];
        const int from_y = s.target_coords[1];
        return !s.grid[new_x][from_y] && !s.grid[from_x][new_y];
      });

  moves.insert(moves.end(), diagonal_moves.begin(), diagonal_moves.end());
  return moves;
}

// Helper function to generate moves from a given position
std::vector<std::tuple<kwi::status::Status, Move, uint>>
generateMovesFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position,
                          const std::vector<std::array<int, 2>> &directions, uint cost,
                          std::function<bool(const kwi::status::Status &, int, int)> isValidMove = nullptr) {
  std::vector<std::tuple<kwi::status::Status, Move, uint>> moves;
  const int x_size = s.grid.size();
  const int y_size = s.grid[0].size();
  const int from_x = position[0];
  const int from_y = position[1];

  for (const auto &dir : directions) {
    const int dx = dir[0];
    const int dy = dir[1];

    int new_x = from_x + dx;
    int new_y = from_y + dy;

    if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size && !s.grid[new_x][new_y]) {
      if (isValidMove && !isValidMove(s, new_x, new_y)) {
        continue;
      }

      kwi::status::Status new_status = s;
      new_status.grid[from_x][from_y] = false;
      new_status.grid[new_x][new_y] = true;

      Move move = {(uint8_t)from_x, (uint8_t)from_y, (uint8_t)new_x, (uint8_t)new_y};
      moves.emplace_back(new_status, move, cost);
    }
  }

  return moves;
}

std::vector<std::tuple<kwi::status::Status, Move, uint>> orthogonalFromPosition(const kwi::status::Status &s,
                                                                                const std::array<int, 2> &position) {

  if (position[0] == static_cast<int>(s.target_coords[0]) && position[1] == static_cast<int>(s.target_coords[1]))
    return orthogonalTargetOnly(s);

  return generateMovesFromPosition(s, position, orthogonal_directions, 100);
}

std::vector<std::tuple<kwi::status::Status, Move, uint>> octilinearFromPosition(const kwi::status::Status &s,
                                                                                const std::array<int, 2> &position) {

  if (position[0] == static_cast<int>(s.target_coords[0]) && position[1] == static_cast<int>(s.target_coords[1])) {
    return octilinearTargetOnly(s);
  }

  std::vector<std::tuple<kwi::status::Status, Move, uint>> moves = orthogonalFromPosition(s, position);

  auto diagonal_moves = generateMovesFromPosition(s, position, diagonal_directions, sqrt2x100,
                                                  [position](const kwi::status::Status &s, int new_x, int new_y) {
                                                    const int from_x = position[0];
                                                    const int from_y = position[1];
                                                    // Ensure intermediate cells are free for diagonal moves
                                                    return !s.grid[new_x][from_y] && !s.grid[from_x][new_y];
                                                  });

  moves.insert(moves.end(), diagonal_moves.begin(), diagonal_moves.end());
  return moves;
}

// Helper function to generate moves from all occupied positions
template <typename NeighborFunction>
std::vector<std::tuple<kwi::status::Status, Move, uint>>
generateMovesFromAllPositions(const kwi::status::Status &s, NeighborFunction neighbor_function) {
  std::vector<std::tuple<kwi::status::Status, Move, uint>> all_moves;

  const int x_size = s.grid.size();
  const int y_size = s.grid[0].size();

  for (int x = 0; x < x_size; ++x) {
    for (int y = 0; y < y_size; ++y) {
      if (s.grid[x][y]) {
        auto moves = neighbor_function(s, {x, y});
        all_moves.insert(all_moves.end(), moves.begin(), moves.end());
      }
    }
  }

  return all_moves;
}

std::vector<std::tuple<kwi::status::Status, Move, uint>> orthogonal(const kwi::status::Status &s) {
  return generateMovesFromAllPositions(s, orthogonalFromPosition);
}

std::vector<std::tuple<kwi::status::Status, Move, uint>> octilinear(const kwi::status::Status &s) {
  return generateMovesFromAllPositions(s, octilinearFromPosition);
}

} // namespace kwi::neighbors
