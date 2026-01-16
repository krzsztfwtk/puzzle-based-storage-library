#include <iostream>
#include <vector>

#include "scenarios/scenarios.h"

int main(int argc, char **params) {
  if (argc == 1) {
    std::cout << "No scenario chosen." << std::endl;
    std::cout << "Provide number of an scenario, eg.:" << std::endl;
    std::cout << params[0] << " 2" << std::endl;
    std::cout << "to run the 2nd scenario." << std::endl;
    std::cout << params[0] << " <scenario_num> <scenario_params>" << std::endl;
    return 0;
  }

  int scenario = atoi(params[1]);

  std::vector<int> scenario_args;
  for (int i = 2; i < argc; ++i) {
    scenario_args.push_back(std::atoi(params[i]));
  }

  if (scenario >= 1 && scenario <= scenarios.size()) {
    scenarios[scenario - 1](scenario_args);
  } else {
    std::cout << "Unknown scenario." << std::endl;
    std::cout << "Choose a number between 1 and " << scenarios.size() << "." << std::endl;
  }

  return 0;
}
