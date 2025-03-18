# puzzle-based-storage-library
library for route programming in puzzle-based storage systems and other similar systems

At this point, the library has implemented single-item retrieval in puzzle-based storage parking systems, incorporating Dijkstra's and A* algorithm and Manhattan Distance, Chebyshev Distance, and an occupancy-based variant of Manhattan Distance heuristics.

The library is implemented in C++.

After building with `make` you can explore preset scenarios. Run program with:

```bash
./bin/main <scenario> <arg1> <arg2> ... <argN>
```

where `scenario` is a number of scenario to run and `arg1`, `arg2`, `argN` are parsed further to the chosen scenario.

Refer to `scenarios.cpp` for example usage of the library. You can also implement your own custom scenarios.

## Example usage

This runs single-item retrieval (scenario01) on 5x5 parking lot with only orthogonal movement allowed. Visualization is enabled on the command line and its speed is set to one parking status per 1.5 s.

```bash
./bin/main 1 5 5 20 1 1500
```

## Features

Below is a summary of the main features and tasks currently implemented or planned:

🔄 - Ongoing work

- **Tasks**  
  - [x] Item retrieval  
    - [x] Single-item  
    - [ ] Multi-items  
  - [ ] 🔄Pattern forming  
  - [ ] Area coverage  
    - [ ] 🔄Single-agent area coverage  
    - [ ] Multi-agent area coverage  
  - [ ] Pathfinding  
    - [x] Single-agent  
    - [ ] Multi-agent  

- **Input/Output Points (Destinations)**  
  - [x] Single IO  
  - [x] Multiple IOs  
  - [x] Other destinations inside grid  

- **Movement Rules**  
  - [x] Orthogonal (Only axis-aligned movement)  
  - [x] Octilinear (axis-aligned + diagonal)  

- **Algorithms**  
  - [x] Optimal (Dijkstra’s)  
  - [x] Heuristic (A*)  
  - [ ] AI  
    - [ ] Reinforcement Learning  
    - [ ] Genetic Algorithms  
    - [ ] Decentralized Neural Networks  

- **Objective Functions**  
  - [x] Minimum number of steps  
  - [x] Minimum movement distance  
  - [ ] 🔄Minimum movement time (with acceleration factors)  
  - [ ] Minimum energy consumption (with acceleration cost, etc.)  
  - [ ] Weighted or multi-criteria functions  

- **Heuristic Functions**  
  - [x] Simple (Manhattan distance, pattern mismatch)  
  - [ ] 🔄Advanced  

- **Movement Types**  
  - [x] Linear movements (one entity at a time)  
  - [ ] 🔄Simultaneous movements (multiple entities moving at once)  

- **Levels (Floors)**  
  - [x] Single-level  
  - [ ] Multi-level  

- **Movement Mechanisms**  
  - [x] Targets move by themselves (e.g., conveyor logic)  
  - [ ] 🔄Targets are moved by something else (AGVs - Automated Guided Vehicles)  

- **Performance Techniques**  
  - [ ] 🔄Multithreading  
  - [ ] AVX (SIMD)  
  - [ ] 🔄Decomposition  
  - [ ] Coroutines  
  - [ ] Dynamic programming  
  - [ ] Precomputed lookup tables  

- **Spots/Cells**  
  - [x] Squares  
  - [ ] 🔄Rectangles with variable width/height  

- **Visualization**  
  - [x] Simple 2D  
  - [ ] Sophisticated 3D
