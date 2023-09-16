#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include <ncurses.h>
#include <algorithm>
#include <deque>

enum class Direction {
    NONE, RIGHT, UP, LEFT, DOWN
};

struct Vec2 { // TODO: maybe replace with std::pair?
    long x, y;
};

struct SnakeRotation {
    Vec2 position;
    Direction rot_from;
    Direction rot_to;
    size_t lifetime;
};

class Snake {
protected:
    Vec2 head;
    Direction direction;
    size_t length;
    std::deque<SnakeRotation> rotations;

public:
    Snake(const size_t length, const Vec2 head, const Direction direction);

    void step(const Vec2 max_xy);
    void rotate(const Direction direction);
    void lengthen(const size_t amount);

    bool is_stuck(const Vec2 max_xy) const;
    void draw(WINDOW *window, const Vec2 max_xy) const;
};

class SnakeGame {
protected:
    const Vec2 window_size;
    WINDOW *window;
    Snake snake;

public:
    SnakeGame();
    virtual ~SnakeGame();

    void step();

    bool is_over() const;
};

#endif // SNAKE_HPP_