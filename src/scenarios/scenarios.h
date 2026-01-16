#pragma once

#include <functional>
#include <vector>

// exp01.1: Single item retrieval, orthogonal movement, 1 IO
void scenario01(const std::vector<int> &args);

// exp01.2: Single item retrieval, octilinear movement, 1 IO
void scenario02(const std::vector<int> &args);

// Single item retrieval, orthogonal movement, 4 IO
void scenario03(const std::vector<int> &args);

// Scenario for exp_02
void scenario04(const std::vector<int> &args);

// Basic example
void scenario05(const std::vector<int> &args);

// Dijkstra's algorithm
void scenario06(const std::vector<int> &args);

extern std::vector<std::function<void(const std::vector<int> &)>> scenarios;