#!/bin/bash

# Usage: ./run.sh N
# This script compiles and runs 0.c to N.c with nicely formatted output

if [ -z "$1" ]; then
  echo "Usage: ./run.sh <max_file_number>"
  exit 1
fi

# Remove old executables (0, 1, 2, ...)
echo "[INFO] Cleaning up executables..."
for ((i=0; i<=$1; i++)); do
  [ -f "$i" ] && rm "$i"
done

# Compile and run 0.c to N.c
for ((i=0; i<=$1; i++)); do
  if [ -f "$i.c" ]; then
    make $i > /dev/null 2>&1
    if [ -f "$i" ]; then
      echo "=========== $i.c =========="
      ./$i
      echo
    else
      echo "[ERROR] Compilation failed for $i.c"
    fi
  else
    echo "[WARNING] File $i.c not found, skipping."
  fi
done

echo "=========== done =========="
