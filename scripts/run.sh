#!/bin/bash

# Compile the a.cpp file
g++ a.cpp -o a.out

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful. Running a.out..."
    # Execute the a.out file
    ./a.out
else
    echo "Compilation failed. Please check your code."
fi
