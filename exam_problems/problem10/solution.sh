#!/bin/bash

tail -n +2 "planets.txt" |\
    sort -t ';' -k 3n |\
    egrep "$(tail -n +2 planets.txt | sort -t ';' -k 3n | tail -n 1 | cut -d ';' -f 2)" |\
    awk -F ';' '{printf("%s\t%s\n",$1,$4)}' | head -n 1
