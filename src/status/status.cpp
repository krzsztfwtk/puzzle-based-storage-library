#include <algorithm>
#include <random>

#include "../types.h"
#include "status.h"

using namespace std;

static int randomInt(int min, int max) {
  static random_device rd;
  static mt19937 gen(rd());
  uniform_int_distribution<> dist(min, max);
  return dist(gen);
}

namespace kwi::status {

Status generate(int x_size, int y_size, int num_occupied) {
  // Ensure num_trues is valid
  if (num_occupied >= x_size * y_size) {
    num_occupied = x_size * y_size - 1;
  }

  vector<vector<bool>> grid(x_size, vector<bool>(y_size, false));

  // Create a list of all possible positions
  vector<std::pair<int, int>> positions;
  for (int i = 0; i < x_size; ++i) {
    for (int j = 0; j < y_size; ++j) {
      positions.emplace_back(i, j);
    }
  }

  // Shuffle the positions and select the first `num_occupied` positions
  shuffle(positions.begin(), positions.end(), mt19937{random_device{}()});
  for (int i = 0; i < num_occupied; ++i) {
    grid[positions[i].first][positions[i].second] = true;
  }

  // Select a random position from the occupied positions as the target
  auto target_pos = positions[randomInt(0, num_occupied - 1)];

  return Status{grid, {static_cast<uint>(target_pos.first), static_cast<uint>(target_pos.second)}};
}

} // namespace kwi::status