#include "scenarios.h"

#include <iostream>
#include <random>

#include "status.h"
#include "dijkstras.h"
#include "a_star.h"
#include "is_final_functions.h"
#include "neighbors.h"
#include "heuristic.h"
#include "cli_visualization.h"
#include "solution.h"
#include "visualization.h"

int countTrues(const std::vector<std::vector<bool>>& pattern) {
    int count = 0;
    for (const auto& row : pattern) {
        for (bool cell : row) {
            if (cell) {
                count++;
            }
        }
    }
    return count;
}

// Function to generate a random destination within given bounds
std::array<uint, 2> generateDestination(uint max_x, uint max_y) {
    // Seed with a random device for true randomness
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::uniform_int_distribution<uint> dist_x(0, max_x - 1);
    std::uniform_int_distribution<uint> dist_y(0, max_y - 1);

    return {dist_x(gen), dist_y(gen)};
}

// This scenerio intentionally does nothing
void scenario00(const std::vector<int>& args) {
    std::cout << "this scenario intentionally does nothing" << std::endl;

    if (!args.empty()) {
        std::cout << "even if you pass args it does nothing :)" << std::endl;
    }
}

// axis movement
void scenario01(const std::vector<int>& args) {
    uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
    uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
    int num_cells = args.size() > 2 ? args[2] : 20;
    int enable_visualization = (args.size() > 3) ? args[3] : 0;
    int visualization_delay_miliseconds = (args.size() > 4) ? args[4] : 1000;

    kwi::status::Status start = kwi::status::generate(x_size, y_size, num_cells);

    std::array<uint, 2> destination = {0, 0};

    auto isFinal = [&destination](const kwi::status::Status &s) {
        return kwi::is_final::isTargetOnDestination(s, destination);
    };

    auto heuristicFunction = [&destination](const kwi::status::Status &current_status) {
        return kwi::heuristic::weightedManhattanDistance(current_status, destination);
    };

    auto solution_with_distance = kwi::route_finding::aStar(
        start,
        isFinal,
        kwi::neighbors::status_move_cost::axis,
        heuristicFunction
    );

    auto solution_moves = solution_with_distance.first;
    uint distance = solution_with_distance.second;

    if (enable_visualization) {
        kwi::visualization::play(start, solution_moves, x_size, y_size, visualization_delay_miliseconds);
    }

    std::cout << solution_moves.size() << " " << distance << std::endl;

}


// planar movement
void scenario02(const std::vector<int>& args) {
    uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
    uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
    int num_cells = args.size() > 2 ? args[2] : 20;
    int enable_visualization = (args.size() > 3) ? args[3] : 0;
    int visualization_delay_miliseconds = (args.size() > 4) ? args[4] : 1000;

    kwi::status::Status start = kwi::status::generate(x_size, y_size, num_cells);

    std::array<uint, 2> destination = {0, 0};

    auto isFinal = [&destination](const kwi::status::Status &s) {
        return kwi::is_final::isTargetOnDestination(s, destination);
    };

    auto heuristicFunction = [&destination](const kwi::status::Status &current_status) {
        return kwi::heuristic::weightedManhattanDistance(current_status, destination);
    };

    auto solution_with_distance = kwi::route_finding::aStar(
        start,
        isFinal,
        kwi::neighbors::status_move_cost::planar,
        heuristicFunction
    );

    auto solution_moves = solution_with_distance.first;
    uint distance = solution_with_distance.second;

    if (enable_visualization) {
        kwi::visualization::play(start, solution_moves, x_size, y_size, visualization_delay_miliseconds);
    }

    std::cout << solution_moves.size() << " " << distance << std::endl;
}

void scenario03(const std::vector<int>& args) { ; }

void scenario04(const std::vector<int>& args) { ; }

void scenario05(const std::vector<int>& args) { ; }

void scenario06(const std::vector<int>& args) { ; }

void scenario07(const std::vector<int>& args) { ; }

// single agent pathfinding
void scenario08(const std::vector<int>& args) {
    uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
    uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
    int num_cells = args.size() > 2 ? args[2] : 20;
    int enable_visualization = (args.size() > 3) ? args[3] : 0;

    kwi::status::Status start = kwi::status::generate(x_size, y_size, num_cells);

    std::array<uint, 2> destination = generateDestination(6, 7);

    start.are_occupied_grid[destination[0]][destination[1]] = false;

    auto isFinal = [&destination](const kwi::status::Status &s) {
        return kwi::is_final::isTargetOnDestination(s, destination);
    };

    auto heuristicFunction = [&destination](const kwi::status::Status &current_status) {
        return kwi::heuristic::manhattanDistance(current_status, destination);
    };

    auto getNeighborsFunction = [](const kwi::status::Status &current_status) {
        return kwi::neighbors::status_cost::planarTargetOnly(current_status);
    };

    auto solution = kwi::route_finding::aStar(
        start,
        isFinal,
        getNeighborsFunction,
        heuristicFunction
    );

    auto solution_steps = solution.first;

    if (enable_visualization) {
        kwi::visualization::play(solution_steps, x_size, y_size);
    }
}

