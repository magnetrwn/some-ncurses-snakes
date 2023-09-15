#!/usr/bin/sh
set -e

clang -c main.c -o main.o
clang -c snake.c -o snake.o
clang main.o snake.o -lncurses -o snake

rm main.o snake.o
