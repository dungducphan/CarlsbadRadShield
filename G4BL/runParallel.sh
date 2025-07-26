#!/bin/bash

# ====================================================================================================================
# Parallel G4BL Job Runner Script
# This script runs multiple G4BL simulations in parallel with unique RUNID parameters
# ====================================================================================================================

# Configuration parameters
G4BL_FILE="stainlesssteel_chammber_halo.g4bl"
NUM_JOBS=80                    # Number of parallel jobs to run
MAX_PARALLEL=20                # Maximum number of jobs to run simultaneously
THICKNESS=19.5                 # Lead thickness parameter (mm)
SHIELDGAP=200                  # Gap between target chamber and shield (mm)

# Create output directory with timestamp
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
OUTPUT_DIR="results_${TIMESTAMP}"
mkdir -p $OUTPUT_DIR

echo "======================================================================================================================"
echo "Starting parallel G4BL simulation"
echo "G4BL file: $G4BL_FILE"
echo "Number of jobs: $NUM_JOBS"
echo "Maximum parallel jobs: $MAX_PARALLEL"
echo "Output directory: $OUTPUT_DIR"
echo "Lead thickness: $THICKNESS mm"
echo "Shield gap: $SHIELDGAP mm"
echo "======================================================================================================================"

# Function to run a single G4BL job
run_g4bl_job() {
    local runid=$1
    local logfile="${OUTPUT_DIR}/run_${runid}.log"
    local errfile="${OUTPUT_DIR}/run_${runid}.err"
    
    echo "Starting job $runid at $(date)"
    
    # Run G4BL with parameters
    g4bl $G4BL_FILE RUNID=$runid THICKNESS=$THICKNESS SHIELDGAP=$SHIELDGAP \
        > $logfile 2> $errfile
    
    local exit_code=$?
    
    if [ $exit_code -eq 0 ]; then
        echo "Job $runid completed successfully at $(date)"
        # Move the output ROOT file to the results directory
        if [ -f "run_${runid}.root" ]; then
            mv "run_${runid}.root" $OUTPUT_DIR/
        fi
    else
        echo "Job $runid failed with exit code $exit_code at $(date)"
    fi
    
    return $exit_code
}

# Export the function so it can be used by parallel processes
export -f run_g4bl_job
export OUTPUT_DIR THICKNESS SHIELDGAP G4BL_FILE

# Track running jobs
declare -a job_pids=()
job_count=0

# Start jobs
for ((i=1; i<=NUM_JOBS; i++)); do
    # Wait if we've reached the maximum number of parallel jobs
    while [ ${#job_pids[@]} -ge $MAX_PARALLEL ]; do
        # Check for completed jobs
        for j in "${!job_pids[@]}"; do
            if ! kill -0 ${job_pids[j]} 2>/dev/null; then
                # Job has finished, remove from array
                unset job_pids[j]
            fi
        done
        # Rebuild array to remove gaps
        job_pids=($(for pid in "${job_pids[@]}"; do echo $pid; done))
        sleep 1
    done
    
    # Start new job in background
    run_g4bl_job $i &
    job_pids+=($!)
    job_count=$((job_count + 1))
    
    echo "Started job $i (PID: ${job_pids[-1]})"
    sleep 0.5  # Small delay to avoid overwhelming the system
done

# Wait for all remaining jobs to complete
echo "Waiting for all jobs to complete..."
for pid in "${job_pids[@]}"; do
    wait $pid
done

echo "======================================================================================================================"
echo "All jobs completed at $(date)"
echo "Output files are in: $OUTPUT_DIR"
echo "======================================================================================================================"

# Summary of results
echo "Summary:"
echo "--------"
root_files=$(ls $OUTPUT_DIR/*.root 2>/dev/null | wc -l)
log_files=$(ls $OUTPUT_DIR/*.log 2>/dev/null | wc -l)
err_files=$(ls $OUTPUT_DIR/*.err 2>/dev/null | wc -l)

echo "ROOT files created: $root_files"
echo "Log files created: $log_files"
echo "Error files created: $err_files"

# Check for any jobs that may have failed
if [ -d "$OUTPUT_DIR" ]; then
    echo ""
    echo "Checking for failed jobs..."
    failed_jobs=0
    for ((i=1; i<=NUM_JOBS; i++)); do
        if [ ! -f "$OUTPUT_DIR/run_${i}.root" ]; then
            echo "  Job $i: ROOT file missing"
            failed_jobs=$((failed_jobs + 1))
        elif [ -s "$OUTPUT_DIR/run_${i}.err" ]; then
            echo "  Job $i: Error file not empty"
            failed_jobs=$((failed_jobs + 1))
        fi
    done
    
    if [ $failed_jobs -eq 0 ]; then
        echo "All jobs completed successfully!"
    else
        echo "Warning: $failed_jobs job(s) may have failed. Check log and error files."
    fi
fi

echo "======================================================================================================================"
