#!/bin/bash

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <number_of_runs> <command>"
    exit 1
fi

NUM_RUNS=$1
shift
COMMAND="$@"

TOTAL_TIME=0
TOTAL_STEPS=0
TOTAL_DISTANCE=0

for i in $(seq 1 $NUM_RUNS); do
    RESULT=$(/usr/bin/time -f "%e" $COMMAND 2>&1)
    
    RUN_TIME=$(echo "$RESULT" | tail -n 1 | awk '{print $1}')
    TOTAL_TIME=$(echo "$TOTAL_TIME + $RUN_TIME" | bc)
    
    OUTPUT=$(echo "$RESULT" | head -n -1)
    STEPS=$(echo "$OUTPUT" | awk '{print $1}')
    DISTANCE=$(echo "$OUTPUT" | awk '{print $2}')
    
    TOTAL_STEPS=$(echo "$TOTAL_STEPS + $STEPS" | bc)
    TOTAL_DISTANCE=$(echo "$TOTAL_DISTANCE + $DISTANCE" | bc)
done

# Calculate averages
AVERAGE_TIME=$(echo "$TOTAL_TIME / $NUM_RUNS" | bc -l)
AVERAGE_STEPS=$(echo "$TOTAL_STEPS / $NUM_RUNS" | bc -l)
AVERAGE_DISTANCE=$(echo "$TOTAL_DISTANCE / $NUM_RUNS" | bc -l)

# Display results
echo "Average time over $NUM_RUNS runs: $AVERAGE_TIME seconds"
echo "Average steps: $AVERAGE_STEPS"
echo "Average distance: $AVERAGE_DISTANCE"
