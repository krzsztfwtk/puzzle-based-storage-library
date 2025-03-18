#include <random>
#include <iostream>
#include <algorithm>

#include "status.h"

using namespace std;

// Generate a random number in the range [min, max]
int randomInt(int min, int max) {
    static random_device rd;
    static mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

namespace kwi::status {

// Generate a random Status object
Status generate(int x_size, int y_size, int num_trues) {
    // Ensure num_trues is valid
    if (num_trues > x_size * y_size) {
        throw invalid_argument("Number of true values cannot exceed grid size.");
    }

    // Initialize the grid with all false values
    vector<vector<bool>> grid(x_size, vector<bool>(y_size, false));

    // Create a list of all possible positions
    vector<std::pair<int, int>> positions;
    for (int i = 0; i < x_size; ++i) {
        for (int j = 0; j < y_size; ++j) {
            positions.emplace_back(i, j);
        }
    }

    // Shuffle the positions and select the first `num_trues` positions
    shuffle(positions.begin(), positions.end(), mt19937{random_device{}()});
    for (int i = 0; i < num_trues; ++i) {
        grid[positions[i].first][positions[i].second] = true;
    }

    // Select a random position from the true positions for the target
    vector<pair<int, int>> true_positions;
    for (int i = 0; i < x_size; ++i) {
        for (int j = 0; j < y_size; ++j) {
            if (grid[i][j]) {
                true_positions.emplace_back(i, j);
            }
        }
    }

    auto target_pos = true_positions[randomInt(0, true_positions.size() - 1)];

    return Status{grid, {static_cast<uint>(target_pos.first), static_cast<uint>(target_pos.second)}};
}

}