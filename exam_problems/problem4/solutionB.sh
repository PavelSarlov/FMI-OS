#!/bin/bash

egrep ":$(id -u):" /etc/passwd | cut -d : -f 6 | xargs -I {} find {} -mindepth 1 -printf "%s %f\n" | sort -n -r | head -n 5
