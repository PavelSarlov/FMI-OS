#!/bin/bash

find ${HOME} -perm 644 | xargs chmod g+w
