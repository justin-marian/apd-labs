#!/bin/bash

# Number of times to run ranksort
NUM_RUNS=2

# Output file
OUTPUT_FILE="results.txt"

# Remove the old results file if it exists
rm -f $OUTPUT_FILE

# Loop to execute ranksort and append results to the output file
for ((i=1; i<=$NUM_RUNS; i++)); do
    echo "Run $i" >> $OUTPUT_FILE
    make run2 >> $OUTPUT_FILE
    echo "" >> $OUTPUT_FILE
done

echo "All runs completed and results saved in $OUTPUT_FILE"
