#!/bin/bash

PASSWD="./example_passwd"

egrep "/home/Inf" "$PASSWD" |\
    egrep ":[[:alpha:]]* [[:alpha:]]*a" |\
    awk -F ':' '{print $1}' |\
    sed -r 's/s[0-9]([0-9]{2})[0-9]*/\1/g' | sort -n | uniq -c | sort -n | tail -n 1
