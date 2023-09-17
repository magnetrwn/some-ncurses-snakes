#!/usr/bin/sh
set -e

COMPILER=clang
CFLAGS='-Wall -Wextra -Werror -Wpedantic -std=c99 -O3'

$COMPILER $CFLAGS -c main.c -o main.o
$COMPILER $CFLAGS -c snake.c -o snake.o
$COMPILER $CFLAGS main.o snake.o -lncurses -o snake

rm main.o snake.o
