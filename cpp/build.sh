#!/usr/bin/sh
set -e

COMPILER=clang++
CFLAGS='-Wall -Wextra -Werror -Wpedantic -std=c++14 -O3'

$COMPILER $CFLAGS -c main.cpp -o main.o
$COMPILER $CFLAGS -c snake.cpp -o snake.o
$COMPILER $CFLAGS main.o snake.o -lncurses -o snake

rm main.o snake.o
