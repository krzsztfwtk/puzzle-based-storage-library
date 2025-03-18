#include <cmath>
#include <vector>
#include <tuple>
#include <array>
#include <functional>

#include "neighbors.h"
#include "moves.h"


const uint sqrt2x100 = static_cast<uint>(sqrt(2.0) * 100);

namespace kwi::neighbors::status_move_cost {

    // Helper function to generate moves for the target only
    template <typename DirectionContainer>
    std::vector<std::tuple<kwi::status::Status, uint, uint>> generateMovesForTarget(
        const kwi::status::Status &s,
        const DirectionContainer &directions,
        uint cost,
        std::function<bool(const kwi::status::Status &, int, int)> isValidMove = nullptr
    ) {
        std::vector<std::tuple<kwi::status::Status, uint, uint>> moves;
        int x_size = s.are_occupied_grid.size();
        int y_size = s.are_occupied_grid[0].size();

        uint old_x = s.target_coords[0];
        uint old_y = s.target_coords[1];

        for (const auto &dir : directions) {
            int dx = dir[0];
            int dy = dir[1];

            int new_x = old_x + dx;
            int new_y = old_y + dy;

            // Check if the move is within bounds
            if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size &&
                !s.are_occupied_grid[new_x][new_y]) {

                // Optionally check if the move is valid
                if (isValidMove && !isValidMove(s, new_x, new_y)) {
                    continue;
                }

                // Update status and encode move
                kwi::status::Status new_status = s;
                new_status.are_occupied_grid[old_x][old_y] = false;
                new_status.are_occupied_grid[new_x][new_y] = true;
                new_status.target_coords[0] = new_x;
                new_status.target_coords[1] = new_y;

                uint move = kwi::moves::encode(old_x, old_y, static_cast<uint>(new_x), static_cast<uint>(new_y));
                moves.emplace_back(new_status, move, cost);
            }
        }

