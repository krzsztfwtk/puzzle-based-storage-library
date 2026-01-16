#include "is_final.h"

#include <algorithm>

using namespace std;

namespace kwi::is_final {

bool isTargetOnDestination(const kwi::status::Status &s, std::array<unsigned int, 2> &dest) {
  return s.target_coords[0] == dest[0] && s.target_coords[1] == dest[1];
}

bool isTargetOnDestination(const kwi::status::Status &s, const std::vector<std::array<unsigned int, 2>> &dest) {
  for (const auto &destination_coords : dest) {
    if (s.target_coords[0] == destination_coords[0] && s.target_coords[1] == destination_coords[1]) {
      return true;
    }
  }
  return false;
}

} // namespace kwi::is_final