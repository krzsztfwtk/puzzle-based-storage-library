#include "dijkstra.h"
#include <queue>

using namespace std;

namespace kwi::route_finding {

pair<vector<Move>, uint>
dijkstra(const status::Status &start_status, const function<bool(const status::Status &)> &isFinal,
         const function<vector<tuple<status::Status, Move, uint>>(const status::Status &)> &get_neighbors) {
  struct DijkstraState {
    status::Status status;
    vector<Move> history;
    uint cost;

    bool operator<(const DijkstraState &other) const { return cost > other.cost; }
  };

  priority_queue<DijkstraState> open_list;
  open_list.push({start_status, {}, 0});

  unordered_map<status::Status, uint, status::Hash> closed_list;
  unordered_map<status::Status, status::Status, status::Hash> came_from;
  closed_list[start_status] = 0;

  while (!open_list.empty()) {
    const DijkstraState current = open_list.top();
    open_list.pop();

    const status::Status current_status = current.status;
    const vector<Move> current_history = current.history;
    const uint current_cost = current.cost;

    if (isFinal(current_status)) [[unlikely]] {
      return {current_history, current_cost};
    }

    if (closed_list[current_status] < current_cost) {
      continue;
    }

    const vector<tuple<status::Status, Move, uint>> neighbors_moves_costs = get_neighbors(current_status);

    for (const auto &neighbor_move_cost : neighbors_moves_costs) {
      status::Status neighbor = std::get<0>(neighbor_move_cost);
      Move move = std::get<1>(neighbor_move_cost);
      uint move_cost = std::get<2>(neighbor_move_cost);
      uint new_cost = current_cost + move_cost;

      const auto it = closed_list.find(neighbor);
      if (it != closed_list.end() && it->second <= new_cost) {
        continue;
      }

      closed_list[neighbor] = new_cost;

      vector<Move> new_history = current_history;
      new_history.push_back(move);

      open_list.push({neighbor, new_history, new_cost});
    }
  }
  return {};
}

} // namespace kwi::route_finding