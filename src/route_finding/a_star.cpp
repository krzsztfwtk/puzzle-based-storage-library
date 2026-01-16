#include "../types.h"
#include <queue>
#include <unordered_map>

#include "a_star.h"

using namespace std;

namespace kwi::route_finding {

pair<vector<Move>, uint>
aStar(const status::Status &start_status, const function<bool(const status::Status &)> &isFinal,
      const function<vector<tuple<status::Status, Move, uint>>(const status::Status &)> &get_neighbors,
      const function<uint(const status::Status &)> &heuristic) {

  struct AStarState {
    status::Status status;
    vector<Move> history;
    uint g_cost;
    uint f_cost;

    bool operator<(const AStarState &other) const { return f_cost > other.f_cost; }
  };

  priority_queue<AStarState> open_list;
  unordered_map<status::Status, uint, status::Hash> closed_list;
  closed_list[start_status] = 0;

  uint h_start = heuristic(start_status);
  open_list.push({start_status, {}, 0, h_start});

  while (!open_list.empty()) {
    const AStarState current = open_list.top();
    open_list.pop();

    const status::Status current_status = current.status;
    const vector<Move> current_history = current.history;
    const uint current_g_cost = current.g_cost;

    if (isFinal(current_status)) [[unlikely]] {
      return {current_history, current_g_cost};
    }

    if (closed_list[current_status] < current_g_cost) {
      continue;
    }

    const vector<tuple<status::Status, Move, uint>> neighbors_moves_costs = get_neighbors(current_status);

    for (const auto &neighbor_move_cost : neighbors_moves_costs) {
      status::Status neighbor = std::get<0>(neighbor_move_cost);
      Move move = std::get<1>(neighbor_move_cost);
      uint move_cost = std::get<2>(neighbor_move_cost);
      uint new_g_cost = current_g_cost + move_cost;

      auto const it = closed_list.find(neighbor);
      if (it != closed_list.end() && it->second <= new_g_cost) {
        continue;
      }

      closed_list[neighbor] = new_g_cost;

      uint h_cost = heuristic(neighbor);
      uint f_cost = new_g_cost + h_cost;

      vector<Move> new_history = current_history;
      new_history.push_back(move);

      open_list.push({neighbor, new_history, new_g_cost, f_cost});
    }
  }
  return {};
}

} // namespace kwi::route_finding