#include "snake.hpp"

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

    if (head.x < 1)
        head.x = max_xy.x;
    else if (head.x > max_xy.x)
        head.x = 1;

    if (head.y < 1)
        head.y = max_xy.y;
    else if (head.y > max_xy.y)
        head.y = 1;

    if (!rotations.empty())
        rotations.erase(
            std::remove_if(rotations.begin(), rotations.end(), [] (SnakeRotation& rotation) {
                if (rotation.lifetime == 1)
                    return true;
                else {
                    rotation.lifetime--;
                    return false;
                }
            }),
            rotations.end()
        );
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

size_t Snake::get_length() const {
    return length;
}

Vec2 Snake::get_head() const {
    return head;
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

        if (it != rotations.end())
            cursor_direction = it->rot_from;

        switch (cursor_direction) {
            case Direction::RIGHT: cursor.x--; break;
            case Direction::UP: cursor.y++; break;
            case Direction::LEFT: cursor.x++; break;
            case Direction::DOWN: cursor.y--; break;
            default: break;
        }
    }

    /*for (auto it = rotations.begin(); it != rotations.end(); it++)
        mvwaddch(window, it->position.y, it->position.x, 'x');*/
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

        if (it != rotations.end())
            cursor_direction = it->rot_from;

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
    : window(nullptr), snake_alive(true), initial_length(initial_length), pellets_amount(pellets_amount) {
    srand(time(nullptr));
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

    window = newwin(window_size.y, window_size.x, 0, 0);

    wtimeout(window, 133);

    snake = std::make_unique<Snake>(Snake(initial_length, window_center, Direction::RIGHT));
    pellets = std::make_unique<Vec2[]>(pellets_amount);

    for (size_t i = 0; i < pellets_amount; i++) {
        pellets[i] = {
            rand() % window_max.x + 1,
            rand() % window_max.y + 1
        };
    }
}

SnakeGame::~SnakeGame() {
    delwin(window);
    endwin();
}

void SnakeGame::step() {
    int ch = wgetch(window);
    switch (ch) {
        case 'w': snake->rotate(Direction::UP); break;
        case 's': snake->rotate(Direction::DOWN); break;
        case 'a': snake->rotate(Direction::LEFT); break;
        case 'd': snake->rotate(Direction::RIGHT); break;
        case 'q': snake_alive = false; return;
        default: break;
    }

    wclear(window);

    mvwprintw(window, 1, 1, " Score: %lu", snake->get_length() - initial_length);
    wattron(window, COLOR_PAIR(1));

    snake->step(window_max);
    snake->draw(window, window_max);

    wattroff(window, COLOR_PAIR(1));
    wattron(window, COLOR_PAIR(2));
    box(window, 0, 0);

    for (size_t i = 0; i < pellets_amount; i++) {
        if (snake->get_head().x == pellets[i].x && snake->get_head().y == pellets[i].y) {
            snake->lengthen(1);
            pellets[i].x = rand() % window_max.x + 1;
            pellets[i].y = rand() % window_max.y + 1;
        }
        mvwprintw(window, pellets[i].y, pellets[i].x, "%c", rand() % 10 + '0');
    }

    if (snake->is_stuck(window_max)) {
        wattron(window, COLOR_PAIR(3));
        // TODO: make this a little nicer
        mvwprintw(window, window_center.y, window_center.x - 6, "  GAME OVER  ");
        mvwprintw(window, window_center.y + 1, window_center.x - 6, "  %lu points   ", snake->get_length() - initial_length);
        wrefresh(window);
        napms(2000);
        wattroff(window, COLOR_PAIR(2));
        snake_alive = false;
        return;
    }

    wrefresh(window);
}

bool SnakeGame::is_over() const {
    return !snake_alive;
}