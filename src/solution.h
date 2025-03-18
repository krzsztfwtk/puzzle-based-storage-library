#pragma once

#include <vector>
#include <tuple>
#include "status.h"

namespace kwi::solution {

// Decode a solution path into a list of moves
std::vector<std::tuple<kwi::status::Status, std::vector<std::array<int, 2>>, std::vector<std::array<int, 2>>>> 
decode(const std::vector<kwi::status::Status> &solution);

// Decode a list of moves into a solution path
std::vector<kwi::status::Status> encode(const kwi::status::Status& start, const std::vector<uint>& moves);

}