#pragma once

#include <vector>
#include <functional>

// This scenerio intentionally does nothing
void scenario00(const std::vector<int>& args);

// Single item retrieval, axis aligned motion, 1 IO
void scenario01(const std::vector<int>& args);

// Single item retrieval, planar motion, 1 IO
void scenario02(const std::vector<int>& args);

// Single item retrieval, planar movement, multithreaded
void scenario03(const std::vector<int>& args);

// Pattern forming, christmas tree 4x5
void scenario04(const std::vector<int>& args);

// Single item retrieval, axis simultaneous motion , 5x5
void scenario05(const std::vector<int>& args);

// Area coverage, single-agent, axis motion, 6x7
void scenario06(const std::vector<int>& args);

// Area coverage, single-agent, planar motion
void scenario07(const std::vector<int>& args);

// Pathfinding, single-agent, planar motion
void scenario08(const std::vector<int>& args);

// Single item retrieval, axis aligned motion, 5x5, 4 IOs
void scenario09(const std::vector<int>& args);

// Single item retrieval
// @args <x_size> <y_size> <io_num> <io_edge_distribution> <occupied_cells> <allow_diagonal_movement> <enable_visualization> <visualization_delay_miliseconds>
// <io_edge_distribution>: false - vertex, true - edge 
//
// vertex:
// 10001
// 00000
// 00000
// 00000
// 10001
//
// edge:
// 00100
// 00000
// 10001
// 00000
// 00100
void scenario10(const std::vector<int>& args);

namespace kwi {
    extern std::vector<std::function<void(const std::vector<int>&)>> scenarios; // Declaration
}