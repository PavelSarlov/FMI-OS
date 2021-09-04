#!/bin/bash

input="$( cat <&0 | egrep "^-?[[:digit:]]+$" | sort -nu)"

max="$( echo "$input" | tr -d '-' | sort -n | tail -n 1)"

echo "$input" |\
    egrep "^-?$max$"
