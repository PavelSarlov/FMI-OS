#!/bin/bash

DIR=$(pwd)

find $DIR -mindepth 1 -maxdepth 1 -type f | xargs rm
find $DIR -mindepth 1 -maxdepth 1 -type d | xargs rmdir
