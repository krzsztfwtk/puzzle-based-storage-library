#!/bin/bash

if [ "$#" -lt 2 ]; then
    echo "Usage: $0 <number_of_runs> <command>"
    exit 1
fi

NUM_RUNS=$1
shift
COMMAND="$@"

TOTAL_TIME=0

for i in $(seq 1 $NUM_RUNS); do
    RUN_TIME=$(/usr/bin/time -f "%e" $COMMAND 2>&1 1>/dev/null)
    TOTAL_TIME=$(echo "$TOTAL_TIME + $RUN_TIME" | bc)
done

AVERAGE_TIME=$(echo "$TOTAL_TIME / $NUM_RUNS" | bc -l)

echo "Average time over $NUM_RUNS runs: $AVERAGE_TIME seconds"