// axis, 4 IO
void scenario09(const std::vector<int>& args) {
    uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
    uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
    int num_cells = args.size() > 2 ? args[2] : 20;
    int enable_visualization = (args.size() > 3) ? args[3] : 0;

    kwi::status::Status start = kwi::status::generate(x_size, y_size, num_cells);

    std::vector<std::array<uint, 2>> destinations = {
        {0, 0}, {0, 4}, {4, 0}, {4, 4}
    };

    auto isFinal = [&destinations](const kwi::status::Status &s) {
        return kwi::is_final::isTargetOnDestination(s, destinations);
    };

    auto heuristicFunction = [&destinations](const kwi::status::Status &current_status) {
        return kwi::heuristic::manhattanDistance(current_status, destinations);
    };

    auto solution = kwi::route_finding::aStar(
        start,
        isFinal,
        kwi::neighbors::status_cost::axis,
        heuristicFunction
    );

    std::cout << solution.first.size();

    auto solution_steps = solution.first;

    if (enable_visualization) {
        kwi::visualization::play(solution_steps, x_size, y_size);
    }
}

// exp_02
void scenario10(const std::vector<int>& args) {
    uint x_size = (uint)(args.size() > 0) ? args[0] : 5;
    uint y_size = (uint)(args.size() > 1) ? args[1] : 5;
    int io_num = (args.size() > 1) ? args[2] : 1;
    int io_edge_distribution = (args.size() > 3) ? args[3] : 0;
    int occupied_cells = (args.size() > 4) ? args[4] : 20;
    int allow_diagonal_movement = (args.size() > 5) ? args[5] : 0;
    int enable_visualization = (args.size() > 6) ? args[6] : 0;
    int visualization_delay_miliseconds = (args.size() > 7) ? args[7] : 1000;

    kwi::status::Status start = kwi::status::generate(x_size, y_size, occupied_cells);

    std::vector<std::array<uint, 2>> destinations;

switch (io_num) {
    case 1:
        if (io_edge_distribution)
            destinations = {{x_size / 2, 0}};
        else
            destinations = {{0, 0}};
        break;

    case 2:
        if (io_edge_distribution)
            destinations = {{x_size / 2, 0}, {x_size / 2, y_size - 1}};
        else
            destinations = {{0, 0}, {x_size - 1, y_size - 1}};
        break;

    case 3:
        if (io_edge_distribution)
            destinations = {{0, y_size / 2}, {x_size / 2, 0}, {x_size / 2, y_size - 1}};
        else
            destinations = {{0, 0}, {0, y_size - 1}, {x_size - 1, y_size - 1}};
        break;

    case 4:
        if (io_edge_distribution)
            destinations = {{0, y_size / 2}, {x_size / 2, 0}, {x_size / 2, y_size - 1}, {x_size - 1, y_size / 2}};
        else
            destinations = {{0, 0}, {0, y_size - 1}, {x_size - 1, 0}, {x_size - 1, y_size - 1}};
        break;

    default:
        break;
}

    auto isFinal = [&destinations](const kwi::status::Status &s) {
        return kwi::is_final::isTargetOnDestination(s, destinations);
    };

    auto heuristicFunction = [&destinations](const kwi::status::Status &current_status) {
        return kwi::heuristic::weightedManhattanDistance(current_status, destinations);
    };

    auto neighborsFunction = kwi::neighbors::status_move_cost::axis;

    if (allow_diagonal_movement)
        neighborsFunction = kwi::neighbors::status_move_cost::planar;

    auto solution_with_distance = kwi::route_finding::aStar(
        start,
        isFinal,
        neighborsFunction,
        heuristicFunction
    );


    auto solution_moves = solution_with_distance.first;
    uint distance = solution_with_distance.second;

    if (enable_visualization)
        kwi::visualization::play(start, solution_moves, x_size, y_size, visualization_delay_miliseconds);

    std::cout << solution_moves.size() << " " << distance << std::endl;

}

void scenario11(const std::vector<int>& args) {

    kwi::status::Status start = {
        {{1, 1, 1, 1, 0},
        {1, 1, 0, 1, 0},
        {1, 0, 1, 1, 1},
        {1, 0, 1, 1, 0},
        {1, 1, 0, 1, 1}},
        {4, 4}

    };

    std::array<uint, 2> destination = {0, 0};

    auto isFinal = [&destination](const kwi::status::Status &s) {
        return kwi::is_final::isTargetOnDestination(s, destination);
    };

    auto heuristicFunction = [&destination](const kwi::status::Status &current_status) {
        return kwi::heuristic::manhattanDistance(current_status, destination);
    };

    auto solution_with_distance = kwi::route_finding::aStar(
        start,
        isFinal,
        kwi::neighbors::status_move_cost::axis,
        heuristicFunction
    );

    auto solution_moves = solution_with_distance.first;

    kwi::visualization::print(start, solution_moves, start.are_occupied_grid.size(), start.are_occupied_grid[0].size());

    std::string out_file = "output.txt";

    kwi::visualization::save_to_file(start, solution_moves, start.are_occupied_grid.size(), 
        start.are_occupied_grid[0].size(), out_file);
}

namespace kwi {
    std::vector<std::function<void(const std::vector<int>&)>> scenarios = {
        scenario00, // Include this to avoid out-of-range access
        scenario01,
        scenario02,
        scenario03,
        scenario04,
        scenario05,
        scenario06,
        scenario07,
        scenario08,
        scenario09,
        scenario10,
        scenario11
    };
}