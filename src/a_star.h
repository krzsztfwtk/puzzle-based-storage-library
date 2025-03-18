#pragma once

#include <vector>
#include <array>
#include <functional>
#include <unordered_map>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <condition_variable>

#include "status.h"
#include "heuristic.h"

namespace kwi::route_finding {

// A* algorithm to find the path
// Allow lambda functions or std::bind as args
std::pair<std::vector<kwi::status::Status>, uint> aStar(
    const kwi::status::Status &start_status,
    const std::function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const std::function<std::vector<std::pair<kwi::status::Status, uint>>
    (const kwi::status::Status &)> &get_neighbors, // Function to generate neighbors
    const std::function<uint(const kwi::status::Status &)> &heuristic // heuristic function to estimate cost
);

// A* algorithm to moves to final status
std::pair<std::vector<uint>, uint> aStar(
    const kwi::status::Status &start_status,
    const std::function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const std::function<std::vector<std::tuple<kwi::status::Status, uint, uint>>
        (const kwi::status::Status &)> &get_neighbors, // Function to generate neighbors, which returns moves to execute
    const std::function<uint(const kwi::status::Status &)> &heuristic // heuristic function to predict future cost
);

// Multithreaded with sync, barriers
std::pair<std::vector<uint>, uint> _aStarMultithreaded(
    const kwi::status::Status &start_status,
    const std::function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const std::function<std::vector<std::tuple<kwi::status::Status, uint, uint>>
        (const kwi::status::Status &)> &get_neighbors, // Function to generate neighbors, which returns moves to execute
    const std::function<uint(const kwi::status::Status &)> &heuristic, // heuristic function to predict future cost
    uint thread_count = std::thread::hardware_concurrency()
);

std::pair<std::vector<uint>, uint> _aStarMultithreaded2(
    const kwi::status::Status &start_status,
    const std::function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const std::function<std::vector<std::tuple<kwi::status::Status, uint, uint>>
        (const kwi::status::Status &, const std::array<int, 2>&)> &get_neighbors,
    const std::function<uint(const kwi::status::Status &)> &heuristic, // heuristic function to predict future cost
    uint thread_count = std::thread::hardware_concurrency()
);

std::pair<std::vector<uint>, uint> _aStarMultithreaded3(
    const kwi::status::Status &start_status,
    const std::function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const std::function<std::vector<std::tuple<kwi::status::Status, uint, uint>>
        (const kwi::status::Status &, const std::array<int, 2>&)> &get_neighbors,
    const std::function<uint(const kwi::status::Status &)> &heuristic // heuristic function to predict future cost
);

}