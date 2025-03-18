#include "dijkstras.h"

using namespace std;

namespace kwi::route_finding {

// Dijkstra's algorithm to find the path
std::vector<kwi::status::Status> dijkstras(
    const kwi::status::Status &start_status,
    const function<bool(const kwi::status::Status &)> &isFinal, // Function to check if a status is final
    const function<vector<pair<kwi::status::Status, uint>>(const kwi::status::Status &)> &get_neighbors // Function to generate neighbors
) {
    // Dijkstra's status to find the path
    struct DijkstrasStatus {
        kwi::status::Status status;
        std::vector<kwi::status::Status> history;
        uint cost;

        // Comparison operator for priority queue
        bool operator<(const DijkstrasStatus &other) const {
            return cost > other.cost; // Priority queue orders by lowest cost
        }
    };

    priority_queue<DijkstrasStatus> open_list;
    open_list.push({start_status, {start_status}, 0});

    // Closed list to track visited states
    unordered_map<kwi::status::Status, uint, kwi::status::Hash> closed_list;

    while (!open_list.empty()) {
        // Get the status with the lowest cost from the priority queue
        auto current = open_list.top();
        open_list.pop();

        kwi::status::Status current_status = current.status;
        vector<kwi::status::Status> current_history = current.history;
        uint current_cost = current.cost;

        // Check if the current status is the final status
        if (isFinal(current_status)) {
            return current_history; // Return the history of statuses leading to the goal
        }

        // Check if the current state is already in the closed list with a lower or equal cost
        if (closed_list.find(current_status) != closed_list.end() && closed_list[current_status] <= current_cost) {
            continue; // Skip this state as it has been processed with a lower or equal cost
        }

        // Add the current status to the closed list with its cost
        closed_list[current_status] = current_cost;

        // Generate all valid neighboring statuses
        vector<pair<kwi::status::Status, uint>> neighbors_with_costs = get_neighbors(current_status);

        for (auto it = neighbors_with_costs.begin(); it != neighbors_with_costs.end(); ++it) {
            kwi::status::Status neighbor = (*it).first;
            uint move_cost = (*it).second;
            uint new_cost = current_cost + move_cost;

            // Check if the neighbor is already in the closed list with a lower or equal cost
            if (closed_list.find(neighbor) != closed_list.end() && closed_list[neighbor] <= new_cost) {
                continue; // Skip this neighbor as it has been processed with a lower or equal cost
            }

            vector<kwi::status::Status> new_history = current_history;
            new_history.push_back(neighbor);

            open_list.push({neighbor, new_history, new_cost});
        }
    }
    return {}; // Return an empty vector if no solution is found
}

}