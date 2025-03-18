#include <cmath>
#include "neighbors.h"

using namespace std;

const uint sqrt2x100 = (uint)(sqrt(2.0) * 100);

namespace kwi::neighbors::status_cost {

// Generate neighboring statuses for axis-aligned motion
// allow only the target to move
vector<pair<kwi::status::Status, uint>> axisTargetOnly(const kwi::status::Status &s) {
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs;

    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    vector<array<int, 2>> directions = {{{1, 0}}, {{-1, 0}}, {{0, 1}}, {{0, -1}}};  // right, left, up, down

    for (auto it = directions.begin(); it != directions.end(); ++it) {
        int dx = (*it)[0];
        int dy = (*it)[1];

        int new_x = s.target_coords[0] + dx;
        int new_y = s.target_coords[1] + dy;

        // Check if the new position is within the grid and not an obstacle
        if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size && !s.are_occupied_grid[new_x][new_y]) {
            kwi::status::Status new_status = s;
            new_status.are_occupied_grid[s.target_coords[0]][s.target_coords[1]] = false;
            new_status.are_occupied_grid[new_x][new_y] = true;
            new_status.target_coords[0] = new_x;
            new_status.target_coords[1] = new_y;
            neighbors_with_costs.push_back({new_status, 100});
        }
    }

    return neighbors_with_costs;
}

// Generate neighboring statuses for planar motion with axis-aligned and diagonal directions
// allow only the target to move
vector<pair<kwi::status::Status, uint>> planarTargetOnly(const kwi::status::Status &s) {
    // Handle axis-aligned moves
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs = axisTargetOnly(s);

    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    vector<array<int, 2>> diagonal_directions = {
        {{1, 1}},  // right-up
        {{1, -1}}, // right-down
        {{-1, 1}}, // left-up
        {{-1, -1}} // left-down
    };

    // Handle diagonal moves
    for (const auto& dir : diagonal_directions) {
        int dx = dir[0];
        int dy = dir[1];

        int new_x = s.target_coords[0] + dx;
        int new_y = s.target_coords[1] + dy;

        // Check if the new position is within the grid and all three positions are free
        if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size &&
            !s.are_occupied_grid[new_x][new_y] &&                // Target position
            !s.are_occupied_grid[new_x][s.target_coords[1]] && // Intermediate x
            !s.are_occupied_grid[s.target_coords[0]][new_y]) { // Intermediate y
            
            kwi::status::Status new_status = s;
            new_status.are_occupied_grid[s.target_coords[0]][s.target_coords[1]] = false;
            new_status.are_occupied_grid[new_x][new_y] = true;
            new_status.target_coords[0] = new_x;
            new_status.target_coords[1] = new_y;
            neighbors_with_costs.push_back({new_status, sqrt2x100});  // Diagonal
        }
    }

    return neighbors_with_costs;
}

// Generate neighboring statuses for axis-aligned motion
// allow only the given cell to move
vector<pair<kwi::status::Status, uint>> axisFromPosition(const kwi::status::Status &s, const array<int, 2> &position) {    
    int x = position[0];
    int y = position[1];

    // if the position is the target
    if (x == (int)s.target_coords[0] && y == (int)s.target_coords[1]) [[unlikely]] {
        return axisTargetOnly(s);
    }
    
    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    vector<pair<kwi::status::Status, uint>> neighbors_with_costs;

    vector<array<int, 2>> directions = {
        {{1, 0}},  // right
        {{-1, 0}}, // left
        {{0, 1}},  // down
        {{0, -1}}  // up
    };

    // Try moving the given cell in all axis-aligned directions
    for (const auto &dir : directions) {
        int dx = dir[0];
        int dy = dir[1];

        int new_x = x + dx;
        int new_y = y + dy;

        // Check if the new position is within the grid and not occupied
        if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size && !s.are_occupied_grid[new_x][new_y]) {
            kwi::status::Status new_status = s;

            // Move the cell
            new_status.are_occupied_grid[x][y] = false;
            new_status.are_occupied_grid[new_x][new_y] = true;

            // Add the new status and its cost (100 for axis-aligned move)
            neighbors_with_costs.push_back({new_status, 100});
        }
    }

    return neighbors_with_costs;
}

