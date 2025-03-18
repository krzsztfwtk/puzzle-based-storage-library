#include "is_final_functions.h"

#include <algorithm>

using namespace std;

namespace kwi::is_final {

bool isPattern(const kwi::status::Status &s, const std::vector<std::vector<bool>> &pattern) {
    const auto &grid = s.are_occupied_grid;

    return std::equal(grid.begin(), grid.end(), pattern.begin(),
        [](const auto &row1, const auto &row2) {
            return std::equal(row1.begin(), row1.end(), row2.begin());
    });
}

bool isCovered(const kwi::status::Status &s) {
    const auto &grid = s.are_occupied_grid;

    return std::all_of(grid.begin(), grid.end(), [](const auto &row) {
        return std::all_of(row.begin(), row.end(), [](bool cell) { return cell; });
    });
}

}