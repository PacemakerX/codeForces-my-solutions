#!/bin/bash

# Define the extensions to keep
extensions="cpp sh out txt md"

# Loop through all files in the current directory
for file in *; do
  # Check if the item is a file
  if [[ -f $file ]]; then
    # Get the file extension
    ext="${file##*.}"
    
    # Check if the file extension is not in the list of allowed extensions
    if ! echo "$extensions" | grep -wq "$ext"; then
      echo "Removing $file"
      rm "$file"
    fi
  fi
done
