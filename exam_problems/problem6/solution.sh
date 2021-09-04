#!/bin/bash

find "$(pwd)" -mindepth 1 -maxdepth 1 |\
    xargs stat -c "%h,%n" |\
    sort -t ',' -k 1nr | head -n 5 |\
    cut -d ',' -f 2- | xargs basename
