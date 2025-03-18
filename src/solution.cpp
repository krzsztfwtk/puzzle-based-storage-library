#include "moves.h"
#include <vector>
#include <tuple>

#include "solution.h"

using namespace std;

namespace kwi::solution {

// Helper to apply a move and return the new status
kwi::status::Status applyMove(const kwi::status::Status& currentStatus, uint old_x, uint old_y, uint new_x, uint new_y) {
    kwi::status::Status newStatus = currentStatus;
    
    // Update the grid and target coordinates
    newStatus.are_occupied_grid[old_x][old_y] = false; // Leave the old position
    newStatus.are_occupied_grid[new_x][new_y] = true;  // Occupy the new position

    // Update target coordinates if the move involves the target
    if (old_x == currentStatus.target_coords[0] && old_y == currentStatus.target_coords[1]) {
        newStatus.target_coords[0] = new_x;
        newStatus.target_coords[1] = new_y;
    }

    return newStatus;
}

vector<tuple<kwi::status::Status, vector<array<int, 2>>, vector<array<int, 2>>>> 
decode(const vector<kwi::status::Status> &solution) {
    vector<tuple<kwi::status::Status, vector<array<int, 2>>, vector<array<int, 2>>>> steps_with_moves;

    int x_size = solution[0].are_occupied_grid.size();
    int y_size = solution[0].are_occupied_grid[0].size();

    for (size_t i = 1; i < solution.size(); ++i) {
        const auto &prev = solution[i - 1];
        const auto &curr = solution[i];

        vector<array<int, 2>> moved_from;
        vector<array<int, 2>> moved_to;

        // Compare the grids to find all moved-from and moved-to cells
        for (int x = 0; x < x_size; ++x) {
            for (int y = 0; y < y_size; ++y) {
                if (prev.are_occupied_grid[x][y] && !curr.are_occupied_grid[x][y]) {
                    // Add the moved-from cell
                    moved_from.push_back({x, y});
                } else if (!prev.are_occupied_grid[x][y] && curr.are_occupied_grid[x][y]) {
                    // Add the moved-to cell
                    moved_to.push_back({x, y});
                }
            }
        }

        // Add the step to the list
        steps_with_moves.emplace_back(curr, moved_from, moved_to);
    }

    return steps_with_moves;
}

// Function to encode the solution path given the initial status and encoded moves
std::vector<kwi::status::Status> encode(const kwi::status::Status& start, const std::vector<uint>& moves) {
    std::vector<kwi::status::Status> solution;
    solution.push_back(start); // Include the start status

    kwi::status::Status currentStatus = start;
    for (const auto& move : moves) {
        
        std::array<std::array<uint, 2>, 2> decoded_moves = kwi::moves::decode(move);

        // Decode the move (assuming move encoding is old_x, old_y, new_x, new_y)
        uint old_x = decoded_moves[0][0];
        uint old_y = decoded_moves[0][1];
        uint new_x = decoded_moves[1][0];
        uint new_y = decoded_moves[1][1];
        
        currentStatus = applyMove(currentStatus, old_x, old_y, new_x, new_y);
        solution.push_back(currentStatus); // Store the new state
    }

    return solution;
}
}