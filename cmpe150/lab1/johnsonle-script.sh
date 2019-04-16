# ==============================================================================
# File: johnsonle-script.#!/bin/sh
# By: Johnson Le
# Date: 04/15/19
# Desc: Iterates all file in directory and echos even numbered lines
# ==============================================================================
 # !/bin/bash
for file in *; do           #iterates all the files
    while read -r line; do  #reads a line
        read -r line        #reads again. Basically ignores the last read.
        echo ""$file": "$line""
    done < "$file"
done
