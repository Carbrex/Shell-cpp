#!/bin/bash

# Condition 1: Create a directory with 50 files of 1GB each
echo "Condition 1: Creating directory with 50 files of 1GB each..."
mkdir -p condition1
for ((i=1; i<=50; i++)); do
    dd if=/dev/zero of=condition1/file$i bs=1G count=1
done
echo "Condition 1 completed."

# Condition 2: Create a directory with 10,000 files of 10MB each
echo "Condition 2: Creating directory with 5,000 files of 10MB each..."
mkdir -p condition2
for ((i=1; i<=5000; i++)); do
    dd if=/dev/zero of=condition2/file$i bs=10M count=1
done
echo "Condition 2 completed."

# Condition 3: Create a directory with 100 files of 10MB each and subdirectories recursively
echo "Condition 3: Creating directory with 50 files of 10MB each and subdirectories recursively..."
mkdir -p condition3
for ((i=1; i<=50; i++)); do
    subdirectory=condition3/subdir$i
    mkdir -p $subdirectory
    for ((j=1; j<=50; j++)); do
        dd if=/dev/zero of=$subdirectory/file$j bs=10M count=1
    done
done
echo "Condition 3 completed."

# Calculate the total number of files in condition3
total_files=$(find condition3 -type f | wc -l)
echo "Total files in condition3: $total_files"