        return moves;
    }

    // Axis-aligned moves for target only
    std::vector<std::tuple<kwi::status::Status, uint, uint>> axisTargetOnly(const kwi::status::Status &s) {
        std::vector<std::array<int, 2>> directions = { { {1, 0} }, { {-1, 0} }, { {0, 1} }, { {0, -1} } };
        return generateMovesForTarget(
            s,
            directions,
            100
        );
    }

    // Planar moves (axis-aligned + diagonal) for target only
    std::vector<std::tuple<kwi::status::Status, uint, uint>> planarTargetOnly(const kwi::status::Status &s) {
        std::vector<std::tuple<kwi::status::Status, uint, uint>> moves = axisTargetOnly(s);

        std::vector<std::array<int, 2>> diagonal_directions = { { {1, 1} }, { {1, -1} }, { {-1, 1} }, { {-1, -1} } };

        auto diagonal_moves = generateMovesForTarget(
            s,
            diagonal_directions,
            sqrt2x100,
            [](const kwi::status::Status &s, int new_x, int new_y) {
                // Ensure intermediate cells are free for diagonal moves
                int old_x = s.target_coords[0];
                int old_y = s.target_coords[1];
                return !s.are_occupied_grid[new_x][old_y] && !s.are_occupied_grid[old_x][new_y];
            }
        );

        moves.insert(moves.end(), diagonal_moves.begin(), diagonal_moves.end());
        return moves;
    }

    // Helper function to generate moves from a given position
    template <typename DirectionContainer>
    std::vector<std::tuple<kwi::status::Status, uint, uint>> generateMovesFromPosition(
        const kwi::status::Status &s,
        const std::array<int, 2> &position,
        const DirectionContainer &directions,
        uint cost,
        std::function<bool(const kwi::status::Status &, int, int)> isValidMove = nullptr
    ) {
        std::vector<std::tuple<kwi::status::Status, uint, uint>> moves;
        int x_size = s.are_occupied_grid.size();
        int y_size = s.are_occupied_grid[0].size();

        int x = position[0];
        int y = position[1];

        for (const auto &dir : directions) {
            int dx = dir[0];
            int dy = dir[1];

            int new_x = x + dx;
            int new_y = y + dy;

            // Check if the move is within bounds and the new position is not occupied
            if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size &&
                !s.are_occupied_grid[new_x][new_y]) {

                // Optionally check if the move is valid
                if (isValidMove && !isValidMove(s, new_x, new_y)) {
                    continue;
                }

                // Update status and encode move
                kwi::status::Status new_status = s;
                new_status.are_occupied_grid[x][y] = false;
                new_status.are_occupied_grid[new_x][new_y] = true;

                uint move = kwi::moves::encode(x, y, new_x, new_y);
                moves.emplace_back(new_status, move, cost);
            }
        }

        return moves;
    }

    // Axis-aligned moves from a given position
    std::vector<std::tuple<kwi::status::Status, uint, uint>> axisFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position) {
        // If position is target, use axisTargetOnly
        if (position[0] == static_cast<int>(s.target_coords[0]) && position[1] == static_cast<int>(s.target_coords[1])) [[ unlikely ]] {
            return axisTargetOnly(s);
        }

        std::vector<std::array<int, 2>> directions = { { {1, 0} }, { {-1, 0} }, { {0, 1} }, { {0, -1} } };

        return generateMovesFromPosition(
            s,
            position,
            directions,
            100
        );
    }

    // Planar moves from a given position
    std::vector<std::tuple<kwi::status::Status, uint, uint>> planarFromPosition(const kwi::status::Status &s, const std::array<int, 2> &position) {
        // If position is target, use planarTargetOnly
        if (position[0] == static_cast<int>(s.target_coords[0]) && position[1] == static_cast<int>(s.target_coords[1])) [[ unlikely ]] {
            return planarTargetOnly(s);
        }

        std::vector<std::tuple<kwi::status::Status, uint, uint>> moves = axisFromPosition(s, position);

        std::vector<std::array<int, 2>> diagonal_directions = { { {1, 1} }, { {1, -1} }, { {-1, 1} }, { {-1, -1} } };

        auto diagonal_moves = generateMovesFromPosition(
            s,
            position,
            diagonal_directions,
            sqrt2x100,
            [position](const kwi::status::Status &s, int new_x, int new_y) {
                int x = position[0];
                int y = position[1];
                // Ensure intermediate cells are free for diagonal moves
                return !s.are_occupied_grid[new_x][y] && !s.are_occupied_grid[x][new_y];
            }
        );

        moves.insert(moves.end(), diagonal_moves.begin(), diagonal_moves.end());
        return moves;
    }

    // Helper function to generate moves from all occupied positions
    template <typename NeighborFunction>
    std::vector<std::tuple<kwi::status::Status, uint, uint>> generateMovesFromAllPositions(
        const kwi::status::Status &s,
        NeighborFunction neighbor_function
    ) {
        std::vector<std::tuple<kwi::status::Status, uint, uint>> all_moves;

        int x_size = s.are_occupied_grid.size();
        int y_size = s.are_occupied_grid[0].size();

        for (int x = 0; x < x_size; ++x) {
            for (int y = 0; y < y_size; ++y) {
                if (s.are_occupied_grid[x][y]) {
                    auto moves = neighbor_function(s, { x, y });
                    all_moves.insert(all_moves.end(), moves.begin(), moves.end());
                }
            }
        }

        return all_moves;
    }

    // Axis-aligned moves from all positions
    std::vector<std::tuple<kwi::status::Status, uint, uint>> axis(const kwi::status::Status &s) {
        return generateMovesFromAllPositions(s, axisFromPosition);
    }

    // Planar moves from all positions
    std::vector<std::tuple<kwi::status::Status, uint, uint>> planar(const kwi::status::Status &s) {
        return generateMovesFromAllPositions(s, planarFromPosition);
    }

    // Axis-aligned coverage moves
    std::vector<std::tuple<kwi::status::Status, uint, uint>> axisCoverage(const kwi::status::Status &s) {
        std::vector<std::array<int, 2>> directions = { { {1, 0} }, { {-1, 0} }, { {0, 1} }, { {0, -1} } };

        // Similar to generateMovesForTarget but marks new positions as occupied
        auto generateCoverageMoves = [](const kwi::status::Status &s,
                                        const std::vector<std::array<int, 2>> &directions,
                                        uint cost) -> std::vector<std::tuple<kwi::status::Status, uint, uint>> {
            std::vector<std::tuple<kwi::status::Status, uint, uint>> moves;
            int x_size = s.are_occupied_grid.size();
            int y_size = s.are_occupied_grid[0].size();

            uint old_x = s.target_coords[0];
            uint old_y = s.target_coords[1];

            for (const auto &dir : directions) {
                int dx = dir[0];
                int dy = dir[1];

                int new_x = old_x + dx;
                int new_y = old_y + dy;

                if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size) {
                    kwi::status::Status new_status = s;
                    new_status.are_occupied_grid[new_x][new_y] = true;
                    new_status.target_coords[0] = new_x;
                    new_status.target_coords[1] = new_y;

                    uint move = kwi::moves::encode(old_x, old_y, static_cast<uint>(new_x), static_cast<uint>(new_y));
                    moves.emplace_back(new_status, move, cost);
                }
            }

            return moves;
        };

        return generateCoverageMoves(s, directions, 100);
    }

    // Planar coverage moves
    std::vector<std::tuple<kwi::status::Status, uint, uint>> planarCoverage(const kwi::status::Status &s) {
        std::vector<std::tuple<kwi::status::Status, uint, uint>> moves = axisCoverage(s);

        std::vector<std::array<int, 2>> diagonal_directions = { { {1, 1} }, { {1, -1} }, { {-1, 1} }, { {-1, -1} } };

        auto generateCoverageMoves = [](const kwi::status::Status &s,
                                        const std::vector<std::array<int, 2>> &directions,
                                        uint cost) -> std::vector<std::tuple<kwi::status::Status, uint, uint>> {
            std::vector<std::tuple<kwi::status::Status, uint, uint>> moves;
            int x_size = s.are_occupied_grid.size();
            int y_size = s.are_occupied_grid[0].size();

            uint old_x = s.target_coords[0];
            uint old_y = s.target_coords[1];

            for (const auto &dir : directions) {
                int dx = dir[0];
                int dy = dir[1];

                int new_x = old_x + dx;
                int new_y = old_y + dy;

                if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size) {
                    kwi::status::Status new_status = s;
                    new_status.are_occupied_grid[new_x][new_y] = true;
                    new_status.target_coords[0] = new_x;
                    new_status.target_coords[1] = new_y;

                    uint move = kwi::moves::encode(old_x, old_y, static_cast<uint>(new_x), static_cast<uint>(new_y));
                    moves.emplace_back(new_status, move, cost);
                }
            }

            return moves;
        };

        auto diagonal_moves = generateCoverageMoves(s, diagonal_directions, sqrt2x100);
        moves.insert(moves.end(), diagonal_moves.begin(), diagonal_moves.end());

        return moves;
    }

}
