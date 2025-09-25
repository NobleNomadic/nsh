#!/bin/bash
# Compile shell, run if 'run' specified
set -e

CC_INPUTS="src/main.c"
CC_TARGET="build/nsh"
CC_FLAGS="-O2"

gcc $CC_FLAGS -o $CC_TARGET $CC_INPUTS

if [[ $1 == "run" ]]; then
  cd build
  ./nsh
  cd ..
fi
