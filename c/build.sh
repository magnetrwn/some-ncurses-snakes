#!/usr/bin/sh
set -e

clang -Wall \
      -Wextra \
      -Wpedantic \
      -O3 \
      -lncurses \
      -o snake snake.c
