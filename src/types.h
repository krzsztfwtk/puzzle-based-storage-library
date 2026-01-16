#pragma once

#include <cstdint>

using uint = unsigned int;

struct Move {
  uint8_t from_x;
  uint8_t from_y;
  uint8_t to_x;
  uint8_t to_y;
};