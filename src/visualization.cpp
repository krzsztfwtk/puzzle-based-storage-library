#include "visualization.h"

#include "cli_visualization.h"
#include "solution.h"

namespace kwi::visualization {

void play(const std::vector<kwi::status::Status> &history, 
        const uint x_size, const uint y_size, int delay_milliseconds) {
    kwi::visualization::CLIVisualization visualization(x_size, y_size);
    visualization.playSolutionPath(history, delay_milliseconds);
}

void play(const kwi::status::Status &start, const std::vector<uint> &moves, 
    const uint x_size, const uint y_size, int delay_milliseconds) {
    auto solution_steps = kwi::solution::encode(start, moves);
    play(solution_steps, x_size, y_size, delay_milliseconds);
}

void print(const std::vector<kwi::status::Status> &history, 
        const uint x_size, const uint y_size) {
    kwi::visualization::CLIVisualization visualization(x_size, y_size);
    visualization.printSolutionPath(history);
}

void print(const kwi::status::Status &start, const std::vector<uint> &moves, 
    const uint x_size, const uint y_size) {
    auto solution_steps = kwi::solution::encode(start, moves);
    print(solution_steps, x_size, y_size);
}

void save_to_file(const std::vector<kwi::status::Status> &history, 
        const uint x_size, const uint y_size, std::string& filename) {
    kwi::visualization::CLIVisualization visualization(x_size, y_size);
    visualization.saveSolutionToFile(history, filename);
}

void save_to_file(const kwi::status::Status &start, const std::vector<uint> &moves, 
    const uint x_size, const uint y_size, std::string& filename) {
    auto solution_steps = kwi::solution::encode(start, moves);
    save_to_file(solution_steps, x_size, y_size, filename);
}

}