#include <numeric>
#include <algorithm>


#include "heuristic.h"

using namespace std;

namespace kwi::heuristic {

uint manhattanDistance(const kwi::status::Status &current_status, const std::vector<std::array<uint, 2>> &destinations) {
    uint min_distance = std::numeric_limits<uint>::max();

    for (const auto &destination : destinations) {
        int dx = abs((int)current_status.target_coords[0] - (int)destination[0]);
        int dy = abs((int)current_status.target_coords[1] - (int)destination[1]);
        uint distance = (dx + dy);

        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    return min_distance * 124;
}

uint weightedManhattanDistance(const kwi::status::Status &current_status, 
    const std::vector<std::array<uint, 2>> &destinations) {
    uint min_distance = std::numeric_limits<uint>::max();

    for (const auto &destination : destinations) {
        uint dx = (uint)abs((int)current_status.target_coords[0] - (int)destination[0]);
        uint dy = (uint)abs((int)current_status.target_coords[1] - (int)destination[1]);

        uint distance = (dx + dy) * 124;

        // Dodaj karę za zajęte komórki w siatce na drodze
        for (uint x = current_status.target_coords[0]; x != destination[0]; x += (destination[0] > x ? 1 : -1)) {
            if (current_status.are_occupied_grid[x][current_status.target_coords[1]]) {
                distance += 64; // Dostosuj wartość kary w razie potrzeby
            }
        }
        for (uint y = current_status.target_coords[1]; y != destination[1]; y += (destination[1] > y ? 1 : -1)) {
            if (current_status.are_occupied_grid[current_status.target_coords[0]][y]) {
                distance += 64; // Dostosuj wartość kary w razie potrzeby
            }
        }

        // Aktualizuj minimalny dystans
        if (distance < min_distance) {
            min_distance = distance;
        }
    }

    return min_distance;
}

}