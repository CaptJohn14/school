# ==============================================================================
# File: johnsonle-script.#!/bin/sh
# By: Johnson Le
# Date: 04/15/19
# Desc: Iterates all file in directory and echos even numbered lines
# ==============================================================================
 # !/bin/bash
FILES=*     #grabs files in directory and throws in list basically
for file in $FILES      #iterate the list
do
    if [ $file != 'johnsonle-script.sh' ]      #skipping the script itself
    then
        count=1     #reset count to 1. only print when even
        while read -r line; do
            if (($count%2==0))      #determines if even number
            then
                echo ""$file": "$line""     #print
            fi
            let count++
        done < "$file"
    fi
done
