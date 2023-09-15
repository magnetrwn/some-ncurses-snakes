#!/usr/bin/sh
set -e

clang -Wall \
      -Wextra \
      -Wpedantic \
      -O1 \
      -lncurses \
      -o snake snake.c
