#!/bin/bash

find "$HOME" -mindepth 1 -type d | xargs chmod 0755