// Generate neighboring statuses for planar motion with axis-aligned and diagonal directions
// allow only the given cell to move
vector<pair<kwi::status::Status, uint>> planarFromPosition(const kwi::status::Status &s, const array<int, 2> &position) {    
    int x = position[0];
    int y = position[1];

    // if the position is the target
    if (x == (int)s.target_coords[0] && y == (int)s.target_coords[1]) [[unlikely]] {
        return planarTargetOnly(s);
    }
    
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs = axisFromPosition(s, position);
    
    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    vector<array<int, 2>> diagonal_directions = {
        {{1, 1}},  // right-up
        {{1, -1}}, // right-down
        {{-1, 1}}, // left-up
        {{-1, -1}} // left-down
    };

    // Handle diagonal moves
    for (const auto& dir : diagonal_directions) {
        int dx = dir[0];
        int dy = dir[1];

        int new_x = x + dx;
        int new_y = y + dy;

        // Check if the new position is within the grid and all three positions are free
        if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size &&
            !s.are_occupied_grid[new_x][new_y] && // New position
            !s.are_occupied_grid[new_x][y] && // Intermediate x
            !s.are_occupied_grid[x][new_y]) { // Intermediate y
            
            kwi::status::Status new_status = s;
            // Move the cell
            new_status.are_occupied_grid[x][y] = false;
            new_status.are_occupied_grid[new_x][new_y] = true;

            neighbors_with_costs.push_back({new_status, sqrt2x100});  // Diagonal
        }
    }

    return neighbors_with_costs;
}


// Generate neighboring statuses for axis-aligned motion
// allow to move from all points
vector<pair<kwi::status::Status, uint>> axis(const kwi::status::Status &s) {
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs;

    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    for (int x = 0; x < x_size; ++x) {
        for (int y = 0; y < y_size; ++y) {
            if (s.are_occupied_grid[x][y]) {
                // Get neighbors from the current position
                vector<pair<kwi::status::Status, uint>> neighbors_from_position = 
                axisFromPosition(s, {x, y});
                
                // Append to the main vector
                neighbors_with_costs.insert(
                    neighbors_with_costs.end(), 
                    neighbors_from_position.begin(), 
                    neighbors_from_position.end()
                );
            } 
        }
    }

    return neighbors_with_costs;
}

// Generate neighboring statuses for planar motion with axis-aligned and diagonal directions
// allow to move from all points
vector<pair<kwi::status::Status, uint>> planar(const kwi::status::Status &s) {
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs;

    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    for (int x = 0; x < x_size; ++x) {
        for (int y = 0; y < y_size; ++y) {
            if (s.are_occupied_grid[x][y]) {
                // Get neighbors from the current position
                vector<pair<kwi::status::Status, uint>> neighbors_from_position = 
                planarFromPosition(s, {x, y});
                
                // Append to the main vector
                neighbors_with_costs.insert(
                    neighbors_with_costs.end(), 
                    neighbors_from_position.begin(), 
                    neighbors_from_position.end()
                );
            } 
        }
    }

    return neighbors_with_costs;
}

// Generate neighboring statuses for coverage scenario axis aligned motion
vector<pair<kwi::status::Status, uint>> axisCoverage(const kwi::status::Status &s) {
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs;

    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    vector<array<int, 2>> directions = {{{1, 0}}, {{-1, 0}}, {{0, 1}}, {{0, -1}}};  // right, left, up, down

    for (auto it = directions.begin(); it != directions.end(); ++it) {
        int dx = (*it)[0];
        int dy = (*it)[1];

        int new_x = s.target_coords[0] + dx;
        int new_y = s.target_coords[1] + dy;

        // Check if the new position is within the grid and not an obstacle
        if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size) {
            kwi::status::Status new_status = s;
            new_status.are_occupied_grid[new_x][new_y] = true;
            new_status.target_coords[0] = new_x;
            new_status.target_coords[1] = new_y;
            neighbors_with_costs.push_back({new_status, 100});
        }
    }

    return neighbors_with_costs;
}

// Generate neighboring statuses for single agent area coverage planar motion
vector<pair<kwi::status::Status, uint>> planarCoverage(const kwi::status::Status &s) {
    vector<pair<kwi::status::Status, uint>> neighbors_with_costs = axisCoverage(s);

    int x_size = s.are_occupied_grid.size();
    int y_size = s.are_occupied_grid[0].size();

    vector<array<int, 2>> diagonal_directions = {
        {{1, 1}},  // right-up
        {{1, -1}}, // right-down
        {{-1, 1}}, // left-up
        {{-1, -1}} // left-down
    };

    for (auto it = diagonal_directions.begin(); it != diagonal_directions.end(); ++it) {
        int dx = (*it)[0];
        int dy = (*it)[1];

        int new_x = s.target_coords[0] + dx;
        int new_y = s.target_coords[1] + dy;

        // Check if the new position is within the grid and not an obstacle
        if (new_x >= 0 && new_x < x_size && new_y >= 0 && new_y < y_size) {
            kwi::status::Status new_status = s;
            new_status.are_occupied_grid[new_x][new_y] = true;
            new_status.target_coords[0] = new_x;
            new_status.target_coords[1] = new_y;
            neighbors_with_costs.push_back({new_status, sqrt2x100});
        }
    }

    return neighbors_with_costs;
}

}