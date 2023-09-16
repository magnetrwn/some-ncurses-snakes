#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include <ncurses.h>
#include <algorithm>
#include <deque>
#include <memory>

enum class Direction {
    NONE, RIGHT, UP, LEFT, DOWN
};

struct Vec2 {
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

    Vec2 get_head() const;
    size_t get_length() const;

    void draw(WINDOW *window, const Vec2 max_xy) const;
};

class SnakeGame {
protected:
    Vec2 window_size;
    Vec2 window_center;
    Vec2 window_max;
    WINDOW *window;

    std::unique_ptr<Snake> snake;
    std::unique_ptr<Vec2[]> pellets;

    bool snake_alive;
    const size_t initial_length;
    const size_t pellets_amount;

public:
    SnakeGame(const size_t initial_length = 5, const size_t pellets_amount = 20);
    virtual ~SnakeGame();

    void step();

    bool is_over() const;
};

#endif // SNAKE_HPP_