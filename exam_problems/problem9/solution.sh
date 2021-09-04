#!/bin/bash

egrep "/home/SI" "./test_passwd" |\
    sort -t ':' -k 1n |\
    cut -d : -f 5,6 |\
    sed -r 's/([[:alpha:]]* [[:alpha:]]*).*:/\1:/g'
