#include <iostream>
#include <barrier>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <algorithm>
#include <execution>

#include "a_star.h"

using namespace std;

namespace kwi::route_finding {

// A* algorithm to find the path
pair<vector<kwi::status::Status>, uint> aStar(
    const kwi::status::Status &start_status,
    const function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const function<vector<pair<kwi::status::Status, uint>>(const kwi::status::Status &)> &get_neighbors, // Function to generate neighbors
    const function<uint(const kwi::status::Status &)> &heuristic // heuristic function to predict future cost
) {
    // A* status to find the path
    struct AStarStatus {
        kwi::status::Status status;
        std::vector<kwi::status::Status> history;
        uint g_cost; // Cost from start to current node (g(n))
        uint f_cost; // Estimated total cost (f(n) = g(n) + h(n))

        // Comparator for priority_queue to create a min-heap based on f_cost
        bool operator<(const AStarStatus &other) const {
            return f_cost > other.f_cost;
        }
    };

    priority_queue<AStarStatus> open_list;
    unordered_map<kwi::status::Status, uint, kwi::status::Hash> closed_list;

    uint h_start = heuristic(start_status);
    open_list.push({start_status, {start_status}, 0, h_start});


    while (!open_list.empty()) {
        // Get the status with the lowest cost from the priority queue
        AStarStatus current = open_list.top();
        open_list.pop();

        kwi::status::Status current_status = current.status;
        vector<kwi::status::Status> current_history = current.history;
        uint current_g_cost = current.g_cost;

        // Check if the current status is the final status
        if (isFinal(current_status)) [[unlikely]] {
            return {current_history, current_g_cost};
        }

        // Check for a better path to the current status
        if (closed_list.find(current_status) != closed_list.end() && 
        closed_list[current_status] <= current_g_cost) {
            continue;
        }

        // Record the best g_cost for the current status
        closed_list[current_status] = current_g_cost;

        // Generate all valid neighboring statuses
        vector<pair<kwi::status::Status, uint>> neighbors_costs = get_neighbors(current_status);

        for (auto neighbour_cost : neighbors_costs) {
            kwi::status::Status neighbor = neighbour_cost.first;
            uint move_cost = neighbour_cost.second;
            uint new_g_cost = current_g_cost + move_cost;

            // Check for a better path to the neighbor
            if (closed_list.find(neighbor) != closed_list.end() &&
                closed_list[neighbor] <= new_g_cost) {
                continue;
            }

            // Compute heuristic for neighbor
            uint h_cost = heuristic(neighbor);
            uint f_cost = new_g_cost + h_cost;

            vector<kwi::status::Status> new_history = current_history; // bottleneck
            new_history.push_back(neighbor);

            // Add neighbor to open list
            open_list.push({neighbor, new_history, new_g_cost, f_cost});
        }
    }
    return {}; // Return an empty vector if no solution is found
}

// A* algorithm to find the path
pair<vector<uint>, uint> aStar(
    const kwi::status::Status &start_status,
    const function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const function<vector<tuple<kwi::status::Status, uint, uint>>(const kwi::status::Status &)> &get_neighbors, // Function to generate neighbors, which returns moves to execute
    const function<uint(const kwi::status::Status &)> &heuristic // heuristic function to predict future cost
) {
    struct AStarStatus {
        kwi::status::Status status;
        vector<uint> history; // History of moves
        uint g_cost;          // Cost from start to current node
        uint f_cost;          // Estimated total cost (f(n) = g(n) + h(n))

        // Comparator for priority_queue to create a min-heap based on f_cost
        bool operator<(const AStarStatus &other) const {
            return f_cost > other.f_cost;
        }
    };

    priority_queue<AStarStatus> open_list;
    unordered_map<kwi::status::Status, uint, kwi::status::Hash> closed_list;

    uint h_start = heuristic(start_status);
    open_list.push({start_status, {}, 0, h_start}); // Start with an empty move history

    while (!open_list.empty()) {
        // Get the status with the lowest cost from the priority queue
        AStarStatus current = open_list.top();
        open_list.pop();

        kwi::status::Status current_status = current.status;
        vector<uint> current_history = current.history;
        uint current_g_cost = current.g_cost;

        // Check if the current status is the final status
        if (isFinal(current_status)) [[unlikely]] {
            return {current_history, current_g_cost};
        }

        // Check for a better path to the current status
        if (closed_list.find(current_status) != closed_list.end() && 
            // closed_list[current_status] <= current_g_cost) { // version 1
            closed_list[current_status] < current_g_cost) { // version 2
            continue; // Skip if a better path already exists
        }

        // Record the best g_cost for the current status
        // closed_list[current_status] = current_g_cost; // version 1

        // Generate all valid neighboring statuses
        vector<tuple<kwi::status::Status, uint, uint>> neighbors_moves_costs = get_neighbors(current_status);

        for (const auto &neighbor_move_cost : neighbors_moves_costs) {
            kwi::status::Status neighbor = std::get<0>(neighbor_move_cost);
            uint move = std::get<1>(neighbor_move_cost);
            uint move_cost = std::get<2>(neighbor_move_cost);
            uint new_g_cost = current_g_cost + move_cost;

            // Check for a better path to the neighbor
            if (closed_list.find(neighbor) != closed_list.end() &&
                closed_list[neighbor] <= new_g_cost) {
                continue;
            }

            closed_list[neighbor] = new_g_cost; // version 2

            // Compute heuristic for neighbor
            uint h_cost = heuristic(neighbor);
            uint f_cost = new_g_cost + h_cost;

            vector<uint> new_history = current_history; // bottleneck
            new_history.push_back(move); // Append the move

            // Add neighbor to open list
            open_list.push({neighbor, new_history, new_g_cost, f_cost});
        }
    }
    return {}; // Return an empty vector if no solution is found
}


} // namespace kwi::route_finding