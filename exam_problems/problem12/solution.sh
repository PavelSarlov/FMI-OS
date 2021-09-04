#!/bin/bash

#find /home/velin -samefile $(find /home/velin -printf "%Ts %p\n" | sort -rn | head -n 1 | awk '{print $2}') -printf "%d %p\n" | sort -rn | head -n 1 | awk '{print $1}'

find $(pwd)/velin/ -samefile $(find $(pwd)/velin/ -printf "%Ts %p\n" | sort -rn | head -n 1 | awk '{print $2}') -printf "%p\n" |\
   sed -E 's/[^\/]*//g' | wc -m
