#pragma once

#include "status.h"

namespace kwi::heuristic {

inline uint manhattanDistance(const kwi::status::Status &current_status, 
    const std::array<uint, 2> &destination) {
    int dx = abs((int)current_status.target_coords[0] - (int)destination[0]);
    int dy = abs((int)current_status.target_coords[1] - (int)destination[1]);
    return (dx + dy) * 124;
}

inline uint chebyshevDistance(const kwi::status::Status &current_status, 
    const std::array<uint, 2> &destination) {
    int dx = abs((int)current_status.target_coords[0] - (int)destination[0]);
    int dy = abs((int)current_status.target_coords[1] - (int)destination[1]);
    return std::max(dx, dy) * 124; // Scaling factor of 100, similar to Manhattan
}

inline uint weightedManhattanDistance(const kwi::status::Status &current_status, 
    const std::array<uint, 2> &destination) {
    uint dx = (uint)abs((int)current_status.target_coords[0] - (int)destination[0]);
    uint dy = (uint)abs((int)current_status.target_coords[1] - (int)destination[1]);

    uint distance = (dx + dy) * 124;

    // Add a penalty for occupied cells in the grid along the path
    for (uint x = current_status.target_coords[0]; x != destination[0]; x += (destination[0] > x ? 1 : -1)) {
        if (current_status.are_occupied_grid[x][current_status.target_coords[1]]) {
            distance += 64; // Adjust penalty value as needed
        }
    }
    for (uint y = current_status.target_coords[1]; y != destination[1]; y += (destination[1] > y ? 1 : -1)) {
        if (current_status.are_occupied_grid[current_status.target_coords[0]][y]) {
            distance += 64; // Adjust penalty value as needed
        }
    }
    return distance;
}

uint manhattanDistance(const kwi::status::Status &current_status, 
    const std::vector<std::array<uint, 2>> &destinations);
    
uint weightedManhattanDistance(const kwi::status::Status &current_status, 
    const std::vector<std::array<uint, 2>> &destinations);

}