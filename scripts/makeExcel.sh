#!/bin/bash

# Directory containing your solutions
DIR="./solutions_codForces"

# Output CSV file
OUTPUT_FILE="problems.csv"

# CSV header
echo "Problem Link,Intuition,Submission Link" > "$OUTPUT_FILE"

# Loop through all matching files
for file in "$DIR"/*; do
    filename=$(basename "$file")
    
    # Match pattern: submission_id_contest_id_problem.ext
    if [[ $filename =~ ^([0-9]+)_([0-9]+)_([A-Z][0-9]*|[A-Z])\..+$ ]]; then
        submission_id="${BASH_REMATCH[1]}"
        contest_id="${BASH_REMATCH[2]}"
        problem_code="${BASH_REMATCH[3]}"

        problem_link="https://codeforces.com/problemset/problem/${contest_id}/${problem_code}"
        submission_link="https://codeforces.com/contest/${contest_id}/submission/${submission_id}"

        # Output as CSV row
        echo "\"$problem_link\",\"\",\"$submission_link\"" >> "$OUTPUT_FILE"
    fi
done

echo "CSV file '$OUTPUT_FILE' created."
