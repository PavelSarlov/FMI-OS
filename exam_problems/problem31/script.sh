#!/bin/bash

if [ "$#" -ne 2 ]; then
    echo "error: invalid number of args"
    exit 1
fi

if [ ! -r "$1" ]; then
    echo "error: invalid fail or missing reading permissions"
    exit 2
fi

cat "$1" |\
    awk -F ',' '{data=$2","$3","$4; if(data in map && map[data]>$1) map[data]=$1; else map[data]=$1}
        END { for(i in map) print map[i]","i}' >"$2"
