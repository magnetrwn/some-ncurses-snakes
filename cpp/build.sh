#!/usr/bin/sh
set -e

clang++ -c main.cpp -o main.o
clang++ -c snake.cpp -o snake.o
clang++ main.o snake.o -lncurses -o snake

rm main.o snake.o
