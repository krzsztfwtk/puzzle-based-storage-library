#pragma once

#include <array>
#include <cstdint>
#include <functional>

typedef unsigned int uint;

namespace kwi::status {

struct Status {
    std::vector<std::vector<bool>> are_occupied_grid;
    uint target_coords[2];

    bool operator==(const Status &other) const {
        return are_occupied_grid == other.are_occupied_grid &&
               target_coords[0] == other.target_coords[0] && 
               target_coords[1] == other.target_coords[1];
    }
};


struct Hash {
    size_t operator()(const Status &s) const {
        size_t hash = 0;

        for (auto row_it = s.are_occupied_grid.begin(); row_it != s.are_occupied_grid.end(); ++row_it) {
            for (auto cell_it = row_it->begin(); cell_it != row_it->end(); ++cell_it) {
                hash = (hash << 1) ^ *cell_it; // Left shift hash by 1 bit and XOR with cell value
            }
        }

        // 31 is a prime number, which helps reduce the likelihood of collisions. 
        // Prime numbers are generally good choices in hash functions to distribute the values more uniformly.

        // Multiplying by 31 can be optimized because it is equivalent to (hash << 5) - hash

        // Incorporate car coordinates into the hash using bitwise operations
        hash = ((hash << 5) - hash) ^ s.target_coords[0]; // Hash the x-coordinate
        hash = ((hash << 5) - hash) ^ s.target_coords[1]; // Hash the y-coordinate

        return hash;
    }
};

Status generate(int x_size, int y_size, int num_trues);

}