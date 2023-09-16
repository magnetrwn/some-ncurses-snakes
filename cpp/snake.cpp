#include "snake.hpp"
#include <algorithm>
#include <stdbool.h>

/* Snake */

Snake::Snake(const size_t length, const Vec2 head, const Direction direction)
    : head(head), direction(direction), length(length) {}

void Snake::step(const Vec2 max_xy) {
    switch (direction) {
        case Direction::RIGHT: head.x++; break;
        case Direction::UP: head.y--; break;
        case Direction::LEFT: head.x--; break;
        case Direction::DOWN: head.y++; break;
        default: break;
    }

    if (head.x < 0)
        head.x = max_xy.x;
    else if (head.x >= max_xy.x)
        head.x = 1;

    if (head.y < 0)
        head.y = max_xy.y;
    else if (head.y >= max_xy.y)
        head.y = 1;

    if (!rotations.empty()) {
        rotations.erase(std::remove_if(rotations.begin(), rotations.end(), [] (const SnakeRotation& rotation) {
            return rotation.lifetime == 0;
        }), rotations.end());
    }
}

void Snake::rotate(const Direction direction) {
    rotations.push_back({
        this->head,
        this->direction,
        direction,
        this->length
    });
    this->direction = direction;
}

void Snake::lengthen(const size_t amount) {
    length += amount;
    for (auto it = rotations.begin(); it != rotations.end(); it++)
        it->lifetime += amount;
}

void Snake::draw(WINDOW *window, const Vec2 max_xy) const {
    Vec2 cursor = this->head;
    Direction cursor_direction = this->direction;

    for (size_t i = 0; i < this->length; i++) {
        if (cursor.x < 1)
            cursor.x = max_xy.x;
        else if (cursor.x > max_xy.x)
            cursor.x = 1;

        if (cursor.y < 1)
            cursor.y = max_xy.y;
        else if (cursor.y > max_xy.y)
            cursor.y = 1;

        mvwaddch(window, cursor.y, cursor.x, '@');

        auto it = std::find_if(rotations.begin(), rotations.end(), [&cursor] (const SnakeRotation& rotation) {
            return rotation.position.x == cursor.x && rotation.position.y == cursor.y;
        });

        if (it != rotations.end()) {
            cursor_direction = it->rot_from;
        }

        switch (cursor_direction) {
            case Direction::RIGHT: cursor.x--; break;
            case Direction::UP: cursor.y++; break;
            case Direction::LEFT: cursor.x++; break;
            case Direction::DOWN: cursor.y--; break;
            default: break;
        }
    }
}

bool Snake::is_stuck(const Vec2 max_xy) const {
    Vec2 cursor = this->head;
    Direction cursor_direction = this->direction;

    for (size_t i = 0; i < this->length; i++) {
        if (cursor.x < 1)
            cursor.x = max_xy.x;
        else if (cursor.x > max_xy.x)
            cursor.x = 1;

        if (cursor.y < 1)
            cursor.y = max_xy.y;
        else if (cursor.y > max_xy.y)
            cursor.y = 1;

        auto it = std::find_if(rotations.begin(), rotations.end(), [&cursor] (const SnakeRotation& rotation) {
            return rotation.position.x == cursor.x && rotation.position.y == cursor.y;
        });

        if (it != rotations.end()) {
            cursor_direction = it->rot_to;
        }

        switch (cursor_direction) {
            case Direction::RIGHT: cursor.x--; break;
            case Direction::UP: cursor.y++; break;
            case Direction::LEFT: cursor.x++; break;
            case Direction::DOWN: cursor.y--; break;
            default: break;
        }

        if (this->head.x == cursor.x && this->head.y == cursor.y)
            return true;
    }

    return false;
}

/* SnakeGame */

SnakeGame::SnakeGame(const size_t initial_length, const size_t pellets_amount)
    : window(nullptr) {
    initscr();
    cbreak();
    noecho();
    curs_set(FALSE);
    keypad(stdscr, TRUE);
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_WHITE);

    getmaxyx(stdscr, window_size.y, window_size.x);
    window_center = {window_size.x / 2, window_size.y / 2};
    window_max = {window_size.x - 2, window_size.y - 2};

    snake = std::make_unique<Snake>(Snake(initial_length, window_center, Direction::RIGHT));
}