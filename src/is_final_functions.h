#pragma once

#include <array>
#include "status.h"

namespace kwi::is_final {

// Is final status functions
inline bool isTargetOnDestination(const kwi::status::Status &s, std::array<uint, 2> &destination_coords) {
    return s.target_coords[0] == destination_coords[0] 
    && s.target_coords[1] == destination_coords[1];
}

// Check if the target is on one of the specified destinations
inline bool isTargetOnDestination(const kwi::status::Status &s, 
    const std::vector<std::array<uint, 2>> &destinations) {
    for (const auto &destination_coords : destinations) {
        if (s.target_coords[0] == destination_coords[0] && s.target_coords[1] == destination_coords[1]) {
            return true;
        }
    }
    return false;
}

bool isPattern(const kwi::status::Status &s, const std::vector<std::vector<bool>> &pattern);

bool isCovered(const kwi::status::Status &s);

}