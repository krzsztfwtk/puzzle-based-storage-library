#include <iostream>
#include <vector>

#include "scenarios.h"

int main(int argc, char ** params) {
    if (argc == 1) {
        std::cout << "No scenario chosen." << std::endl;
        std::cout << "Provide number of an scenario as an only parameter, eg." << std::endl;
        std::cout << params[0] << " 2" << std::endl;
        std::cout << "to run the 2nd scenario." << std::endl;
        return 0;
    }

    int scenario = atoi(params[1]);

    std::vector<int> scenario_args;
    for (int i = 2; i < argc; ++i) {
        scenario_args.push_back(std::atoi(params[i]));
    }

    // Validate and execute scenario
    if (scenario >= 0 && scenario <= (int)kwi::scenarios.size()) {
        kwi::scenarios[scenario](scenario_args);
    } else {
        std::cout << "Unknown scenario." << std::endl;
        std::cout << "Choose a number between 1 and " << kwi::scenarios.size() << "." << std::endl;
    }

    return 0;
}
