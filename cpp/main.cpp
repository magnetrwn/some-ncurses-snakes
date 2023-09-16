#include "snake.hpp"

int main() {
    SnakeGame game;
    while (!game.is_over())
        game.step();
    return 0;
}