#!/bin/bash

awk -F ':' '$6 ~ /home/SI {print $6}' /etc/passwd |\
    xargs stat -c "%Z %n" |\
    awk '($1>=155168000) && ($1<=1551176100) {print $2}' |\
    xargs -I {} egrep {} /etc/passwd |\
    sed -E 's/s([0-9]*):.*:([[:alpha:]]* [[:alpha:]]*),.*/\1\t\2/g'
