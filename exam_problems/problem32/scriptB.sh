#!/bin/bash

input="$( cat <&0 | egrep "^-?[[:digit:]]+$" | sort -nu)"

echo "$input" |\
    awk 'function abs(v) { return v<0 ? -v : v } 
    { num=abs($1); dsum=0; while(num>0) {dsum+=num%10; num=int(num/10) }  print $1,dsum }' |\
        sort -t ' ' -k 2n -k 1nr |\
        tail -n 1 |\
        cut -d ' ' -f 1 
