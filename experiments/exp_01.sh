#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_runs>"
    echo "Example: $0 100"
    echo "Please provide the number of runs as the first argument."
    echo "Number of runs is the number of times each configuration executes."
    exit 1
fi

# Number of iterations
NUM_RUNS=$1

# Create necessary directories for results
mkdir -p ./results
mkdir -p ./results/exp_01

# Experiment Report File
REPORT_FILE="./results/exp_01/exp_01.txt"

# Experiment description
cat <<EOT > "$REPORT_FILE"
Experiment: A* Pathfinding Comparison

Experiment Purpose:
This experiment compares the performance of two A* Puzzle-based storage pathfinding scenarios on a 
different-sized grid with random initial status. The performance is measured in terms 
of the number of steps in the solution path, distance, and execution time.

Scenarios:
1. Single item retrieval A* with axis-aligned movement only
2. Single item retrieval A* with planar motion (axis-aligned + diagonal)

Single I/O cell in (0, 0)

Methodology:
- Each scenario was executed $NUM_RUNS times in each configuration with random initial status.
- Results include:
  1. Number of steps in the solution path.
  2. Distance in the solution assuming the cells are squares of side 100.
  3. Execution time.

System Information:
EOT

# Collect System Information
{
    echo "Operating System: $(lsb_release -d | cut -f2)"
    echo "Kernel Version: $(uname -r)"
    echo "CPU Model: $(lscpu | grep 'Model name' | awk -F: '{print \$2}' | xargs)"
    echo "CPU Cores: $(lscpu | grep '^CPU(s):' | awk -F: '{print \$2}' | xargs)"
    echo "CPU max MHz: $(lscpu | grep '^CPU max MHz:' | awk -F: '{print \$2}' | xargs)"
    echo "Total Memory: $(free -h | grep 'Mem:' | awk '{print \$2}')"
    echo "Available Memory: $(free -h | grep 'Mem:' | awk '{print \$7}')"
} >> "$REPORT_FILE"

# results CSV file
RAW_CSV_FILE="./results/exp_01/exp_01_raw.csv"
echo "grid,movement,fill,steps,distance,time,run" > "$RAW_CSV_FILE"

run_scenario() {
    local scenario=$1
    local grid_x=$2
    local grid_y=$3
    local fill=$4

    local grid="${grid_x}x${grid_y}"
    local movement=""
    if [ "$scenario" -eq 1 ]; then
        movement="axis"
    elif [ "$scenario" -eq 2 ]; then
        movement="planar"
    else
        movement="unknown"
    fi

    echo "Running Scenario $scenario ($movement movement) on grid ${grid} with $fill cells filled, $NUM_RUNS runs..."
    for ((i = 1; i <= NUM_RUNS; i++)); do
        start_time=$(date +%s%N)
        result=$(./bin/main $scenario $grid_x $grid_y $fill)
        end_time=$(date +%s%N)
        elapsed_time=$(( (end_time - start_time) / 1000000 )) # Convert to milliseconds

        steps=$(echo "$result" | awk '{print $1}')
        distance=$(echo "$result" | awk '{print $2}')
        echo "$grid,$movement,$fill,$steps,$distance,$elapsed_time,$i" >> "$RAW_CSV_FILE"
    done
}

# Configure grids
grids=("3x3" "3x4" "3x5" "4x4" "4x5" "4x6" "5x5" "5x6" "5x7" "6x6")

# Run scenario for each grid and fill configuration
for grid in "${grids[@]}"; do
    grid_x=$(echo "$grid" | cut -d'x' -f1)
    grid_y=$(echo "$grid" | cut -d'x' -f2)
    total_cells=$((grid_x * grid_y))

    # Loop over all possible fills from 1 to total_cells-1
    for ((fill = 1; fill <= total_cells - 1; fill++)); do
        run_scenario 1 "$grid_x" "$grid_y" "$fill"
        run_scenario 2 "$grid_x" "$grid_y" "$fill"
    done
done

# Append Results Summary to Report
cat <<EOT >> "$REPORT_FILE"

Results Summary:
Results are saved in the raw CSV file: ./results/exp_01_raw.csv

CSV file columns:
grid,movement,fill,steps,distance,time,run

EOT

# Final message
echo "Experiment completed. Results saved to $REPORT_FILE and raw CSV in $RAW_CSV_FILE."
