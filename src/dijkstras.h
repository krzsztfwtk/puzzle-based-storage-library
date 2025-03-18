#pragma once

#include <vector>
#include <array>
#include <functional>
#include <unordered_map>
#include <queue>
#include "status.h"

namespace kwi::route_finding {

// Dijkstra's algorithm to find the path
// Allow lambda functions or std::bind as args
std::vector<kwi::status::Status> dijkstras(
    const kwi::status::Status &start_status,
    const std::function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const std::function<std::vector<std::pair<kwi::status::Status, uint>>
    (const kwi::status::Status &)> &get_neighbors // Function to generate neighbors
);

}