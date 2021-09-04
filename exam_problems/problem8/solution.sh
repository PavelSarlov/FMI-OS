#!/bin/bash

find -H $(egrep "pesho:" /etc/passwd | awk -F ':' '{print $6}') -mindepth 1 -type f -printf "%T@ %n %i\n" |\
    awk '$2>1 {print $0}' | sort -rn | head -n 1 |  awk '{print $3}'
