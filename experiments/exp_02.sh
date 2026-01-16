#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <number_of_runs>"
    echo "Example: $0 100"
    echo "Please provide the number of runs as the first argument."
    exit 1
fi

NUM_RUNS=$1

# Define the grids to test
GRIDS=("5x5")

# Range of io_num positions
IO_NUMS=(1 2 3 4)

# I/O distributions:
# 0 - vertex distribution
# 1 - edge distribution
IO_DISTRIBUTIONS=(0 1)

# Movement modes:
# 0 - orthogonal
# 1 - octilinear
MOVEMENTS=(0 1)

BASE_DIR="./results/exp_02"
mkdir -p "$BASE_DIR"

REPORT_FILE="$BASE_DIR/exp_02.txt"

cat <<EOT > "$REPORT_FILE"
Experiment: Single Item Retrieval with multiple I/O points

Experiment Purpose:
Compare performance of single-item retrieval under various configurations:
- I/O placement on vertices vs edges of the grid
- I/O number between 1 and 4, inclusive
- Various numbers of occupied cells (fill)
- Orthogonal vs octilinear movement

Execution time, number of steps and distance are measured.

Methodology:
- For each combination of (io_distribution, io_num, fill), run the scenario $NUM_RUNS times.
- Test both orthogonal and octilinear movement.
- For each grid, fill values range from 1 to (total_cells - 1).

System Information:
EOT

{
    echo "Operating System: $(lsb_release -d | cut -f2)"
    echo "Kernel Version: $(uname -r)"
    echo "CPU Model: $(lscpu | grep 'Model name' | awk -F: '{print \$2}' | xargs)"
    echo "CPU Cores: $(lscpu | grep '^CPU(s):' | awk -F: '{print \$2}' | xargs)"
    echo "CPU max MHz: $(lscpu | grep '^CPU max MHz:' | awk -F: '{print \$2}' | xargs)"
    echo "Total Memory: $(free -h | grep 'Mem:' | awk '{print \$2}')"
    echo "Available Memory: $(free -h | grep 'Mem:' | awk '{print \$7}')"
} >> "$REPORT_FILE"

RAW_CSV_FILE="./results/exp_02/exp_02.csv"
echo "grid,distribution,movement,io_num,fill,steps,distance,time,run" > "$RAW_CSV_FILE"

get_distribution_name() {
    local dist=$1
    if [ "$dist" -eq 0 ]; then
        echo "vertex"
    else
        echo "edge"
    fi
}

# Run the scenario and append results to the CSV file
run_scenario() {
    local grid_x=$1
    local grid_y=$2
    local io_dist=$3
    local io_num=$4
    local fill=$5
    local allow_diag=$6

    local grid="${grid_x}x${grid_y}"
    local dist_name
    dist_name=$(get_distribution_name "$io_dist")

    local movement=""
    if [ "$allow_diag" -eq 0 ]; then
        movement="orthogonal"
    else
        movement="octilinear"
    fi

    for ((i = 1; i <= NUM_RUNS; i++)); do
        start_time=$(date +%s%N)
        result=$(./bin/main 4 $grid_x $grid_y $io_num $io_dist $fill $allow_diag)
        end_time=$(date +%s%N)
        elapsed_time=$(( (end_time - start_time) / 1000000 )) # Convert to milliseconds

        steps=$(echo "$result" | awk '{print $1}')
        distance=$(echo "$result" | awk '{print $2}')

        # Append the result to the CSV:
        echo "$grid,$dist_name,$movement,$io_num,$fill,$steps,$distance,$elapsed_time,$i" >> "$RAW_CSV_FILE"
    done
}

# Iterate over grids and configurations
for grid in "${GRIDS[@]}"; do
    grid_x=$(echo "$grid" | cut -d'x' -f1)
    grid_y=$(echo "$grid" | cut -d'x' -f2)
    total_cells=$((grid_x * grid_y))
    echo "Processing grid: $grid"

    for io_dist in "${IO_DISTRIBUTIONS[@]}"; do
        echo "  Using distribution: $io_dist"
        for io_num in "${IO_NUMS[@]}"; do
            echo "  IO num: $io_num"
            for ((fill = 1; fill <= total_cells - 1; fill++)); do
                echo "  Fill: $fill"
                run_scenario $grid_x $grid_y $io_dist $io_num $fill 0
                run_scenario $grid_x $grid_y $io_dist $io_num $fill 1
            done
        done
    done
done

cat <<EOT >> "$REPORT_FILE"

Results Summary:
Results are saved in the raw CSV file: ./results/exp_02_raw.csv

EOT

echo "Experiment completed. Results saved to $REPORT_FILE and raw CSV in $RAW_CSV_FILE."
