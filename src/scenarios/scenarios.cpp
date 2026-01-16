#include "scenarios.h"

std::vector<std::function<void(const std::vector<int> &)>> scenarios = {
    scenario01, // exp01.1
    scenario02, // exp01.2
    scenario03, // multiple IOs example
    scenario04, // exp02
    scenario05, // basic library use example
    scenario06  // Dijkstra's algorithm
};